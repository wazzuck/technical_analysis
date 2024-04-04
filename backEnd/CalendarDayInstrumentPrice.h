/* CalendarDayInstrumentPrice.h
 *
 * Bottom level building block class to store values for 1 instruments for 1 day.
 *
 * It has an associated csv file loader class FundamentalLoader.h
 *
 * These values are loaded from the daily download from the EOD Data service.
 *
 * These additional Technical Analyis values are then calculated
 *
 * TODO Add field descriptions inline
 *
 */

#pragma once

#include <vector>
#include <string>

using namespace std;

namespace eod
{
class CalendarDayInstrumentPrice
{
public:
  CalendarDayInstrumentPrice();
  void setMnemonic ( const string& mnemonic );
  void setDate ( const string& date );
  void setOpen ( const string& s );
  void setHigh ( const string& s );
  void setLow ( const string& s );
  void setClose ( const string& s );
  void setVolume ( const string& s );
  void setEMAFast ( const double& d );
  void setEMASlow ( const double& d );
  void setMACD ( const double& d );

  string getMnemonic();
  string getDate();
  float getOpen();
  float getHigh();
  float getLow();
  float getClose();
  int getVolume();
  double getEMAFast();
  double getEMASlow();
  double getMACD();

  void setValues ( const vector<string> &v );
  void printCalendarDayInstrumentPrice();

private:
  string mMnemonic;
  string mDate;
  float mOpen;
  float mHigh;
  float mLow;
  float mClose;
  int mVolume;
  double mEMASlow;
  double mEMAFast;
  double mMACD;
};
}
