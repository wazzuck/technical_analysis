#include "Scratch.h"
#include "Logger.h"
#include "ToolKit.h"

#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main ()
{
    LOG("Hello");

    string instrumentLine2 = "BT.A,20231123,123.15,125,121.6,122.65,2788357";

    Scratch s;

    CalendarDayInstrumentPrice cdip;

    ToolKit tk;

	cdip.setValues(tk.split(instrumentLine2, ','));

    s.addCalendarDayInstrumentPrice("20231123", cdip);

    return 0;
}
