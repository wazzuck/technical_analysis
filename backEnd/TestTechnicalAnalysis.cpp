//C++ standard library headers
#include <iostream> //for cout
#include <fstream> //for reading in a file
#include <regex>
#include <chrono>
#include <filesystem>
#include <vector>

/*
#include <iostream>
#include <fstream> //for reading in a file
#include <filesystem>
#include <map>
#include <chrono>
*/

#include "EodDate.h"
#include "CalendarDayInstrumentPrice.h"
#include "InstrumentPrices.h"
#include "ToolKit.h"
#include "Logger.h"
#include "PriceLoader.h"
#include "TechnicalAnalysis.h"

#include "ta-lib/ta_libc.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

namespace fs = filesystem;

int main(int argc, char *argv[])
{
        const char *HOME_DIRECTORY = getenv("HOME");
        const auto DATA_DIRECTORY_BASE_PATH = string(HOME_DIRECTORY) + "/data/";

		EodDate nowDate;

		string initial_date_to_be_loaded = "";

        if(argc > 1)
        {
            initial_date_to_be_loaded = argv[1];
            LOG("Running custom start date " << argv[1]);
            nowDate.setDate(initial_date_to_be_loaded);
        }
        else
        {
            initial_date_to_be_loaded = nowDate.getDate();
        }

        string date_to_be_loaded = initial_date_to_be_loaded;

		cout << "DATE " << date_to_be_loaded << endl;

		int number_of_tries = 100; //This is set to an arbitrarily large number of retries
		string starting_directory = "";
		bool initial_load_directory_exists = 0;

        string initial_load_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;
        for (int i = 0; i < number_of_tries; i++)
        {
            if (fs::exists(initial_load_directory))
            {
                starting_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;
                initial_load_directory_exists = 1;
                break;
            }
            else
            {
                WLOG("initial_load_directory  " << initial_load_directory << " does not exist, trying to load reference data from a previous date");
                nowDate.setPreviousDate();
                date_to_be_loaded = nowDate.getDate();
                initial_load_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;
            }
        }	

        LOG("Testing TechnicalAnalysis Class");

		TLOG("Loading Reference Data");
        RefDataInstrument testRdi;
        const string refArr[] = {"VOD","VODAFONE GROUP PLC","ORD USD0.20 20/21","GB00BH4HKS39","SHRS","Shares","Telecommunications","Telecommunications","24/02/2014","United Kingdom","GBX","MAIN MARKET","Premium Equity Commercial Companies","SET1","FE10"};
        vector<string> refVec (refArr, refArr + sizeof(refArr) / sizeof(refArr[0]) );
        testRdi.setValues(refVec);

        TLOG("Loading Fundamental Data");
        Fundamental fund;
        static const string arr[] = {"VOD", "Vodafone Group Plc", "Telecommunications Services", "Mobile Telecommunications", "0", "-0.05", "0", "265276800", "0", "5.15", "138.68", "91.72"};
        //static const string arr[] = {"AAS","Aberdeen Standard Asia Focus Plc","Equity Investment Instruments","Equity Investment Instruments","0","-0.5","0","0","0","0","0","0"};
        vector<string> fundVec (arr, arr + sizeof(arr) / sizeof(arr[0]) );
        fund.setValues(fundVec);
    
		TLOG("Creating InstrumentPrices objects and copying Reference and Fundamental data");
        InstrumentPrices ip;
        ip.rdi = testRdi;
        ip.funda = fund;
        
		vector<string> dataDirectorySubDirectoryVector;

        //The next section removes quotes "" from before and after the absolute directory path for each dataDirectorySubDirectory in DATA_DIRECTORY_BASE_PATH
        stringstream ss;
        for (const auto & dataDirectorySubDirectory : fs::directory_iterator(DATA_DIRECTORY_BASE_PATH))
        {
            //DLOG(dataDirectorySubDirectory);

            ss.str("");
            ss << dataDirectorySubDirectory;
            string directory = ss.str();
            directory.erase(remove(directory.begin(),directory.end(),'\"'),directory.end());
            dataDirectorySubDirectoryVector.insert(dataDirectorySubDirectoryVector.begin(), directory);
        }

        PriceLoader pl;
        TechnicalAnalysis ta;

		//To avoid having to load in all of the data store in all of the files,
		//Instead limit the data loaded to just what is required for the technical
		//analysis calculations.

		const int MAX_DAYS_LOADED = ta.fastPeriod + 90;

        int days_loaded_count = 0;
        for (auto j = 0; j < (int) dataDirectorySubDirectoryVector.size(); j++)
        {
            const string DIRECTORY_TO_LOAD_FROM = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;

            if (find(dataDirectorySubDirectoryVector.begin(), dataDirectorySubDirectoryVector.end(), DIRECTORY_TO_LOAD_FROM) != dataDirectorySubDirectoryVector.end())
            {
                string file_to_load = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded + "/LSE_" + date_to_be_loaded;

                typedef map<string, CalendarDayInstrumentPrice*> cdipMapType;
                cdipMapType* cdipMapPointer;
                //cdipMapPointer = new cdipMapType();
                cdipMapPointer = pl.getPrices(file_to_load);

				string mnemonic;

				for (auto const&[key, val] : *cdipMapPointer)
                {
                    mnemonic = key;
					//DLOG( << mnemonic);
                    if(mnemonic == ip.rdi.getMnemonic())
                    {
                        ip.addCdip(date_to_be_loaded, (*cdipMapPointer)[mnemonic]);
                    }
                }

				DLOG("Loading of the price data for " << date_to_be_loaded << " complete");

                days_loaded_count++;

                if (days_loaded_count == MAX_DAYS_LOADED)
                {
                    LOG("Loaded the required number of days EOD price data");
                    break;
                }
            }
            date_to_be_loaded = nowDate.getNextDate();
        }

        //DLOG("Running Market Cap");
        //ip.pa.t->setMarketCap = ta.getMarketCap(ip.getNextLastCalendarDayInstrumentClosePrice(0), ip.funda.getShares());

        DLOG("Running fastEMA");
        ip.pa.t->emaFastPeriodVecPtr = ta.taCalculateEMA(ip.pa.getCdipMapPointer(), ta.fastPeriod);

        DLOG("Running slowEMA");
        ip.pa.t->emaSlowPeriodVecPtr = ta.taCalculateEMA(ip.pa.getCdipMapPointer(), ta.slowPeriod);

        DLOG("Running MACD");
        ip.pa.t->macdVecPtr = ta.taCalculateMACD(ip.pa.getCdipMapPointer());

        ip.PrintInstrumentPrices();

        /*

        exit(0);

		//ta.taGetEMA(ip.pa.getCdipMapPointer());
        //ip.ipSetEMA(ta.taGetEMA(), fastPeriod);
		//ta.clearEMAVec();

        //ta.setPercentageChanges(ip, 2);
        //LOG("Percentage change " << ta.getPercentageChanges()[2]);

        /*
         *
		ta.taSetEMA(cdipMap, slowPeriod);
		ip.ipSetEMA(ta.taGetEMA(), slowPeriod);
		ta.clearEMAVec();

		ta.taSetMACD(cdipMap, fastPeriod, slowPeriod, signalPeriod);
		ip.ipSetMACD(ta.taGetMACD(), fastPeriod + 7);

		ta.taSetStochastic(cdipMap);
		ip.ipSetStochastic(ta.taGetStochastic());
		ta.clearStochasticVec();

		ta.setOneDayPercentageChange(ip.getNextLastCalendarDayInstrumentClosePrice(0), ip.getNextLastCalendarDayInstrumentClosePrice(1));
		LOG("One Day " << ta.getOneDayPercentageChange());

		ta.setThreeDayPercentageChange(ip.getNextLastCalendarDayInstrumentClosePrice(0), ip.getNextLastCalendarDayInstrumentClosePrice(3));
		LOG("Three day " << ta.getThreeDayPercentageChange());

		ta.setFiveDayPercentageChange(ip.getNextLastCalendarDayInstrumentClosePrice(0), ip.getNextLastCalendarDayInstrumentClosePrice(5));
		LOG("Five day " << ta.getFiveDayPercentageChange());

		ip.PrintInstrumentPrices();
		exit(0);

		*/

        /*
        //Sneaky TA Lib Stuff
        TA_RetCode res = TA_Initialize();
        if(res != TA_SUCCESS)
        {
            fprintf( stderr, "Error TA_Initialize: %d", res);
            return -1;
        }

        const int closePriceArray_size = sizeof(closePriceArray) / sizeof(double);
        // TA_MACD_Lookback returns number of elements used to initialize a moving average (33 in this case)
        // 12, 26, 9 - are default MACD specific periods
    
        int res_size = closePriceArray_size - TA_MACD_Lookback(12, 26, 9);
        // res_size is 17 - MACD can't be calculated for first 33 values as they're used for its initialization

        // dynamically allocate output arrays
        double* outMACD = new double[res_size];
        double* outMACDSignal = new double[res_size];
        double* outMACDHist = new double[res_size];

        int outBegIdx= 0;
        int outNbElement = 0;

        // get TA_MACD results
        res = TA_MACD(0, closePriceArray_size-1,                       // data range
                    closePriceArray,                                 // data pointer
                    12, 26, 9,                            // TA Func specific args
                    &outBegIdx, &outNbElement,            // relative index and size of results
                    outMACD, outMACDSignal, outMACDHist); // arrays of results

        if (res == TA_SUCCESS) {
            for (int i = 0; i < outNbElement; i++){
                printf("Result for day #%d outMACD: %f outMACDSignal: %f outMACDHist: %f\n",
                    outBegIdx+i, *(outMACD+i), *(outMACDSignal+i), *(outMACDHist+i));
            }
        } else
            fprintf( stderr, "Error TA_MACD: %d", res);

        delete outMACD;
        delete outMACDSignal;
        delete outMACDHist;

        res = TA_Shutdown();
        if(res != TA_SUCCESS)
        {
            fprintf( stderr, "Error TA_Shutdown: %d", res);
            return -1;
        }
        
        */
        
        return 0;
}
