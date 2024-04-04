#include "EodDate.h"

/*Some helpful information.
 * time_t is an epoch timestamp e.g.the date 20200307 is 1583539200 in epoch time.
 * chrono::system_clock::time_point is a time_point.
 * A time_point object expresses a point in time relative to a clock's epoch.
 * Reference https://en.cppreference.com/w/cpp/chrono/time_point */

using namespace std;

namespace eod
{
EodDate::EodDate()
  : now_time_point ( chrono::system_clock::now() ) //Get a time_point from the system clock
  , mIn_time_t ( chrono::system_clock::to_time_t ( now_time_point ) ) //Convert to_time_t to time_t
{
}

void EodDate::setDate ( const string inDate )
{
  /* Creating a variable named time of type struct tm and initializing all of the
   *  differnt subvalues (hours, minutes seconds etc) to all zeros.
   *  The struct tm is a C++ structure that holds the components of a calendar time,
   *  called the broken-down time */
  struct tm time = {};

  //Input stream class to operate on strings.
  istringstream iss ( inDate );

  //get_time() is a std function to convert iss stream input to tm struct time
  iss >> get_time ( &time, "%Y%m%d" );

  //Converts struct time back to an epoch time mIn_time_t
  mIn_time_t = mktime ( &time );

  //After various conversions above sets the time_point now = method parameter inDate
  now_time_point = chrono::system_clock::from_time_t ( mIn_time_t );
}

void EodDate::setPreviousDate()
{
  now_time_point = now_time_point - chrono::hours ( 24 ); //Subtract 24 hours
  mIn_time_t = chrono::system_clock::to_time_t ( now_time_point );
}

string EodDate::getDate()
{
  stringstream ss;

  //localtime() Convert time_t to tm as local time struct
  //put_time() convert struct time struct to stream
  ss << put_time ( localtime ( &mIn_time_t ), "%Y%m%d" );
  string date_string = ss.str();
  return date_string;
}

string EodDate::getNextDate()
{
  setPreviousDate();
  stringstream ss;
  ss << put_time ( localtime ( &mIn_time_t ), "%Y%m%d" );
  string date_string = ss.str();
  return date_string;
}
}
