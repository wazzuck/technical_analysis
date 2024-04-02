/*
 * Class to read the list of instruments on the official LSE stock exchange spreadsheet (list is updated monthtly)
*/

#pragma once

#include <string>
#include <cstring>
#include <vector>
#include "ToolKit.h"

using namespace std;
using namespace EodToolKit;

namespace eod
{
	class RefDataInstrument
	{
	public:
		RefDataInstrument();
        //void copyRdi(const RefDataInstrument inRdi);
		void setMnemonic(const string& mnemonic);
        void setIssuerName(const string& issuerName);
        void setInstrumentName(const string& instrumentName);
        void setIsin(const string& isin);
        void setMifirIdentifierCode(const string& mifirIdentifierCode);
        void setMifirIdentifierDescription(const string& mifirIdentifierDescription);
        void setIcbIndustry(const string& icbIndustry);
        void setIcbSuperSectorName(const string& icbSuperSectorName);
        void setStartDate(const string& startDate);
        void setCountryOfIncorporation(const string& countryOfIncorporation);
        void setTradingCurrency(const string& tradingCurrency);
        void setLseMarket(const string& lseMarket);
        void setFcaListingCategory(const string& fcaListingCategory);
        void setMarketSegmentCode(const string& marketSegmentCode);
        void setMarketSectorCode(const string& marketSectorCode);

        string getMnemonic();
        string getIssuerName();
		string getInstrumentName();
		string getIsin();
		string getMifirIdentifierCode();
		string getMifirIdentifierDescription();
		string getIcbIndustry();
		string getIcbSuperSectorName();
        string getStartDate();
		string getCountryOfIncorporation();
		string getTradingCurrency();
		string getLseMarket();
		string getFcaListingCategory();
		string getMarketSegmentCode();
		string getMarketSectorCode();
        
		void setValues(const vector<string> &instrumentVector);
		void printRefDataInstrument();

        ToolKit tk;
        
	protected:
		string mMnemonic;
		string mIssuerName;
		string mInstrumentName;
		string mIsin;
		string mMifirIdentifierCode;
		string mMifirIdentifierDescription;
		string mIcbIndustry;
		string mIcbSuperSectorName;
        string mStartDate;
		string mCountryOfIncorporation;
		string mTradingCurrency;
		string mLseMarket;
		string mFcaListingCategory;
		string mMarketSegmentCode;
		string mMarketSectorCode;
	};
}
