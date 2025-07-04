#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#include "CalendarDayInstrumentPrice.h"
#include "TechnicalAnalysis.h"
#include "ta-lib/ta_libc.h"
#include "Logger.h"

using namespace std;
using namespace eod;

namespace eod
{
TechnicalAnalysis::TechnicalAnalysis()
  : fastPeriod ( 12 )
  , slowPeriod ( 26 )
  , signalPeriod ( 9 )
{
  TA_Initialize();
}

double TechnicalAnalysis::taGetPercentageChanges ( map <string, CalendarDayInstrumentPrice *> *cdipMap, int number_of_days )
{
  auto last_cdip = ( *cdipMap ).rbegin();
  //last_cdip->second->printCalendarDayInstrumentPrice();
  auto last_close_price = last_cdip->second->getClose();

  auto n_last_cdip = next ( ( *cdipMap ).rbegin(), number_of_days );
  //n_last_cdip->second->printCalendarDayInstrumentPrice();
  auto n_last_close_price = n_last_cdip->second->getClose();

  double percentage_change = ( ( last_close_price - n_last_close_price ) / n_last_close_price ) * 100;
  //DLOG("percentage_change " << percentage_change);

  return percentage_change;
}


/*
 * TODO I'd prefer to move the check for whether enough days have been loaded from the Main function to inside TA.'
 *
bool TechnicalAnalysis::maxDaysCheck()
{
if (MAX_DAYS_LOADED < fastPeriod)
{
  LOG("NOT ENOUGH DAYS OF EOD DATA LOADED");
  exit(1);
}
}
*/

//EMA - Exponential Moving Average
//Best video on calculating EMA
//https://www.youtube.com/watch?v=ezcwBDsDviE
vector <double> *TechnicalAnalysis::taCalculateEMA ( map <string, CalendarDayInstrumentPrice *> *cdipMapPtr, int numberOfDays )
{

  /*
   * I'll leave this here for now, but it might be more useful to print this out after the calculations are completed'
   *
   *
   *
  for (const auto& cdip : (*cdipMapPtr))
  {
      cdip.second->printCalendarDayInstrumentPrice();
  }
  */


  // Create a closePriceArray the size of the cdipMap that is input
  double closePriceArray[ ( *cdipMapPtr ).size()];

  //Copy each close price from the map into the array
  int count = 0;
  for ( auto item: ( *cdipMapPtr ) ) {
    closePriceArray[count] = item.second->getClose();
    count++;
  }

  //Find out how big the closePriceArray is.
  const int closePriceArraySize = sizeof ( closePriceArray ) / sizeof ( double );


  //Find out how big the vector that contains the results should be.
  int resultsSize = closePriceArraySize - TA_EMA_Lookback ( numberOfDays );



  /*
   *
   *
   * To be removed

  cout << "numberOfDays " << numberOfDays << endl;
  cout << "closePriceArraySize " << closePriceArraySize << endl;
  cout << "resultsSize " << resultsSize << endl;

  exit(0);

  */

  // dynamically allocate output arrays
  double *outEMA = new double[resultsSize]; // Pointer to an array

  int outBegIdx= 0;
  int outNbElement = 0;
  int outBegIdxEMA= 0;

  TA_RetCode res = TA_EMA ( 0, closePriceArraySize -1,        // data range
                            closePriceArray,                  // data pointer
                            numberOfDays,                     // TA Func specific args
                            &outBegIdx, &outNbElement,        // relative index and size of results
                            outEMA );                         // arrays of results

  if ( res == TA_SUCCESS ) {

    for (int i = 0; i < outNbElement; i++)
    {
        printf("Result for day #%d outEMA: %f\n", outBegIdxEMA+i, *(outEMA+i));
    }

  }
  else {
    fprintf ( stderr, "Error TA_EMA: %d", res );
    exit ( 1 );
  }

  outEMAVecPtr = new vector<double> ();

  for ( int i = 0; i < resultsSize; i++ ) {



    cout << outEMA + i << endl;



    outEMAVecPtr->push_back ( * ( outEMA + i ) );


  }



  return outEMAVecPtr;
}






/*
  vector<double> TechnicalAnalysis::taGetEMA()
  {
      return outEMAVec;
  }

  void TechnicalAnalysis::clearEMAVec()
  {
      outEMAVec.clear();
  }
*/

/* MACD - Moving Average Convergence Divergence
 *
 * https://www.youtube.com/watch?v=eob4wv2v--k&ab_channel=RaynerTeo
 *
 * MACD Calculation
 * To bring in this oscillating indicator that fluctuates above and below zero, a simple MACD calculation is required. By subtracting the 26-day exponential moving average (EMA) of a security's price from a 12-day moving
 * average of its price, an oscillating indicator value comes into play. Once a trigger line (the nine-day EMA) is added, the comparison of the two creates a trading picture. If the MACD value is higher than the nine-day EMA,
 * it is considered a bullish moving average crossover.
 *
 *It's helpful to note there are a few well-known ways to use the MACD:
 *
 *Foremost is the watching for divergences or a crossover of the center line of the histogram; the MACD illustrates buy opportunities above zero and sell opportunities below.
 *Another is noting the moving average line crossovers and their relationship to the center line.
 */

vector<double> *TechnicalAnalysis::taCalculateMACD ( map<string, CalendarDayInstrumentPrice *> *cdipMapPtr )
{
  double closePriceArray[ ( *cdipMapPtr ).size()];

  int count = 0;

  for ( auto item: ( *cdipMapPtr ) ) {
    closePriceArray[count] = item.second->getClose();
    count++;
  }

  const int closePriceArray_size = sizeof ( closePriceArray ) / sizeof ( double );

  DLOG ( "taSetMACD closePriceArray_size: " << closePriceArray_size );

  int res_size = closePriceArray_size - TA_MACD_Lookback ( fastPeriod, slowPeriod, signalPeriod );
  // res_size is 17 - MACD can't be calculated for first 33 values as they're used for its initialization

  DLOG ( "res_size: " << res_size );

  // dynamically allocate output arrays
  double *outMACD = new double[res_size];
  double *outMACDSignal = new double[res_size];
  double *outMACDHist = new double[res_size];

  int outBegIdx = 0;
  int outNbElement = 0;

  // get TA_MACD results
  TA_RetCode res = TA_MACD ( 0, closePriceArray_size-1,                     // data range
                             closePriceArray,                                 // data pointer
                             fastPeriod,
                             slowPeriod,
                             signalPeriod,                            // TA Func specific args
                             &outBegIdx, &outNbElement,            // relative index and size of results
                             outMACD, outMACDSignal, outMACDHist ); // arrays of results

  if ( res == TA_SUCCESS ) {
    /*
    for (int i = 0; i < outNbElement; i++){
        printf("Result for day #%d outMACD: %f outMACDSignal: %f outMACDHist: %f\n",
            outBegIdx+i, *(outMACD+i), *(outMACDSignal+i), *(outMACDHist+i));
    }
    */
  }
  else {
    fprintf ( stderr, "Error TA_MACD: %d", res );
  }

  outMACDVecPtr = new vector<double> ();

  for ( int i = 0; i < res_size; i++ ) {
    outMACDVecPtr->push_back ( * ( outMACD + i ) );
  }

  delete[] outMACD;
  delete[] outMACDSignal;
  delete[] outMACDHist;

  return outMACDVecPtr;
}

/*
vector<double> TechnicalAnalysis::taGetMACD()
{
    return outMACDVec;
}

void TechnicalAnalysis::clearMACDVec()
{
    outMACDVec.clear();
}
*/




/*Stochastic Oscillator
* https://www.investopedia.com/articles/trading/08/macd-stochastic-double-cross.asp
*
* Explainer video
* https://www.youtube.com/watch?v=THKFlE3119E&ab_channel=RaynerTeo

* For each day calculate the stochastic value and apply it directly back into the ip object
* This will iterate over a single insturment's cdip data
* There are two components to the stochastic oscillator: the %K and the %D. The %K is the main line indicating the number of time periods, and the %D is the moving average of the %K.
*
* Understanding how the stochastic is formed is one thing, but knowing how it will react in different situations is more important. For instance:
*
* Common triggers occur when the %K line drops below 20—the stock is considered oversold, and it is a buying signal.
* If the %K peaks just below 100 and heads downward, the stock should be sold before that value drops below 80.
* Generally, if the %K value rises above the %D, then a buy signal is indicated by this crossover, provided the values are under 80. If they are above this value, the security is considered overbought.
*
* https://www.investopedia.com/articles/technical/073001.asp
*
* Good explainer video on K + D line.
*
* It can be beneficial to use stochastics in conjunction with and an oscillator like the relative strength index (RSI) together.
* TODO Impement relative strength index (RSI)
*
* CP=Most recent closing price
* L14=Lowest price of the 14 previous trading sessions
* H14=Highest price of the same 14 previous trading sessions
*
*
*/

vector<double> *TechnicalAnalysis::taCalculateStochastic ( map <string, CalendarDayInstrumentPrice *> *cdipMap ) {

  outStochasticVec = new vector<double>();

  int count = 0;

  for (auto cdip = ( * cdipMap ).rbegin(); cdip != ( * cdipMap ).rend(); ++cdip) {

    // According to CMC here https://www.cmcmarkets.com/en-gb/trading-guides/what-is-a-stochastic-indicator
    // The high / low should be taken from the last 14 days?
    // At the moment there is an issue where the sample is currently coming fromt he same day and if the price does not move then
    // a division by 0 leads to a "nan" float which is then rejected when being put in the sqlite DB

    int numberOfDaysLookBack = 14;

    double high;
    double low;
    double close;

    double lowToCloseRange;
    double lowToHighRange;
    double stochastic;

    // Start with excluding the last element, then the last two, and so on
    for (size_t excludeCount = 1; excludeCount < ( * cdipMap ).size(); ++excludeCount) {
        // Determine the iterator to stop at
        auto stopIt = ( * cdipMap ).end();
        for (size_t i = 0; i < excludeCount; ++i) {
            --stopIt;
        }

        for (auto cdip = ( * cdipMap ).begin(); cdip != stopIt; ++cdip) {
            // Create a new raw pointer to a new map containing the reduced elements
            map<string, CalendarDayInstrumentPrice*>* reducedMap = new map<string, CalendarDayInstrumentPrice*>(cdipMap->begin(), stopIt);

            high = taGetHighest ( reducedMap , numberOfDaysLookBack);
            low = taGetLowest ( reducedMap, numberOfDaysLookBack);

            close = cdip->second->getClose();
        }

        lowToCloseRange = close - low;
        lowToHighRange = high - low;
        stochastic = ( lowToCloseRange / lowToHighRange ) * 100;

        outStochasticVec->push_back ( stochastic );
    }

    count += 1;
    if ( count == slowPeriod ) {
      break;
    }
  }

  /*
  for (int value : ( *outStochasticVec ) ) {
    ELOG ("Stochastic "<< value);
  }
  */

  DLOG("outStochasticVec size " << ( *outStochasticVec ).size());

//  exit(0);

  return outStochasticVec;
}

/* Get lowest price over the last numberDays
*  Look through all the low values to find which is lowest */
double TechnicalAnalysis::taGetLowest ( map <string, CalendarDayInstrumentPrice *> *cdipMap, int numberDays )
{
  //LOG("Running taGetLowest");

  float lowest = 0.0;
  float low = 0.0;

  int count = 0;
  //for (auto cdip: ip.cdipMap)

  // Iterate through the map in reverse order
  for ( auto rit = ( * cdipMap ).rbegin(); rit != ( *cdipMap ).rend(); ++rit ) {
    //TLOG("Checking for date " << rit->second.getDate());

    if ( count == numberDays - 1 ) {
      //LOG("taGetLowest count == numberDays break");
      break;
    }

    if ( count == 0 ) {
      lowest = rit->second->getLow();
      //TLOG("Setting initial lowest value " << lowest);
      count++;
      continue;
    }
    else {
      low = rit->second->getLow();
      //LOG("Checking low value " << to_string(low));

      if ( low < lowest ) {
        lowest = low;
        //LOG("Found new lowest value " << to_string(low));
      }

      count++;
    }
  }
  return lowest;
}

/* Get highest price over the last numberDays
*  Look through all the low values to find which is highest */
double TechnicalAnalysis::taGetHighest ( map <string, CalendarDayInstrumentPrice *> *cdipMap, int numberDays )
{
  //LOG("Running taGetHighest");

  double highest = 0.0;
  double high = 0.0;
  int count = 0;

  // Iterate through the map in reverse order
  for ( auto rit = ( *cdipMap ).rbegin(); rit != ( *cdipMap ).rend(); ++rit ) {
    //TLOG("Checking for date " << rit->second.getDate());

    if ( count == numberDays - 1 ) {
      //LOG("taGetHighest count == numberDays break");
      break;
    }

    if ( count == 0 ) {
      highest = rit->second->getHigh();
      //TLOG("Setting initial highest value " << highest);
      count++;
      continue;
    }
    else {
      high = rit->second->getHigh();
      //LOG("Checking high value " << to_string(high));

      if ( high > highest ) {
        highest = high;
        //LOG("Found new highest value " << to_string(high));
      }

      count++;
    }
  }
  return highest;
}

/*Price Position Oscillator
 *
 * https://www.youtube.com/watch?v=T4O4n8QtWCU
/*
    void TechnicalAnalysis::taSetPPO(map <string, CalendarDayInstrumentPrice> cdipMap)
  {
        double MACD = 0.0;
        double EMA = 0.0;
        double PPO = 0.0;

        for (auto item: cdipMap)
        {
            MACD = item.second.getMACD();

            if (MACD == 0)
            {
                continue;
            }

            EMA = item.second.getEMAFastPeriod();

            PPO = MACD / EMA * 100;

            // DLOG("=======");
            // DLOG("MACD :: " << MACD);
            // DLOG("EMA :: " << EMA);
            // DLOG("PPO :: " << PPO);

            outPPOVec.push_back(PPO);

            //closePriceArray[count] = item.second.getClose();
            //count++;
        }
    }


    vector<double> TechnicalAnalysis::taGetPPO()
    {
        return outPPOVec;
    }

    void TechnicalAnalysis::clearPPOVec()
    {
        outPPOVec.clear();
    }

     /*Buy / Sell Indicator
     *
     * MACD and Stochastic: A Double-Cross Strategy
     *
     * https://www.investopedia.com/articles/trading/08/macd-stochastic-double-cross.asp
     * This article aims to encourage traders to look for and identify a simultaneous bullish MACD crossover along with a bullish stochastic crossover and use these indicators as the entry point to trade.
     *
     *
     * */


/*
void TechnicalAnalysis::taSetBSI(map <string, CalendarDayInstrumentPrice> cdipMap, float LastMACD, float NextLastMACD)
{
   for (int k = 0; k < cdipMap.size() -1; k ++)
   {
       string BSI = "UNDEF";

       //DLOG("####taSetBSI beginning of comparison ");

       if (NextLastMACD == 0)
       {
           continue;
       }

       //DLOG("LastMACD: " << LastMACD);
       //DLOG("NextLastMACD: " << NextLastMACD);

       if (LastMACD > 0 and NextLastMACD < 0)
       {
           //cout << "LastMACD > 0 and NextLastMACD < 0" << endl;
           BSI = "CROSS. MACD has turned positive. Weak Buy indicator";
       }
       else if (LastMACD < 0 and NextLastMACD > 0)
       {
           //cout << "LastMACD < 0 and NextLastMACD > 0" << endl;
           BSI = "CROSS. MACD has turned negative. Weak Sell indicator";
       }
       else if (LastMACD > 0 and NextLastMACD > 0 and LastMACD < NextLastMACD)
       {
           //cout << "LastMACD > 0 and NextLastMACD > 0 and LastMACD < NextLastMACD" << endl;
           BSI = "Positive MACD is getting smaller. Upside momentum is weakening";
       }
       else if (LastMACD > 0 and NextLastMACD > 0 and LastMACD > NextLastMACD)
       {
           //cout << "LastMACD > 0 and NextLastMACD > 0 and LastMACD > NextLastMACD"  << endl;
           BSI = "Positive MACD is getting larger. Upside momentum is strengthening";
       }
       else if (LastMACD < 0 and NextLastMACD < 0 and LastMACD > NextLastMACD)
       {
           //cout << "LastMACD < 0 and NextLastMACD < 0 and LastMACD > NextLastMACD"  << endl;
           BSI = "MACD is less negative. Downside momentum is weakening";
       }
       else if (LastMACD < 0 and NextLastMACD < 0 and LastMACD < NextLastMACD)
       {
           //out << "LastMACD < 0 and NextLastMACD < 0 and LastMACD < NextLastMACD"  << endl;
           BSI = "Negative MACD is more negative. Downside momentum is strengthening";
       }

//            else if (LastMACD == NextLastMACD)
//            {
//                //cout << "LastMACD == NextLastMACD"  << endl;
//                BSI = "No change in MACD";
//            }

       outBSIVec.push_back(BSI);

       //cout << "BSI :: " << BSI << endl;
   }
}


vector<string> TechnicalAnalysis::taGetBSI()
{
   return outBSIVec;
}

void TechnicalAnalysis::clearBSIVec()
{
   outBSIVec.clear();
}
*/


/* Market Cap
*
*
*/
double TechnicalAnalysis::taGetMarketCap ( float LastPrice, int numberShares )
{
  //TODO This appears to broken in the EOD download data with the numberShares set as "0" for a large number os stocks.
  // Not sure how to fix this?

  return LastPrice * numberShares;
}
}
