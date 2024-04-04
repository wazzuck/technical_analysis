#include <iostream>
#include <map>

#include "PriceLoader.h"
#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace eod;

int main()
{
  LOG ( "Testing PriceLoader Class" );

  string fileToLoad = "/home/neville/data/20231101/LSE_20231101";

  PriceLoader pl;

  typedef map<string, CalendarDayInstrumentPrice *> cdipMapType;
  cdipMapType *cdipMapPointer;

  cdipMapPointer = pl.getPrices ( fileToLoad );
  /*
  for (auto const& pair : testMap) {
      cout << pair.first << endl;
      if (pair.first == "")
      {
          cout << "Found Blank" << endl;
          exit(0);
      }
  }
  */
  //TODO BUG SLA is blank?
  //testMap["AGK"].printCalendarDayInstrumentPrice();

  for ( auto const&[key, value] : *cdipMapPointer ) {
    ( *cdipMapPointer ) [key]->printCalendarDayInstrumentPrice();
  }

  return 0;
}
