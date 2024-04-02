#include <iostream>
#include <vector>

#include "PriceAnalysis.h"
#include "Logger.h"
#include "ToolKit.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{
	LOG("Testing PriceAnalysis Class");

	/*
	string cdipInput1A = "BT.A,20231123,123.15,125,121.6,122.65,2788357";
	string cdipInput2A = "VOD,20231123,123.15,125,121.6,122.65,2788357";

	string cdipInput1B = "BT.A,20231122,99.9,99.9,99.9,99.9,2788357";
	string cdipInput2B = "VOD,20231122,123.15,125,121.6,122.65,2788357";

	ToolKit tk;

	CalendarDayInstrumentPrice cdip1A, cdip2A, cdip1B, cdip2B;
	*/

	for (int i = 0; i < 10; i++)
	{
		PriceAnalysis *p = new PriceAnalysis();
		DLOG("p->t " << p->t);
	}

	exit(0);

	/*
	cdip1A.setValues(tk.split(cdipInput1A, ','));
	pa1.addCalendarDayInstrumentPrice("20231123", &cdip1A);
	cdip1B.setValues(tk.split(cdipInput1B, ','));
	pa1.addCalendarDayInstrumentPrice("20231122", &cdip1B);
	LOG("Last Price " << pa1.getNextLastCalendarDayInstrumentClosePrice(0));

	LOG("This should be an address " << pa1.getCdipMapPointer());
	exit(0);

	/*
	cdip2A.setValues(tk.split(cdipInput2A, ','));
	pa2.addCalendarDayInstrumentPrice("20231123", &cdip2A);
	cdip2B.setValues(tk.split(cdipInput2B, ','));
	pa2.addCalendarDayInstrumentPrice("20231122", &cdip2B);
	*/

	return 0;
}
