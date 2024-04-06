#include <regex>
#include <fstream>
#include <filesystem>
#include <map>

#include "CalendarDayInstrumentPrice.h"
#include "PriceLoader.h"
#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

namespace fs = filesystem;

namespace eod
{
PriceLoader::PriceLoader()
{
  cdipMapPointer = new cdipMapType();
  cdipMap = *cdipMapPointer;
}

map<string, CalendarDayInstrumentPrice *> *PriceLoader::getPrices ( const string fileNameStem )
{
  string fileName = "";
  bool isCsv = false;

  if ( fs::exists ( fileNameStem + ".txt" ) ) {
    fileName = fileNameStem + ".txt";
  }

  else if ( fs::exists ( fileNameStem + ".csv" ) ) {
    fileName = fileNameStem + ".csv";
    isCsv = true;
  }
  else {
    LOG ( "Neither " + fileName + ".txt or " + fileName + ".csv" << "  exist" );
    exit ( 1 );
  }

  DLOG ( "Loading cdipMap for " << fileName );

  ifstream inputStream ( fileName );

  string line = "" ;

  ToolKit tk;

  if ( isCsv == true ) {
    //DLOG("Removing header in .csv file
    getline ( inputStream, line );
  }

  regex header ( "^Sym" );

  while ( getline ( inputStream, line ) ) {
    CalendarDayInstrumentPrice *cdipPtr = new CalendarDayInstrumentPrice();

    if ( regex_search ( line, header ) ) {
      //DLOG("Found header, skipping line");
      break;
    }

    cdipPtr->setValues ( tk.split ( line, ',' ) );
    ( *cdipMapPointer ) [cdipPtr->getMnemonic()] = cdipPtr;
  }

  inputStream.close();

  LOG ( "Loading of the EOD price data for " << fileName << " complete" );

  return cdipMapPointer;
}

void PriceLoader::printPrices ( const string mnemonic )
{
  ( *cdipMapPointer ) [mnemonic]->printCalendarDayInstrumentPrice();
}
}
