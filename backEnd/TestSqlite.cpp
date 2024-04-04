#include <stdio.h>
#include <iostream>
#include <sqlite3.h>
#include <string>

#include "Sqlite.h"
#include "InstrumentPrices.h"
#include "CalendarDayInstrumentPrice.h"
#include "ToolKit.h"
#include "Sqlite.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{

  cout << "Testing Sqlite Class" << endl;

  cout << "Hello" << endl;

  Sqlite sqli ( "/dev/shm/test.db" );

  RefDataInstrument testRdi;

  const string refArr[] = {"VOD","BT-AAFONE GROUP PLC","ORD USD0.20 20/21","GB00BH4HKS39","SHRS","Shares","Telecommunications","Telecommunications","24/02/2014","United Kingdom","GBX","MAIN MARKET","Premium Equity Commercial Companies","SET1","FE10"};

  vector<string> refVec ( refArr, refArr + sizeof ( refArr ) / sizeof ( refArr[0] ) );

  testRdi.setValues ( refVec );

  Fundamental fund;

  static const string arr[] = {"VOD","Vodafone Group Plc","Telecommunications Services","Mobile Telecommunications","0","-0.05","0","265276800","0","5.15","138.68","91.72"};

  vector<string> fundVec ( arr, arr + sizeof ( arr ) / sizeof ( arr[0] ) );

  fund.setValues ( fundVec );

  InstrumentPrices *ip = new InstrumentPrices();

  ip->rdi = testRdi;
  ip->funda = fund;

  string instrument_20200212 = "VOD,20200212,150.74,155.52,150.16,152.64,46428587";
  string instrument_20200213 = "VOD,20200213,151.18,153.3,150.5,151.76,38589780";

  vector<string> instrumentVector= {instrument_20200212, instrument_20200213};

  ToolKit tk;

  for ( vector<string>::iterator t=instrumentVector.begin(); t!=instrumentVector.end(); ++t ) {
    CalendarDayInstrumentPrice *cdip = new CalendarDayInstrumentPrice();

    cdip->setValues ( tk.split ( *t, ',' ) );

    ip->pa.addCalendarDayInstrumentPrice ( cdip->getDate(), cdip );

    //cdip.printCalendarDayInstrumentPrice();
  }
  /*
  sqli.rdiSqlite = ip->rdi;
  sqli.addReferenceData();

  sqli.fundSqlite = ip.funda;
  sqli.addFundamental();
  */

  //sqli.cdipMapSqlite = ip->pa.getCdipMapPointer();
  sqli.addPriceData ( ip );

  //sqli.addTechnical();

  sqli.closeDB();

  return 0;
}
