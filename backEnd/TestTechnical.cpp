//C++ standard library headers
#include <iostream> //for cout
#include <fstream> //for reading in a file
#include <regex>
#include <chrono>
#include <filesystem>
#include <vector>

/*
#include <iostream>
#include <fstream> //for reading in a file
#include <filesystem>
#include <map>
#include <chrono>
*/

#include "EodDate.h"
#include "CalendarDayInstrumentPrice.h"
#include "InstrumentPrices.h"
#include "ToolKit.h"
#include "Logger.h"
#include "PriceLoader.h"
#include "TechnicalAnalysis.h"

#include "ta-lib/ta_libc.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{

  DLOG ( "Running TestTechnical.exe" );

  return 0;
}
