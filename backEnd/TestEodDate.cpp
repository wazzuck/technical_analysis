#include <iostream>

#include "EodDate.h"
#include "Logger.h"

using namespace std;
using namespace eod;

int main()
{
  LOG ( "Testing EodDate Class" );

  EodDate nowDate;

  string starting_date = "20200307";

  LOG ( "Starting date is " << starting_date );

  nowDate.setDate ( starting_date );

  for ( int i=5; i>0; i-- ) {
    LOG ( << nowDate.getNextDate() );
  }

  return 0;
}
