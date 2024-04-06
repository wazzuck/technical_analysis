//C++ standard library headers
#include <filesystem>
#include <map>

//User-defined library headers
#include "RefDataInstrument.h"
#include "RefDataLoader.h"
#include "CalendarDayInstrumentPrice.h"
#include "Logger.h"

using namespace std;
using namespace eod;
namespace fs = filesystem;

int main ( int argc, char *argv[] )
{
  LOG ( "Testing RefDataLoader Class" );

  string ref_data_directory = "/home/neville/data/";
  string test_date="20240208";

  if ( argc > 1 ) {
    test_date = argv[1];
  }

  string load_directory = ref_data_directory + test_date;
  RefDataLoader rdl;
  map< string, RefDataInstrument > *ref_data_map;
  ref_data_map = rdl.loadRefData ( load_directory );
  ( *ref_data_map )["BU_P"].printRefDataInstrument();

  return 0;
}
