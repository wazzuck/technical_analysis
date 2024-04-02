#include <iostream>
#include <vector>
#include "RefDataInstrument.h"
#include <string>
#include <algorithm>
#include <regex>
#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace EodToolKit;

namespace eod
{
	RefDataInstrument::RefDataInstrument()
        :mMnemonic("")
		,mIssuerName("")
		,mInstrumentName("")
		,mIsin("")
		,mMifirIdentifierCode("")
		,mMifirIdentifierDescription("")
		,mIcbIndustry("")
		,mIcbSuperSectorName("")
        ,mStartDate("")
		,mCountryOfIncorporation("")
		,mTradingCurrency("")
		,mLseMarket("")
		,mFcaListingCategory("")
		,mMarketSegmentCode("")
		,mMarketSectorCode("")
	{
    }

    void RefDataInstrument::setMnemonic(const string& tidm)
	{
		RefDataInstrument::mMnemonic = tidm;
        
        //if tidm ends with "." remove the "."
        char lastChar = mMnemonic.back();
        if (strncmp(&lastChar, ".", 1) == 0)
        {
            //DLOG("has a " << lastChar);
            mMnemonic.pop_back();
        }

		//Replace BP.A with BP_A
		regex pattern("^..\..");
		if (regex_match(mMnemonic, pattern))
		{
			//TLOG("found regex");
			mMnemonic.replace(2, 1, "_");
		}
	}

    string RefDataInstrument::getMnemonic()
	{
		return mMnemonic;	
    }
	
	void RefDataInstrument::setIssuerName(const string& issuerName)
	{
		mIssuerName = issuerName;
		//TODO This is to remove apostrophes as they're difficult to match in a string comparison
		mIssuerName = tk.removePunctuation(mIssuerName);
	}

	string RefDataInstrument::getIssuerName()
	{
		return mIssuerName;
	}
	
	void RefDataInstrument::setInstrumentName(const string& instrumentName)
	{
		mInstrumentName = instrumentName;
		//TODO This is to remove apostrophes as they're difficult to match in a string comparison
		mInstrumentName = tk.removePunctuation(mInstrumentName);
	}

	string RefDataInstrument::getInstrumentName()
	{
		return mInstrumentName;
	}
	
    void RefDataInstrument::setIsin(const string& isin)
	{
		mIsin = isin;
	}    
	
    string RefDataInstrument::getIsin()
	{
		return mIsin;
	}
	
	void RefDataInstrument::setMifirIdentifierCode(const string& mifirIdentifierCode)
	{
		mMifirIdentifierCode = mifirIdentifierCode;
	}
	
	string RefDataInstrument::getMifirIdentifierCode()
	{
		return mMifirIdentifierCode;
	}
	
	void RefDataInstrument::setMifirIdentifierDescription(const string& mifirIdentifierDescription)
	{
		mMifirIdentifierDescription = mifirIdentifierDescription;
	}
	
	string RefDataInstrument::getMifirIdentifierDescription()
	{
		return mMifirIdentifierDescription;
	}

	void RefDataInstrument::setIcbIndustry(const string& icbIndustry)
	{
		mIcbIndustry = tk.removeSpaces(icbIndustry);
	}

	string RefDataInstrument::getIcbIndustry()
	{
		return mIcbIndustry;
	}

    void RefDataInstrument::setIcbSuperSectorName(const string& icbSuperSectorName)
	{
		mIcbSuperSectorName = tk.removeSpaces(icbSuperSectorName);
        //DLOG("NEV in " << icbSuperSectorName);
        //DLOG("NEV out " << mIcbSuperSectorName);
	}

    string RefDataInstrument::getIcbSuperSectorName()
	{
		return mIcbSuperSectorName;
	}

    void RefDataInstrument::setStartDate(const string& startDate)
	{
		mStartDate = startDate;
	}
	
    string RefDataInstrument::getStartDate()
	{
		return mStartDate;
	}

    void RefDataInstrument::setCountryOfIncorporation(const string& countryOfIncorporation)
	{
        //TODO Remove spaces function seems to break this
        mCountryOfIncorporation = countryOfIncorporation;
	}
	
    string RefDataInstrument::getCountryOfIncorporation()
	{
		return mCountryOfIncorporation;
	}
	
    void RefDataInstrument::setTradingCurrency(const string& tradingCurrency)
	{
		mTradingCurrency = tradingCurrency;
	}
	
    string RefDataInstrument::getTradingCurrency()
	{
		return mTradingCurrency;
	}

	void RefDataInstrument::setLseMarket(const string& lseMarket)
	{
		mLseMarket = lseMarket;
	}
		
	string RefDataInstrument::getLseMarket()
	{
		return mLseMarket;
	}

	void RefDataInstrument::setFcaListingCategory(const string& fcaListingCategory)
	{
        //mFcaListingCategory = fcaListingCategory;
		mFcaListingCategory = tk.removeSpaces(fcaListingCategory);
	}

	string RefDataInstrument::getFcaListingCategory()
	{
		return mFcaListingCategory;
	}

	void RefDataInstrument::setMarketSegmentCode(const string& marketSegmentCode)
	{
		mMarketSegmentCode = marketSegmentCode;
	}

	string RefDataInstrument::getMarketSegmentCode()
	{
		return mMarketSegmentCode;
	}

	void RefDataInstrument::setMarketSectorCode(const string& marketSectorCode)
	{
		mMarketSectorCode = marketSectorCode;	
    }

	string RefDataInstrument::getMarketSectorCode()
	{
		return mMarketSectorCode;
    }

	void RefDataInstrument::setValues(const vector<string> &v)
	{
		RefDataInstrument::setMnemonic(v[0]);
		RefDataInstrument::setIssuerName(v[1]);
		RefDataInstrument::setInstrumentName(v[2]);
		RefDataInstrument::setIsin(v[3]);
		RefDataInstrument::setMifirIdentifierCode(v[4]);
		RefDataInstrument::setMifirIdentifierDescription(v[5]);
		RefDataInstrument::setIcbIndustry(v[6]);
		RefDataInstrument::setIcbSuperSectorName(v[7]);
		RefDataInstrument::setStartDate(v[8]);
		RefDataInstrument::setCountryOfIncorporation(v[9]);
		RefDataInstrument::setTradingCurrency(v[10]);
		RefDataInstrument::setLseMarket(v[11]);
		RefDataInstrument::setFcaListingCategory(v[12]);
		RefDataInstrument::setMarketSegmentCode(v[13]);
		RefDataInstrument::setMarketSectorCode(v[14]);
	}

	void RefDataInstrument::printRefDataInstrument()
	{
		DLOG("#########################################");
		DLOG("Printing reference data for :: " << mMnemonic);
		DLOG("#########################################");
		DLOG("mMnemonic :: " << mMnemonic);
		DLOG("mIssuerName :: " << mIssuerName);
        DLOG("mInstrumentName :: " << mInstrumentName);
		DLOG("mIsin :: " << mIsin);
		DLOG("mMifirIdentifierCode :: " << mMifirIdentifierCode);
		DLOG("mMifirIdentifierDescription :: " << mMifirIdentifierDescription);
		DLOG("mIcbIndustry :: " << mIcbIndustry);
		DLOG("mIcbSuperSectorName :: " << mIcbSuperSectorName);
		DLOG("mStartDate :: " << mStartDate);
		DLOG("mCountryOfIncorporation :: " << mCountryOfIncorporation);
		DLOG("mTradingCurrency :: " << mTradingCurrency);
		DLOG("mLseMarket :: " << mLseMarket);
		DLOG("mFcaListingCategory :: " << mFcaListingCategory);
		DLOG("mMarketSegmentCode :: " << mMarketSegmentCode);
		DLOG("mMarketSectorCode :: " << mMarketSectorCode);
	}
}
