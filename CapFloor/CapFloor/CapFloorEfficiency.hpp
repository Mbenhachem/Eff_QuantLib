///* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
//
///*!
// Copyright (C) 2005, 2006, 2007, 2009 StatPro Italia srl
//
// This file is part of QuantLib, a free-software/open-source library
// for financial quantitative analysts and developers - http://quantlib.org/
//
// QuantLib is free software: you can redistribute it and/or modify it
// under the terms of the QuantLib license.  You should have received a
// copy of the license along with this program; if not, please email
// <quantlib-dev@lists.sf.net>. The license is also available online at
// <http://quantlib.org/license.shtml>.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the license for more details.
//*/
//
//// the only header you need to use QuantLib
//#include <ql/quantlib.hpp>
//
//#ifdef BOOST_MSVC
///* Uncomment the following lines to unmask floating-point
//   exceptions. Warning: unpredictable results can arise...
//
//   See http://www.wilmott.com/messageview.cfm?catid=10&threadid=9481
//   Is there anyone with a definitive word about this?
//*/
//// #include <float.h>
//// namespace { unsigned int u = _controlfp(_EM_INEXACT, _MCW_EM); }
//#endif
//
//
//
////#include "CapFloorEfficiency.hpp"
//#include <boost/foreach.hpp>
//#include <boost/timer.hpp>
//#include <iostream>
//#include <iomanip>
//#include "CurveData.hpp"
//#include "CapFloorVolatility.h"
//
//using namespace QuantLib;
//
//#ifdef BOOST_MSVC
//#  ifdef QL_ENABLE_THREAD_SAFE_OBSERVER_PATTERN
//#    include <ql/auto_link.hpp>
//#    define BOOST_LIB_NAME boost_system
//#    include <boost/config/auto_link.hpp>
//#    undef BOOST_LIB_NAME
//#    define BOOST_LIB_NAME boost_thread
//#    include <boost/config/auto_link.hpp>
//#    undef BOOST_LIB_NAME
//#  endif
//#endif
//
//
//#if defined(QL_ENABLE_SESSIONS)
//namespace QuantLib {
//
//    Integer sessionId() { return 0; }
//
//}
//#endif
//
//namespace {
//
//	struct CapFloorParmeters {
//		// common parameters
//		Date settlement;
//		std::vector<Real> nominals;
//		BusinessDayConvention convention;
//		Frequency frequency;
//		Integer Duration;
//		boost::shared_ptr<IborIndex> index;
//		Calendar calendar;
//		Natural fixingDays;
//		RelinkableHandle<YieldTermStructure> termStructure;
//		DayCounter termStructureDayCounter;
//		Schedule schedule;
//		Date today;
//		Handle<OptionletVolatilityStructure> caplets;
//		boost::shared_ptr<CapFloorTermVolSurface> capVolSurface;
//		//RelinkableHandle<OptionletVolatilityStructure> caplets;
//		Handle<OptionletVolatilityStructure> capletsVolatilies;
//		CapFloorVolatility cVol;
//		// Fill out with some sample market data
//
//		// Some initializations
//		CapFloorParmeters() : nominals(1, 100){
//			frequency = Semiannual;
//			index = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));
//			calendar = TARGET();
//			convention = ModifiedFollowing;
//			// Date today = calendar.adjust(Date::todaysDate());
//			today = Date(6, October, 2014);
//			Settings::instance().evaluationDate() = today;
//			Natural settlementDays = 2;
//			fixingDays = 2;
//			settlement = calendar.advance(Settings::instance().evaluationDate(), fixingDays, Days);
//			termStructureDayCounter = Actual360();
//
//
//			CurveData cData;
//			// Fill out with some sample market data
//			sampleMktData(cData);
//
//			// Build a curve linked to this market data
//			boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
//			termStructure.linkTo(ocurve);
//			cVol.termStructure.linkTo(ocurve);
//
//			Duration = 10;
//
//			Date startDate = calendar.adjust(Date::todaysDate());
//			Date endDate = calendar.advance(startDate, Duration*Years, convention);
//			schedule = Schedule(startDate, endDate, Period(frequency), calendar,
//				convention, convention,
//				DateGeneration::Forward, false);
//
//		}
//
//
//		// Leg defition
//		Leg DefinedLeg(const Date& startDate, Integer duration) {
//			Date endDate = calendar.advance(startDate, duration*Years, convention);
//			Schedule schedule(startDate, endDate, Period(frequency), calendar,
//				convention, convention,
//				DateGeneration::Forward, false);
//
//			return IborLeg(schedule, index)
//				.withNotionals(nominals)
//				.withPaymentDayCounter(Actual360())
//				.withPaymentAdjustment(convention)
//				.withFixingDays(0)
//				.inArrears(true);
//
//		}
//
//		// Pricing Engine defition
//		boost::shared_ptr<BlackCapFloorEngine> DefinedEngine() {
//
//			Date todaysDate = Date(8, Apr, 2015);
//
//			//capletsVolatilies = buildCurveOptionletAtmVol(cVol);
//			//capletsVolatilies = buildOptionletSurfaceVol(cVol);
//			capletsVolatilies = buildOptionletCurveVol(cVol);
//			//capletsVolatilies = buildFlatCurveVol(cVol);
//			//capVol->enableExtrapolation();
//			boost::shared_ptr<BlackCapFloorEngine> strippedVolEngine(new
//				BlackCapFloorEngine(termStructure, capletsVolatilies));
//
//#pragma region For Print 
//			std::vector<Period> optionT;
//			optionT.push_back(Period(6, Months));
//			optionT.push_back(Period(18, Months));
//			optionT.push_back(Period(42, Months));
//			optionT.push_back(Period(54, Months));
//			optionT.push_back(Period(66, Months));
//			optionT.push_back(Period(78, Months));
//
//
//			std::vector<Rate> strikesT;
//			strikesT.push_back(0.005);
//			strikesT.push_back(0.01);
//			strikesT.push_back(0.015);
//			strikesT.push_back(0.02);
//			strikesT.push_back(0.025);
//			strikesT.push_back(0.03);
//			strikesT.push_back(0.035);
//			strikesT.push_back(0.04);
//			strikesT.push_back(0.045);
//			strikesT.push_back(0.05);
//			strikesT.push_back(0.055);
//			strikesT.push_back(0.06);
//			strikesT.push_back(0.065);
//
//
//			Date fistDate(8, Apr, 2015);
//			Date secondDate(8, Apr, 2016);
//			Date thirdDate(9, Apr, 2018);
//			Date fourthDate(8, Apr, 2019);
//			Date fivethDate(8, Apr, 2020);
//			Date sixthDate(8, Apr, 2021);
//
//
//			/*std::cout << " Dates    |" << "smile section " << std::endl;
//			std::cout << "              " << io::rate(strikesT[0]) << "      " << io::rate(strikesT[1]) << "      " << io::rate(strikesT[2]) << "      "
//			<< io::rate(strikesT[3]) << "        " << io::rate(strikesT[4]) << "      " << io::rate(strikesT[5]) << "      "
//			<< io::rate(strikesT[6]) << "      " << io::rate(strikesT[7]) << "      " << io::rate(strikesT[8]) << "      "
//			<< io::rate(strikesT[9]) << "      " << io::rate(strikesT[10]) << "      " << io::rate(strikesT[11]) << "      "
//			<< io::rate(strikesT[12]) << "      " << std::endl;
//
//			std::cout << std::endl;
//			std::cout << std::endl;
//			boost::shared_ptr<SmileSection> smsection;
//			for (Size tenorIndex = 0; tenorIndex < optionT.size(); ++tenorIndex) {
//
//			Date asDate = calendar.advance(todaysDate, optionT[tenorIndex], ModifiedFollowing, false);
//			std::cout << io::short_date(asDate) << "     ";
//
//			for (Size strikeIndex = 0; strikeIndex < strikesT.size(); ++strikeIndex) {
//
//			smsection = capletsVolSurface->smileSection(asDate, true);
//			std::cout << io::rate(smsection->volatility(strikesT[strikeIndex])) << "      ";
//
//			}
//			std::cout << std::endl;
//			}*/
//
//			/*std::cout << std::endl;
//			std::cout << std::endl;*/
//
//			/*			std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(0).size() << std::endl;
//						std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(0).size() << std::endl;
//
//						std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(2).size() << std::endl;
//						std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(2).size() << std::endl;
//
//						std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(4).size() << std::endl;
//						std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(4).size() << std::endl;
//						std::cout << "  Date from Tenor " << " Atm   " << " optionlet vol   " << std::endl;*/
//
//			//for (Size i = 0; i < optionTenors.size(); ++i) {
//			//	std::vector<Volatility> optionLetVol = optionletStripperCurve->optionletVolatilities(i);
//			//	for (Size strikeIndex = 0; strikeIndex < optionLetVol.size(); ++strikeIndex) {
//			//		std::cout << io::short_date(capletFlatVolSurface->optionDateFromTenor(optionTenors[i])) << "          "
//			//			<< io::rate(atmVectVolCurve[strikeIndex]) << "          "
//			//			<< io::rate(optionLetVol[strikeIndex]) << std::endl;
//			//	}
//			//	std::cout << std::endl;
//			//	std::cout << std::endl;
//			//}
//			/*std::cout << std::endl;
//			std::cout << std::endl;
//			std::cout << "  Fixing Date " << " Payement Date     |" << " Atm   " << std::endl;
//
//			for (Size strikeIndex = 0; strikeIndex < optionletFixingDates.size(); ++strikeIndex) {
//			std::cout << io::short_date(optionletFixingDates[strikeIndex]) << "          "
//			<< io::short_date(optionletPaymentDates[strikeIndex]) << "          "
//			<< io::rate(atmVectVolCurve[strikeIndex]) << std::endl;
//			}
//
//			std::cout << std::endl;
//			std::cout << std::endl;*/
//			/* Pour afficher les volatilités */
//			//std::cout << " Dates    |" << "vol surface    |" << " vol curve " << std::endl;
//
//			//for (Size strikeIndex = 0; strikeIndex < strikesT.size(); ++strikeIndex) {
//			//	std::cout << std::endl;
//			//	std::cout << std::endl;
//			//	std::cout << " Strikes  " << strikesT[strikeIndex] << std::endl;
//
//			//	for (Size tenorIndex = 0; tenorIndex < optionT.size(); ++tenorIndex) {
//			//		Volatility strippedVol1 = capletsVolSurface->volatility(optionT[tenorIndex],
//			//			strikesT[strikeIndex], true);
//
//			//		Volatility strippedVol2 = capletsVolCurve->volatility(optionT[tenorIndex],
//			//			strikesT[strikeIndex], true);
//			//		
//			//		// vol from flat vol surface (for comparison only)
//			//		Volatility flatVol = flatTermVolSurface->volatility(optionT[tenorIndex],
//			//			strikesT[strikeIndex], true);
//
//			//		std::cout << io::short_date(capletsVolSurface->optionDateFromTenor(optionT[tenorIndex])) << "        "
//			//			<< io::rate(strippedVol1) << "          "
//			//				<< io::rate(strippedVol2) << "              " 
//			//				/*<<io::rate(volImply) */<< std::endl;
//			//		}
//			//	}
//
//#pragma endregion 
//
//
//
//
//			return strippedVolEngine;
//		}
//
//		//Cap floor defintion
//		boost::shared_ptr<CapFloor> DefinedCapFloor(CapFloor::Type type,
//			const Leg& leg,
//			Rate strike,
//			Volatility volatility) {
//			boost::shared_ptr<CapFloor> result;
//			switch (type) {
//			case CapFloor::Cap:
//				result = boost::shared_ptr<CapFloor>(
//					new Cap(leg, std::vector<Rate>(1, strike)));
//				break;
//			case CapFloor::Floor:
//				result = boost::shared_ptr<CapFloor>(
//					new Floor(leg, std::vector<Rate>(1, strike)));
//				break;
//			default:
//				QL_FAIL("unknown cap/floor type");
//			}
//			result->setPricingEngine(DefinedEngine());
//			return result;
//		}
//
//
//
//
//		std::string typeToString(CapFloor::Type type) {
//			switch (type) {
//			case CapFloor::Cap:
//				return "cap";
//			case CapFloor::Floor:
//				return "floor";
//			case CapFloor::Collar:
//				return "collar";
//			default:
//				QL_FAIL("unknown cap/floor type");
//			}
//		}
//
//	};
//
//	}
//
//	void Printvolatility(CapFloorParmeters &capfloorEfficiecy){
//
//		std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
//		Handle<OptionletVolatilityStructure> capletsVolatilies = capfloorEfficiecy.capletsVolatilies;
//		//Handle<YieldTermStructure> termStructure = capfloorEfficiecy.termStructure;
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << std::endl;
//		Calendar  calendar = TARGET();
//		std::vector <Rate> forwards;
//#pragma region Vetor of Dates
//
//
//
//		std::vector <Date > dates;
//		dates.push_back(Date(8, Oct, 2014));
//		dates.push_back(Date(8, Apr, 2015));
//		dates.push_back(Date(8, Oct, 2015));
//		dates.push_back(Date(8, Apr, 2016));
//		dates.push_back(Date(10, Oct, 2016));
//		dates.push_back(Date(10, Apr, 2017));
//		dates.push_back(Date(9, Oct, 2017));
//		dates.push_back(Date(9, Apr, 2018));
//		dates.push_back(Date(8, Oct, 2018));
//		dates.push_back(Date(8, Apr, 2019));
//		dates.push_back(Date(8, Oct, 2019));
//		dates.push_back(Date(8, Apr, 2020));
//		dates.push_back(Date(8, Oct, 2020));
//		dates.push_back(Date(8, Apr, 2021));
//		dates.push_back(Date(8, Oct, 2021));
//		dates.push_back(Date(8, Apr, 2022));
//		dates.push_back(Date(10, Oct, 2022));
//		dates.push_back(Date(10, Apr, 2023));
//		dates.push_back(Date(9, Oct, 2023));
//		dates.push_back(Date(8, Apr, 2024));
//
//		// Option Tenors
//		std::vector<Period> optionTenors;
//		optionTenors.push_back(Period(1, Years));
//		optionTenors.push_back(Period(2, Years));
//		optionTenors.push_back(Period(3, Years));
//		optionTenors.push_back(Period(4, Years));
//		optionTenors.push_back(Period(5, Years));
//		optionTenors.push_back(Period(6, Years));
//		optionTenors.push_back(Period(7, Years));
//		optionTenors.push_back(Period(8, Years));
//		optionTenors.push_back(Period(9, Years));
//		optionTenors.push_back(Period(10, Years));
//
//		std::vector<Period> optionT;
//		optionT.push_back(Period(18, Months));
//		optionT.push_back(Period(30, Months));
//		optionT.push_back(Period(42, Months));
//		optionT.push_back(Period(54, Months));
//		optionT.push_back(Period(66, Months));
//		optionT.push_back(Period(78, Months));
//		optionT.push_back(Period(90, Months));
//		optionT.push_back(Period(102, Months));
//		optionT.push_back(Period(114, Months));
//		optionT.push_back(Period(126, Months));
//
//		std::vector<Rate> strikes;
//		//Rate r = termStructure->forwardRate(optionTenors[0],)
//		strikes.push_back(0.51019);
//		strikes.push_back(0.6177);
//		strikes.push_back(0.79329);
//		strikes.push_back(1.02286);
//		strikes.push_back(1.24417);
//		strikes.push_back(1.45262);
//		strikes.push_back(1.63617);
//		strikes.push_back(1.8014);
//		strikes.push_back(1.94932);
//		strikes.push_back(2.08208);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//		strikes.push_back(0.01);
//#pragma endregion  
//		//CurveData cData;
//		//// Fill out with some sample market data
//		//sampleMktData(cData);
//		//boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
//		std::cout << " -------------------------VOLATILITY  -------------------------            |" << std::endl;
//		std::cout << std::endl;
//
//		std::cout << "Date              |" << "Volatility          |" << std::endl;
//		Date startDate(6, October, 2014);
//
//		Volatility vol;
//		Rate strike;
//		Size i = 0;
//		//BOOST_FOREACH(Date d, dates)
//		BOOST_FOREACH(Period d, optionTenors)
//		{
//			strike = strikes[i] / 100;
//			vol = capletsVolatilies->volatility(d, strike, true);
//			DayCounter dc = Actual365Fixed();
//			Integer fixing = 2;
//			/*Date FixingDate = calendar.advance(d, -fixing, Days);
//			Period P(Semiannual);
//			Time Idays = daysBetween(startDate, d);*/
//			//std::cout << io::short_date(d) << "            ";
//			std::cout << io::short_period(d) << "            ";
//			std::cout << std::setprecision(10) << io::volatility(vol) << "            ";
//			std::cout << std::endl;
//
//			++i;
//		}
//		std::string s;
//		std::cout << std::endl;
//		std::cout << "----------------------------------------------------|" << std::endl;
//		std::cout << std::endl;
//		std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
//		std::cin >> s;
//
//	}
//
//	void PrintCurve(CapFloorParmeters &capfloorEfficiecy){
//
//		std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
//
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << std::endl;
//		Calendar  calendar = TARGET();
//		std::vector <Rate> forwards;
//#pragma region Vetor of Dates
//		std::vector <Date > dates;
//		dates.push_back(Date(7, October, 2014));
//		dates.push_back(Date(15, October, 2014)); dates.push_back(Date(22, October, 2014)); dates.push_back(Date(29, October, 2014));
//		dates.push_back(Date(10, November, 2014)); dates.push_back(Date(8, December, 2014)); dates.push_back(Date(8, January, 2015));
//		dates.push_back(Date(9, Feb, 2015)); dates.push_back(Date(9, March, 2015)); dates.push_back(Date(8, April, 2015));
//		dates.push_back(Date(8, July, 2015));
//		dates.push_back(Date(8, October, 2015));
//
//
//		dates.push_back(Date(8, April, 2016)); dates.push_back(Date(10, October, 2016)); dates.push_back(Date(10, April, 2017));
//		dates.push_back(Date(9, October, 2017)); dates.push_back(Date(9, April, 2018)); dates.push_back(Date(8, October, 2018));
//		dates.push_back(Date(8, April, 2019)); dates.push_back(Date(8, October, 2019)); dates.push_back(Date(8, April, 2020));
//		dates.push_back(Date(8, October, 2020));
//
//
//		dates.push_back(Date(8, April, 2021)); dates.push_back(Date(8, October, 2021)); dates.push_back(Date(8, April, 2022));
//		dates.push_back(Date(10, October, 2022)); dates.push_back(Date(10, April, 2023)); dates.push_back(Date(9, October, 2023));
//		dates.push_back(Date(8, April, 2024)); dates.push_back(Date(8, October, 2024)); dates.push_back(Date(8, April, 2025));
//		dates.push_back(Date(8, October, 2025));
//
//		dates.push_back(Date(8, April, 2026)); dates.push_back(Date(8, October, 2026)); dates.push_back(Date(8, April, 2027));
//		dates.push_back(Date(8, October, 2027)); dates.push_back(Date(10, April, 2028)); dates.push_back(Date(9, October, 2028));
//		dates.push_back(Date(9, April, 2029)); dates.push_back(Date(8, October, 2029)); dates.push_back(Date(8, April, 2030));
//		dates.push_back(Date(8, October, 2030));
//
//		dates.push_back(Date(8, April, 2031)); dates.push_back(Date(8, October, 2031)); dates.push_back(Date(8, April, 2032));
//		dates.push_back(Date(8, October, 2032)); dates.push_back(Date(8, April, 2033)); dates.push_back(Date(10, October, 2033));
//		dates.push_back(Date(10, April, 2034)); dates.push_back(Date(9, October, 2034)); dates.push_back(Date(9, April, 2035));
//		dates.push_back(Date(8, October, 2035));
//
//
//		dates.push_back(Date(8, April, 2036)); dates.push_back(Date(8, October, 2036)); dates.push_back(Date(8, April, 2037));
//		dates.push_back(Date(8, October, 2037)); dates.push_back(Date(8, April, 2038)); dates.push_back(Date(10, October, 2038));
//		dates.push_back(Date(8, April, 2039)); dates.push_back(Date(10, October, 2039)); dates.push_back(Date(9, April, 2040));
//		dates.push_back(Date(8, October, 2040));
//
//		dates.push_back(Date(8, April, 2041)); dates.push_back(Date(8, October, 2041)); dates.push_back(Date(8, April, 2042));
//		dates.push_back(Date(8, October, 2042)); dates.push_back(Date(8, April, 2043)); dates.push_back(Date(8, October, 2043));
//		dates.push_back(Date(8, April, 2044)); dates.push_back(Date(10, October, 2044)); dates.push_back(Date(10, April, 2045));
//		dates.push_back(Date(9, October, 2045));
//
//		dates.push_back(Date(9, April, 2046)); dates.push_back(Date(8, October, 2046)); dates.push_back(Date(8, April, 2047));
//		dates.push_back(Date(8, October, 2047)); dates.push_back(Date(8, April, 2048)); dates.push_back(Date(8, October, 2048));
//		dates.push_back(Date(8, April, 2049)); dates.push_back(Date(8, October, 2049)); dates.push_back(Date(8, April, 2050));
//		dates.push_back(Date(10, October, 2050));
//
//		dates.push_back(Date(10, April, 2051)); dates.push_back(Date(9, October, 2051)); dates.push_back(Date(8, April, 2052));
//		dates.push_back(Date(8, October, 2052)); dates.push_back(Date(8, April, 2053)); dates.push_back(Date(8, October, 2053));
//		dates.push_back(Date(8, April, 2054)); dates.push_back(Date(8, October, 2054)); dates.push_back(Date(8, April, 2055));
//		dates.push_back(Date(8, October, 2055));
//
//		dates.push_back(Date(10, April, 2056)); dates.push_back(Date(9, October, 2056)); dates.push_back(Date(9, April, 2057));
//		dates.push_back(Date(8, October, 2057));	dates.push_back(Date(8, April, 2058)); dates.push_back(Date(8, October, 2058));
//		dates.push_back(Date(8, April, 2059)); dates.push_back(Date(8, October, 2059)); dates.push_back(Date(8, April, 2060));
//		dates.push_back(Date(8, October, 2060));
//
//		dates.push_back(Date(8, April, 2061)); dates.push_back(Date(10, October, 2061)); dates.push_back(Date(10, April, 2062));
//		dates.push_back(Date(9, October, 2062)); dates.push_back(Date(9, April, 2063));	dates.push_back(Date(8, October, 2063));
//		dates.push_back(Date(8, April, 2064)); dates.push_back(Date(8, October, 2064));
//
//#pragma endregion  
//		CurveData cData;
//		// Fill out with some sample market data
//		sampleMktData(cData);
//		boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
//		std::cout << " -------------------------MARKET DATA  -------------------------            |" << std::endl;
//		std::cout << std::endl;
//
//		std::cout << "Date              |" << "IDays          |" << "Zero Rate:        |" << "Discount Rate       |" << "Forward Rate        |" << std::endl;
//		Date startDate(6, October, 2014);
//		Date endD = Date(8, October, 2064);
//		BOOST_FOREACH(Date d, dates)
//		{
//			DayCounter dc = Actual365Fixed();
//			Integer fixing = 2;
//			Date FixingDate = calendar.advance(d, -fixing, Days);
//			Period P(Semiannual);
//			Time Idays = daysBetween(startDate, d);
//			Date d1 = d + 6 * Months;
//			std::cout << io::short_date(d) << "            ";
//			std::cout << Idays << "            ";/*
//
//			std::cout << std::setprecision(15) << 100 * capfloorEfficiecy.termStructure->zeroRate(d, dc, Continuous).rate() << "            ";
//			std::cout << std::setprecision(15)  << capfloorEfficiecy.termStructure->discount(d) << "            ";
//			std::cout << std::setprecision(15) << 100 * capfloorEfficiecy.termStructure->forwardRate(d, d1, dc, Simple).rate();*/
//			/*if (endD > d1)
//				std::cout << 100 * capfloorEfficiecy.termStructure->forwardRate(d, P, dc, Continuous, Semiannual).rate();
//				else*/
//			std::cout << std::setprecision(15) << 100 * ocurve->zeroRate(d, dc, Continuous).rate() << "            ";
//			std::cout << std::setprecision(15) << ocurve->discount(d) << "            ";
//			std::cout << std::setprecision(15) << 100 * ocurve->forwardRate(d, d1, dc, Continuous).rate();
//			std::cout << std::endl;
//
//
//		}
//		std::string s;
//		std::cout << std::endl;
//		std::cout << "----------------------------------------------------|" << std::endl;
//		std::cout << std::endl;
//		std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
//		std::cin >> s;
//
//	}
//
//	void PrintResults(Real strikes, Volatility vol, Real nominal, Date maturityDate, Date startDate, boost::shared_ptr<CapFloor> InstrumentEfficiency, CapFloor::Type InstrumentType, CapFloorParmeters &capfloorEfficiecy){
//		std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
//		Size length = SchedulDates.size();
//		std::cout << std::endl;
//		std::cout << std::endl;
//		Date CapMaturityDate;
//		Date CapSartDate;
//		Date endDate;
//
//		DayCounter dc = Actual365Fixed();
//		std::vector<boost::shared_ptr<CapFloor> > caplets;
//		/*if (InstrumentType == CapFloor::Cap) {
//			std::cout << "Start Dates    |" << "End Dates     |" << " Fixing Dates " << "              IDays " << "        NPV Caplet|" << "        Strike of Optionlet |" << "       Zero Rate " << "         Index Rate " << std::endl;
//
//			for (Size i = 0; i< length - 1; i++) {
//			caplets.push_back(InstrumentEfficiency->optionlet(i));
//			CapMaturityDate = caplets[i]->maturityDate();
//			CapSartDate = caplets[i]->startDate();
//			Time Idays = daysBetween(CapSartDate, CapMaturityDate);
//			caplets[i]->setPricingEngine(capfloorEfficiecy.DefinedEngine());
//			std::cout << io::short_date(CapSartDate) << "        "
//			<< io::short_date(CapMaturityDate) << "        "
//			<< io::short_date(capfloorEfficiecy.index->fixingDate(CapSartDate)) << "              "
//			<< Idays << "            "
//			<< caplets[i]->NPV() << "            "
//			<< io::rate(caplets[i]->capRates()[0]) << "          "
//			<< io::rate(capfloorEfficiecy.termStructure->zeroRate(CapMaturityDate, dc, Continuous).rate()) << "              " <<
//			io::rate(capfloorEfficiecy.index->fixing(CapSartDate)) << std::endl;
//			}
//			}
//			else if (InstrumentType == CapFloor::Floor){
//			std::cout << "Start Dates    |" << "End Dates     |" << " Fixing Dates " << "              IDays " << "        NPV Caplet|" << "        Strike of Optionlet |" << "      Zero Rate " << "         Index Rate " << std::endl;
//			for (Size i = 0; i< length - 1; i++) {
//			caplets.push_back(InstrumentEfficiency->optionlet(i));
//			CapMaturityDate = caplets[i]->maturityDate();
//			CapSartDate = caplets[i]->startDate();
//			Time Idays = daysBetween(CapSartDate, CapMaturityDate);
//			caplets[i]->setPricingEngine(capfloorEfficiecy.DefinedEngine());
//			std::cout << io::short_date(CapSartDate) << "        "
//			<< io::short_date(CapMaturityDate) << "        "
//			<< io::short_date(capfloorEfficiecy.index->fixingDate(CapSartDate)) << "              "
//			<< Idays << "            "
//			<< caplets[i]->NPV() << "            "
//			<< io::rate(caplets[i]->floorRates()[0]) << "          "
//			<< io::rate(capfloorEfficiecy.termStructure->zeroRate(CapMaturityDate, dc, Continuous).rate()) << "              " <<
//			io::rate(capfloorEfficiecy.index->fixing(CapSartDate)) << std::endl;
//			}
//			}
//			else{ QL_FAIL("unknown cap/floor type"); }*/
//
//		//Leg leg = InstrumentEfficiency->floatingLeg();
//		//boost::shared_ptr<FloatingRateCoupon> FloRateCoupon = InstrumentEfficiency->lastFloatingRateCoupon();
//		//	std::cout << "Dates              |" << "            "  << "Cash Flow     |" << std::endl; 
//		//BOOST_FOREACH(auto cashF, leg){
//		//	
//		//	Date d = cashF->date();
//		//	Real Amount = capfloorEfficiecy.termStructure->discount(d)*cashF->amount();
//		//	std::cout <<d   << "              "  << Amount << std::endl; 
//		//}
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << "---------------------------------------------------|" << std::endl;
//		std::cout << " Strike = " << io::rate(strikes) << std::endl;
//		//std::cout << " Volatility = " << io::volatility(vol) << std::endl;
//		std::cout << " Notional = " << nominal << std::endl;
//		std::cout << " Start Date = " << io::short_date(startDate) << std::endl;
//		std::cout << " End Date = " << io::short_date(maturityDate) << std::endl;
//		std::cout << " Option type = " << InstrumentType << "......";
//		std::cout << " NPV = " << InstrumentEfficiency->NPV() << std::endl;
//		//BlackScholesCalculator bsCalculator
//		std::cout << std::endl;
//		std::cout << "----------------------------------------------------|" << std::endl;
//		std::cout << std::endl;
//		std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
//
//	}
//	void Inputparmeters(CapFloor::Type & InstrumentType, Integer & lenghtInYears, Real & strikes, Real & vol, Real & notional){
//
//		std::string s;
//		std::string const cap = "1";
//		std::string const floor = "2";
//		std::cout << " Choisir un produit : 1: Cap, 2: Floor ";
//		std::cin >> s;
//		if (s == cap)
//			InstrumentType = CapFloor::Cap;
//		else if (s == floor)
//			InstrumentType = CapFloor::Floor;
//		else
//			QL_FAIL("unknown cap/floor produit");
//		std::cout << " Entrer la  duree en annee    ";
//		std::cin >> s;
//		try{ lenghtInYears = boost::lexical_cast<Integer>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			lenghtInYears = 0;
//		}
//		while (lenghtInYears <= 0) {
//			std::cout << " Entrer la  duree en annee  ";
//			std::cin >> s;
//			try
//			{
//				lenghtInYears = boost::lexical_cast<Integer>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				lenghtInYears = 0;
//			}
//		}
//		std::cout << " Entrer le  Strike  ";
//		std::cin >> s;
//		try{ strikes = boost::lexical_cast<Real>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			strikes = 0;
//		}
//
//		while (strikes == 0) {
//			std::cout << " Entrer le  Strike   ";
//			std::cin >> s;
//			try
//			{
//				strikes = boost::lexical_cast<Real>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				strikes = 0;
//			}
//		}
//
//		/*std::cout << " Entrer la volatilite   ";
//		std::cin >> s ;
//		try{vol = boost::lexical_cast<Real>(s);}
//		catch (boost::bad_lexical_cast const&)
//		{vol = -1;}
//		while (vol < 0) {
//		std::cout << " Entrer la  volatilite  ";
//		std::cin >> s;
//		try
//		{
//		vol = boost::lexical_cast<Real>(s);
//		}
//		catch (boost::bad_lexical_cast const&)
//		{
//		vol = -1;
//		}
//		}*/
//		std::cout << " Entrer le  nominal   ";
//		std::cin >> s;
//		try{ notional = boost::lexical_cast<Real>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			notional = 0;
//		}
//
//		while (notional == 0) {
//			std::cout << " Entrer le  nominal   ";
//			std::cin >> s;
//			try
//			{
//				notional = boost::lexical_cast<Real>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				notional = 0;
//			}
//		}
//		std::cout << std::endl;
//
//	}
//
//	boost::shared_ptr<VanillaSwap>  PricingSwap(Real nominal, Date today, Integer lenghtInYears, Rate fixedRate) {
//
//		CurveData cData;
//		// Fill out with some sample market data
//		sampleMktData(cData);
//		boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
//
//		RelinkableHandle<YieldTermStructure> discountingTermStructure;
//		// the one used for forward rate forecasting
//		RelinkableHandle<YieldTermStructure> forecastingTermStructure;
//
//		forecastingTermStructure.linkTo(ocurve);
//		discountingTermStructure.linkTo(ocurve);
//
//
//		Calendar  calendar = TARGET();
//
//		// fixed leg
//		Frequency fixedLegFrequency = Semiannual;
//		BusinessDayConvention fixedLegConvention = ModifiedFollowing;
//		BusinessDayConvention floatingLegConvention = ModifiedFollowing;
//		DayCounter fixedLegDayCounter = Actual365Fixed();
//		DayCounter floatingLegDayCounter = Actual360();
//
//		// floating leg
//		Frequency floatingLegFrequency = Semiannual;
//		boost::shared_ptr<IborIndex> euriborIndex(
//			new Euribor6M(forecastingTermStructure));
//		Spread spread = 0.0;
//
//		VanillaSwap::Type swapType = VanillaSwap::Payer;
//
//		Date maturity = today + lenghtInYears*Years;
//
//		Schedule fixedSchedule(today, maturity,
//			Period(fixedLegFrequency),
//			calendar, fixedLegConvention,
//			fixedLegConvention,
//			DateGeneration::ThirdWednesday, false);
//		Schedule floatSchedule(today, maturity,
//			Period(floatingLegFrequency),
//			calendar, floatingLegConvention,
//			floatingLegConvention,
//			DateGeneration::Forward, false, today);
//
//		boost::shared_ptr<VanillaSwap> swap(new VanillaSwap(swapType, nominal,
//			fixedSchedule, fixedRate, fixedLegDayCounter,
//			floatSchedule, euriborIndex, spread,
//			floatingLegDayCounter));
//
//		boost::shared_ptr<PricingEngine> swapEngine(
//			new DiscountingSwapEngine(discountingTermStructure));
//
//		swap->setPricingEngine(swapEngine);
//
//
//		return swap;
//	}
//
//
//	void PrintCashFlowSwap(boost::shared_ptr< VanillaSwap > mySwap){
//
//
//		Schedule schedule = mySwap->floatingSchedule();
//		std::vector <Date > SchedulDates = schedule.dates();
//
//		std::cout << "Start              |" << "End              |" << "Fixing  |"
//			<< "IDays        |" << "Index Rate|" << std::endl;
//
//		BOOST_FOREACH(Date d, SchedulDates){
//			Date startDate = schedule.previousDate(d);
//
//
//			if (startDate != Date()) {
//				Date FixingDate = mySwap->iborIndex()->fixingDate(startDate);
//				Date endDate = schedule.nextDate(d);
//				Time Idays = daysBetween(startDate, endDate);
//
//				//Date d1 = d + 6 * Months;
//				std::cout << io::short_date(startDate) << "       " <<
//					io::short_date(endDate) << "        " <<
//					io::short_date(FixingDate) << "       " <<
//					Idays << "            " <<
//					100 * mySwap->iborIndex()->fixing(FixingDate, true) << "         " << std::endl;
//
//			}
//		}
//		std::cout << std::endl;
//		std::cout << std::endl;
//		Schedule Fixschedule = mySwap->fixedSchedule();
//		std::vector <Date > FixedSchedulDates = Fixschedule.dates();
//
//		std::cout << "Start              |" << "End              |" << /*"Fixing  |"
//			<<*/ "IDays        |" << "Index Rate|" << std::endl;
//
//		BOOST_FOREACH(Date d, FixedSchedulDates){
//			Date startDate = Fixschedule.previousDate(d);
//
//
//			if (startDate != Date()) {
//				//Date FixingDate = Fixschedule.
//				Date endDate = Fixschedule.nextDate(d);
//				Time Idays = daysBetween(startDate, endDate);
//
//				//Date d1 = d + 6 * Months;
//				std::cout << io::short_date(startDate) << "       " <<
//					io::short_date(endDate) << "        " <<
//					//io::short_date(FixingDate) << "       " <<
//					Idays << "            " <<
//					/*100 * mySwap->iborIndex()->fixing(FixingDate, true) << "         " <<*/ std::endl;
//
//			}
//		}
//
//
//
//
//		std::vector< boost::shared_ptr< CashFlow > > floatingleg = mySwap->floatingLeg();
//		std::vector< boost::shared_ptr< CashFlow > > fixedleg = mySwap->fixedLeg();
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << "Date            |" << "Fixed Leg Cash FLow              |" << " Floating Leg Cash FLow              |" << std::endl;
//
//		BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, fixedleg){
//			std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << "                  " << std::endl;
//		}
//
//		std::cout << std::endl;
//		std::cout << " Date            |" << " Floating Leg Cash FLow              |" << std::endl;
//
//		BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, floatingleg){
//			std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;
//
//		}
//		std::cout << std::endl;
//		std::cout << std::endl;
//	}
//
//
//	void PrintCashFlowSwap(Swap mySwap){
//
//		std::cout << std::endl;
//		std::cout << std::endl;
//		std::cout << "Date            |" << "Fixed Leg Cash FLow              |" << " Floating Leg Cash FLow              |" << std::endl;
//
//		BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, mySwap.leg(0)){
//			std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << "                  " << std::endl;
//		}
//
//		std::cout << std::endl;
//		std::cout << " Date            |" << " Floating Leg Cash FLow              |" << std::endl;
//
//		BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, mySwap.leg(1)){
//			std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;
//
//		}
//		std::cout << std::endl;
//		std::cout << std::endl;
//	}
//
//
//	void InputSwapParmeters(Real & fixedRate, Integer & lenghtInYears, Real & notional){
//		std::string s;
//		std::cout << " Entrer le  taux fixe  ";
//		std::cin >> s;
//		try{ fixedRate = boost::lexical_cast<Real>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			fixedRate = 0;
//		}
//
//		while (fixedRate == 0) {
//			std::cout << " Entrer le  taux fixe   ";
//			std::cin >> s;
//			try
//			{
//				fixedRate = boost::lexical_cast<Real>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				fixedRate = 0;
//			}
//		}
//
//		std::cout << " Entrer la  duree en annee    ";
//		std::cin >> s;
//		try{ lenghtInYears = boost::lexical_cast<Integer>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			lenghtInYears = 0;
//		}
//		while (lenghtInYears <= 0) {
//			std::cout << " Entrer la  duree en annee  ";
//			std::cin >> s;
//			try
//			{
//				lenghtInYears = boost::lexical_cast<Integer>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				lenghtInYears = 0;
//			}
//		}
//		std::cout << " Entrer le  nominal   ";
//		std::cin >> s;
//		try{ notional = boost::lexical_cast<Real>(s); }
//		catch (boost::bad_lexical_cast const&)
//		{
//			notional = 0;
//		}
//
//		while (notional == 0) {
//			std::cout << " Entrer le  nominal   ";
//			std::cin >> s;
//			try
//			{
//				notional = boost::lexical_cast<Real>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				notional = 0;
//			}
//		}
//		std::cout << std::endl;
//	}
//
//	Swap swapPricing(Real nominal, Date today, Integer lenghtInYears, Rate fixedRate){
//		CapFloorParmeters vars;
//
//		Date maturity = today + lenghtInYears* Years;
//		Calendar calendar = TARGET();
//		Schedule schedule(today, maturity, Period(Semiannual), calendar,
//			ModifiedFollowing, ModifiedFollowing,
//			DateGeneration::Forward, false);
//		DayCounter dayCounter = Actual365Fixed();
//		std::vector<Real> nominals(2 * lenghtInYears, nominal);
//
//		CurveData cData;
//		// Fill out with some sample market data
//		sampleMktData(cData);
//		boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
//
//		RelinkableHandle<YieldTermStructure> discountingTermStructure;
//		// the one used for forward rate forecasting
//		RelinkableHandle<YieldTermStructure> forecastingTermStructure;
//
//		forecastingTermStructure.linkTo(ocurve);
//		discountingTermStructure.linkTo(ocurve);
//		boost::shared_ptr<IborIndex> index(new Euribor6M(forecastingTermStructure));
//
//		std::vector<Rate> coupons(2 * lenghtInYears, fixedRate);
//		/*for (Size i = 0; i < lenghtInYears; ++i){
//			coupons[i] = fixedRate;
//			}*/
//		coupons[0] = 0.015;
//		coupons[1] = 0.015;
//		coupons[2] = 0.016;
//		coupons[3] = 0.016;
//		coupons[4] = 0.018;
//		coupons[5] = 0.018;
//		coupons[6] = 0.02;
//		coupons[7] = 0.02;
//		coupons[8] = 0.025;
//		coupons[9] = 0.025;
//
//		Leg fixedLeg = FixedRateLeg(schedule)
//			.withNotionals(nominals)
//			.withCouponRates(coupons, dayCounter, Simple, Semiannual);
//
//		std::vector<Real> gearings;
//		std::vector<Rate> spreads;
//		Natural fixingDays = 2;
//
//
//
//		Leg floatingLeg = IborLeg(schedule, index)
//			.withNotionals(nominals)
//			.withPaymentDayCounter(Actual360());
//		//.withFixingDays(fixingDays);
//
//
//
//		Swap swap(fixedLeg, floatingLeg);
//		swap.setPricingEngine(boost::shared_ptr<PricingEngine>(
//			new DiscountingSwapEngine(discountingTermStructure)));
//		return swap;
//	}
//
//
//	void TestSwap(){
//
//		/* PARAMETERS */
//		Date today(10, October, 2014);
//		Real notional = 1000000.0;
//		Integer lenghtInYears = 10;
//		Rate fixedRate = 0.022125;
//
//
//		int Fin = 1;
//		std::string s;
//
//		while (Fin != 0) {
//			std::cout << " Parametres de pricing SWAP   " << std::endl;
//			std::cout << std::endl;
//
//			InputSwapParmeters(fixedRate, lenghtInYears, notional);
//			//boost::shared_ptr<VanillaSwap>  mySwap = PricingSwap(notional, today, lenghtInYears, fixedRate);
//			//std::cout << " --------------------- " << std::endl;
//			//std::cout << std::endl;
//			//std::cout << " NPV FixedLeg  = " << std::setprecision(10) << mySwap->fixedLegNPV() << std::endl;
//			//std::cout << " NPV FloatingLeg  = " << std::setprecision(10) << mySwap->floatingLegNPV() << std::endl;
//			//std::cout << " NPV  = " << std::setprecision(10) << mySwap->NPV() << std::endl;
//			//std::cout << " Fair Rate  = " << std::setprecision(10) << mySwap->fairRate() << std::endl;
//			//std::cout << std::endl;
//			//std::cout << " --------------------- " << std::endl;
//			//PrintCashFlowSwap(mySwap);
//
//			Swap testswap = swapPricing(notional, today, lenghtInYears, fixedRate);
//			std::cout << " --------------------- " << std::endl;
//			std::cout << std::endl;
//			std::cout << " NPV FixedLeg  = " << std::setprecision(10) << testswap.legNPV(0) << std::endl;
//			std::cout << " NPV FloatingLeg  = " << std::setprecision(10) << testswap.legNPV(1) << std::endl;
//			std::cout << " NPV  = " << std::setprecision(10) << testswap.NPV() << std::endl;
//			//std::cout << " Delta   = " << std::setprecision(10) << << std::endl;
//
//			std::cout << std::endl;
//			std::cout << " --------------------- " << std::endl;
//
//			std::cout << std::endl;
//			std::cout << " --------------------- " << std::endl;
//
//
//			PrintCashFlowSwap(testswap);
//			std::cout << std::endl;
//			std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
//			std::cin >> s;
//			try
//			{
//				Fin = boost::lexical_cast<int>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				Fin = 0;
//			}
//		}
//
//	}
//
//	void TestCapFloor(){
//		// Parmameters
//		CapFloorParmeters capfloorEfficiecy;
//		Integer Duration = 7;
//		Volatility vol = 0.1;
//		Rate strikes = 0.022;
//		Calendar calendar = TARGET();
//		//Date startDate = calendar.adjust(Date::todaysDate());
//		Date startDate = Date(15, Oct, 2014);
//		//Date startDate(10, October, 2014);
//
//		Real notional = 1000000;
//		capfloorEfficiecy.nominals[0] = notional;
//		std::string s;
//		int Fin = 1;
//		std::istringstream convert;
//		CapFloor::Type InstrumentType = CapFloor::Cap;
//		//PrintCurve( capfloorEfficiecy);
//
//		while (Fin != 0) {
//			//Inputparmeters(InstrumentType, Duration,strikes, vol, notional);
//			capfloorEfficiecy.nominals[0] = notional;
//
//			//Define Leg
//			Leg leg = capfloorEfficiecy.DefinedLeg(startDate, Duration);
//
//			////Define the product
//			boost::shared_ptr<CapFloor> InstrumentEfficiency =
//				capfloorEfficiecy.DefinedCapFloor(InstrumentType, leg, strikes, vol);
//
//
//			// Print rseults
//			PrintResults(strikes, vol, capfloorEfficiecy.nominals[0], InstrumentEfficiency->maturityDate(), startDate, InstrumentEfficiency, InstrumentType, capfloorEfficiecy);
//			Printvolatility(capfloorEfficiecy);
//			std::cin >> s;
//			try
//			{
//				Fin = boost::lexical_cast<int>(s);
//			}
//			catch (boost::bad_lexical_cast const&)
//			{
//				Fin = 0;
//			}
//		}
//	}
//



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
#include "CurveData.hpp"
#include "CapFloorVolatility.h"

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

	struct CapFloorParmeters {
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
		//RelinkableHandle<OptionletVolatilityStructure> caplets;
		Handle<OptionletVolatilityStructure> capletsVolatilies;
		CapFloorVolatility cVol;
		// Fill out with some sample market data

		// Some initializations
		CapFloorParmeters() : nominals(1, 100){
			frequency = Semiannual;
			index = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));
			calendar = TARGET();
			convention = ModifiedFollowing;
			// Date today = calendar.adjust(Date::todaysDate());
			today = Date(6, October, 2014);
			Settings::instance().evaluationDate() = today;
			Natural settlementDays = 2;
			fixingDays = 2;
			settlement = calendar.advance(Settings::instance().evaluationDate(), fixingDays, Days);
			termStructureDayCounter = Actual360();

			CurveData cData;
			// Fill out with some sample market data
			sampleMktData(cData);

			// Build a curve linked to this market data
			boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
			termStructure.linkTo(ocurve);
			cVol.termStructure.linkTo(ocurve);

			Duration = 10;

			Date startDate = calendar.adjust(Date::todaysDate());
			Date endDate = calendar.advance(startDate, Duration*Years, convention);
			schedule = Schedule(startDate, endDate, Period(frequency), calendar,
				convention, convention,
				DateGeneration::Forward, false);

		}


		// Leg defition
		Leg DefinedLeg(const Date& startDate, Integer duration) {
			Date endDate = calendar.advance(startDate, duration*Years, convention);
			Schedule schedule(startDate, endDate, Period(frequency), calendar,
				convention, convention,
				DateGeneration::Forward, false);
			return IborLeg(schedule, index)
				.withNotionals(nominals)
				.withPaymentDayCounter(Actual360())
				.withPaymentAdjustment(convention)
				.withFixingDays(fixingDays);
		}

		// Pricing Engine defition
		boost::shared_ptr<BlackCapFloorEngine> DefinedEngine() {

			Date todaysDate = Date(8, Apr, 2015);

			capletsVolatilies = buildCurveOptionletAtmVol(cVol);
			//capletsVolatilies = buildOptionletSurfaceVol(cVol);
			//capletsVolatilies = buildOptionletCurveVol(cVol);
			//capletsVolatilies = buildFlatCurveVol(cVol);
			//capVol->enableExtrapolation();
			boost::shared_ptr<BlackCapFloorEngine> strippedVolEngine(new
				BlackCapFloorEngine(termStructure, capletsVolatilies));

#pragma region For Print 
			std::vector<Period> optionT;
			optionT.push_back(Period(6, Months));
			optionT.push_back(Period(18, Months));
			optionT.push_back(Period(42, Months));
			optionT.push_back(Period(54, Months));
			optionT.push_back(Period(66, Months));
			optionT.push_back(Period(78, Months));


			std::vector<Rate> strikesT;
			strikesT.push_back(0.005);
			strikesT.push_back(0.01);
			strikesT.push_back(0.015);
			strikesT.push_back(0.02);
			strikesT.push_back(0.025);
			strikesT.push_back(0.03);
			strikesT.push_back(0.035);
			strikesT.push_back(0.04);
			strikesT.push_back(0.045);
			strikesT.push_back(0.05);
			strikesT.push_back(0.055);
			strikesT.push_back(0.06);
			strikesT.push_back(0.065);


			Date fistDate(8, Apr, 2015);
			Date secondDate(8, Apr, 2016);
			Date thirdDate(9, Apr, 2018);
			Date fourthDate(8, Apr, 2019);
			Date fivethDate(8, Apr, 2020);
			Date sixthDate(8, Apr, 2021);


			/*std::cout << " Dates    |" << "smile section " << std::endl;
			std::cout << "              " << io::rate(strikesT[0]) << "      " << io::rate(strikesT[1]) << "      " << io::rate(strikesT[2]) << "      "
			<< io::rate(strikesT[3]) << "        " << io::rate(strikesT[4]) << "      " << io::rate(strikesT[5]) << "      "
			<< io::rate(strikesT[6]) << "      " << io::rate(strikesT[7]) << "      " << io::rate(strikesT[8]) << "      "
			<< io::rate(strikesT[9]) << "      " << io::rate(strikesT[10]) << "      " << io::rate(strikesT[11]) << "      "
			<< io::rate(strikesT[12]) << "      " << std::endl;

			std::cout << std::endl;
			std::cout << std::endl;
			boost::shared_ptr<SmileSection> smsection;
			for (Size tenorIndex = 0; tenorIndex < optionT.size(); ++tenorIndex) {

			Date asDate = calendar.advance(todaysDate, optionT[tenorIndex], ModifiedFollowing, false);
			std::cout << io::short_date(asDate) << "     ";

			for (Size strikeIndex = 0; strikeIndex < strikesT.size(); ++strikeIndex) {

			smsection = capletsVolSurface->smileSection(asDate, true);
			std::cout << io::rate(smsection->volatility(strikesT[strikeIndex])) << "      ";

			}
			std::cout << std::endl;
			}*/

			/*std::cout << std::endl;
			std::cout << std::endl;*/

			/*			std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(0).size() << std::endl;
			std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(0).size() << std::endl;

			std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(2).size() << std::endl;
			std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(2).size() << std::endl;

			std::cout << "  Strike " << optionletStripperCurve->optionletStrikes(4).size() << std::endl;
			std::cout << "  vol  " << optionletStripperCurve->optionletVolatilities(4).size() << std::endl;
			std::cout << "  Date from Tenor " << " Atm   " << " optionlet vol   " << std::endl;*/

			//for (Size i = 0; i < optionTenors.size(); ++i) {
			//	std::vector<Volatility> optionLetVol = optionletStripperCurve->optionletVolatilities(i);
			//	for (Size strikeIndex = 0; strikeIndex < optionLetVol.size(); ++strikeIndex) {
			//		std::cout << io::short_date(capletFlatVolSurface->optionDateFromTenor(optionTenors[i])) << "          "
			//			<< io::rate(atmVectVolCurve[strikeIndex]) << "          "
			//			<< io::rate(optionLetVol[strikeIndex]) << std::endl;
			//	}
			//	std::cout << std::endl;
			//	std::cout << std::endl;
			//}
			/*std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "  Fixing Date " << " Payement Date     |" << " Atm   " << std::endl;

			for (Size strikeIndex = 0; strikeIndex < optionletFixingDates.size(); ++strikeIndex) {
			std::cout << io::short_date(optionletFixingDates[strikeIndex]) << "          "
			<< io::short_date(optionletPaymentDates[strikeIndex]) << "          "
			<< io::rate(atmVectVolCurve[strikeIndex]) << std::endl;
			}

			std::cout << std::endl;
			std::cout << std::endl;*/
			/* Pour afficher les volatilités */
			//std::cout << " Dates    |" << "vol surface    |" << " vol curve " << std::endl;

			//for (Size strikeIndex = 0; strikeIndex < strikesT.size(); ++strikeIndex) {
			//	std::cout << std::endl;
			//	std::cout << std::endl;
			//	std::cout << " Strikes  " << strikesT[strikeIndex] << std::endl;

			//	for (Size tenorIndex = 0; tenorIndex < optionT.size(); ++tenorIndex) {
			//		Volatility strippedVol1 = capletsVolSurface->volatility(optionT[tenorIndex],
			//			strikesT[strikeIndex], true);

			//		Volatility strippedVol2 = capletsVolCurve->volatility(optionT[tenorIndex],
			//			strikesT[strikeIndex], true);
			//		
			//		// vol from flat vol surface (for comparison only)
			//		Volatility flatVol = flatTermVolSurface->volatility(optionT[tenorIndex],
			//			strikesT[strikeIndex], true);

			//		std::cout << io::short_date(capletsVolSurface->optionDateFromTenor(optionT[tenorIndex])) << "        "
			//			<< io::rate(strippedVol1) << "          "
			//				<< io::rate(strippedVol2) << "              " 
			//				/*<<io::rate(volImply) */<< std::endl;
			//		}
			//	}

#pragma endregion 


			return strippedVolEngine;
		}

		Handle<OptionletVolatilityStructure>
			makeCapFloorVolatility(const Date& refdate) {

			Volatility vols[] = { 15.0,
				16.13064, 16.13064,
				17.4428, 17.4428, 17.4428, 17.4428,
				17.48913, 16.534, 16.534 };

			std::vector<Date> dates;
			dates.push_back(Date(2, Apr, 2015));

			dates.push_back(Date(6, Oct, 2015));
			dates.push_back(Date(6, Apr, 2016));

			dates.push_back(Date(6, Oct, 2016));
			dates.push_back(Date(6, Apr, 2017));
			dates.push_back(Date(5, Oct, 2017));
			dates.push_back(Date(5, Apr, 2018));

			dates.push_back(Date(4, Oct, 2018));
			dates.push_back(Date(4, Apr, 2019));
			dates.push_back(Date(4, Oct, 2019));

			/*Volatility vols[] = { 15.0,
			16.13064, 17.4428,
			17.48913, 16.5633};

			std::vector<Date> dates;
			dates.push_back(Date(8, Apr, 2015));
			dates.push_back(Date(6, Apr, 2016));

			dates.push_back(Date(9, Apr, 2018));
			dates.push_back(Date(8, Apr, 2019));
			dates.push_back(Date(8, Apr, 2020));
			*/
			std::vector<Volatility> capletVols;
			for (Size i = 0; i <dates.size(); ++i) {
				capletVols.push_back(vols[i] / 100);
			}
			return Handle<OptionletVolatilityStructure>(
				boost::shared_ptr<OptionletVolatilityStructure>((new CapletVarianceCurve(refdate, dates,
				capletVols, Actual360()))));
		}

		//Cap floor defintion
		boost::shared_ptr<CapFloor> DefinedCapFloor(CapFloor::Type type,
			const Leg& leg,
			Rate strike,
			Volatility volatility) {
			boost::shared_ptr<CapFloor> result;
			switch (type) {
			case CapFloor::Cap:
				result = boost::shared_ptr<CapFloor>(
					new Cap(leg, std::vector<Rate>(1, strike)));
				break;
			case CapFloor::Floor:
				result = boost::shared_ptr<CapFloor>(
					new Floor(leg, std::vector<Rate>(1, strike)));
				break;
			default:
				QL_FAIL("unknown cap/floor type");
			}
			result->setPricingEngine(DefinedEngine());
			return result;
		}

	};

	std::string typeToString(CapFloor::Type type) {
		switch (type) {
		case CapFloor::Cap:
			return "cap";
		case CapFloor::Floor:
			return "floor";
		case CapFloor::Collar:
			return "collar";
		default:
			QL_FAIL("unknown cap/floor type");
		}
	}

}

void Printvolatility(CapFloorParmeters &capfloorEfficiecy){

	std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
	Handle<OptionletVolatilityStructure> capletsVolatilies = capfloorEfficiecy.capletsVolatilies;
	//Handle<YieldTermStructure> termStructure = capfloorEfficiecy.termStructure;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	Calendar  calendar = TARGET();
	std::vector <Rate> forwards;
#pragma region Vetor of Dates



	std::vector <Date > dates;
	dates.push_back(Date(8, Oct, 2014));
	dates.push_back(Date(8, Apr, 2015));
	dates.push_back(Date(8, Oct, 2015));
	dates.push_back(Date(8, Apr, 2016));
	dates.push_back(Date(10, Oct, 2016));
	dates.push_back(Date(10, Apr, 2017));
	dates.push_back(Date(9, Oct, 2017));
	dates.push_back(Date(9, Apr, 2018));
	dates.push_back(Date(8, Oct, 2018));
	dates.push_back(Date(8, Apr, 2019));
	dates.push_back(Date(8, Oct, 2019));
	dates.push_back(Date(8, Apr, 2020));
	dates.push_back(Date(8, Oct, 2020));
	dates.push_back(Date(8, Apr, 2021));
	dates.push_back(Date(8, Oct, 2021));
	dates.push_back(Date(8, Apr, 2022));
	dates.push_back(Date(10, Oct, 2022));
	dates.push_back(Date(10, Apr, 2023));
	dates.push_back(Date(9, Oct, 2023));
	dates.push_back(Date(8, Apr, 2024));

	// Option Tenors
	std::vector<Period> optionTenors;
	optionTenors.push_back(Period(1, Years));
	optionTenors.push_back(Period(2, Years));
	optionTenors.push_back(Period(3, Years));
	optionTenors.push_back(Period(4, Years));
	optionTenors.push_back(Period(5, Years));
	optionTenors.push_back(Period(6, Years));
	optionTenors.push_back(Period(7, Years));
	optionTenors.push_back(Period(8, Years));
	optionTenors.push_back(Period(9, Years));
	optionTenors.push_back(Period(10, Years));

	std::vector<Period> optionT;
	optionT.push_back(Period(18, Months));
	optionT.push_back(Period(30, Months));
	optionT.push_back(Period(42, Months));
	optionT.push_back(Period(54, Months));
	optionT.push_back(Period(66, Months));
	optionT.push_back(Period(78, Months));
	optionT.push_back(Period(90, Months));
	optionT.push_back(Period(102, Months));
	optionT.push_back(Period(114, Months));
	optionT.push_back(Period(126, Months));

	std::vector<Rate> strikes;
	//Rate r = termStructure->forwardRate(optionTenors[0],)
	strikes.push_back(0.51019);
	strikes.push_back(0.6177);
	strikes.push_back(0.79329);
	strikes.push_back(1.02286);
	strikes.push_back(1.24417);
	strikes.push_back(1.45262);
	strikes.push_back(1.63617);
	strikes.push_back(1.8014);
	strikes.push_back(1.94932);
	strikes.push_back(2.08208);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
	strikes.push_back(0.01);
#pragma endregion  
	//CurveData cData;
	//// Fill out with some sample market data
	//sampleMktData(cData);
	//boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
	std::cout << " -------------------------VOLATILITY  -------------------------            |" << std::endl;
	std::cout << std::endl;

	std::cout << "Date              |" << "Volatility          |" << std::endl;
	Date startDate(6, October, 2014);

	Volatility vol;
	Rate strike;
	Size i = 0;
	//BOOST_FOREACH(Date d, dates)
	BOOST_FOREACH(Period d, optionTenors)
	{
		strike = strikes[i] / 100;
		vol = capletsVolatilies->volatility(d, strike, true);
		DayCounter dc = Actual365Fixed();
		Integer fixing = 2;
		/*Date FixingDate = calendar.advance(d, -fixing, Days);
		Period P(Semiannual);
		Time Idays = daysBetween(startDate, d);*/
		//std::cout << io::short_date(d) << "            ";
		std::cout << io::short_period(d) << "            ";
		std::cout << std::setprecision(10) << io::volatility(vol) << "            ";
		std::cout << std::endl;

		++i;
	}
	std::string s;
	std::cout << std::endl;
	std::cout << "----------------------------------------------------|" << std::endl;
	std::cout << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
	std::cin >> s;

}

void PrintCurve(CapFloorParmeters &capfloorEfficiecy){

	std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	Calendar  calendar = TARGET();
	std::vector <Rate> forwards;
#pragma region Vetor of Dates
	std::vector <Date > dates;
	dates.push_back(Date(7, October, 2014));
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
	CurveData cData;
	// Fill out with some sample market data
	sampleMktData(cData);
	boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);
	std::cout << " -------------------------MARKET DATA  -------------------------            |" << std::endl;
	std::cout << std::endl;

	std::cout << "Date              |" << "IDays          |" << "Zero Rate:        |" << "Discount Rate       |" << "Forward Rate        |" << std::endl;
	Date startDate(6, October, 2014);
	Date endD = Date(8, October, 2064);
	BOOST_FOREACH(Date d, dates)
	{
		DayCounter dc = Actual365Fixed();
		Integer fixing = 2;
		Date FixingDate = calendar.advance(d, -fixing, Days);
		Period P(Semiannual);
		Time Idays = daysBetween(startDate, d);
		Date d1 = d + 6 * Months;
		std::cout << io::short_date(d) << "            ";
		std::cout << Idays << "            ";/*

											 std::cout << std::setprecision(15) << 100 * capfloorEfficiecy.termStructure->zeroRate(d, dc, Continuous).rate() << "            ";
											 std::cout << std::setprecision(15)  << capfloorEfficiecy.termStructure->discount(d) << "            ";
											 std::cout << std::setprecision(15) << 100 * capfloorEfficiecy.termStructure->forwardRate(d, d1, dc, Simple).rate();*/
		/*if (endD > d1)
		std::cout << 100 * capfloorEfficiecy.termStructure->forwardRate(d, P, dc, Continuous, Semiannual).rate();
		else*/
		std::cout << std::setprecision(15) << 100 * ocurve->zeroRate(d, dc, Continuous).rate() << "            ";
		std::cout << std::setprecision(15) << ocurve->discount(d) << "            ";
		std::cout << std::setprecision(15) << 100 * ocurve->forwardRate(d, d1, dc, Continuous).rate();
		std::cout << std::endl;


	}
	std::string s;
	std::cout << std::endl;
	std::cout << "----------------------------------------------------|" << std::endl;
	std::cout << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
	std::cin >> s;

}

void PrintResults(Real strikes, Volatility vol, Real nominal, Date maturityDate, Date startDate, boost::shared_ptr<CapFloor> InstrumentEfficiency, CapFloor::Type InstrumentType, CapFloorParmeters &capfloorEfficiecy){
	std::vector <Date > SchedulDates = capfloorEfficiecy.schedule.dates();
	Size length = SchedulDates.size();
	std::cout << std::endl;
	std::cout << std::endl;
	Date CapMaturityDate;
	Date CapSartDate;
	Date endDate;

	DayCounter dc = Actual365Fixed();
	std::vector<boost::shared_ptr<CapFloor> > caplets;
	/*if (InstrumentType == CapFloor::Cap) {
	std::cout << "Start Dates    |" << "End Dates     |" << " Fixing Dates " << "              IDays " << "        NPV Caplet|" << "        Strike of Optionlet |" << "       Zero Rate " << "         Index Rate " << std::endl;

	for (Size i = 0; i< length - 1; i++) {
	caplets.push_back(InstrumentEfficiency->optionlet(i));
	CapMaturityDate = caplets[i]->maturityDate();
	CapSartDate = caplets[i]->startDate();
	Time Idays = daysBetween(CapSartDate, CapMaturityDate);
	caplets[i]->setPricingEngine(capfloorEfficiecy.DefinedEngine());
	std::cout << io::short_date(CapSartDate) << "        "
	<< io::short_date(CapMaturityDate) << "        "
	<< io::short_date(capfloorEfficiecy.index->fixingDate(CapSartDate)) << "              "
	<< Idays << "            "
	<< caplets[i]->NPV() << "            "
	<< io::rate(caplets[i]->capRates()[0]) << "          "
	<< io::rate(capfloorEfficiecy.termStructure->zeroRate(CapMaturityDate, dc, Continuous).rate()) << "              " <<
	io::rate(capfloorEfficiecy.index->fixing(CapSartDate)) << std::endl;
	}
	}
	else if (InstrumentType == CapFloor::Floor){
	std::cout << "Start Dates    |" << "End Dates     |" << " Fixing Dates " << "              IDays " << "        NPV Caplet|" << "        Strike of Optionlet |" << "      Zero Rate " << "         Index Rate " << std::endl;
	for (Size i = 0; i< length - 1; i++) {
	caplets.push_back(InstrumentEfficiency->optionlet(i));
	CapMaturityDate = caplets[i]->maturityDate();
	CapSartDate = caplets[i]->startDate();
	Time Idays = daysBetween(CapSartDate, CapMaturityDate);
	caplets[i]->setPricingEngine(capfloorEfficiecy.DefinedEngine());
	std::cout << io::short_date(CapSartDate) << "        "
	<< io::short_date(CapMaturityDate) << "        "
	<< io::short_date(capfloorEfficiecy.index->fixingDate(CapSartDate)) << "              "
	<< Idays << "            "
	<< caplets[i]->NPV() << "            "
	<< io::rate(caplets[i]->floorRates()[0]) << "          "
	<< io::rate(capfloorEfficiecy.termStructure->zeroRate(CapMaturityDate, dc, Continuous).rate()) << "              " <<
	io::rate(capfloorEfficiecy.index->fixing(CapSartDate)) << std::endl;
	}
	}
	else{ QL_FAIL("unknown cap/floor type"); }*/

	//Leg leg = InstrumentEfficiency->floatingLeg();
	//boost::shared_ptr<FloatingRateCoupon> FloRateCoupon = InstrumentEfficiency->lastFloatingRateCoupon();
	//	std::cout << "Dates              |" << "            "  << "Cash Flow     |" << std::endl; 
	//BOOST_FOREACH(auto cashF, leg){
	//	
	//	Date d = cashF->date();
	//	Real Amount = capfloorEfficiecy.termStructure->discount(d)*cashF->amount();
	//	std::cout <<d   << "              "  << Amount << std::endl; 
	//}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "---------------------------------------------------|" << std::endl;
	std::cout << " Strike = " << io::rate(strikes) << std::endl;
	//std::cout << " Volatility = " << io::volatility(vol) << std::endl;
	std::cout << " Notional = " << nominal << std::endl;
	std::cout << " Start Date = " << io::short_date(startDate) << std::endl;
	std::cout << " End Date = " << io::short_date(maturityDate) << std::endl;
	std::cout << " Option type = " << InstrumentType << "......";
	std::cout << " NPV = " << InstrumentEfficiency->NPV() << std::endl;
	//BlackScholesCalculator bsCalculator
	std::cout << std::endl;
	std::cout << "----------------------------------------------------|" << std::endl;
	std::cout << std::endl;
	std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";

}
void Inputparmeters(CapFloor::Type & InstrumentType, Integer & lenghtInYears, Real & strikes, Real & vol, Real & notional){

	std::string s;
	std::string const cap = "1";
	std::string const floor = "2";
	std::cout << " Choisir un produit : 1: Cap, 2: Floor ";
	std::cin >> s;
	if (s == cap)
		InstrumentType = CapFloor::Cap;
	else if (s == floor)
		InstrumentType = CapFloor::Floor;
	else
		QL_FAIL("unknown cap/floor produit");
	std::cout << " Entrer la  duree en annee    ";
	std::cin >> s;
	try{ lenghtInYears = boost::lexical_cast<Integer>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		lenghtInYears = 0;
	}
	while (lenghtInYears <= 0) {
		std::cout << " Entrer la  duree en annee  ";
		std::cin >> s;
		try
		{
			lenghtInYears = boost::lexical_cast<Integer>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			lenghtInYears = 0;
		}
	}
	std::cout << " Entrer le  Strike  ";
	std::cin >> s;
	try{ strikes = boost::lexical_cast<Real>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		strikes = 0;
	}

	while (strikes == 0) {
		std::cout << " Entrer le  Strike   ";
		std::cin >> s;
		try
		{
			strikes = boost::lexical_cast<Real>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			strikes = 0;
		}
	}

	/*std::cout << " Entrer la volatilite   ";
	std::cin >> s ;
	try{vol = boost::lexical_cast<Real>(s);}
	catch (boost::bad_lexical_cast const&)
	{vol = -1;}
	while (vol < 0) {
	std::cout << " Entrer la  volatilite  ";
	std::cin >> s;
	try
	{
	vol = boost::lexical_cast<Real>(s);
	}
	catch (boost::bad_lexical_cast const&)
	{
	vol = -1;
	}
	}*/
	std::cout << " Entrer le  nominal   ";
	std::cin >> s;
	try{ notional = boost::lexical_cast<Real>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		notional = 0;
	}

	while (notional == 0) {
		std::cout << " Entrer le  nominal   ";
		std::cin >> s;
		try
		{
			notional = boost::lexical_cast<Real>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			notional = 0;
		}
	}
	std::cout << std::endl;

}

boost::shared_ptr<VanillaSwap>  PricingSwap(Real nominal, Date today, Integer lenghtInYears, Rate fixedRate) {

	CurveData cData;
	// Fill out with some sample market data
	sampleMktData(cData);
	boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);

	RelinkableHandle<YieldTermStructure> discountingTermStructure;
	// the one used for forward rate forecasting
	RelinkableHandle<YieldTermStructure> forecastingTermStructure;

	forecastingTermStructure.linkTo(ocurve);
	discountingTermStructure.linkTo(ocurve);


	Calendar  calendar = TARGET();

	// fixed leg
	Frequency fixedLegFrequency = Semiannual;
	BusinessDayConvention fixedLegConvention = ModifiedFollowing;
	BusinessDayConvention floatingLegConvention = ModifiedFollowing;
	DayCounter fixedLegDayCounter = Actual365Fixed();
	DayCounter floatingLegDayCounter = Actual360();

	// floating leg
	Frequency floatingLegFrequency = Semiannual;
	boost::shared_ptr<IborIndex> euriborIndex(
		new Euribor6M(forecastingTermStructure));
	Spread spread = 0.0;

	VanillaSwap::Type swapType = VanillaSwap::Payer;

	Date maturity = today + lenghtInYears*Years;

	Schedule fixedSchedule(today, maturity,
		Period(fixedLegFrequency),
		calendar, fixedLegConvention,
		fixedLegConvention,
		DateGeneration::ThirdWednesday, false);
	Schedule floatSchedule(today, maturity,
		Period(floatingLegFrequency),
		calendar, floatingLegConvention,
		floatingLegConvention,
		DateGeneration::Forward, false, today);

	boost::shared_ptr<VanillaSwap> swap(new VanillaSwap(swapType, nominal,
		fixedSchedule, fixedRate, fixedLegDayCounter,
		floatSchedule, euriborIndex, spread,
		floatingLegDayCounter));

	boost::shared_ptr<PricingEngine> swapEngine(
		new DiscountingSwapEngine(discountingTermStructure));

	swap->setPricingEngine(swapEngine);


	return swap;
}


void PrintCashFlowSwap(boost::shared_ptr< VanillaSwap > mySwap){


	Schedule schedule = mySwap->floatingSchedule();
	std::vector <Date > SchedulDates = schedule.dates();

	std::cout << "Start              |" << "End              |" << "Fixing  |"
		<< "IDays        |" << "Index Rate|" << std::endl;

	BOOST_FOREACH(Date d, SchedulDates){
		Date startDate = schedule.previousDate(d);


		if (startDate != Date()) {
			Date FixingDate = mySwap->iborIndex()->fixingDate(startDate);
			Date endDate = schedule.nextDate(d);
			Time Idays = daysBetween(startDate, endDate);

			//Date d1 = d + 6 * Months;
			std::cout << io::short_date(startDate) << "       " <<
				io::short_date(endDate) << "        " <<
				io::short_date(FixingDate) << "       " <<
				Idays << "            " <<
				100 * mySwap->iborIndex()->fixing(FixingDate, true) << "         " << std::endl;

		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	Schedule Fixschedule = mySwap->fixedSchedule();
	std::vector <Date > FixedSchedulDates = Fixschedule.dates();

	std::cout << "Start              |" << "End              |" << /*"Fixing  |"
																   <<*/ "IDays        |" << "Index Rate|" << std::endl;

	BOOST_FOREACH(Date d, FixedSchedulDates){
		Date startDate = Fixschedule.previousDate(d);


		if (startDate != Date()) {
			//Date FixingDate = Fixschedule.
			Date endDate = Fixschedule.nextDate(d);
			Time Idays = daysBetween(startDate, endDate);

			//Date d1 = d + 6 * Months;
			std::cout << io::short_date(startDate) << "       " <<
				io::short_date(endDate) << "        " <<
				//io::short_date(FixingDate) << "       " <<
				Idays << "            " <<
				/*100 * mySwap->iborIndex()->fixing(FixingDate, true) << "         " <<*/ std::endl;

		}
	}




	std::vector< boost::shared_ptr< CashFlow > > floatingleg = mySwap->floatingLeg();
	std::vector< boost::shared_ptr< CashFlow > > fixedleg = mySwap->fixedLeg();
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Date            |" << "Fixed Leg Cash FLow              |" << " Floating Leg Cash FLow              |" << std::endl;

	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, fixedleg){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << "                  " << std::endl;
	}

	std::cout << std::endl;
	std::cout << " Date            |" << " Floating Leg Cash FLow              |" << std::endl;

	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, floatingleg){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;

	}
	std::cout << std::endl;
	std::cout << std::endl;
}


void PrintCashFlowSwap(Swap mySwap){

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Date            |" << "Fixed Leg Cash FLow              |" << " Floating Leg Cash FLow              |" << std::endl;

	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, mySwap.leg(0)){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << "                  " << std::endl;
	}

	std::cout << std::endl;
	std::cout << " Date            |" << " Floating Leg Cash FLow              |" << std::endl;

	BOOST_FOREACH(boost::shared_ptr< CashFlow > Cash, mySwap.leg(1)){
		std::cout << io::short_date(Cash->date()) << "     " << Cash->amount() << std::endl;

	}
	std::cout << std::endl;
	std::cout << std::endl;
}


void InputSwapParmeters(Real & fixedRate, Integer & lenghtInYears, Real & notional){
	std::string s;
	std::cout << " Entrer le  taux fixe  ";
	std::cin >> s;
	try{ fixedRate = boost::lexical_cast<Real>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		fixedRate = 0;
	}

	while (fixedRate == 0) {
		std::cout << " Entrer le  taux fixe   ";
		std::cin >> s;
		try
		{
			fixedRate = boost::lexical_cast<Real>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			fixedRate = 0;
		}
	}

	std::cout << " Entrer la  duree en annee    ";
	std::cin >> s;
	try{ lenghtInYears = boost::lexical_cast<Integer>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		lenghtInYears = 0;
	}
	while (lenghtInYears <= 0) {
		std::cout << " Entrer la  duree en annee  ";
		std::cin >> s;
		try
		{
			lenghtInYears = boost::lexical_cast<Integer>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			lenghtInYears = 0;
		}
	}
	std::cout << " Entrer le  nominal   ";
	std::cin >> s;
	try{ notional = boost::lexical_cast<Real>(s); }
	catch (boost::bad_lexical_cast const&)
	{
		notional = 0;
	}

	while (notional == 0) {
		std::cout << " Entrer le  nominal   ";
		std::cin >> s;
		try
		{
			notional = boost::lexical_cast<Real>(s);
		}
		catch (boost::bad_lexical_cast const&)
		{
			notional = 0;
		}
	}
	std::cout << std::endl;
}

Swap swapPricing(Real nominal, Date today, Integer lenghtInYears, Rate fixedRate){
	CapFloorParmeters vars;

	Date maturity = today + lenghtInYears* Years;
	Calendar calendar = TARGET();
	Schedule schedule(today, maturity, Period(Semiannual), calendar,
		ModifiedFollowing, ModifiedFollowing,
		DateGeneration::Forward, false);
	DayCounter dayCounter = Actual365Fixed();
	std::vector<Real> nominals(2 * lenghtInYears, nominal);

	CurveData cData;
	// Fill out with some sample market data
	sampleMktData(cData);
	boost::shared_ptr<YieldTermStructure> ocurve = buildCurve(cData);

	RelinkableHandle<YieldTermStructure> discountingTermStructure;
	// the one used for forward rate forecasting
	RelinkableHandle<YieldTermStructure> forecastingTermStructure;

	forecastingTermStructure.linkTo(ocurve);
	discountingTermStructure.linkTo(ocurve);
	boost::shared_ptr<IborIndex> index(new Euribor6M(forecastingTermStructure));

	std::vector<Rate> coupons(2 * lenghtInYears, fixedRate);
	/*for (Size i = 0; i < lenghtInYears; ++i){
	coupons[i] = fixedRate;
	}*/
	coupons[0] = 0.015;
	coupons[1] = 0.015;
	coupons[2] = 0.016;
	coupons[3] = 0.016;
	coupons[4] = 0.018;
	coupons[5] = 0.018;
	coupons[6] = 0.02;
	coupons[7] = 0.02;
	coupons[8] = 0.025;
	coupons[9] = 0.025;

	Leg fixedLeg = FixedRateLeg(schedule)
		.withNotionals(nominals)
		.withCouponRates(coupons, dayCounter, Simple, Semiannual);

	std::vector<Real> gearings;
	std::vector<Rate> spreads;
	Natural fixingDays = 2;



	Leg floatingLeg = IborLeg(schedule, index)
		.withNotionals(nominals)
		.withPaymentDayCounter(Actual360());
	//.withFixingDays(fixingDays);



	Swap swap(fixedLeg, floatingLeg);
	swap.setPricingEngine(boost::shared_ptr<PricingEngine>(
		new DiscountingSwapEngine(discountingTermStructure)));
	return swap;
}


void TestSwap(){

	/* PARAMETERS */
	Date today(10, October, 2014);
	Real notional = 1000000.0;
	Integer lenghtInYears = 10;
	Rate fixedRate = 0.022125;


	int Fin = 1;
	std::string s;

	while (Fin != 0) {
		std::cout << " Parametres de pricing SWAP   " << std::endl;
		std::cout << std::endl;

		InputSwapParmeters(fixedRate, lenghtInYears, notional);
		//boost::shared_ptr<VanillaSwap>  mySwap = PricingSwap(notional, today, lenghtInYears, fixedRate);
		//std::cout << " --------------------- " << std::endl;
		//std::cout << std::endl;
		//std::cout << " NPV FixedLeg  = " << std::setprecision(10) << mySwap->fixedLegNPV() << std::endl;
		//std::cout << " NPV FloatingLeg  = " << std::setprecision(10) << mySwap->floatingLegNPV() << std::endl;
		//std::cout << " NPV  = " << std::setprecision(10) << mySwap->NPV() << std::endl;
		//std::cout << " Fair Rate  = " << std::setprecision(10) << mySwap->fairRate() << std::endl;
		//std::cout << std::endl;
		//std::cout << " --------------------- " << std::endl;
		//PrintCashFlowSwap(mySwap);

		Swap testswap = swapPricing(notional, today, lenghtInYears, fixedRate);
		std::cout << " --------------------- " << std::endl;
		std::cout << std::endl;
		std::cout << " NPV FixedLeg  = " << std::setprecision(10) << testswap.legNPV(0) << std::endl;
		std::cout << " NPV FloatingLeg  = " << std::setprecision(10) << testswap.legNPV(1) << std::endl;
		std::cout << " NPV  = " << std::setprecision(10) << testswap.NPV() << std::endl;
		std::cout << std::endl;
		std::cout << " --------------------- " << std::endl;

		std::cout << std::endl;
		std::cout << " --------------------- " << std::endl;

		PrintCashFlowSwap(testswap);
		std::cout << std::endl;
		std::cout << " Taper 1 pour continuer ,  0 pour Quitter  : ";
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

void TestCapFloor(){
	// Parmameters
	CapFloorParmeters capfloorEfficiecy;
	Integer Duration = 10;
	Volatility vol = 0.1;
	Rate strikes = 0.02;
	Calendar calendar = TARGET();
	//Date startDate = calendar.adjust(Date::todaysDate());
	Date startDate = Date(8, Oct, 2014);
	//Date startDate(10, October, 2014);

	Real notional = 1000000;
	capfloorEfficiecy.nominals[0] = notional;
	std::string s;
	int Fin = 1;
	std::istringstream convert;
	CapFloor::Type InstrumentType = CapFloor::Cap;
	//PrintCurve( capfloorEfficiecy);

	while (Fin != 0) {
		//Inputparmeters(InstrumentType, Duration,strikes, vol, notional);
		capfloorEfficiecy.nominals[0] = notional;

		//Define Leg
		Leg leg = capfloorEfficiecy.DefinedLeg(startDate, Duration);

		//Define the product
		boost::shared_ptr<CapFloor> InstrumentEfficiency =
			capfloorEfficiecy.DefinedCapFloor(InstrumentType, leg, strikes, vol);

		// Print rseults
		PrintResults(strikes, vol, capfloorEfficiecy.nominals[0], InstrumentEfficiency->maturityDate(), startDate, InstrumentEfficiency, InstrumentType, capfloorEfficiecy);
		Printvolatility(capfloorEfficiecy);
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

