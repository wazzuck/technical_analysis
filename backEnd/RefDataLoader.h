#pragma once

#include "RefDataInstrument.h"

/*
 * Class to load reference data from csv
*/

using namespace std;

namespace eod
{
class RefDataLoader
{
public:
  map < string, RefDataInstrument >* loadRefData( string load_directory );
};
}
