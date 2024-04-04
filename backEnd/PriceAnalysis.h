/* Parent class for both CalendarDayInstrumentPrice and Technical data */

#ifndef PRICE_ANALYSIS_H
#define PRICE_ANALYSIS_H

#pragma once

#include "CalendarDayInstrumentPrice.h"
#include "Technical.h"
#include <map>

using namespace std;

namespace eod
{
//Forward declations of child classes
class Technical;
class CalendarDayInstrumentPrice;

class PriceAnalysis //Working title
{
public:
  PriceAnalysis();
  ~PriceAnalysis();
  void addCalendarDayInstrumentPrice ( const string& date, CalendarDayInstrumentPrice* cdipPtr );
  CalendarDayInstrumentPrice* getCalendarDayInstrumentPrice ( const string& date );
  map<string, CalendarDayInstrumentPrice*>* getCdipMapPointer();
  typedef map<string, CalendarDayInstrumentPrice*> cdipMapType;
  //cdipMapType* cdipMapPointer;

  Technical* t;
  void setMarketCap ( double inMarketCap );
  float getNextLastCalendarDayInstrumentClosePrice ( int numberToGoBack );
private:
  map<string, CalendarDayInstrumentPrice*> cdipMap;
};
}

#endif
