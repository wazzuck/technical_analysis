/* Loads all the end-of-day data from the daily txt/csv file into a <map> of CalendarDayInstrumentPrice(s).  */

#pragma once
#include "CalendarDayInstrumentPrice.h"
#include <vector>
#include <map>

using namespace std;

namespace eod
{
	class PriceLoader
	{
	//Class to store the data for each instrument, can this be a vector?
	public:
        PriceLoader();
        map<string, CalendarDayInstrumentPrice*>* getPrices(const string fileName);
        void printPrices(const string mnemonic);

		typedef map<string, CalendarDayInstrumentPrice*> cdipMapType;
		cdipMapType* cdipMapPointer;
        
	private:
		map<string, CalendarDayInstrumentPrice*> cdipMap;
	};
}
