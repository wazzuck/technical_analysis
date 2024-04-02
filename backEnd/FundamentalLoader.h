#pragma once

#include <string>
#include "Fundamental.h"
#include "FundamentalLoader.h"

/*
 * Class to load reference data from csv
*/

using namespace std;

namespace eod
{
	class FundamentalLoader{
	public:
        FundamentalLoader();
        map<string,Fundamental> getFundamentalData(string fileName);
    private:
        map<string,Fundamental> fundamentalMap;
        map<string,Fundamental> loadFundamental(map<string,Fundamental> FundamentalMap, string fileName);
	};
}
