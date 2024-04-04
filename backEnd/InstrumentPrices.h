/* Class that has this structure
 *
 * It's used to bind these 3 data storage classes together so that all data for all instruments can be passed around and easily accessed.
 *
 * - RefDataInstrument rdi;
 * - Fundamental funda;
 * - PriceAnalysis
 */

#pragma once

#include <string>
#include <map>
#include <vector>

#include "RefDataInstrument.h"
#include "Fundamental.h"
#include "PriceAnalysis.h"

using namespace std;
using namespace eod;

namespace eod
{
class InstrumentPrices: public RefDataInstrument // I can't remember, but I think this is an example of inhertence
{
public:
  InstrumentPrices();
  ~InstrumentPrices();

  RefDataInstrument rdi;
  Fundamental funda;
  void addCdip ( string inDate, CalendarDayInstrumentPrice* cdipPtr );
  CalendarDayInstrumentPrice* getCdip ( string inDate );

  const float getNextLastCalendarDayInstrumentClosePrice ( int numberToGoBack );

  void setMarketCap ( double inMarketCap );

  void ipSetSMA ( vector <double> inSMA, int numDays );
  void ipSetEMA ( vector <double> inEMA, const int numDays );
  void ipSetMACD ( vector<double> inMACD, int inFastPeriod );
  void ipSetStochastic ( vector<double> inStochastic );
  void ipSetPPO ( vector<double> inPPO );
  void ipSetBSI ( vector <string> );

  float getNextLastMACD ( int numberToGoBack );

  PriceAnalysis pa;  //TODO Maybe pa should be private with an interface through InstrumentPrices. THis is the quicket way to work in the meantime.

  void PrintInstrumentPrices();
private:
};
}
