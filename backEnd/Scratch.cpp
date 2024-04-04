#include "Scratch.h"
namespace eod
{
Scratch::Scratch()
{
  cdipMapPointer = new cdipMapType();
}

void Scratch::addCalendarDayInstrumentPrice ( string key, CalendarDayInstrumentPrice cdip )
{
  //cdipMapPointer->insert(make_pair(key&, cdip));
  ( *cdipMapPointer ) [key] = cdip;
}
}
