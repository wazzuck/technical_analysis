/*
Returns change in price as a +/- percentrage (Not sure if currency change is any good?)
Can be used to filter by market cap to filter out low volatility stocks with wide spreads.
*/

#pragma once

using namespace std;

namespace eod
{
class TechnicalAnalysis
{
public:
  TechnicalAnalysis();

  vector <double>* taCalculateEMA ( map<string, CalendarDayInstrumentPrice*>* cdipMap, int numberOfDays );
  vector <double>* taCalculateMACD ( map <string, CalendarDayInstrumentPrice*>* cdipMap );
  vector<double>* taCalculateStochastic(map <string, CalendarDayInstrumentPrice*>* cdipMap );
  //vector<double> taGetPPO();
  //vector<string> taG<etBSI();
  double taGetLowest ( map <string, CalendarDayInstrumentPrice*>* cdipMap, int range );
  double taGetHighest ( map <string, CalendarDayInstrumentPrice*>* cdipMap, int range );

  double taGetPercentageChanges(map< string, CalendarDayInstrumentPrice* >* cdipMap, int number_of_days);
  double taGetMarketCap ( float LastPrice, int numberShares );

  int fastPeriod;
  int slowPeriod;
  int signalPeriod;

private:
  vector<double> *outEMAVecPtr;
  vector<double> *outMACDVecPtr;
  vector<double> *outStochasticVec;
  //vector<double> outPPOVec;
  //vector<string> outBSIVec; //TODO replace with enum
  //bool maxDaysCheck; //TODO Common check to see if there is enough data loaded
};
}
