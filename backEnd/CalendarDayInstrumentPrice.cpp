#include <iostream>
#include <regex>
#include <vector>
#include "CalendarDayInstrumentPrice.h"
#include "Logger.h"
#include "ToolKit.h"

//TODO Move to ToolKit Used for date convertion
#include <iomanip>                                                                                                                                            
#include <iostream>                                  
#include <sstream>
#include <ctime>

using namespace EodToolKit;

namespace eod
{
	CalendarDayInstrumentPrice::CalendarDayInstrumentPrice()
        : mMnemonic("")
		, mDate("")
        , mOpen(0.0)
		, mHigh(0.0)
		, mLow(0.0)
		, mClose(0.0)
        , mVolume(0)
		, mEMASlow(0.0)
		, mEMAFast(0.0)
		, mMACD(0.0)
	{
	}

	void CalendarDayInstrumentPrice::setMnemonic(const string& mnemonic)
    {
		//TLOG( << mnemonic);
		mMnemonic = mnemonic;

	   	regex dot_L(".*\*.L");

		if (regex_search(mMnemonic, dot_L))
		{
			//DLOG("Removing the .L from " << mMnemonic);
			mMnemonic.erase(mMnemonic.length()-2);
        }

		regex pattern("^..\..");
		if (regex_match(mMnemonic, pattern))
		{
			//TLOG("found regex");
			mMnemonic.replace(2, 1, "_");
		}
	}

	string CalendarDayInstrumentPrice::getMnemonic()
    {
		return mMnemonic;
	}

	void CalendarDayInstrumentPrice::setDate(const string& date)
	{
		mDate = date;

		if (mDate.find('-') != string::npos)
		{
			//DLOG("Converting timestamp");

			tm tm = {}; 
			istringstream ss(mDate);
			ss >> get_time(&tm, "%d-%b-%Y");

			int year = tm.tm_year + 1900; // tm_year is years since 1900
			int month = tm.tm_mon + 1; // tm_mon is months since January, 0-11
			int day = tm.tm_mday; // tm_mday is the day of the month, 1-31

			int intDate = year * 10000 + month * 100 + day;

			//cout << "Printing date " << intDate << endl; // Outputs: 20231123

			mDate = to_string(intDate);
		}
	}

	string CalendarDayInstrumentPrice::getDate()
	{
		return mDate;
	}

	void CalendarDayInstrumentPrice::setOpen(const string& open)
	{
        mOpen = stof(open);
	}

	float CalendarDayInstrumentPrice::getOpen()
    {
		return mOpen;
	}

	void CalendarDayInstrumentPrice::setHigh(const string& high)
	{
		mHigh = stof(high);
	}

	float CalendarDayInstrumentPrice::getHigh()
    {
		return mHigh;
	}

	void CalendarDayInstrumentPrice::setLow(const string& low)
	{
		mLow = stof(low);
	}

	float CalendarDayInstrumentPrice::getLow()
    {
		return mLow;
	}

	void CalendarDayInstrumentPrice::setClose(const string& close)
	{
		mClose = stof(close);
	}

	float CalendarDayInstrumentPrice::getClose()
    {
		return mClose;
	}

	void CalendarDayInstrumentPrice::setVolume(const string& volume)
	{
		mVolume = stoi(volume);
	}

	int CalendarDayInstrumentPrice::getVolume()
	{
		return mVolume;
	}

	void CalendarDayInstrumentPrice::setEMAFast(const double& emaFast)
	{
		mEMAFast = emaFast;
	}

	double CalendarDayInstrumentPrice::getEMAFast()
	{
		return mEMAFast;
	}

	void CalendarDayInstrumentPrice::setEMASlow(const double& emaSlow)
	{
		mEMASlow = emaSlow;
	}

	double CalendarDayInstrumentPrice::getEMASlow()
	{
		return mEMASlow;
	}

	void CalendarDayInstrumentPrice::setMACD(const double& macd)
	{
		mMACD = macd;
	}

	double CalendarDayInstrumentPrice::getMACD()
	{
		return mMACD;
	}

	void CalendarDayInstrumentPrice::setValues(const vector<string> &v)
	{
        CalendarDayInstrumentPrice::setMnemonic(v[0]);
		CalendarDayInstrumentPrice::setDate(v[1]);
        CalendarDayInstrumentPrice::setOpen(v[2]);
		CalendarDayInstrumentPrice::setHigh(v[3]);
		CalendarDayInstrumentPrice::setLow(v[4]);
		CalendarDayInstrumentPrice::setClose(v[5]);
        CalendarDayInstrumentPrice::setVolume(v[6]);
	}

	void CalendarDayInstrumentPrice::printCalendarDayInstrumentPrice()
	{
		DLOG("#########################################");
		DLOG("Printing CalendarDayInstrumentPrice Data for " << mMnemonic + " " + mDate);
		DLOG("#########################################");
		DLOG("Mnemonic :: " << mMnemonic);
        DLOG("Date :: " << mDate);
        DLOG("Open :: " << mOpen);
		DLOG("High :: " << mHigh);
		DLOG("Low :: " << mLow);
		DLOG("Close :: " << mClose);
        DLOG("Volume :: " << mVolume);
		DLOG("emaFast :: " << mEMAFast);
		DLOG("emaSlow :: " << mEMASlow);
        DLOG("macd :: " << mMACD);
	}
}
