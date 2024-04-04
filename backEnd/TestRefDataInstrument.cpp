#include <iostream>
#include <vector>

#include "RefDataInstrument.h"
#include "Logger.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{
  LOG ( << "Testing RefDataInstrument Class" );

  RefDataInstrument inst;

  //string instrument = "BP.,BP PLC,$0.25,GB0007980591,SHRS,Shares,Energy,Energy,24/11/2003,United Kingdom,GBX,MAIN MARKET,Premium Equity Commercial Companies,SET1,FE10";
  //string instrument = "BP.A,BP PLC,8% CUM 1ST PRF #1,GB0001385250,SHRS,Shares,Energy,Energy,13/12/2004,United Kingdom,GBX,MAIN MARKET,Standard Shares,SSQ3,SQS3";
  //string instrument = "CARR,CARR'S GROUP PLC,ORD 2.5P,GB00BRK01058,SHRS,Shares,Consumer Staples ,"Food, Beverage and Tobacco ",14/01/2015,United Kingdom,GBX,MAIN MARKET,Premium Equity Commercial Companies,SSMM,SSC5";
  string instrument = "CARR,CARR'S GROUP PLC,ORD 2.5P,GB00BRK01058,SHRS,Shares,Consumer Staples ,Food, Beverage and Tobacco ,14/01/2015,United Kingdom,GBX,MAIN MARKET,Premium Equity Commercial Companies,SSMM,SSC5";

  ToolKit tk;
  inst.setValues ( tk.split ( instrument, ',' ) );

  LOG ( << inst.getMnemonic() );

  inst.printRefDataInstrument();

  return 0;
}
