#include "CalendarDayInstrumentPrice.h"
#include <map>

#pragma once

using namespace std;

namespace eod
{
class Scratch
{
public:
  typedef map<string, eod::CalendarDayInstrumentPrice> cdipMapType;
  cdipMapType* cdipMapPointer;
  //map<str, CalendarDayInstrumentPrice> cdipMap;

  void addCalendarDayInstrumentPrice ( string key, CalendarDayInstrumentPrice cdip );
};
}
