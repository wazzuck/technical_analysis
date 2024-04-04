/* Fundamental.h
 *
 * Class to store these values from the EOD Data Service
 * It has an associated csv file loader class FundamentalLoader.h
 *
 * TODO Describe what each value is in plain English
 *
 * Mnemonic
 * Name
 * Sector
 * Industry
 * PE
 * EPS
 * DivYield
 * Shares
 * DPS
 * PEG
 * PtS
 * PtB
 */

#pragma once

#include <string>
#include <vector>

using namespace std;

//Symbol  Name  Sector  Industry  PE  EPS DivYield  Shares  DPS PEG PtS PtB

namespace eod
{
class Fundamental
{
public:
  Fundamental();
  void setMnemonic ( const string& mnemonic );
  void setName ( const string& name );
  void setSector ( const string& sector );
  void setIndustry ( const string& industry );
  void setPE ( const string& PE );
  void setEPS ( const string& EPS );
  void setDivYield ( const string& icbIndustry );
  void setShares ( const string& divYield );
  void setDPS ( const string& DPS );
  void setPEG ( const string& PEG );
  void setPtS ( const string& PtS );
  void setPtB ( const string& PtB );

  string getMnemonic();
  string getName();
  string getSector();
  string getIndustry();
  string getPE();
  string getEPS();
  string getDivYield();
  const unsigned long int getShares();
  string getDPS();
  string getPEG();
  string getPtS();
  string getPtB();

  void setValues ( const vector<string> &instrumentVector );
  void printFundamental();

protected:
  string mMnemonic;
  string mName;
  string mSector;
  string mIndustry;
  string mPE;
  string mEPS;
  string mDivYield;
  unsigned long int mShares;
  string mDPS;
  string mPEG;
  string mPtS;
  string mPtB;
};
}
