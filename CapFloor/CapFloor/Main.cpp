
 #include <stdlib.h>
//#include "CapFloorEfficiency.hpp"
#include "SwaptionEfficiency.hpp"
#include "TestLmm.h"
int main(int, char* []) {
	
    try {
		//PrintVolatility();
	//TestSwaption();
	TestLmmCapFloor();
	//TestCapFloor();
		//TestSwap();
		/*std::string s;
		std::string const capFloor ="1" ;
		std::string const swaption ="2" ;
			std::cout << " Choisir 1: Cap/Floor, 2: Swaption   " ;
			std::cin >> s ;
			if (s == capFloor) 
				TestCapFloor();
			else if (s==swaption) 
				TestSwaption();
			else
			QL_FAIL("unknown produit");
      */

		return 0;
   } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
		
		system("pause");
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }

   }
