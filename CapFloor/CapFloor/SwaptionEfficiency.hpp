/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*!
 Copyright (C) 2005, 2006, 2007, 2009 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

// the only header you need to use QuantLib
#include <ql/quantlib.hpp>

#ifdef BOOST_MSVC
/* Uncomment the following lines to unmask floating-point
   exceptions. Warning: unpredictable results can arise...

   See http://www.wilmott.com/messageview.cfm?catid=10&threadid=9481
   Is there anyone with a definitive word about this?
*/
// #include <float.h>
// namespace { unsigned int u = _controlfp(_EM_INEXACT, _MCW_EM); }
#endif



//#include "CapFloorEfficiency.hpp"
#include <boost/foreach.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <iomanip>
#include "CapFloorEfficiency.hpp"
#include "SwaptionVolatility.h"
using namespace QuantLib;

#ifdef BOOST_MSVC
#  ifdef QL_ENABLE_THREAD_SAFE_OBSERVER_PATTERN
#    include <ql/auto_link.hpp>
#    define BOOST_LIB_NAME boost_system
#    include <boost/config/auto_link.hpp>
#    undef BOOST_LIB_NAME
#    define BOOST_LIB_NAME boost_thread
#    include <boost/config/auto_link.hpp>
#    undef BOOST_LIB_NAME
#  endif
#endif


#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

	Integer sessionId() { return 0; }

}
#endif

namespace {
	struct SwaptionPrameters {
		// common parameters
		Date today;
		Date settlement;
		Real nominal;
		Calendar calendar;
		BusinessDayConvention fixedConvention;
		BusinessDayConvention floatingConvention;
		Frequency fixedFrequency, floatingFrequency;
		DayCounter termStructureDayCounter, fixedDayCount;

		Period floatingTenor;
		boost::shared_ptr<IborIndex> index;
		Natural settlementDays;
		RelinkableHandle<YieldTermStructure> termStructure;
		Handle<SwaptionVolatilityStructure> volatility;

		// Pricing Engine defition for the swaption
		boost::shared_ptr<PricingEngine> DefineEngineEUR() {
		        return boost::shared_ptr<PricingEngine>(
					   new BlackSwaptionEngine(termStructure, volatility));
			
		}
		boost::shared_ptr<PricingEngine> DefineEngineAMER() {

			Real a = 0.0226, sigma = 0.13;
			// For American Swaption
			const boost::shared_ptr<HullWhite> model(new HullWhite(termStructure));
			return boost::shared_ptr<PricingEngine>(
				new TreeSwaptionEngine(model, 50, termStructure));
		}
		boost::shared_ptr<PricingEngine> DefineEngineBERM() {
			Real a = 0.0226, sigma = 0.13;
			
			const boost::shared_ptr<HullWhite> model(new HullWhite(termStructure));
			return boost::shared_ptr<PricingEngine>(
				new TreeSwaptionEngine(model, 50, termStructure));
		}

		//Definition of Swap: the underlying
		boost::shared_ptr<VanillaSwap> DefineSwap(VanillaSwap::Type type, Date swapStartDate, Integer duration, Rate fixedRate) {
			//settlement = calendar.advance(exerciseDate, settlementDays, Days);
			Date maturityDate = calendar.advance(swapStartDate, duration, Years,
				floatingConvention);

			Schedule fixedSchedule(swapStartDate, maturityDate, Period(fixedFrequency),
				calendar, fixedConvention, fixedConvention,
				DateGeneration::Forward, false);

			Schedule floatSchedule(swapStartDate, maturityDate,
				Period(floatingFrequency),
				calendar, floatingConvention,
				floatingConvention,
				DateGeneration::Forward, false);

			boost::shared_ptr<VanillaSwap> swap(
				new VanillaSwap(type, nominal,
				fixedSchedule, fixedRate, fixedDayCount,
				floatSchedule, index, 0.0,
				index->dayCounter()));

			swap->setPricingEngine(boost::shared_ptr<PricingEngine>(
				new DiscountingSwapEngine(termStructure)));

			return swap;
		}

		// Defition of Swaption 
		boost::shared_ptr<Swaption> DefineSwaption(int style,
			const boost::shared_ptr<VanillaSwap>& swap,
			const Date& earliestDate,
			const Date& exercise,
			Volatility volatility
			) {

			std::vector<Date> exerciseDates;
			const Leg& leg = swap->fixedLeg();
			for (Size i = 0; i<leg.size(); i++) {
				boost::shared_ptr<Coupon> coupon =
					boost::dynamic_pointer_cast<Coupon>(leg[i]);
				exerciseDates.push_back(coupon->accrualStartDate());
			}

			if (style == 1){// For European Swaption
				Settlement::Type settlementType = Settlement::Physical;
				boost::shared_ptr<Swaption> result(new Swaption(swap,
					boost::shared_ptr<Exercise>(
					new EuropeanExercise(exercise)),
					settlementType));
				result->setPricingEngine(DefineEngineEUR());
				return result;
			}
			else if(style == 2){// For American Swaption
				Settlement::Type settlementType = Settlement::Physical;
				boost::shared_ptr<Swaption> result(new Swaption(swap,
					boost::shared_ptr<Exercise>(
					new AmericanExercise(earliestDate, exercise, false)),
					settlementType));
				result->setPricingEngine(DefineEngineAMER());
				return result;
			}else{ // For Bermudian Exercice
				Settlement::Type settlementType = Settlement::Physical;
				boost::shared_ptr<Swaption> result(new Swaption(swap,
					boost::shared_ptr<Exercise>(
					new BermudanExercise(exerciseDates)),
					settlementType));

				result->setPricingEngine(DefineEngineBERM());
				return result;
			}
           /* */
		
        }


			
		 // initialize
        SwaptionPrameters() {
            settlementDays = 2;
            nominal = 1000000.0;
			fixedDayCount = Actual365Fixed();
            fixedConvention = ModifiedFollowing;
            fixedFrequency = Semiannual;
            floatingFrequency = Semiannual;
            index = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));
            floatingConvention = index->businessDayConvention();
            floatingTenor = index->tenor();

           // calendar = index->fixingCalendar();
			calendar = TARGET();
			today = Date(6,Oct,2014);
           // today = calendar.adjust(Date::todaysDate());
            Settings::instance().evaluationDate() = today;
            settlement = calendar.advance(today,settlementDays,Days);
			CurveData cData;
			// Fill out with some sample market data
			sampleMktData(cData);

			// Build a curve linked to this market data
			boost::shared_ptr<YieldTermStructure> ocurve=buildCurve(cData);

			// Link the curve to the term structure
			termStructure.linkTo(ocurve);

			SwaptionVolatility vol;
			vol.termStructure.linkTo(ocurve);
			volatility = vol.atmVolMatrix;
			
			/*schedule = Schedule(settlement,calendar.advance(settlement,duration,Years,
                                             floatingConvention),
                                   Period(floatingFrequency),
                                   calendar,floatingConvention,
                                   floatingConvention,
                                   DateGeneration::Forward,false);*/
        }
    };

}


void PrintCurve( SwaptionPrameters SwaptionEfficiency){}
void PrintCashFlowSwap( boost::shared_ptr<Swaption> InstrumentEfficiency){
	 boost::shared_ptr< VanillaSwap > Underlying = InstrumentEfficiency->underlyingSwap();
  std::vector< boost::shared_ptr< CashFlow > > floatingleg = Underlying->floatingLeg();
  std::vector< boost::shared_ptr< CashFlow > > fixedleg = Underlying->fixedLeg();
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Date            |" <<  "Fixed Leg Cash FLow              |" <<  std::endl;
  BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, fixedleg){
	  std::cout <<  io::short_date(Cash->date())<<"     "<< Cash->amount() << std::endl;
  }

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << " Date            |" <<  " Floating Leg Cash FLow              |" <<  std::endl;

  BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, floatingleg){
	  std::cout <<  io::short_date(Cash->date())<<"     "<< Cash->amount() << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}
void PrintResultsSwap(Real strikes, Volatility vol , Real nominal, Date maturityDate,  boost::shared_ptr<Swaption> InstrumentEfficiency, SwaptionPrameters &SwaptionEfficiency){
	 
  boost::shared_ptr< VanillaSwap > Underlying = InstrumentEfficiency->underlyingSwap();
  boost::shared_ptr< IborIndex > index = Underlying->iborIndex();
  Schedule schedule =   Underlying->floatingSchedule() ;  
  std::vector <Date > SchedulDates = schedule.dates();

  Integer fixing = 2;
  Calendar  calendar= TARGET();
 std::cout << "Start              |" <<  "End              |" <<  "Fixing  |" 
	 <<    "IDays        |"  << "Index Rate|" <<   std::endl; 

  BOOST_FOREACH(Date d, SchedulDates){

		Date startDate = schedule.previousDate(d);
		if (startDate!= Date()) {
		Date FixingDate = index->fixingDate(startDate);

		Date endDate = schedule.nextDate(d);
		Time Idays = daysBetween(startDate, endDate);
		Date d1=d+6*Months;
		std::cout <<  io::short_date(startDate) << "       "  << 
			io::short_date(endDate) << "        "  << 
			io::short_date(FixingDate) << "       "  << 
			 Idays << "            "  << 
			100*index->fixing(FixingDate)<< "         " << std::endl;
		}
	}
		std::cout <<  std::endl;
		std::cout << std::endl;
		std::cout <<"---------------------------------------------------|"<< std::endl;
		std::cout << " Strike = " << io::rate(strikes) <<std::endl;
		std::cout << " Notional = "<< nominal << std::endl;
		std::cout << " Maturity Date = " << io::short_date(maturityDate) << std::endl;
		std::cout << std::endl;

		std::cout << " Fixed Leg Swap NPV = "<< InstrumentEfficiency->underlyingSwap()->fixedLegNPV()<< std::endl;
		std::cout << std::endl;
		std::cout << " Floating Leg Swap  NPV = " << InstrumentEfficiency->underlyingSwap()->floatingLegNPV() <<std::endl;
		std::cout << std::endl;
		std::cout << " Swap::  " <<  InstrumentEfficiency->underlyingSwap()->type() <<"   NPV :" << InstrumentEfficiency->underlyingSwap()->NPV() <<std::endl;
		std::cout << " Swap Fair Rate =   " << InstrumentEfficiency->underlyingSwap()->fairRate() << std::endl;
		std::cout << std::endl;
		std::cout << " SWAPTION  NPV = " << InstrumentEfficiency->NPV() <<std::endl;
		std::cout <<"----------------------------------------------------|"<< std::endl;
		std::cout << std::endl;
		std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : " ;
}
void InputparmetersSwaption(int & style, Real & strikes, Real & vol , Real & notional, VanillaSwap::Type & type){

		std::string s ;
		std::string const pay ="1" ;
		std::string const receive ="2" ;
		std::cout << " Choisir le style.  1: EUROPEAN, 2: AMERICAN, 3 : BERMUDIAN  ";
		std::cin >> s;
		try{ style = boost::lexical_cast<Real>(s); }
		catch (boost::bad_lexical_cast const&)
		{
			style = 1;
		}
			std::cout << " Entrer le type de Swap. Choisir 1: Payeur, 2: Receveur   " ;
			std::cin >> s ;
			if (s == pay) 
				type = VanillaSwap::Payer;
			else if (s==receive) 
				type = VanillaSwap::Receiver;
			else
			QL_FAIL("unknown Swap type");

		std::istringstream convert;
			std::cout << " Entrer le  Strike  ";
			std::cin >> s ;
			try{strikes = boost::lexical_cast<Real>(s);}
			catch (boost::bad_lexical_cast const&)
			{strikes = 0;}

			while ( strikes==0 ) {
				std::cout << " Entrer le  Strike   " ;
				std::cin >> s ;
				try
				{strikes = boost::lexical_cast<Real>(s);}
				catch (boost::bad_lexical_cast const&)
				{strikes = 0;}
				}
			/*std::cout << " Entrer la volatilite   ";
			std::cin >> s ;
			try{vol = boost::lexical_cast<Real>(s);}
			catch (boost::bad_lexical_cast const&)
			{vol = 0;}
			
			while ( vol < 0 ) {
				std::cout << " Entrer la  volatilite  ";
				std::cin >> s ;
				try
				{vol = boost::lexical_cast<Real>(s);}
				catch (boost::bad_lexical_cast const&)
				{vol = -1;}
				
				}*/
			std::cout << " Entrer le  nominal   ";
			std::cin >> s ;
			try{notional = boost::lexical_cast<Real>(s);}
			catch (boost::bad_lexical_cast const&)
			{notional = 0;}
			while (notional==0 ) {
				std::cout << " Entrer le  nominal   ";
				std::cin >> s ;
				try
				{notional = boost::lexical_cast<Real>(s);}
				catch (boost::bad_lexical_cast const&)
				{notional = 0;}
				std::cout << " nominal = " << io::rate(notional) <<std::endl;
				}
		
}

Matrix volswaptionLMM(std::vector<Date> optionDate, std::vector<Date> swapDate, std::vector<Rate> strike){
	int n = optionDate.size();
	int m = swapDate.size();
	Matrix V(m, n);
	Size i, j;
	bool extrapolate = false;
	DayCounter  basis = Actual360();
	std::vector<Time> optionTime(n);
	std::vector<Time> swapLenght(m);
		SwaptionVolatility vol;
	Handle<SwaptionVolatilityStructure> volatilityMatrix = vol.atmVolMatrix;
	Date RefDate = Date(8, Oct, 2014);
	Time optionTimes;
	Time swapTimes;

for (j = 0; j < optionDate.size(); j++){
			for (i = 0; i < swapDate.size(); i++){/*
			optionTime[j] = basis.yearFraction(optionDate[j], optionDate[j + 1]);
			swapLenght[i] = basis.yearFraction(swapDate[i], swapDate[i + 1]);*/
				optionTimes = basis.yearFraction(RefDate, optionDate[j]);
				swapTimes = basis.yearFraction(RefDate, swapDate[i]);
				Volatility v = volatilityMatrix->volatility(optionTimes, swapTimes, strike[j], extrapolate);
			V[i][j] = v;
		}
	}
	return V;
}
void PrintVolatility(){
	std::vector<Date> optionDate;
	optionDate.push_back(Date(8, Apr, 2015));
	optionDate.push_back(Date(8, Oct, 2015));
	optionDate.push_back(Date(8, Apr, 2016));
	optionDate.push_back(Date(10, Oct, 2016));
	optionDate.push_back(Date(10, Apr, 2017));
	optionDate.push_back(Date(9, Oct, 2017));
	optionDate.push_back(Date(9, Apr, 2018));
	optionDate.push_back(Date(8, Oct, 2018));
	optionDate.push_back(Date(8, Apr, 2019));
	optionDate.push_back(Date(8, Oct, 2019));
	optionDate.push_back(Date(8, Apr, 2020));
	optionDate.push_back(Date(8, Oct, 2020));
	optionDate.push_back(Date(8, Apr, 2021));
	optionDate.push_back(Date(8, Oct, 2021));
	optionDate.push_back(Date(8, Apr, 2022));
	optionDate.push_back(Date(10, Oct, 2022));
	optionDate.push_back(Date(10, Apr, 2023));
	optionDate.push_back(Date(9, Oct, 2023));
	optionDate.push_back(Date(8, Apr, 2024));

	std::vector<Date> SwapDate;
	SwapDate.push_back(Date(8, Apr, 2015));
	SwapDate.push_back(Date(8, Oct, 2015));
	SwapDate.push_back(Date(8, Apr, 2016));
	SwapDate.push_back(Date(10, Oct, 2016));
	SwapDate.push_back(Date(10, Apr, 2017));
	SwapDate.push_back(Date(9, Oct, 2017));
	SwapDate.push_back(Date(9, Apr, 2018));
	SwapDate.push_back(Date(8, Oct, 2018));
	SwapDate.push_back(Date(8, Apr, 2019));
	SwapDate.push_back(Date(8, Oct, 2019));
	SwapDate.push_back(Date(8, Apr, 2020));
	SwapDate.push_back(Date(8, Oct, 2020));
	SwapDate.push_back(Date(8, Apr, 2021));
	SwapDate.push_back(Date(8, Oct, 2021));
	SwapDate.push_back(Date(8, Apr, 2022));
	SwapDate.push_back(Date(10, Oct, 2022));
	SwapDate.push_back(Date(10, Apr, 2023));
	SwapDate.push_back(Date(9, Oct, 2023));
	SwapDate.push_back(Date(8, Apr, 2024));

	std::vector<Rate> strikes;
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	strikes.push_back(0.1);
	Matrix Vmatrix = volswaptionLMM(optionDate, SwapDate, strikes);
	std::cout << "-----------------------Voltility Matrixx----------------------------|" << std::endl;

	for (Size j = 0; j < optionDate.size(); j++){
		for (Size i = 0; i < SwapDate.size(); i++){
			std::cout << std::setprecision(2) << "   " << io::rate(Vmatrix[i][j]);
				}
		std::cout << std::endl;
	}
	std::string s;
	
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter ";
	std::cin >> s;

}

void TestSwaption(){
	// Parmameters
	SwaptionPrameters SwaptionEfficiency;
	Date today = Date(6, Oct, 2014);
	VanillaSwap::Type type;
	Integer Duration = 3;
	Volatility vol= 0.15;
	Rate strikes= 0.015;
	Rate fixedRate = 0.1;
	Real notional = 1000000.0;
	int Style = 1; // 1 == EUROPEAN, 2 = AMERICAN,  3 = BERMUDIAN
	Date exerciseDate = Date(8, Oct, 2018);
	//Date exerciseDate = SwaptionEfficiency.calendar.advance(SwaptionEfficiency.today ,1*Years);
	Date earliestDate = SwaptionEfficiency.calendar.advance(today, 2 * Days);
	Date swapStartDate = SwaptionEfficiency.calendar.advance(exerciseDate, 2 * Days);
	std::string s ;
	int Fin = 1;
	std::cout << " Pricing d'un SWAPTION " << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter "; 
	std::cin >> s ;
	try
	{Fin = boost::lexical_cast<int>(s);}
	catch (boost::bad_lexical_cast const&)
	{Fin = 0;}
	while (Fin != 0) {
		InputparmetersSwaption(Style, strikes,vol , notional, type);
		SwaptionEfficiency.nominal = notional;

		if (Style == 1){
			earliestDate = SwaptionEfficiency.calendar.advance(exerciseDate, 2 * Days);
			// For europeann earliestDate = swapStartDate
		}
		else if (Style==2){
			earliestDate = SwaptionEfficiency.calendar.advance(today, 2 * Days);
			Duration = 7;

		}
		else{
		//	earliestDate = SwaptionEfficiency.calendar.advance(today, 6 * Months);
			earliestDate = SwaptionEfficiency.calendar.advance(today, 2 * Days);
			Duration = 7;
		}
		//Definition of the swap
		boost::shared_ptr<VanillaSwap> swap = SwaptionEfficiency.DefineSwap(type, earliestDate, Duration, strikes);
			//Definition of the swaption
		boost::shared_ptr<Swaption> swaption = SwaptionEfficiency.DefineSwaption(Style,swap, earliestDate,exerciseDate, vol);

		//// Print rseults
		PrintCashFlowSwap( swaption);
		PrintResultsSwap(strikes,  vol,  SwaptionEfficiency.nominal,  exerciseDate,  swaption, SwaptionEfficiency);
		std::cout << std::endl;
		std::cout << " Taper 1 pour continuer ,  0 pour Quitter " ;
		std::cin >> s ;
		try
		{Fin = boost::lexical_cast<int>(s);}
		catch (boost::bad_lexical_cast const&)
		{Fin = 0;}
	}

}
