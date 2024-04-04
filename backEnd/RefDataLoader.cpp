/******************************************************
 * File description
 *
 * This is a class file that is used to read the entire reference data file.
 *
 * Firstly it will filter IN only certain segments, e.g. the SET1, aka FTSE 100.
 * This is to remove that other asset classes such as bond and funds.
 *
 * Secondly it will filter IN only certain countries, e.g. "United Kingdom".
 * This is to remove companies that are not easily tradable in the UK such as secondarly listed instruments. that have a high cost for tradinig as they are denominted in a foreign currency and carry additional fees.
 *
 * TODO What does the "PIN" filter do? For stock splits / corporate actions?
 */

//C++ standard library headers
#include <regex>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iostream>

//User-defined library headers
#include "RefDataLoader.h"
#include "ToolKit.h"
#include "RefDataInstrument.h"
#include "Logger.h"

using namespace std;
namespace fs = filesystem;
using namespace EodToolKit;

namespace eod
{
RefDataLoader::RefDataLoader()
{
  map<string,RefDataInstrument> refDataInstrumentMap;
}

map<string,RefDataInstrument> RefDataLoader::loadRefData ( map<string,RefDataInstrument> refDataInstrumentMap, string initial_load_directory )
{
  /*
  Here's where to get the exchange code descriptions
  https://docs.londonstockexchange.com/sites/default/files/documents/millennium-exchange-business-parameters.xls
  */

  //TODO Look at full list of Segmeent Codes and decides what else should go in

  /* SET1 - FTSE 100
   * STMM - FTSE 250
   * SET3 - OTHER
   * SSMM - Small Cap
   * AMSM - AIM
   */

  //TODO BUG FOR SOME REASON THERE ARE MISSING TABLES ON THE RPI. POSSIBLY BROKEN ON ARM PROCESSESORS?

  //regex segment_filter("SET1|STMM|SET3|SSMM|AMSM");
  regex segment_filter ( "SET1|STMM|SET3|SSMM|AMSM" );
  regex country_filter ( "United Kingdom" );
  regex instrument_filter ( "PIN" ); //Filter out corporate actions such as 10:1 stock splits.

  string reference_data_file = initial_load_directory + "/lseCompanyList.csv";

  LOG ( "Reading company data from " << reference_data_file );
  if ( fs::exists ( reference_data_file ) ) {
    LOG ( << reference_data_file << " exists" );
  } else {
    ELOG ( << reference_data_file << " doesn't exist" );
    exit ( 1 );
  }

  /* Now that we know that the reference data file exists, load in the csv data to refDataInstrumentMap
  *  Return refDataInstrumentMap
  */

  ifstream inputStream ( reference_data_file );

  string line_read_in = "";

  regex end_of_file ( "^,,," ); //Last line is a series of empty commas
  regex start_of_data ( "^TIDM" ); //Column header above the first line of data begins with TIDM

  ToolKit tk; //Used for the delimiter split function

  int number_of_instruments_in_file = 0;
  int number_of_instruments_loaded = 0;

  regex quotes_in_data ( "\",\"" );

  while ( getline ( inputStream, line_read_in ) ) {
    if ( regex_search ( line_read_in, start_of_data ) ) {
      LOG ( "Read start of data, now reading csv into refDataInstrumentMap" );
      while ( getline ( inputStream, line_read_in ) ) {
        if ( regex_search ( line_read_in, end_of_file ) ) {
          LOG ( "Reached end of file" );
          break;
        } else {
          vector<string> x = tk.split ( line_read_in, ',' );

          //TODO Is a vector really the best way to do this considering I know the number of elements?
          RefDataInstrument temp;

          temp.setValues ( x );
          string mnemonic = temp.getMnemonic();

          /*
          if (regex_search(line_read_in, quotes_in_data))
          {
            TLOG("Found quotes " << line_read_in);
            tk.displayVector(x);
            temp.printRefDataInstrument();
            exit(0);
          }
          */

          if ( regex_search ( temp.getMarketSegmentCode(), segment_filter ) and regex_search ( temp.getCountryOfIncorporation(), country_filter ) and not regex_search ( mnemonic, instrument_filter ) ) {
            refDataInstrumentMap[mnemonic] = temp;
            //refDataInstrumentMap[mnemonic].printRefDataInstrument();
          }
          x.clear();
          number_of_instruments_in_file++;
        }
      }

      number_of_instruments_loaded = refDataInstrumentMap.size();
      LOG ( << to_string ( number_of_instruments_loaded ) << " insturments loaded from a total of " << to_string ( number_of_instruments_in_file ) << " in " << reference_data_file );
    }
  }
  inputStream.close();
  return refDataInstrumentMap;
}

map<string,RefDataInstrument> RefDataLoader::getRefData ( string initial_load_directory )
{
  return loadRefData ( refDataInstrumentMap, initial_load_directory );
}
}
