/* Class that has this structure
 * 
 * It's used to bind these classes together for each instrument
 * 
 * - RefDataInstrument rdi;
 * - Fundamental funda;
 * - PriceAnalysis
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "InstrumentPrices.h"
#include "PriceAnalysis.h"
#include "Logger.h"

using namespace std;
using namespace eod;

namespace eod
{
    InstrumentPrices::InstrumentPrices()
    {
        //pa = new PriceAnalysis(); // Allocate memory for pa
    }

    InstrumentPrices::~InstrumentPrices()
    {
        DLOG("Calling ~InstrumentPrices() destructor");
        //delete pa;
    }

    void InstrumentPrices::addCdip(string inDate, CalendarDayInstrumentPrice* cdipPtr)
    {
        //DLOG("InstruentPrices incoming cdipPtr " << cdipPtr);
        pa.addCalendarDayInstrumentPrice(inDate, cdipPtr);
    }

    CalendarDayInstrumentPrice* InstrumentPrices::getCdip(string inDate)
    {
        //DLOG("InstruentPrices outgoing pointer " << pa.getCalendarDayInstrumentPrice(inDate));
        return pa.getCalendarDayInstrumentPrice(inDate);
    }

    const float InstrumentPrices::getNextLastCalendarDayInstrumentClosePrice(int numberToGoBack = 0)
    {
        return pa.getNextLastCalendarDayInstrumentClosePrice(numberToGoBack);
    }

    void InstrumentPrices::setMarketCap(double inMarketCap)
    {
        pa.setMarketCap(inMarketCap);
    }


    void InstrumentPrices::PrintInstrumentPrices()
    {
		string mnemonic = rdi.getMnemonic();

        LOG("Printing Instrument Prices for " << mnemonic);
        rdi.printRefDataInstrument();
        funda.printFundamental();

        // Assuming pa is initialized correctly
        map<string, CalendarDayInstrumentPrice*>* cdipMapPtr = pa.getCdipMapPointer();

        DLOG("cdipMap.size() " << (*cdipMapPtr).size());

        for (const auto& pair : (*cdipMapPtr))
        {
            CalendarDayInstrumentPrice* cdipPtr = pair.second;
            DLOG("InstrumentPrices PrintInstrumentPrices cdipPtr " << cdipPtr);
            if (cdipPtr != nullptr)
            {
                cdipPtr->printCalendarDayInstrumentPrice(); // Assuming the function name is printCalendarDayInstrumentPrice
            }
            else
            {
                cout << "No CalendarDayInstrumentPrice object found for this date." << endl;
            }
        }

        //This may no longer be requried
        //pa.t->printTechnical(mnemonic);
    }
}
