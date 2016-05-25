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
//#include "SwaptionEfficiency.cpp"
//#include <stdlib.h>
//#include <boost/timer.hpp>
//#include <iostream>
//#include <iomanip>
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
//
//
//namespace {
//
//    struct CapFloorEfficiecy {
//        // common parameters
//        Date settlement;
//        std::vector<Real> nominals;
//        BusinessDayConvention convention;
//        Frequency frequency;
//        boost::shared_ptr<IborIndex> index;
//        Calendar calendar;
//        Natural fixingDays;
//        RelinkableHandle<YieldTermStructure> termStructure;
//		DayCounter termStructureDayCounter;
//		double tolerance ;
//  
//
//        // initialize
//        CapFloorEfficiecy(): nominals(1,100) {
//            frequency = Semiannual;
//            index = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));
//            calendar = index->fixingCalendar();
//            convention = ModifiedFollowing;
//			//Date today(14, Jan, 2016);
//            Date today = calendar.adjust(Date::todaysDate());
//			
//            Settings::instance().evaluationDate() = today;
//            Natural settlementDays = 2;
//            fixingDays = 2;
//            settlement = calendar.advance(today,settlementDays,Days);
//			termStructureDayCounter =  ActualActual(ActualActual::ISDA);
//
//			boost::shared_ptr<YieldTermStructure> flatRate(new FlatForward(settlement, 0.6, termStructureDayCounter, Continuous, frequency));
//			termStructure.linkTo(flatRate);
//        }
//
//        // Leg defition
//        Leg DefinedLeg(const Date& startDate, Integer duration) {
//            Date endDate = calendar.advance(startDate,duration*Years,convention);
//            Schedule schedule(startDate, endDate, Period(frequency), calendar,
//                              convention, convention,
//                              DateGeneration::Forward, false);
//            return IborLeg(schedule, index)
//                .withNotionals(nominals)
//                .withPaymentDayCounter(index->dayCounter())
//                .withPaymentAdjustment(convention)
//                .withFixingDays(fixingDays);
//        }
//
//        boost::shared_ptr<PricingEngine> DefinedEngine(Volatility volatility) {
//            Handle<Quote> vol(boost::shared_ptr<Quote>(
//                                                new SimpleQuote(volatility)));
//            return boost::shared_ptr<PricingEngine>(
//                                new BlackCapFloorEngine(termStructure, vol));
//        }
//
//        boost::shared_ptr<CapFloor> DefinedCapFloor(CapFloor::Type type,
//                                                 const Leg& leg,
//                                                 Rate strike,
//                                                 Volatility volatility) {
//            boost::shared_ptr<CapFloor> result;
//            switch (type) {
//              case CapFloor::Cap:
//                result = boost::shared_ptr<CapFloor>(
//                                  new Cap(leg, std::vector<Rate>(1, strike)));
//                break;
//              case CapFloor::Floor:
//                result = boost::shared_ptr<CapFloor>(
//                                new Floor(leg, std::vector<Rate>(1, strike)));
//                break;
//              default:
//                QL_FAIL("unknown cap/floor type");
//            }
//            result->setPricingEngine(DefinedEngine(volatility));
//            return result;
//        }
//
//    };
//
//
//    std::string typeToString(CapFloor::Type type) {
//        switch (type) {
//          case CapFloor::Cap:
//            return "cap";
//          case CapFloor::Floor:
//            return "floor";
//          case CapFloor::Collar:
//            return "collar";
//          default:
//            QL_FAIL("unknown cap/floor type");
//        }
//    }
//
//}
//
//int main(int, char* []) {
//
//    try {
//
//        boost::timer timer;
//        std::cout << std::endl;
//
//		// Parmameters
//		CapFloorEfficiecy capfloorEfficiecy;
//		Integer Duration = 5;
//		Volatility vol= 0.15;
//		Rate strikes= 0.1;
//		Date startDate = capfloorEfficiecy.termStructure->referenceDate();
//		Real notional = 100;
//		capfloorEfficiecy.nominals[0] = notional;
//		//Deifne Leg
//		Leg leg = capfloorEfficiecy.DefinedLeg(startDate, Duration);
//
//		//Define the product
//		CapFloor::Type typeCap = CapFloor::Cap ;
//		boost::shared_ptr<CapFloor> CapEfficiency =
//							capfloorEfficiecy.DefinedCapFloor(typeCap,leg, strikes,vol);
//
//		Real res1 = CapEfficiency->NPV();
//
//		CapFloor::Type typeFloor = CapFloor::Floor ;
//
//		boost::shared_ptr<CapFloor> FloorEfficiency =
//							capfloorEfficiecy.DefinedCapFloor(typeFloor,leg, strikes,vol);
//	
//		//capfloorEfficiecy.index->
//		Real res2 =  FloorEfficiency->NPV();
//		
//		// Print rseults
//		std::cout << " Strike = " << io::rate(strikes) <<std::endl;
//		std::cout << " Volatility = " << io::volatility(vol) <<std::endl;
//		std::cout << " Notional = "<< capfloorEfficiecy.nominals[0] << std::endl;
//		std::cout << " Start Date = " << startDate << std::endl;
//		std::cout << " End Date = " << CapEfficiency->maturityDate() << std::endl;
//		std::cout << " Option type = "  << typeCap << ".......";
//	    std::cout << " NPV = " << res1 <<std::endl;
//		std::cout << " Option type = "  << typeFloor << "......";
//	    std::cout << " NPV  = " << res2 <<std::endl;
//		std::cout << std::endl;
//		system("pause");
//
//
//		std::string s ;
//		int Fin = 1;
//		std::cout << " Entrer de nouveaux parametres " << std::endl;
//		std::cout << " Taper 1 pour continuer ,  0 pour Quitter " << std::endl;
//		std::cin >> s ;
//		try
//		{Fin = boost::lexical_cast<int>(s);}
//		catch (boost::bad_lexical_cast const&)
//		{Fin = 0;}
//		std::istringstream convert;
//		while (Fin != 0) {
//			std::cout << " Entrer le  Strike   ";
//			std::cin >> s ;
//			try{strikes = boost::lexical_cast<Real>(s);}
//			catch (boost::bad_lexical_cast const&)
//			{strikes = 0;}
//			std::cout << " Strike = " << io::rate(strikes) <<std::endl;
//
//			while (strikes==0 ) {
//				std::cout << " Entrer le  Strike   ";
//				std::cin >> s ;
//				try
//				{strikes = boost::lexical_cast<Real>(s);}
//				catch (boost::bad_lexical_cast const&)
//				{strikes = 0;}
//				std::cout << " Strike = " << io::rate(strikes) <<std::endl;
//				}
//
//			std::cout << " Entrer la volatilite   " ;
//			std::cin >> s ;
//			try{vol = boost::lexical_cast<Real>(s);}
//			catch (boost::bad_lexical_cast const&)
//			{vol = 0;}
//			std::cout << " Volatilite = " << io::rate(vol) <<std::endl;
//
//			while ( vol==0 ) {
//				std::cout << " Entrer la  volatilite   ";
//				std::cin >> s ;
//				try
//				{vol = boost::lexical_cast<Real>(s);}
//				catch (boost::bad_lexical_cast const&)
//				{vol = 0;}
//				std::cout << " Volatilite = " << io::rate(vol) <<std::endl;
//				}
//			std::cout << " Entrer le  nominal   ";
//			std::cin >> s ;
//			try{notional = boost::lexical_cast<Real>(s);}
//			catch (boost::bad_lexical_cast const&)
//			{notional = 0;}
//			std::cout << " nominal = " << io::rate(notional) <<std::endl;
//			while (notional==0 ) {
//				std::cout << " Entrer le  nominal   ";
//				std::cin >> s ;
//				try
//				{notional = boost::lexical_cast<Real>(s);}
//				catch (boost::bad_lexical_cast const&)
//				{notional = 0;}
//				std::cout << " nominal = " << io::rate(notional) <<std::endl;
//				}
//		
//				capfloorEfficiecy.nominals[0] = notional;
//				//Define the product
//					CapFloor::Type typeCap = CapFloor::Cap ;
//					boost::shared_ptr<CapFloor> CapEfficiency =
//										capfloorEfficiecy.DefinedCapFloor(typeCap,leg, strikes,vol);
//
//					 res1 = CapEfficiency->NPV();
//					CapFloor::Type typeFloor = CapFloor::Floor ;
//
//					boost::shared_ptr<CapFloor> FloorEfficiency =
//										capfloorEfficiecy.DefinedCapFloor(typeFloor,leg, strikes,vol);
//					 res2 =  FloorEfficiency->NPV();
//					// Print rseults
//					std::cout << std::endl;
//					std::cout <<"|---------------------------------------------------|"<< std::endl;
//					std::cout << " Strike = " << io::rate(strikes) <<std::endl;
//					std::cout << " Volatility = " << io::volatility(vol) <<std::endl;
//					std::cout << " Notional = "<< capfloorEfficiecy.nominals[0] << std::endl;
//					std::cout << " Start Date = " << startDate << std::endl;
//					std::cout << " End Date = " << CapEfficiency->maturityDate() << std::endl;
//					std::cout << " Option type = "  << typeCap << ".......";
//					std::cout << " NPV = " << res1 <<std::endl;
//					std::cout << " Option type = "  << typeFloor << "......";
//					std::cout << " NPV  = " << res2 <<std::endl;
//					std::cout << std::endl;
//					std::cout <<"|----------------------------------------------------|"<< std::endl;
//					std::cout << std::endl;
//					std::cout << " Taper 1 pour continuer ,  0 pour Quitter " << std::endl;
//					std::cin >> s ;
//					try
//					{Fin = boost::lexical_cast<int>(s);}
//					catch (boost::bad_lexical_cast const&)
//					{Fin = 0;}
//				}
//		std::cout << std::endl;
//		 system("pause");
//		 SwaptionPrameters swaptionParameters ;
//		 
//		return 0;
//   } catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//
//        return 1;
//    } catch (...) {
//        std::cerr << "unknown error" << std::endl;
//        return 1;
//    }
//
//   }
//
