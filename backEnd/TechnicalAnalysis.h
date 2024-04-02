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

        vector <double>* taCalculateEMA(map<string, CalendarDayInstrumentPrice*>* cdipMapPtr, int numberOfDays);
        vector <double>* taCalculateMACD(map <string, CalendarDayInstrumentPrice*>* cdipMap);

        //vector<double> taGetEMA();
        //void clearEMAVec();
        //vector<double> taGetMACD();
        //void clearMACDVec();

        //void taSetStochastic(map <string, CalendarDayInstrumentPrice> cdipMap);
        vector<double> taGetStochastic();
        void clearStochasticVec();
        
        //void taSetPPO(map <string, CalendarDayInstrumentPrice> cdipMap);
        vector<double> taGetPPO();
        void clearPPOVec();
        
        //void taSetBSI(map <string, CalendarDayInstrumentPrice> cdipMap, float LastMACD, float NextLastMACD);
        vector<string> taGetBSI();
        void clearBSIVec();
        
        double taGetLowest(map <string, CalendarDayInstrumentPrice> cdipMap, int range);
        double taGetHighest(map <string, CalendarDayInstrumentPrice> cdipMap, int range);

        //void setOneDayPercentageChange(double LastPrice, double ComparisonPrice);
        //void setThreeDayPercentageChange(double LastPrice, double ComparisonPrice);
        //void setFiveDayPercentageChange(double LastPrice, double ComparisonPrice);
        double getPercentageChanges(double LastPrice, double ComparisonPrice);

        /*
        double getOneDayPercentageChange();
        double getThreeDayPercentageChange();
        double getFiveDayPercentageChange();
        */

        double getMarketCap(float LastPrice, int numberShares);

        int fastPeriod;
		int slowPeriod;
		int signalPeriod;

    private:
        vector<double> *outEMAVecPtr;
        vector<double> *outMACDVecPtr;
        //vector<double> outStochasticVec;
        //vector<double> outPPOVec;
        //vector<string> outBSIVec; //TODO replace with enum
	   	//double mOneDayPercentageChange;
        //double mThreeDayPercentageChange;
        //double mFiveDayPercentageChange;
          //Used for EMA / SMA calculations
		bool maxDaysCheck; //TODO Common check to see if there is enough data loaded
    };
}
