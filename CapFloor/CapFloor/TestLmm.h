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
//#include "CurveData.hpp"
//#include "CapFloorVolatility.h"

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

	struct CapFloorLmm {
		// common parameters
		Date settlement;
		std::vector<Real> nominals;
		BusinessDayConvention convention;
		Frequency frequency;
		Integer Duration;
		boost::shared_ptr<IborIndex> index;
		Calendar calendar;
		Natural fixingDays;
		RelinkableHandle<YieldTermStructure> termStructure;
		DayCounter termStructureDayCounter;
		Schedule schedule;
		Date today;
		Handle<OptionletVolatilityStructure> caplets;
		boost::shared_ptr<CapFloorTermVolSurface> capVolSurface;

		Handle<OptionletVolatilityStructure> capletsVolatilies;
		//CapFloorVolatility cVol;
		// Fill out with some sample market data
		Date startDate;
		// Some initializations


		BusinessDayConvention fixedConvention;
		BusinessDayConvention floatingConvention;
		Frequency fixedFrequency, floatingFrequency;
		DayCounter fixedDayCount;

		CapFloorLmm() : nominals(1, 100){
			frequency = Semiannual;
			index = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));
			//index = boost::shared_ptr<IborIndex>(new EURLibor6M(termStructure));
			calendar = TARGET();
			convention = ModifiedFollowing;
			Duration = 5;
			termStructureDayCounter = Actual360();
			fixedDayCount = Actual365Fixed();
			fixedConvention = ModifiedFollowing;
			//fixedConvention = Unadjusted;
			fixedFrequency = Semiannual;
			floatingFrequency = Semiannual;
			floatingConvention = ModifiedFollowing;
			//floatingTenor = index->tenor();
			Date todaysDate =
				index->fixingCalendar().adjust(Date(6, October, 2014));
			Settings::instance().evaluationDate() = todaysDate;
#pragma region Vetor of Dates
			std::vector <Date > dates;
			//dates.push_back(Date(6, October, 2014));
			dates.push_back(Date(8, October, 2014));
			dates.push_back(Date(15, October, 2014)); dates.push_back(Date(22, October, 2014)); dates.push_back(Date(29, October, 2014));
			dates.push_back(Date(10, November, 2014)); dates.push_back(Date(8, December, 2014)); dates.push_back(Date(8, January, 2015));
			dates.push_back(Date(9, Feb, 2015)); dates.push_back(Date(9, March, 2015)); dates.push_back(Date(8, April, 2015));
			dates.push_back(Date(8, July, 2015));
			dates.push_back(Date(8, October, 2015));


			dates.push_back(Date(8, April, 2016)); dates.push_back(Date(10, October, 2016)); dates.push_back(Date(10, April, 2017));
			dates.push_back(Date(9, October, 2017)); dates.push_back(Date(9, April, 2018)); dates.push_back(Date(8, October, 2018));
			dates.push_back(Date(8, April, 2019)); dates.push_back(Date(8, October, 2019)); dates.push_back(Date(8, April, 2020));
			dates.push_back(Date(8, October, 2020));


			dates.push_back(Date(8, April, 2021)); dates.push_back(Date(8, October, 2021)); dates.push_back(Date(8, April, 2022));
			dates.push_back(Date(10, October, 2022)); dates.push_back(Date(10, April, 2023)); dates.push_back(Date(9, October, 2023));
			dates.push_back(Date(8, April, 2024)); dates.push_back(Date(8, October, 2024)); dates.push_back(Date(8, April, 2025));
			dates.push_back(Date(8, October, 2025));

			dates.push_back(Date(8, April, 2026)); dates.push_back(Date(8, October, 2026)); dates.push_back(Date(8, April, 2027));
			dates.push_back(Date(8, October, 2027)); dates.push_back(Date(10, April, 2028)); dates.push_back(Date(9, October, 2028));
			dates.push_back(Date(9, April, 2029)); dates.push_back(Date(8, October, 2029)); dates.push_back(Date(8, April, 2030));
			dates.push_back(Date(8, October, 2030));

			dates.push_back(Date(8, April, 2031)); dates.push_back(Date(8, October, 2031)); dates.push_back(Date(8, April, 2032));
			dates.push_back(Date(8, October, 2032)); dates.push_back(Date(8, April, 2033)); dates.push_back(Date(10, October, 2033));
			dates.push_back(Date(10, April, 2034)); dates.push_back(Date(9, October, 2034)); dates.push_back(Date(9, April, 2035));
			dates.push_back(Date(8, October, 2035));


			dates.push_back(Date(8, April, 2036)); dates.push_back(Date(8, October, 2036)); dates.push_back(Date(8, April, 2037));
			dates.push_back(Date(8, October, 2037)); dates.push_back(Date(8, April, 2038)); dates.push_back(Date(10, October, 2038));
			dates.push_back(Date(8, April, 2039)); dates.push_back(Date(10, October, 2039)); dates.push_back(Date(9, April, 2040));
			dates.push_back(Date(8, October, 2040));

			dates.push_back(Date(8, April, 2041)); dates.push_back(Date(8, October, 2041)); dates.push_back(Date(8, April, 2042));
			dates.push_back(Date(8, October, 2042)); dates.push_back(Date(8, April, 2043)); dates.push_back(Date(8, October, 2043));
			dates.push_back(Date(8, April, 2044)); dates.push_back(Date(10, October, 2044)); dates.push_back(Date(10, April, 2045));
			dates.push_back(Date(9, October, 2045));

			dates.push_back(Date(9, April, 2046)); dates.push_back(Date(8, October, 2046)); dates.push_back(Date(8, April, 2047));
			dates.push_back(Date(8, October, 2047)); dates.push_back(Date(8, April, 2048)); dates.push_back(Date(8, October, 2048));
			dates.push_back(Date(8, April, 2049)); dates.push_back(Date(8, October, 2049)); dates.push_back(Date(8, April, 2050));
			dates.push_back(Date(10, October, 2050));

			dates.push_back(Date(10, April, 2051)); dates.push_back(Date(9, October, 2051)); dates.push_back(Date(8, April, 2052));
			dates.push_back(Date(8, October, 2052)); dates.push_back(Date(8, April, 2053)); dates.push_back(Date(8, October, 2053));
			dates.push_back(Date(8, April, 2054)); dates.push_back(Date(8, October, 2054)); dates.push_back(Date(8, April, 2055));
			dates.push_back(Date(8, October, 2055));

			dates.push_back(Date(10, April, 2056)); dates.push_back(Date(9, October, 2056)); dates.push_back(Date(9, April, 2057));
			dates.push_back(Date(8, October, 2057));	dates.push_back(Date(8, April, 2058)); dates.push_back(Date(8, October, 2058));
			dates.push_back(Date(8, April, 2059)); dates.push_back(Date(8, October, 2059)); dates.push_back(Date(8, April, 2060));
			dates.push_back(Date(8, October, 2060));

			dates.push_back(Date(8, April, 2061)); dates.push_back(Date(10, October, 2061)); dates.push_back(Date(10, April, 2062));
			dates.push_back(Date(9, October, 2062)); dates.push_back(Date(9, April, 2063));	dates.push_back(Date(8, October, 2063));
			dates.push_back(Date(8, April, 2064)); dates.push_back(Date(8, October, 2064));
#pragma endregion
#pragma region Vetor of Zero Rates
			std::vector <Rate> rates;
			//rates.push_back(0.0010443);
			rates.push_back(0.0010443);
			rates.push_back(0.0010362); rates.push_back(0.0010974); rates.push_back(0.0011738);
			rates.push_back(0.0012927); rates.push_back(0.0017803); rates.push_back(0.002284);
			rates.push_back(0.0026595); rates.push_back(0.0030738); rates.push_back(0.0032179);
			rates.push_back(0.0035759);
			rates.push_back(0.0041893);


			rates.push_back(0.0048391); rates.push_back(0.0054949); rates.push_back(0.0063631);
			rates.push_back(0.0072329); rates.push_back(0.008357); rates.push_back(0.009485);
			rates.push_back(0.0105954); rates.push_back(0.0117163); rates.push_back(0.012785);
			rates.push_back(0.013861);


			rates.push_back(0.0148198); rates.push_back(0.01579); rates.push_back(0.016666);
			rates.push_back(0.0175575); rates.push_back(0.0183587); rates.push_back(0.0191663);
			rates.push_back(0.0198951); rates.push_back(0.020634); rates.push_back(0.0212737);
			rates.push_back(0.0219226);

			rates.push_back(0.024871); rates.push_back(0.02354); rates.push_back(0.024029);
			rates.push_back(0.024439); rates.push_back(0.0248467); rates.push_back(0.025188);
			rates.push_back(0.0255331); rates.push_back(0.0257927); rates.push_back(0.026058);
			rates.push_back(0.0262584);

			rates.push_back(0.0264638); rates.push_back(0.0266057); rates.push_back(0.0267497);
			rates.push_back(0.0268551); rates.push_back(0.0268641); rates.push_back(0.0268515);
			rates.push_back(0.0267405); rates.push_back(0.0269932); rates.push_back(0.0272895);
			rates.push_back(0.027331);


			rates.push_back(0.02736848); rates.push_back(0.024064); rates.push_back(0.0274328);
			rates.push_back(0.027442); rates.push_back(0.0274546); rates.push_back(0.0274679);
			rates.push_back(0.0272456); rates.push_back(0.0270252); rates.push_back(0.0272445);
			rates.push_back(0.0274675);

			rates.push_back(0.0274549); rates.push_back(0.027443); rates.push_back(0.0274397);
			rates.push_back(0.0274369); rates.push_back(0.0272012); rates.push_back(0.0269648);
			rates.push_back(0.0270072); rates.push_back(0.0270501); rates.push_back(0.0270936);
			rates.push_back(0.0271378);

			rates.push_back(0.0271825); rates.push_back(0.027228); rates.push_back(0.072745);
			rates.push_back(0.0273209); rates.push_back(0.0273679); rates.push_back(0.074158);
			rates.push_back(0.0274386); rates.push_back(0.027622); rates.push_back(0.0274859);
			rates.push_back(0.0275101);

			rates.push_back(0.0275347); rates.push_back(0.027559); rates.push_back(0.0275854);
			rates.push_back(0.0276115); rates.push_back(0.0276376); rates.push_back(0.0276648);
			rates.push_back(0.0276857); rates.push_back(0.027707); rates.push_back(0.027729);
			rates.push_back(0.0277511);

			rates.push_back(0.0277736); rates.push_back(0.0277964); rates.push_back(0.0278196);
			rates.push_back(0.0278434);	rates.push_back(0.0278674); rates.push_back(0.0278919);
			rates.push_back(0.02792222); rates.push_back(0.0279528); rates.push_back(0.0279837);
			rates.push_back(0.0280156);

			rates.push_back(0.0280473); rates.push_back(0.0280795); rates.push_back(0.0281121);
			rates.push_back(0.0281451); rates.push_back(0.0281788);	rates.push_back(0.028219);
			rates.push_back(0.028219); rates.push_back(0.028219);
#pragma endregion

			termStructure.linkTo(boost::shared_ptr<YieldTermStructure>(
				new ZeroCurve(dates, rates, termStructureDayCounter)));
		}



		boost::shared_ptr<CapFloor>  TestCapletPricing(CapFloor::Type type,
			Rate strike, Integer Duration){

			const Size size = 2* Duration;
			boost::shared_ptr<LiborForwardModelProcess> process(
				new LiborForwardModelProcess(size, index));

			// set-up the model
			/*	const Real a = 0.02;
				const Real b = 0.12;
				const Real c = 0.1;
				const Real d = 0.01;*/

			const Real a = 0.025;
			const Real b = 0.12;
			const Real c = 0.1;
			const Real d = 0.01;

			boost::shared_ptr<LmVolatilityModel> volaModel(
				new LmLinearExponentialVolatilityModel(process->fixingTimes(), a, b, c, d));

			boost::shared_ptr<LmCorrelationModel> corrModel(
				new LmLinearExponentialCorrelationModel(size, 0.1, 0.1));

			boost::shared_ptr<LiborForwardModel> liborModel(
				new LiborForwardModel(process, volaModel, corrModel));

			boost::shared_ptr<AnalyticCapFloorEngine> LmEngine(
				new AnalyticCapFloorEngine(liborModel, termStructure));

			Leg flows = process->cashFlows();

			boost::shared_ptr<CapFloor> result = boost::shared_ptr<CapFloor>(
				new Cap(flows, std::vector<Rate>(size, strike)));
			result->setPricingEngine(LmEngine);

			return result;
		}

		boost::shared_ptr<Swaption>  TestSwaptionPricing(Rate swapRate, Real notional){

			const Size size = 20;
			const Size steps = 8 * size;
			boost::shared_ptr<LiborForwardModelProcess> process(
				new LiborForwardModelProcess(size, index));

			// set-up the model
			const Real a = 0.02;
			const Real b = 0.4;
			const Real c = 0.12;
			const Real d = 0.01;

			boost::shared_ptr<LmVolatilityModel> volaModel(
				new LmLinearExponentialVolatilityModel(process->fixingTimes(), a, b, c, d));

			boost::shared_ptr<LmCorrelationModel> corrModel(
				new LmLinearExponentialCorrelationModel(size, 0.1, 0.1));


			// set-up pricing engine
		/*	process->setCovarParam(boost::shared_ptr<LfmCovarianceParameterization>(
				new LfmCovarianceProxy(volaModel, corrModel)));
*/
			boost::shared_ptr<LiborForwardModel>
				liborModel(new LiborForwardModel(process, volaModel, corrModel));

			
			DayCounter dayCounter = termStructureDayCounter;
			BusinessDayConvention convention = index->businessDayConvention();
			Date settlement = index->forwardingTermStructure()->referenceDate();

			boost::shared_ptr<SwaptionVolatilityMatrix> m =	liborModel->getSwaptionVolatilityMatrix();
			
			Date MaturityDate = calendar.adjust(Date(8, Oct, 2018));
			Date SwapStart = calendar.advance(MaturityDate, 2, Days);
			//Date SwapMaturity = Date(8, Oct, 2021);
			Date SwapMaturity = calendar.adjust(calendar.advance(SwapStart, 3, Years));
			// For Amercian Style
			/*Date earliestDate = calendar.adjust(Date(8, Oct, 2014));
			Date AmericanStart = calendar.adjust(Date(10, Oct, 2014));
			Date AmericanEnd = calendar.adjust(calendar.advance(AmericanStart, 7, Years));
			SwapStart = AmericanStart;
			SwapMaturity = AmericanEnd;*/
			// End For Amercian Style
			
			Schedule fixedSchedule(SwapStart, SwapMaturity, Period(fixedFrequency),
				calendar, fixedConvention, fixedConvention,
				DateGeneration::Forward, false);

			Schedule floatSchedule(SwapStart, SwapMaturity,
				Period(floatingFrequency),
				calendar, floatingConvention,
				floatingConvention,
				DateGeneration::Forward, false);
			
			boost::shared_ptr<VanillaSwap> USwap(
				new VanillaSwap(VanillaSwap::Payer, notional,
				fixedSchedule, swapRate, fixedDayCount,
				floatSchedule, index, 0.0, dayCounter));
			USwap->setPricingEngine(boost::shared_ptr<PricingEngine>(
				new DiscountingSwapEngine(index->forwardingTermStructure())));

			boost::shared_ptr<PricingEngine> engine(
				new LfmSwaptionEngine(liborModel,
				index->forwardingTermStructure()));

			Settlement::Type settlementType = Settlement::Physical;
			boost::shared_ptr<Swaption> swaption(new Swaption(USwap,
				boost::shared_ptr<Exercise>(
				new EuropeanExercise(MaturityDate)),
				settlementType));
			/*boost::shared_ptr<Swaption> swaption(new Swaption(USwap,
				boost::shared_ptr<Exercise>(new AmericanExercise(earliestDate, AmericanEnd, false)),
				settlementType));*/

			swaption->setPricingEngine(engine);
			return swaption;
		}

	};

	}


void PrintResultCapFloor(Real strikes,  Real nominal, Date maturityDate, Date startDate, boost::shared_ptr<CapFloor> InstrumentEfficiency, CapFloor::Type InstrumentType, CapFloorLmm &capfloorEfficiecy){
	std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
	Size length = SchedulDates.size();
	std::cout << std::endl;
	std::cout << std::endl;
	Date CapMaturityDate;
	Date CapSartDate;
	Date endDate;

	DayCounter dc = Actual365Fixed();
	std::vector<boost::shared_ptr<CapFloor> > caplets;
	//startDate = InstrumentEfficiency->;

	startDate = InstrumentEfficiency->startDate();

	maturityDate=InstrumentEfficiency->maturityDate();
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "---------------------------------------------------|" << std::endl;
	std::cout << " Strike = " << io::rate(strikes) << std::endl;
	std::cout << " Notional = " << nominal << std::endl;
	std::cout << " Start Date = " << io::short_date(startDate) << std::endl;
	std::cout << " End Date = " << io::short_date(maturityDate) << std::endl;
	std::cout << " Option type = " << InstrumentType << "......";
	std::cout << " NPV = " << nominal*InstrumentEfficiency->NPV() << std::endl;
	
	std::cout << std::endl;
	std::cout << "----------------------------------------------------|" << std::endl;
	std::cout << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";

}
void PrintCashFlowSwapLMM(boost::shared_ptr<Swaption> InstrumentEfficiency){
	boost::shared_ptr< VanillaSwap > Underlying = InstrumentEfficiency->underlyingSwap();
	std::vector< boost::shared_ptr< CashFlow > > floatingleg = Underlying->floatingLeg();
	std::vector< boost::shared_ptr< CashFlow > > fixedleg = Underlying->fixedLeg();
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Date            |" << "Fixed Leg Cash FLow              |" << std::endl;
	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, fixedleg){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << " Date            |" << " Floating Leg Cash FLow              |" << std::endl;

	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, floatingleg){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}
void PrintResultSwaption(Real strikes, Real nominal, Date maturityDate, Date startDate, boost::shared_ptr<Swaption> InstrumentEfficiency, CapFloor::Type InstrumentType, CapFloorLmm &capfloorEfficiecy){

	boost::shared_ptr< VanillaSwap > Underlying = InstrumentEfficiency->underlyingSwap();
	boost::shared_ptr< IborIndex > index = Underlying->iborIndex();
	Schedule schedule = Underlying->floatingSchedule();
	std::vector <Date > SchedulDates = schedule.dates();
	
	Integer fixing = 2;
	Calendar  calendar = TARGET();
	std::cout << "Start              |" << "End              |" << "Fixing  |"
		<< "IDays        |" << "Index Rate|" << std::endl;

	BOOST_FOREACH(Date d, SchedulDates){
		// Date FixingDate = calendar.advance(d, -fixing ,Days);

		Date startDate = schedule.previousDate(d);
		if (startDate != Date()) {
			Date FixingDate = index->fixingDate(startDate);

			Date endDate = schedule.nextDate(d);
			Time Idays = daysBetween(startDate, endDate);
			Date d1 = d + 6 * Months;
			std::cout << io::short_date(startDate) << "       " <<
				io::short_date(endDate) << "        " <<
				io::short_date(FixingDate) << "       " <<
				Idays << "            " <<
				100 * index->fixing(FixingDate) << "         " << std::endl;
		}
	}

	Schedule fixedschedule = Underlying->fixedSchedule();
	std::vector <Date > fixedSchedulDates = fixedschedule.dates();

	
	std::cout << "Start              |" << "End              |" 
		<< "IDays        |" << " Rate|" << std::endl;

	BOOST_FOREACH(Date d, fixedSchedulDates){
		// Date FixingDate = calendar.advance(d, -fixing ,Days);

		Date startDate = fixedschedule.previousDate(d);
		if (startDate != Date()) {

			Date endDate = fixedschedule.nextDate(d);
			Time Idays = daysBetween(startDate, endDate);
			Date d1 = d + 6 * Months;
			std::cout << io::short_date(startDate) << "       " <<
				io::short_date(endDate) << "        " <<
				
				Idays << "            " << std::endl;
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "---------------------------------------------------|" << std::endl;
	std::cout << " Strike = " << io::rate(strikes) << std::endl;
	//std::cout << " Volatility = " << io::volatility(vol) <<std::endl;
	std::cout << " Notional = " << nominal << std::endl;
	std::cout << " Maturity Date = " << io::short_date(maturityDate) << std::endl;
	std::cout << std::endl;

	std::cout << " Fixed Leg Swap NPV = " << InstrumentEfficiency->underlyingSwap()->fixedLegNPV() << std::endl;
	std::cout << std::endl;
	std::cout << " Floating Leg Swap  NPV = " << InstrumentEfficiency->underlyingSwap()->floatingLegNPV() << std::endl;
	std::cout << std::endl;
	std::cout << " Swap::  " << InstrumentEfficiency->underlyingSwap()->type() << "   NPV :" << InstrumentEfficiency->underlyingSwap()->NPV() << std::endl;
	std::cout << " Swap Fair Rate =   " << InstrumentEfficiency->underlyingSwap()->fairRate() << std::endl;
	std::cout << std::endl;
	std::cout << " SWAPTION  NPV = " << InstrumentEfficiency->NPV() << std::endl;
	//Date ValuationDate = InstrumentEfficiency->valuationDate();
	//std::cout << " Valuation Date = " << io::short_date(ValuationDate) << std::endl;
	std::cout << "----------------------------------------------------|" << std::endl;
	std::cout << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";

}

void TestLmmCapFloor(){
	// Parmameters
	CapFloorLmm capfloorEfficiecy;
	Integer Duration = 7;
	Rate strikes = 0.02;
	Calendar calendar = TARGET();
	
	Date startDate = Date(8, Oct, 2014);
	Date maturiDate = Date(8, Oct, 2018);
	Real notional = 1000000;
	capfloorEfficiecy.nominals[0] = notional;
	std::string s;
	int Fin = 1;
	std::istringstream convert;
	CapFloor::Type InstrumentType = CapFloor::Cap;
	Volatility vol;

	while (Fin != 0) {
		Inputparmeters(InstrumentType, Duration, strikes, vol, notional);
		capfloorEfficiecy.nominals[0] = notional;
	/*	boost::shared_ptr<CapFloor> InstrumentEfficiency =
			capfloorEfficiecy.TestCapletPricing(InstrumentType, strikes, Duration);*/

		boost::shared_ptr<Swaption> InstrumentEfficiency =
			capfloorEfficiecy.TestSwaptionPricing(strikes, notional);
		//Print rseults
	//	PrintCashFlowSwapLMM(InstrumentEfficiency);
		PrintResultSwaption(strikes, capfloorEfficiecy.nominals[0], maturiDate, startDate, InstrumentEfficiency, InstrumentType, capfloorEfficiecy);
		
		std::cin >> s;
		try
		{
			Fin = boost::lexical_cast<int>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			Fin = 0;
		}
	}
}

