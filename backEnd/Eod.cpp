
//  File description
//
// This is the main file of the Eod program.
//
// This program is designed to perform technical analaysis using end-of-day data.
//
// Program Layout
//
// - Load reference data.
//      This is a list of instruments downloaded from a file on the exchange website
//
// - RefDataInstrument.h
//
// - RefDataLoader.h
//
// - Load fundamental data
//
// - Fundamental.h
//
// - EodDate
//      Class for handling the dates, uses for recursively going back over the days starting with the most recent day
//
// - InstrumentPrices.h
//      Parent class of fundamental,  and reference data classes
//
// - CalendarDayInstrumentPrice/h
//      A single days EOD prices for a single instrument
//
// - PriceLoader.h
//
// - ItemNames.h
//
// - ToolKit.h
//
// - TechnicalAnalysis.h
//
// - Sqlite.h
//
// Usage //TODO move this documentation to a man page -?/--help method
//
//  Eod.exe (Optional Start Date)
//
//    Start Date allow for testing from a specific date
//

//C system headers
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

//C++ standard library headers
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

//User-defined library headers
#include "EodDate.h"
#include "RefDataLoader.h"
#include "PriceLoader.h"
#include "InstrumentPrices.h"
#include "Technical.h"
#include "FundamentalLoader.h"
#include "TechnicalAnalysis.h"
#include "Logger.h"

//Third party headers
#include "Sqlite.h"
#include "ta-lib/ta_libc.h" // Uses a BSD licensed Technical Analysis library see https://ta-lib.org/

using namespace std;
using namespace eod;

namespace fs = filesystem;

//##############################################################################
// GLOBALS
//##############################################################################
mutex mtx;
condition_variable cv;
int num_active_threads = 0;
int max_running_threads = 1;

int main ( int argc, char *argv[] )
{
  //Find out how many CPU cores are available.
  unsigned int cores = thread::hardware_concurrency();

  if ( cores == 0 || cores == 1 ) {
    WLOG ( "Defaulting to 1 worker thread." );
  }
  else {
    LOG ( "Number of available cores: " << cores );
    max_running_threads = cores - 1; // Save a single core for any other processes running.
  }

  //Data is stored in dated direstories where refence, fundamental and price data is snapshotted each day.
  string starting_directory = "";

  const char *HOME_DIRECTORY = getenv ( "HOME" );
  const auto DATA_DIRECTORY_BASE_PATH = string ( HOME_DIRECTORY ) + "/data/";

  const auto start = chrono::system_clock::now();
  const time_t start_time = chrono::system_clock::to_time_t ( start );

  LOG ( "################################################" );
  LOG ( "Starting eod at " << ctime ( &start_time ) );
  LOG ( "################################################" );

  EodDate NowDate;

  //Keep the initial_date_to_be_loaded for later to give the .db database file the initial date in the filename
  string initial_date_to_be_loaded = "";

  if ( argc > 1 ) {
    initial_date_to_be_loaded = argv[1];
    LOG ( "Running custom start date " << argv[1] );
    NowDate.setDate ( initial_date_to_be_loaded );
  }
  else {
    initial_date_to_be_loaded = NowDate.getDate();
  }

  string date_to_be_loaded = initial_date_to_be_loaded;

  //##############################################################################
  // The next section of the program is concerned with loading in reference data.
  //##############################################################################
  // If a directory with today's date is not found the program will try to look further
  // back in the calendar. Useful if running tests on a day where there's no data
  // e.g. a weekend. The number of allowed days is stricter in the live environment
  // vs a laptop which is used for testing.

  // Above the getenv method is used to get the $HOME environment variable.
  // For some reason this does not work for $HOSTNAME.
  char hostname[HOST_NAME_MAX];
  gethostname ( hostname, HOST_NAME_MAX );
  string hostname_string = string ( hostname );
  LOG ( "Running on " << hostname_string );

  int number_of_tries = 0;

  if ( hostname_string == "penguin" ) {
    LOG ( "Running in development environment" );
    number_of_tries=100; //Set to an arbitrarily large number of retries.
  }
  else {
    LOG ( "Running in live environment" );
    number_of_tries = 2; //Set to 2 for issue recreation in the live environment on weekends.
  }

  bool initial_load_directory_exists = 0;

  string initial_load_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;

  for ( int i = 0; i < number_of_tries; i++ ) {
    if ( fs::exists ( initial_load_directory ) ) {
      starting_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;
      initial_load_directory_exists = 1;
      break;
    }
    else {
      WLOG ( "initial_load_directory  " << initial_load_directory << " does not exist, trying to load reference data from a previous date" );
      NowDate.setPreviousDate();
      date_to_be_loaded = NowDate.getDate();
      initial_load_directory = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;
    }
  }

  if ( initial_load_directory_exists == 0 ) {
    ELOG ( "Could not find an initial starting directory to load from. Exiting" );
    exit ( 1 );
  }

  RefDataLoader RefLoader;
  map< string, RefDataInstrument > *reference_data_map;
  reference_data_map = RefLoader.loadRefData ( initial_load_directory );

  //##############################################################################
  // The next section of the program is concerned with loading in fundamental data.
  //##############################################################################
  FundamentalLoader FundaLoader;
  string fundamental_file = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded + "/" + "Fundamentals_LSE.txt";
  map<string,Fundamental> fundamental_data_map;
  fundamental_data_map = FundaLoader.getFundamentalData ( fundamental_file );

  //##############################################################################
  // The next section covers the creation of an instrument price object for each instrument in referenceDataMap
  //##############################################################################

  map<string, InstrumentPrices *> *instrument_prices_map = nullptr;
  instrument_prices_map = new map<string, InstrumentPrices *>();

  for ( auto element : *reference_data_map ) {
    //             CalendarDayInstrumentPrice holds 1 EOD price for 1 day for 1 instrument
    //             e.g. "VOD,20200212,150.74,155.52,150.16,152.64,46428587"
    //
    //             CalendarDayInstrumentPrice<s> <-- note the 's' holds 1 full EOD days worth of CalendarDayInstrumentPrice, essentially loading a full EOD .csv file into memory.
    //
    //             InstrumentPrices is a class that makes all of the data easy to reference by sorting the data into a map of InstrumentPrices, where each InstrumentPrice contains the following:
    //
    //             InstrumentPriceMap
    //
    //      * KEY mnemonic (VOD)
    //      -> VALUE InsrumentPrice Object
    //              - Reference Data
    //              - Fundamental Data
    //        - Price data stored in a map
    //                - All the prices for the instrument are held in a Calendar Day Instrument Price <map> called cdipMap
    //        - Technical analysis calulated data
    //
    //          NOTE The reference data can be out of date, the fundamental data is representative of the current list of actively traded instruments.
    //          At least for now both the reference data and the funademental data both need to be present. If the data is in the reference data,
    // but not in the fundamental data then the instrument is dropped in the next section and excluded from any later parts of the program.

    string mnemonic = element.first;

    //Iterate over the fundamentalDataMap and only create an InstrumentPrice object if the instrument is also in the referenceDataMap.
    if ( fundamental_data_map.find ( mnemonic ) != fundamental_data_map.end() ) {
      //For each instrument found in the reference data file add a new InstrumentPrices object into a map (instrumentPriceMap) that is referenced by mnemonic.

      InstrumentPrices *ip = new InstrumentPrices;

      ip->funda = fundamental_data_map[mnemonic];
      ip->rdi = ( *reference_data_map ) [mnemonic];

      //As above the price data is loaded in the next section, but create the instrumentPriceMap entry to hold that data.
      ( *instrument_prices_map ) [mnemonic] = ip;

      //For now just add the reference data and fundamental data. The price data will be loaded later on.
    }
    else {
      //WLOG( << mnemonic << " is in the reference data but not the fundamental data. Not creating an instrumentPriceMap[" << mnemonic << "] entry");
      continue;
    }
  }

  //##############################################################################
  // The next section is concerned with loading in EOD price data.
  //##############################################################################
  LOG ( "Loading EOD Price Data" );
  vector<string> data_sub_directories;

  //The next section removes quotes "" from before and after the absolute directory path for each dataDirectorySubDirectory in DATA_DIRECTORY_BASE_PATH
  //  e.g. For each dataSubDirectory
  //  DEBUG : "/home/neville/data/20240206"
  //  DEBUG : "/home/neville/data/20240207"
  //  DEBUG : "/home/neville/data/20240208"

  stringstream ss;

  for ( const auto &data_sub_directory : fs::directory_iterator ( DATA_DIRECTORY_BASE_PATH ) ) {
    //DLOG(<< dataSubDirectory);

    ss.str ( "" );
    ss << data_sub_directory;
    string directory = ss.str();
    directory.erase ( remove ( directory.begin(),directory.end(),'\"' ),directory.end() );
    data_sub_directories.insert ( data_sub_directories.begin(), directory );
  }

  // * For each dated directory in the DATA_DIRECTORY_BASE_PATH load the price data from the LSE_<date>.txt/csv file into
  // * a temporary CalendarDayInstrumentPrice map (cdipMap).
  // *
  // * cdipMap
  // * * KEY Mnemonic
  // * -> VALUE CaledarDayInstrumentPrice Object
  //       *
  // * CalendarDayInstrumentPrice Object has the basic charactaristics.
  // * string Mnemonic
  // * string Date
  // * float ClosePrice
  // * ...
  //       * See CalendarDayInstrumentPrice.h for full list of data stored.
  // *
  // * Next loop through the list of instruments loaded from the EOD prices. Check the instrument is also in the reference data.
  //       * If it is then copy (TODO Change to pointer) the price data into the instrumentPriceMap[mnemonic]cdipMap.
  //       * If it is !not then there is no point in keeping the data so erase() the instrumentPriceMap[mnemonic) from the map.
  // *
  //       * 3 Do the opposite to the above, but going through the list of instruments loaded from the reference data.
  //       * If it's not then there's no point in keeping the data, if we only have reference data, but not price data.
  // *
  // * Check if MAX_DAYS_LOADED as this int represents the maximum required number of days.
  // * If future technical calculations require more days to be loaded then increase MAX_DAYS_LOADED
  // * Once the loop has reached that max number of EOD prices then stop loading price data.
  //To avoid having to load in all of the data store in all of the files,
  //Instead limit the data loaded to just what is required for the technical
  //analysis calculations.

  const int MINIMUM_REQUIRED_DAYS = 120;

  vector<thread> threads;

  int days_loaded_count = 0;

  for ( auto j = 0; j < ( int ) data_sub_directories.size(); j++ ) {
    const string DIRECTORY_TO_LOAD_FROM = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded;

    if ( find ( data_sub_directories.begin(), data_sub_directories.end(), DIRECTORY_TO_LOAD_FROM ) != data_sub_directories.end() ) {
      string file_to_load = DATA_DIRECTORY_BASE_PATH + date_to_be_loaded + "/LSE_" + date_to_be_loaded;

      //START OF LAMBDA WRAPPER FUNCTION
      auto lambdaWrapper = [=] ( string file_to_load, string date_to_be_loaded, map<string, InstrumentPrices *> ( *instrumentPricesMap ) ) {
        //DLOG("num_active_threads" << num_active_threads);
        unique_lock<mutex> lock ( mtx );
        cv.wait ( lock, [&]() {
          return num_active_threads < max_running_threads;
        } );
        num_active_threads++;

        return [=]() {
          PriceLoader pl;

          //Create a calendar day instrument price map, indexed by the string date.
          typedef map<string, CalendarDayInstrumentPrice *> cdipMapType;
          cdipMapType *cdipMapPointer;

          //For each EOD data file run it through a single PriceLoader object which returns a CalendarDayInstrumentPrice map pointer
          cdipMapPointer = pl.getPrices ( file_to_load );

          string mnemonic;

          //For the most recently loaded day go through each instrument and add into the instruentPriceMap
          for ( auto const&[key, val] : *cdipMapPointer ) {
            mnemonic = key;

            //Loop through all the instrumentPriceMap by each menmonic and check that the instrument is also in the most recently loaded day's prices.
            //if (cdipMap.contains(mnemonic))
            if ( ( *instrumentPricesMap ).contains ( mnemonic ) ) {
              ( *instrumentPricesMap ) [mnemonic]->addCdip ( date_to_be_loaded, ( *cdipMapPointer ) [mnemonic] );
            }
            else {
              //Most likely a corporate action leading to a delisting
              //LOG(<< mnemonic << " is in the reference and fundamental data but not in price data for " << date_to_be_loaded);

              //TODO enable this line to delete unneeded instrument
              //Need to test this before using properly as deleting objects in memory can cause unexpeced results.
              //Requires an iterator
              //delete ( *instrumentPricesMap )[mnemonic];
            }
          }

          num_active_threads--;
          cv.notify_one();
        };
      };
      //END OF LAMBDA WRAPPER FUNCTION

      threads.emplace_back ( lambdaWrapper ( file_to_load, date_to_be_loaded, instrument_prices_map ) );

      days_loaded_count++;

      if ( days_loaded_count == MINIMUM_REQUIRED_DAYS ) {
        //Required number of threads now defined break out of loop and start the threads running.
        break;
      }
    }

    date_to_be_loaded = NowDate.getNextDate();
  }

  // Wait for all threads to finish
  for ( auto &thread : threads ) {
    thread.join();
  }

  LOG ( "Loaded the required number of days EOD price data" );

  //##############################################################################
  // Technical Analysis section
  //##############################################################################
  map<string, InstrumentPrices *>::iterator instrument_price_iter = ( *instrument_prices_map ).begin();

  while ( instrument_price_iter != ( *instrument_prices_map ).end() ) {
    map<string, CalendarDayInstrumentPrice *> *cdip_map = instrument_price_iter->second->pa.getCdipMapPointer();

    TechnicalAnalysis *ta = new TechnicalAnalysis();

    vector<double> *ema_slow_period_vector = new vector<double>;
    vector<double> *ema_fast_period_vector = new vector<double>;
    vector<double> *macd_vector = new vector<double>;

    ema_slow_period_vector = ta->taCalculateEMA ( cdip_map, ta->slowPeriod );
    ema_fast_period_vector = ta->taCalculateEMA ( cdip_map, ta->fastPeriod );

    //There are problems with talib MACD
    //https://community.backtrader.com/topic/1934/standard-macd-vs-talib-macd-different-values
    //TODO Write own simple MACD to get started??
    //HACK There is aligment issues as the MACD is offset by 1 day
    //TODO WHY DOES THE SIGNAL PERIOD MATTER? THIS HACK SEEMS TO WORK?
    //HACK I have no idea why the first 7 are skipped?

    macd_vector = ta->taCalculateMACD ( instrument_price_iter->second->pa.getCdipMapPointer() );
    //ipElement->second->pa.t->setMACD(ta->taCalculateMACD(ipElement->second->pa.getCdipMapPointer()));

    //TODO MACD has the smallest number of samples and by only storing those values we are wasting some of the additonally EMA calculations, maybe only calculate what is requried?
    //i.e.
    // macdPtr 87    <-- Stop loading the data when we reach 87, thow other ema values away.
    // emaSlowPeriodVecPtr 95
    // emaFastPeriodVecPtr 109

    int count = 0;
    int macdVecSize = ( *macd_vector ).size();

    for ( auto it = cdip_map->begin(); it != cdip_map->end(); ++it ) {
      if ( count == macdVecSize ) {
        break;
      }

      //DLOG ( "(*ema_fast_period_vector)[count] " << ( *ema_fast_period_vector ) [count] );
      it->second->setEMAFast ( ( *ema_fast_period_vector ) [count] );
      //DLOG ( "(*ema_slow_period_vector)[count] " << ( *ema_slow_period_vector ) [count] );
      it->second->setEMASlow ( ( *ema_slow_period_vector ) [count] );
      //DLOG ( "(*macdVecPtr)[count] " << ( *macd_vector ) [count] );
      it->second->setMACD ( ( *macd_vector ) [count] );

      count++;
    }
    instrument_price_iter++;
  }

  LOG ( "Finished calculating technical data" );

  //##############################################################################
  // Sqlite
  //##############################################################################

  //Give the output database filename the date of the latest chronoligocal date loaded
  //Output the database file to the /dev/shm/ ram drive.
  string databaseFilename = "/dev/shm/" + initial_date_to_be_loaded + ".eod.db";

  if ( fs::exists ( databaseFilename ) ) {
    ELOG ( "Database with filename " << databaseFilename << " already exists" );
    ELOG ( "Removing " << databaseFilename );
    remove ( databaseFilename.c_str() );
  }

  Sqlite sqli ( databaseFilename );

  //for (auto ipElement = instrumentPriceMap.cbegin(); ipElement != instrumentPriceMap.cend() ; )
  //for (auto key : instrumentPriceMap)

  for ( auto [key, value] : ( *instrument_prices_map ) ) {
    sqli.addReferenceData ( ( *instrument_prices_map ) [key] );
    sqli.addFundamental ( ( *instrument_prices_map ) [key] );
    sqli.addPriceData ( ( *instrument_prices_map ) [key] );


    //Getting difference since previous day(s)
    //
    // ta.setPercentageChanges(value, 1);
    // ta.setPercentageChanges(value, 3);
    // ta.setPercentageChanges(value, 5);
    // ta.setMarketCap(value);
    //
    // sqli.addTechnical();
    //Do market wide calculations for 3,5,30
    //Do performance verus the market to find market beaters
  }

  sqli.closeDB();

  // BUG CURRENTLY TWO BUGS
  // Instrument CBUY is loaded from reference data, but is not removed when it is not in price data

  const auto end = chrono::system_clock::now();
  const time_t end_time = chrono::system_clock::to_time_t ( end );
  const time_t runtime = end_time - start_time;

  LOG ( "################################################" );
  LOG ( "End eod at " << ctime ( &end_time ) );
  LOG ( "Runtime " << runtime << " seconds" );
  LOG ( "################################################" );

  exit ( 0 );
}
