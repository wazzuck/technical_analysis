/* Class to hold the results from the TechnicalAnalysis class */

#ifndef TECHNICAL_H
#define TECHNICAL_H

#pragma once

#include <vector>
#include <string>

using namespace std;

namespace eod
{
class Technical
{
public:
  Technical();
  ~Technical();

  void setMarketCap ( double inMarketCap );
  void setEMASlowPeriod ( vector <double>* emaSlowPeriod );
  void setEMAFastPeriod ( vector <double>* emaFastPeriod );
  void setMACD ( vector <double>* macd );

  void setStochastic ( const double& );
  void setPPO ( const double& );
  void setBSI ( const string& );

  double getEMASlowPeriod();
  double getEMAFastPeriod();
  double getMACD();
  // double getStochastic();
  // double getPPO();
  // string getBSI();
  void setValues ( const vector<string> &v );
  void printTechnical ( const string mnemonic );

  //TODO These need to be made private and accessed through a public method
  vector <double> *emaSlowPeriodVecPtr;
  vector <double> *emaFastPeriodVecPtr;
  vector <double> *macdVecPtr;

private:
  double mMarketCap;

  double mOneDayPercentageChange;
  double mThreeDayPercentageChange;
  double mFiveDayPercentageChange;
};
}

#endif
