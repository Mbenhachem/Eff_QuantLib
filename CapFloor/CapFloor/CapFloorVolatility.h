#include <boost/format.hpp>

#include <ql/quantlib.hpp>

using namespace QuantLib;


struct CapFloorVolatility {
	Volatility flatVol ;
	std::vector<Volatility> vols;
	std::vector<Date> dates;

	// Option Tenors
	std::vector<Period> optionTenors;

	// Strikes
	std::vector<Rate> strikes;
	RelinkableHandle<YieldTermStructure> termStructure;
	boost::shared_ptr<IborIndex> indexVol;
	CapFloorVolatility(){
		indexVol = boost::shared_ptr<IborIndex>(new Euribor6M(termStructure));

		//cap volatility smile matrix
		optionTenors.push_back(Period(1, Years));
		optionTenors.push_back(Period(2, Years));
		optionTenors.push_back(Period(4, Years));
		optionTenors.push_back(Period(5, Years));
		optionTenors.push_back(Period(6, Years));
	


		strikes.push_back(0.005);
		strikes.push_back(0.045);
		strikes.push_back(0.05);
		strikes.push_back(0.055);


		vols.push_back(15.0);
		vols.push_back(15.0);
		vols.push_back(15.559140850451);
		vols.push_back(16.124460052841);
		vols.push_back(16.455537507837);
		vols.push_back(16.78223163356);
		vols.push_back(16.78223163356);
		vols.push_back(17.10892575928);
		vols.push_back(17.435619885014);
		vols.push_back(17.46545623243);
		vols.push_back(17.4886221618537);
		vols.push_back(17.03635);
		vols.push_back(16.56839);
		vols.push_back(16.56333);
		vols.push_back(16.56333);
		vols.push_back(16.56333);
		vols.push_back(16.56333);
		vols.push_back(16.56333);
		vols.push_back(16.56333);
		vols.push_back(16.56333);




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
		//dates.push_back(Date(8, Oct, 2021));


	}
};
#pragma region AtM and surface volatility	
/* Constuction of the cap floor volatility smile matrix*/
boost::shared_ptr<CapFloorTermVolSurface> buildTermSurfaceVol(CapFloorVolatility &cv)
{
	Calendar calendar = TARGET();
	Date today(6, October, 2014);
	today = calendar.adjust(today);
	Settings::instance().evaluationDate() = today;
	Natural settlementDays = 0;
	Date  settlementDate = calendar.adjust(today + settlementDays * Days);
	DayCounter DayCounter = Actual365Fixed();


	Matrix matVols(cv.optionTenors.size(), cv.strikes.size());
	matVols[0][0] = 0.0; matVols[0][1] = 0.15;	matVols[0][2] = 0.14;	matVols[0][3] = 0.155;
	matVols[1][0] = 0.0; matVols[1][1] = 0.161; matVols[1][2] = 0.150;	matVols[1][3] = 0.159;
	matVols[2][0] = 0.0; matVols[2][1] = 0.168; matVols[2][2] = 0.162;	matVols[2][3] = 0.166;
	matVols[3][0] = 0.0; matVols[3][1] = 0.17;	matVols[3][2] = 0.163;	matVols[3][3] = 0.169;
	matVols[4][0] = 0.0; matVols[4][1] = 0.172; matVols[4][2] = 0.162;	matVols[4][3] = 0.169;

	boost::shared_ptr<CapFloorTermVolSurface> TermVolSurface = boost::shared_ptr<CapFloorTermVolSurface>(new CapFloorTermVolSurface(settlementDays, calendar, ModifiedFollowing, cv.optionTenors, cv.strikes, matVols, DayCounter));

	return TermVolSurface;
}


/*Constuction of the atm  cap floor volatility matrix*/
Handle<CapFloorTermVolCurve> buildTermVolCurve(CapFloorVolatility &cv)
{
	Calendar calendar = TARGET();
	Date today(6, October, 2014);
	today = calendar.adjust(today);
	Settings::instance().evaluationDate() = today;
	Natural settlementDays = 0;
	Date  settlementDate = calendar.adjust(today + settlementDays * Days);
	DayCounter DayCounter = Actual365Fixed();


	std::vector<Volatility>atmTermV;
	atmTermV.push_back(14.2);
	atmTermV.push_back(15.4);
	atmTermV.push_back(16.1);
	atmTermV.push_back(16.2);
	atmTermV.push_back(16.2);
	

	std::vector<Handle<Quote> > atmTermVolHandle;
	Handle<CapFloorTermVolCurve> TermVolCurve;
	atmTermVolHandle.resize(cv.optionTenors.size());
	for (Size i = 0; i<cv.optionTenors.size(); ++i) {
		atmTermVolHandle[i] = Handle<Quote>(boost::shared_ptr<Quote>(new
			SimpleQuote(atmTermV[i])));
	}

	TermVolCurve = Handle<CapFloorTermVolCurve>(
		boost::shared_ptr<CapFloorTermVolCurve>(new
		CapFloorTermVolCurve(0, calendar, ModifiedFollowing, cv.optionTenors, atmTermVolHandle, DayCounter)));

	return TermVolCurve;
}

/* Constuction of the forward-forward volatility  (i.e. caplet/floorlet volatilities) stripping from  the cap floor vol surface (CapFloorTermVolSurface) */
Handle<OptionletVolatilityStructure> buildOptionletSurfaceVol(CapFloorVolatility &cv)
{
	boost::shared_ptr<CapFloorTermVolSurface> TermVolSurface = buildTermSurfaceVol(cv);

	boost::shared_ptr<OptionletStripper> stripper1(new OptionletStripper1(TermVolSurface, cv.indexVol));

	boost::shared_ptr<StrippedOptionletAdapter> strippedOptionletAdapter1 =
		boost::shared_ptr<StrippedOptionletAdapter>(new	StrippedOptionletAdapter(stripper1));

	Handle<OptionletVolatilityStructure>capletsVolSurface((strippedOptionletAdapter1));

	capletsVolSurface->enableExtrapolation();

	return capletsVolSurface;
}

/* Constuction of the forward-forward volatility  (i.e. caplet/floorlet volatilities) stripping from  the cap floor atm term vol of a  CapFloorTermVolCurve*/
Handle<OptionletVolatilityStructure> buildOptionletCurveVol(CapFloorVolatility &cv)
{
	boost::shared_ptr<CapFloorTermVolSurface> TermVolSurface = buildTermSurfaceVol(cv);

	boost::shared_ptr<OptionletStripper1> stripper1(new OptionletStripper1(TermVolSurface, cv.indexVol));

	boost::shared_ptr<StrippedOptionletAdapter> strippedOptionletAdapter1 =
		boost::shared_ptr<StrippedOptionletAdapter>(new	StrippedOptionletAdapter(stripper1));

	Handle<OptionletVolatilityStructure>capletsVolSurface((strippedOptionletAdapter1));

	capletsVolSurface->enableExtrapolation();

	
	Handle<CapFloorTermVolCurve> TermVolCurve = buildTermVolCurve(cv);

	boost::shared_ptr<OptionletStripper> optionletStripperCurve(new
		OptionletStripper2(stripper1, TermVolCurve));

	boost::shared_ptr<StrippedOptionletAdapter> strippedOptionletAdapter2(new
		StrippedOptionletAdapter(optionletStripperCurve));

	Handle<OptionletVolatilityStructure> capletsVolCurve(strippedOptionletAdapter2);
	capletsVolCurve->enableExtrapolation();

	return capletsVolCurve;
}
#pragma endregion 

/* Constuction of the optionlet volatility */
Handle<OptionletVolatilityStructure> buildCurveOptionletAtmVol(CapFloorVolatility &cv)
{
	Calendar calendar = TARGET();
	Date today(6, October, 2014);
	today = calendar.adjust(today);
	Settings::instance().evaluationDate() = today;
	DayCounter volDayCounter = Actual360();
	

	std::vector<Volatility> capletVols;
	for (Size i = 0; i <cv.dates.size(); ++i) {
		capletVols.push_back(cv.vols[i] / 100);
	}
	 Handle<OptionletVolatilityStructure>capVol (
		boost::shared_ptr<OptionletVolatilityStructure>((new CapletVarianceCurve(today, cv.dates,
		capletVols, volDayCounter))));
	capVol->enableExtrapolation();
	return capVol;
}

#pragma region Flat volatility
Handle<OptionletVolatilityStructure> buildFlatCurveVol(CapFloorVolatility &cv)
{
	Calendar calendar = TARGET();

	Date today(6, October, 2014);
	today = calendar.adjust(today);
	Settings::instance().evaluationDate() = today;
	Natural settlementDays = 2;
	Date  settlementDate = calendar.adjust(today + settlementDays * Days);
	DayCounter volDayCounter = Actual365Fixed();

	cv.flatVol = 0.16;
	Matrix termV = Matrix(cv.optionTenors.size(), cv.strikes.size(), cv.flatVol);

	boost::shared_ptr<CapFloorTermVolSurface>  flatTermVolSurface = boost::shared_ptr<CapFloorTermVolSurface>(new
		CapFloorTermVolSurface(0, calendar, ModifiedFollowing,
		cv.optionTenors, cv.strikes,
		termV, volDayCounter));
	//cv.indexVol = boost::shared_ptr<IborIndex>(new Euribor6M());
	boost::shared_ptr<OptionletStripper1> optionletStripper1(new
		OptionletStripper1(flatTermVolSurface, cv.indexVol));

	boost::shared_ptr<StrippedOptionletAdapter> strippedOptionletAdapter(new
		StrippedOptionletAdapter(optionletStripper1));

	Handle<OptionletVolatilityStructure> capletFlatVolSurface(strippedOptionletAdapter);
	capletFlatVolSurface->enableExtrapolation();

	//atm cap/Floor Flat volatility curve
	std::vector<Handle<Quote> >  curveVHandle(cv.optionTenors.size());
	for (Size i = 0; i<cv.optionTenors.size(); ++i)
		curveVHandle[i] = Handle<Quote>(boost::shared_ptr<Quote>(new
		SimpleQuote(cv.flatVol)));

	Handle<CapFloorTermVolCurve> flatTermVolCurve = Handle<CapFloorTermVolCurve>(
		boost::shared_ptr<CapFloorTermVolCurve>(new	CapFloorTermVolCurve(today, calendar, ModifiedFollowing, cv.optionTenors,
		curveVHandle, Actual365Fixed())));

	boost::shared_ptr<OptionletStripper> optionletStripper2(new OptionletStripper2(optionletStripper1, flatTermVolCurve));

	boost::shared_ptr<StrippedOptionletAdapter> strippedOptionletAdapterFlat2(new StrippedOptionletAdapter(optionletStripper2));

	Handle<OptionletVolatilityStructure> capletFlatVolCurve(strippedOptionletAdapterFlat2);

	capletFlatVolCurve->enableExtrapolation();
	return capletFlatVolCurve;
}

#pragma endregion 