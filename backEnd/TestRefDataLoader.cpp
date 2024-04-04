//C++ standard library headers
#include <filesystem>
#include <map>

//User-defined library headers
#include "RefDataInstrument.h"
#include "RefDataLoader.h"
#include "CalendarDayInstrumentPrice.h"
#include "Logger.h"
//#include <fstream> //for reading in a file

using namespace std;
using namespace eod;
namespace fs = filesystem;

int main ( int argc, char *argv[] )
{

  LOG ( "Testing RefDataLoader Class" );

  string ref_data_directory = "/home/neville/data/";
  string test_date="20231123";

  if ( argc > 1 ) {
    test_date = argv[1];
  }

  string start_date_folder = ref_data_directory + "/" + test_date;

  string company_List = start_date_folder;

  RefDataLoader rdl;

  map<string,RefDataInstrument> test_ref_data_map;

  test_ref_data_map = rdl.getRefData ( company_List );

  test_ref_data_map["VOD"].printRefDataInstrument();
  test_ref_data_map["CARR"].printRefDataInstrument();

  return 0;
}
