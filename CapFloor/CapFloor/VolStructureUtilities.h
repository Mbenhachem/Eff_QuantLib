#include <boost/format.hpp>

#include <ql/quantlib.hpp>

using namespace QuantLib;

struct SwaptionTenors {
	std::vector<Period> options;
	std::vector<Period> swaps;
};
struct MarketConventions {
	Calendar calendar;
	BusinessDayConvention optionBdc;
	DayCounter dayCounter;
	void setConventions() {
		calendar = TARGET();
		optionBdc = ModifiedFollowing;
		dayCounter = Actual365Fixed();
	}
};
struct SwaptionAtmVolatility {
	SwaptionTenors tenors;
	Matrix vols;
	std::vector<std::vector<Handle<Quote> > > volsHandle;
	void setMarketData() {
		tenors.swaps.resize(11);
		tenors.swaps[0] = Period(1, Months);
		tenors.swaps[1] = Period(3, Months);
		tenors.swaps[2] = Period(6, Months);
		tenors.swaps[3] = Period(1, Years);
		tenors.swaps[4] = Period(2, Years);
		tenors.swaps[5] = Period(5, Years);
		tenors.swaps[6] = Period(7, Years);
		tenors.swaps[7] = Period(10, Years);
		tenors.swaps[8] = Period(15, Years);
		tenors.swaps[9] = Period(20, Years);
		tenors.swaps[10] = Period(30, Years);

		tenors.options.resize(13);
		tenors.options[0] = Period(1, Months);
		tenors.options[1] = Period(2, Months);
		tenors.options[2] = Period(3, Months);
		tenors.options[3] = Period(6, Months);
		tenors.options[4] = Period(1, Years);
		tenors.options[5] = Period(2, Years);
		tenors.options[6] = Period(3, Years);
		tenors.options[7] = Period(5, Years);
		tenors.options[8] = Period(7, Years);
		tenors.options[9] = Period(10, Years);
		tenors.options[10] = Period(15, Years);
		tenors.options[11] = Period(20, Years);
		tenors.options[12] = Period(30, Years);

		vols = Matrix(tenors.options.size(), tenors.swaps.size());
		vols[0][0] = 0.19765;	vols[0][1] = 0.183525;	vols[0][2] = 0.1671;	vols[0][3] = 0.1557;	vols[0][4] = 0.14587;	vols[0][5] = 0.138;		vols[0][6] = 0.1305;	vols[0][7] = 0.12447;	vols[0][8] = 0.11857;	vols[0][9] = 0.11325;	vols[0][10] = 0.10695; 
		vols[1][0] = 0.19275;	vols[1][1] = 0.1803125; vols[1][2] = 0.164875;	vols[1][3] = 0.15361;	vols[1][4] = 0.14433;	vols[1][5] = 0.1365;	vols[1][6] = 0.12966;	vols[1][7] = 0.123787;	vols[1][8] = 0.118275;	vols[1][9] = 0.113225;	vols[1][10] = 0.106825; 
		vols[2][0] = 0.18785;	vols[2][1] = 0.1771;	vols[2][2] = 0.16265;	vols[2][3] = 0.1515;	vols[2][4] = 0.1428;	vols[2][5] = 0.135;		vols[2][6] = 0.12883;	vols[2][7] = 0.1231;	vols[2][8] = 0.11797;	vols[2][9] = 0.1132;	vols[2][10] = 0.1067; 
		vols[3][0] = 0.183575;	vols[3][1] = 0.169525;	vols[3][2] = 0.1571;	vols[3][3] = 0.145925;	vols[3][4] = 0.13765;	vols[3][5] = 0.13133;	vols[3][6] = 0.12587;	vols[3][7] = 0.1207;	vols[3][8] = 0.11635;	vols[3][9] = 0.112075;	vols[3][10] = 0.105675;
		vols[4][0] = 0.179375;	vols[4][1] = 0.1645;	vols[4][2] = 0.1525875; vols[4][3] = 0.1428;	vols[4][4] = 0.13497;	vols[4][5] = 0.12917;	vols[4][6] = 0.1241;	vols[4][7] = 0.1195;	vols[4][8] = 0.115337;	vols[4][9] = 0.1113625; vols[4][10] = 0.1054125; 
		vols[5][0] = 0.175175;	vols[5][1] = 0.159475;	vols[5][2] = 0.148075;	vols[5][3] = 0.139675;	vols[5][4] = 0.13222;	vols[5][5] = 0.127;		vols[5][6] = 0.12232;	vols[5][7] = 0.1183;	vols[5][8] = 0.114325;	vols[5][9] = 0.11065;	vols[5][10] = 0.10515;
		vols[6][0] = 0.1553;	vols[6][1] = 0.1433750; vols[6][2] = 0.1353250; vols[6][3] = 0.1295;	vols[6][4] = 0.12455;	vols[6][5] = 0.12067;	vols[6][6] = 0.1167;	vols[6][7] = 0.113675;	vols[6][8] = 0.110825;	vols[6][9] = 0.10805;	vols[6][10] = 0.102975;
		vols[7][0] = 0.145925;	vols[7][1] = 0.134925;	vols[7][2] = 0.127975;	vols[7][3] = 0.12277;	vols[7][4] = 0.1191;	vols[7][5] = 0.116;		vols[7][6] = 0.1127;	vols[7][7] = 0.1107;	vols[7][8] = 0.10815;	vols[7][9] = 0.10605;	vols[7][10] = 0.1003975;
		vols[8][0] = 0.137725;	vols[8][1] = 0.12815;	vols[8][2] = 0.12225;	vols[8][3] = 0.11765;	vols[8][4] = 0.11442;	vols[8][5] = 0.1123;	vols[8][6] = 0.10975;	vols[8][7] = 0.1079;	vols[8][8] = 0.105825;	vols[8][9] = 0.104075;	vols[8][10] = 0.09844;
		vols[9][0] = 0.131375;	vols[9][1] = 0.122875;	vols[9][2] = 0.1177;	vols[9][3] = 0.113325;	vols[9][4] = 0.11067;	vols[9][5] = 0.10866;	vols[9][6] = 0.10647;	vols[9][7] = 0.10503;	vols[9][8] = 0.103425;	vols[9][9] = 0.10195;	vols[9][10] = 0.09689;
		vols[10][0] = 0.12613;	vols[10][1] = 0.11861;	vols[10][2] = 0.11371;	vols[10][3] = 0.10955;	vols[10][4] = 0.10695;	vols[10][5] = 0.10517;	vols[10][6] = 0.10336;	vols[10][7] = 0.109431;	vols[10][8] = 0.1004925;vols[10][9] = 0.0992288; vols[10][10] = 0.0944825; 
		vols[11][0] = 0.1209;	vols[11][1] = 0.11435;	vols[11][2] = 0.109725;	vols[11][3] = 0.10577;	vols[11][4] = 0.103225;	vols[11][5] = 0.101667; vols[11][6] = 0.1025;	vols[11][7] = 0.0988575;vols[11][8] = 0.09756;	vols[11][9] = 0.0965075; vols[11][10] = 0.092075;
		vols[12][0] = 0.11662;	vols[12][1] = 0.11025;	vols[12][2] = 0.10589;	vols[12][3] = 0.10222;	vols[12][4] = 0.089;	vols[12][5] = 0.098766; vols[12][6] = 0.09876;	vols[12][7] = 0.0960429;vols[12][8] = 0.0950127;vols[12][9] = 0.0941221; vols[12][10] = 0.0900605;

		volsHandle.resize(tenors.options.size());
		for (Size i = 0; i<tenors.options.size(); i++){
			volsHandle[i].resize(tenors.swaps.size());
			for (Size j = 0; j<tenors.swaps.size(); j++)
				// every handle must be reassigned, as the ones created by
				// default are all linked together.
				volsHandle[i][j] = Handle<Quote>(boost::shared_ptr<Quote>(new
				SimpleQuote(vols[i][j])));
		}
	};
}; 