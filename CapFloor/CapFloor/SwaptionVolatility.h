#include <boost/format.hpp>

#include <ql/quantlib.hpp>
#include "VolStructureUtilities.h"
using namespace QuantLib;

struct SwaptionVolatility {
	// global data
	Date referenceDate;
	MarketConventions conventions;
	SwaptionAtmVolatility atmVol;
	RelinkableHandle<YieldTermStructure> termStructure;
	RelinkableHandle<SwaptionVolatilityStructure> atmVolMatrix;
	Real tolerance;

	// cleanup
	SavedSettings backup;

	// setup
	SwaptionVolatility() {
		conventions.setConventions();
		atmVol.setMarketData();
		Settings::instance().evaluationDate() =
			conventions.calendar.adjust(Date::todaysDate());
		atmVolMatrix = RelinkableHandle<SwaptionVolatilityStructure>(
			boost::shared_ptr<SwaptionVolatilityStructure>(new
			SwaptionVolatilityMatrix(conventions.calendar,
			conventions.optionBdc,
			atmVol.tenors.options,
			atmVol.tenors.swaps,
			atmVol.volsHandle,
			conventions.dayCounter)));
		/*termStructure.linkTo(
			boost::shared_ptr<YieldTermStructure>(new
			FlatForward(0, conventions.calendar,
			0.05, Actual365Fixed())));*/
	}

	

	
};
