// File description
//
// This is a class file that is used to read the entire reference data file.
//
// Firstly it will filter IN only certain segments, e.g. the SET1, aka FTSE 100.
// This is to remove that other asset classes such as bond and funds.
//
// Secondly it will filter IN only certain countries, e.g. "United Kingdom".
// This is to remove companies that are not easily tradable in the UK such as secondarly listed instruments. that have a high cost for tradinig as they are denominted in a foreign currency and carry additional fees.

#include <regex>
#include <fstream>
#include <filesystem>

#include "RefDataLoader.h"
#include "ToolKit.h"
#include "RefDataInstrument.h"
#include "Logger.h"

using namespace std;
namespace fs = filesystem;
using namespace EodToolKit;

namespace eod
{

map< string, RefDataInstrument > *RefDataLoader::loadRefData ( string load_directory )
{
  // Here is where to get the exchange code descriptions
  // https://docs.londonstockexchange.com/sites/default/files/documents/millennium-exchange-business-parameters.xls
  //
  // TODO Look at full list of Segmeent Codes and decides what else should go in
  // SET1 - FTSE 100
  // STMM - FTSE 250
  // SET3 - OTHER
  // SSMM - Small Cap
  // AMSM - AIM

  regex segment_filter ( "SET1|STMM|SET3|SSMM|AMSM" );
  regex country_filter ( "United Kingdom" );
  regex instrument_filter ( "PIN" ); //Filter out corporate actions such as 10:1 stock splits.
  string reference_data_file = load_directory + "/lseCompanyList.csv";

  LOG ( "Reading company data from " << reference_data_file );

  if ( fs::exists ( reference_data_file ) ) {
    LOG ( << reference_data_file << " exists" );
  }
  else {
    ELOG ( << reference_data_file << " doesn't exist" );
    exit ( 1 );
  }

  ifstream input_stream ( reference_data_file );
  string line_read_in = "";
  regex end_of_file ( "^,,," ); //Last line is a series of empty commas
  regex start_of_data ( "^TIDM" ); //Column header above the first line of data begins with TIDM

  ToolKit tk; //Used for the delimiter split function

  int number_of_instruments_in_file = 0;
  int number_of_instruments_loaded = 0;

  regex quotes_in_data ( "\",\"" );
  vector<string> split_line;

  map < string, RefDataInstrument > *ref_data_instrument_map = new map < string, RefDataInstrument >;

  while ( getline ( input_stream, line_read_in ) ) {
    if ( regex_search ( line_read_in, start_of_data ) ) {
      LOG ( "Start of data found, now reading actual csv data into ref_data_instrument_map" );

      while ( getline ( input_stream, line_read_in ) ) {
        if ( regex_search ( line_read_in, end_of_file ) ) {
          LOG ( "Reached end of file" );
          break;
        }
        else {
          split_line = tk.split ( line_read_in, ',' );

          RefDataInstrument rdi;
          rdi.setValues ( split_line );
          string mnemonic = rdi.getMnemonic();

          //Filter out unwanted instruments as we go
          if ( regex_search ( rdi.getMarketSegmentCode(), segment_filter ) and regex_search ( rdi.getCountryOfIncorporation(), country_filter ) and not regex_search ( mnemonic, instrument_filter ) ) {
            ( *ref_data_instrument_map ) [mnemonic] = rdi;
          }
          number_of_instruments_in_file++;
        }
      }

      number_of_instruments_loaded = ( * ref_data_instrument_map ).size();
      LOG ( << to_string ( number_of_instruments_loaded ) << " insturments loaded from a total of " << to_string ( number_of_instruments_in_file ) << " in " << reference_data_file );
    }
  }

  input_stream.close();

  return ref_data_instrument_map;
}
}
