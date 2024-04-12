/*
DO I HAVE A TechAn object attached to each cdip?

HOW DO I DO THIS?

DO I MARRY THEM UP IN THE SQL?

I THINK ONE TechAn may feed into another TechAn so best assocaite them to the cdip in some way?

Split the CDIP class into 2 parts?

For each date:-
- Basic cdip data
- Technicals to store the results of the technical analysis calculations
- Parent class so that both can be bound by a single date in in a DATE -> CLASS map

- Call the class ideas?
Data
NumericData
PriceAndTechnical

- Keep the Technical class seperate for the TechnicalAnalysis class, i.e. the class doing all of the calculations.lllll
*/

#include "Logger.h"
#include "Technical.h"
#include <vector>
#include <string>

//using namespace eod;
using namespace std;

namespace eod
{
Technical::Technical()
  : mMarketCap ( 0.0 )
    /*
    , mOneDayPercentageChange(0.0)
    , mThreeDayPercentageChange(0.0)
    , mFiveDayPercentageChange(0.0)
    , mSMASlowPeriod(0.0)
    , mSMAFastPeriod(0.0)
    , mEMASlowPeriod(0.0)
    , mEMAFastPeriod(0.0)
    , mMACD(0.0)
    , mStochastic(0.0)
    , mPPO(0.0)
    , mBSI("UNDEF")
    */
{
  emaFastPeriodVecPtr = new vector<double>();
}

Technical::~Technical()
{
}

void Technical::setMarketCap ( double inMarketCap )
{
  mMarketCap = inMarketCap;
}

void Technical::setEMASlowPeriod ( vector <double> *emaSlowPeriod )
{
  emaSlowPeriodVecPtr = emaSlowPeriod;
}

void Technical::setEMAFastPeriod ( vector <double> *emaFastPeriod )
{
  emaFastPeriodVecPtr = emaFastPeriod;
}

void Technical::setMACD ( vector <double> *macd )
{
  macdVecPtr = macd;
}

    // void Technical::setStochastic(const double& stochastic)
    // {
    //     mStochastic = stochastic;
    // }

// void Technical::setValues ( const vector<string> &v )
// {
//   LOG ( "Test" );
//           CalendarDayInstrumentPrice::setMnemonic(v[0]);
//           CalendarDayInstrumentPrice::setDate(v[1]);
//           CalendarDayInstrumentPrice::setOpen(v[2]);
//           CalendarDayInstrumentPrice::setHigh(v[3]);
//           CalendarDayInstrumentPrice::setLow(v[4]);
//           CalendarDayInstrumentPrice::setClose(v[5]);
//           CalendarDayInstrumentPrice::setVolume(v[6]);
// }

void Technical::printTechnical ( const string mnemonic )
{
  DLOG ( "#########################################" );
  DLOG ( "Printing Technical data for " << mnemonic );
  DLOG ( "#########################################" );

  //DLOG("Printing address of emaSlowPeriodVecPtr " << emaSlowPeriodVecPtr);

  DLOG ( "Market Cap :: " << mMarketCap );

  for ( const auto element : *emaSlowPeriodVecPtr ) {
    DLOG ( "Print emaSlowPeriod " << element );
  }

  for ( const auto &element : *emaFastPeriodVecPtr ) {
    DLOG ( "Printing emaFastPeriod " << element );
  }

  for ( const auto &element : *macdVecPtr ) {
    DLOG ( "Printing macd " << element );
  }

  /*
  DLOG("One Day Percentage Change :: " << mOneDayPercentageChange);
  DLOG("Three Day Percentage Change :: " << mThreeDayPercentageChange);
  DLOG("Five Day Percentage Change :: " << mFiveDayPercentageChange);
  DLOG("SMASlowPeriod :: " << mSMASlowPeriod);
  DLOG("SMAFastPeriod :: " << mSMAFastPeriod);
  DLOG("EMASlowPeriod :: " << mEMASlowPeriod);
  DLOG("EMAFastPeriod :: " << mEMAFastPeriod);
  DLOG("MACD :: " << mMACD);
  DLOG("Stochastic :: " << mStochastic);
  DLOG("PPO :: " << mPPO);
  DLOG("BSI :: " << mBSI);
  */
}
}
