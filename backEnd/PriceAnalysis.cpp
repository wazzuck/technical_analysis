#include "PriceAnalysis.h"
#include "Technical.h"
#include "CalendarDayInstrumentPrice.h"
#include "Logger.h"

namespace eod
{
PriceAnalysis::PriceAnalysis()
{
  t = new Technical;
  //cdipMapPointer = new cdipMapType(); // Allocate memory for the map pointer
  //cdipMap = *cdipMapPointer; // Assign the allocated memory to the member variable
  //DLOG("cdipMapPointer << " << cdipMapPointer);
  cdipMap = map<string, CalendarDayInstrumentPrice *>();
}

PriceAnalysis::~PriceAnalysis()
{
  DLOG ( "Calling ~PriceAnalysis() destructor" );

  //TODO Impelement either smart pointers or manual memory cleanup
  /*
  delete t;

  for (auto& pair : cdipMap)
  {
      delete pair.second;
  }

  */

  //cdipMap.clear();
  //delete cdipMapPointer; // Delete the dynamically allocated map pointer
}

void PriceAnalysis::addCalendarDayInstrumentPrice ( const string &date, CalendarDayInstrumentPrice *cdipPtr )
{
  cdipMap[date] = cdipPtr; // Access map directly using the instance's map
}

float PriceAnalysis::getNextLastCalendarDayInstrumentClosePrice ( int numberToGoBack = 0 )
{
  int j = 0;
  float returnPrice;

  for ( auto it = cdipMap.rbegin(); it != cdipMap.rend(); it++ ) {
    if ( j == numberToGoBack ) {
      returnPrice = it->second->getClose();
    }

    j++;
  }

  return returnPrice;
}

CalendarDayInstrumentPrice *PriceAnalysis::getCalendarDayInstrumentPrice ( const string &date )
{
  auto it = cdipMap.find ( date );

  if ( it != cdipMap.end() ) {
    return it->second;
  }

  //TODO What are the cirumstances where a cdip would not be found?
  return nullptr; // If not found
}

void PriceAnalysis::setMarketCap ( double inMarketCap )
{
  t->setMarketCap ( inMarketCap );
}

map<string, CalendarDayInstrumentPrice *> *PriceAnalysis::getCdipMapPointer()
{
  return &cdipMap;
}
}
