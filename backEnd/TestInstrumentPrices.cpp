#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

#include "InstrumentPrices.h"
#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{
  LOG ( "Testing InstrumentPrice Class" );

  string mnemonic = "VOD";

  RefDataInstrument testRdi;
  const string refArr[] = {"VOD","VODAFONE GROUP PLC","ORD USD0.20 20/21","GB00BH4HKS39","SHRS","Shares","Telecommunications","Telecommunications","24/02/2014","United Kingdom","GBX","MAIN MARKET","Premium Equity Commercial Companies","SET1","FE10"};
  vector<string> refVec ( refArr, refArr + sizeof ( refArr ) / sizeof ( refArr[0] ) );
  testRdi.setValues ( refVec );

  Fundamental fund;
  static const string arr[] = {"VOD","Vodafone Group Plc","Telecommunications Services","Mobile Telecommunications","0","-0.05","0","265276800","0","5.15","138.68","91.72"};
  vector<string> fundVec ( arr, arr + sizeof ( arr ) / sizeof ( arr[0] ) );
  fund.setValues ( fundVec );

  InstrumentPrices ip;

  ip.rdi = testRdi;
  ip.funda = fund;

  map<string, InstrumentPrices> instrumentPriceMap;
  instrumentPriceMap[mnemonic] = ip;

  CalendarDayInstrumentPrice cdip1, cdip2, cdip3;

  ToolKit tk;
  cdip1.setValues ( tk.split ( "VOD,20200212,150.74,155.52,150.16,152.64,46428587", ',' ) );
  instrumentPriceMap[mnemonic].addCdip ( "20200212", &cdip1 );

  cdip2.setValues ( tk.split ( "VOD,20200213,151.18,153.3,150.5,151.76,38589780", ',' ) );
  instrumentPriceMap[mnemonic].addCdip ( "20200213", &cdip2 );

  cdip3.setValues ( tk.split ( "VOD,20200214,151.18,153.3,150.5,151.76,38589780", ',' ) );
  instrumentPriceMap[mnemonic].addCdip ( "20200214", &cdip3 );

  instrumentPriceMap[mnemonic].PrintInstrumentPrices();

  return 0;
}
