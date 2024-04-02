#pragma once

#include <string>
#include "RefDataInstrument.h"

/*
 * Class to load reference data from csv
*/

using namespace std;

namespace eod
{
	class RefDataLoader{
	public:
        RefDataLoader();
        map<string,RefDataInstrument> getRefData(string fileName);
    private:
        map<string,RefDataInstrument> refDataInstrumentMap;
        map<string,RefDataInstrument> loadRefData(map<string,RefDataInstrument> RefDataInstrumentMap, string fileName);
	};
}
