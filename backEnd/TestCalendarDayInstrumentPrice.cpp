#include <iostream>

#include "CalendarDayInstrumentPrice.h"
#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{
  LOG ( "Testing CalendarDayInstrumentPrice Class" );

  //string instrumentLine1 = "VOD,20200212,150.74,155.52,150.16,152.64,46428587";
  //string instrumentLine2 = "VOD.L,2023-May-26,150.74,155.52,150.16,152.64,46428587";
  string instrumentLine2 = "BT.A,20231123,123.15,125,121.6,122.65,2788357";

  CalendarDayInstrumentPrice cdip;

  ToolKit tk;

  cdip.setValues ( tk.split ( instrumentLine2, ',' ) );
  cdip.printCalendarDayInstrumentPrice();

  return 0;
}
