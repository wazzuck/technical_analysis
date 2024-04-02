#include <string.h>

#include <regex>
#include <iostream>
#include <vector>

#include "Fundamental.h"
#include "Logger.h"
#include "ToolKit.h"

using namespace std;
using namespace EodToolKit;

namespace eod
{
	Fundamental::Fundamental()
        :mMnemonic("")
		,mName("")
		,mSector("")
		,mIndustry("")
		,mPE("")
		,mEPS("")
		,mDivYield("")
		,mShares(0)
        ,mDPS("")
		,mPEG("")
		,mPtS("")
		,mPtB("")
	{
    }

    void Fundamental::setMnemonic(const string& mnemonic)
	{
       	//TLOG( << mnemonic);
		mMnemonic = mnemonic;
		
		regex pattern("^..\..");
	    if (regex_match(mnemonic, pattern))
		{
			//TLOG("found regex");
		    mMnemonic.replace(2, 1, "_");
		}
	}

    string Fundamental::getMnemonic()
	{
		return mMnemonic;
	}

	void Fundamental::setName(const string& name)
	{
		mName = name;

		ToolKit tk;
		mName = tk.removePunctuation(mName);
	}

	string Fundamental::getName()
	{
		return mName;
	}

	void Fundamental::setSector(const string& sector)
	{
		mSector = sector;
	}

	string Fundamental::getSector()
	{
		return mSector;
	}

    void Fundamental::setIndustry(const string& industry)
	{
		mIndustry = industry;
	}    

    string Fundamental::getIndustry()
	{
		return mIndustry;
	}    

	void Fundamental::setPE(const string& PE)
	{
		mPE = PE;
	}
	
	string Fundamental::getPE()
	{
		return mPE;
	}
	
	void Fundamental::setEPS(const string& EPS)
	{
		mEPS = EPS;
	}
	
	string Fundamental::getEPS()
	{
		return mEPS;
	}

	void Fundamental::setDivYield(const string& divYield)
	{
		mDivYield = divYield;
	}

	string Fundamental::getDivYield()
	{
		return mDivYield;
	}

    void Fundamental::setShares(const string& shares)
	{
		mShares = stoll(shares);
	}

    const unsigned long int Fundamental::getShares()
	{
		return mShares;
	}

    void Fundamental::setDPS(const string& DPS)
	{
		mDPS = DPS;
	}

    string Fundamental::getDPS()
	{
		return mDPS;
	}

    void Fundamental::setPEG(const string& PEG)
	{
		mPEG = PEG;
	}
	
    string Fundamental::getPEG()
	{
		return mPEG;
	}
	
    void Fundamental::setPtS(const string& PtS)
	{
		mPtS = PtS;
	}
	
    string Fundamental::getPtS()
	{
		return mPtS;
	}
	
	void Fundamental::setPtB(const string& PtB)
	{
		mPtB = PtB;
	}
	
	string Fundamental::getPtB()
	{
		return mPtB;
	}

	void Fundamental::setValues(const vector<string> &v)
	{        
		Fundamental::setMnemonic(v[0]);
		Fundamental::setName(v[1]);
		Fundamental::setSector(v[2]);
		Fundamental::setIndustry(v[3]);
		Fundamental::setPE(v[4]);
		Fundamental::setEPS(v[5]);
		Fundamental::setDivYield(v[6]);
		Fundamental::setShares(v[7]);
		Fundamental::setDPS(v[8]);
		Fundamental::setPEG(v[9]);
		Fundamental::setPtS(v[10]);
		Fundamental::setPtB(v[11]);
	}

	void Fundamental::printFundamental()
	{
		DLOG("#########################################");
		DLOG("Printing fundamental data for " << mMnemonic);
		DLOG("#########################################");
		DLOG("Mnemonic :: " << mMnemonic);
		DLOG("Name :: " << mName);
        DLOG("Sector :: " << mSector);
        DLOG("Industry :: " << mIndustry);
		DLOG("PE :: " << mPE);
		DLOG("EPS :: " << mEPS);
		DLOG("Dividend Yield :: " << mDivYield);
		DLOG("Number of shares " << mShares);
		DLOG("DPS :: " << mDPS);
		DLOG("PEG :: " << mPEG);
		DLOG("PtS :: " << mPtS);
		DLOG("PtB :: " << mPtB);
	}
}
