#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <string.h>

#include "Sqlite.h"
#include "RefDataInstrument.h"
#include "TechnicalAnalysis.h"
#include "Logger.h"

#include <filesystem>
namespace fs = filesystem;

using namespace std;

namespace eod
{
Sqlite::Sqlite ( string absoluteDatabaseFileName )
  :sql ( "" )
  ,zErrMsg ( 0 )
  ,returnCode ( 0 )
  ,refCount ( 1 )
  ,fundCount ( 1 )
  ,priceTableCount ( 1 )
  ,technicalCount ( 1 )
{
  if ( fs::exists ( absoluteDatabaseFileName ) ) {
    WLOG ( "Database with filename " << absoluteDatabaseFileName << " already exists" );
    WLOG ( "Removing "  << absoluteDatabaseFileName );
    remove ( absoluteDatabaseFileName.c_str() );
  }

  LOG ( "Opening " << absoluteDatabaseFileName );
  returnCode = sqlite3_open ( absoluteDatabaseFileName.c_str(), &db );

  if ( returnCode ) {
    ELOG ( "Error: " << sqlite3_errmsg ( db ) );
    exit ( 1 );
  }
  else {
    LOG ( "Opened database successfully" );
  }

  LOG ( "Creating RefData Table" );

  string sql = "CREATE TABLE REFDATA(\n"
               "\t\tREFDATA_ID INT PRIMARY         KEY         NOT NULL,\n" \
               "\t\tMNEMONIC                       TEXT        NOT NULL,\n" \
               "\t\tINSTRUMENT_NAME                TEXT        NOT NULL,\n" \
               "\t\tISSUER_NAME                    TEXT        NOT NULL,\n" \
               "\t\tISIN                           TEXT        NOT NULL,\n" \
               "\t\tMIFIR_IDENTIFIER_CODE          TEXT        NOT NULL,\n" \
               "\t\tMIFIR_IDENTIFIER_DESCRIPTION   TEXT        NOT NULL,\n" \
               "\t\tICB_INDUSTRY                   TEXT        NOT NULL,\n" \
               "\t\tICB_SUPER_SECTOR_NAME          TEXT        NOT NULL,\n" \
               "\t\tSTART_DATE                     TEXT        NOT NULL,\n" \
               "\t\tCOUNTRY_OF_INCORPORATION       TEXT        NOT NULL,\n" \
               "\t\tTRADING_CURRENCY               TEXT        NOT NULL,\n" \
               "\t\tLSE_MARKET                     TEXT        NOT NULL,\n" \
               "\t\tFCA_LISTING_CATEGORY           TEXT        NOT NULL,\n" \
               "\t\tMARKET_SEGMENT_CODE            TEXT        NOT NULL,\n" \
               "\t\tMARKET_SECTOR_CODE             TEXT        NOT NULL\n);";

  runSql ( sql );

  LOG ( "Creating Fundamental Table" );

  sql = "CREATE TABLE FUNDAMENTAL(\n"
        "\t\tFUNDAMENTAL_ID INT PRIMARY     KEY         NOT NULL,\n" \
        "\t\tMNEMONIC                       TEXT        NOT NULL,\n" \
        "\t\tNAME                           TEXT        NOT NULL,\n" \
        "\t\tSECTOR                         TEXT        NOT NULL,\n" \
        "\t\tINDUSTRY                       TEXT        NOT NULL,\n" \
        "\t\tPE                             TEXT        NOT NULL,\n" \
        "\t\tEPS                            TEXT        NOT NULL,\n" \
        "\t\tDIV_YIELD                      TEXT        NOT NULL,\n" \
        "\t\tSHARES                         INTEGER     NOT NULL,\n" \
        "\t\tDPS                            TEXT        NOT NULL,\n" \
        "\t\tPEG                            TEXT        NOT NULL,\n" \
        "\t\tPTS                            TEXT        NOT NULL,\n" \
        "\t\tPTB                            TEXT        NOT NULL\n);";

  runSql ( sql );

  /*
  LOG("Creating Technical Table");

  sql = "CREATE TABLE TECHNICAL(\n"
      "\t\tTECHNICAL_ID INT PRIMARY           KEY     NOT NULL,\n" \
      "\t\tMNEMONIC                           TEXT    NOT NULL,\n" \
      "\t\tPERCENTAGE_DIFFERENCE_1            REAL    NOT NULL,\n" \
      "\t\tPERCENTAGE_DIFFERENCE_3            REAL    NOT NULL,\n" \
      "\t\tPERCENTAGE_DIFFERENCE_5            REAL    NOT NULL,\n" \
      "\t\tMARKET_CAP                         REAL    NOT NULL,\n" \
      "\t\tLATEST_PPO                         REAL    NOT NULL\n);";

  runSql(sql);
  */
}

void Sqlite::addReferenceData ( InstrumentPrices *ip )
{
  LOG ( "Adding " << ip->rdi.getMnemonic() << " to REFDATA" );

  sql = "INSERT INTO REFDATA VALUES(" + \
        to_string ( refCount ) + ", '" + \
        ip->rdi.getMnemonic() + "', '" + \
        ip->rdi.getIssuerName() + "', '" + \
        ip->rdi.getInstrumentName() + "', '" + \
        ip->rdi.getIsin() + "', '" + \
        ip->rdi.getMifirIdentifierCode() + "', '" + \
        ip->rdi.getMifirIdentifierDescription() + "', '" + \
        ip->rdi.getIcbIndustry() + "', '" + \
        ip->rdi.getIcbSuperSectorName() + "', '" + \
        ip->rdi.getStartDate() + "', '" + \
        ip->rdi.getCountryOfIncorporation() + "', '" + \
        ip->rdi.getTradingCurrency() + "', '" + \
        ip->rdi.getLseMarket() + "', '" + \
        ip->rdi.getFcaListingCategory() + "', '" + \
        ip->rdi.getMarketSegmentCode() + "', '" + \
        ip->rdi.getMarketSectorCode() + "');";

  runSql ( sql );

  refCount++;
}

void Sqlite::addFundamental ( InstrumentPrices *ip )
{
  LOG ( "Adding " << ip->rdi.getMnemonic() << " to FUNDMENTAL" );

  sql = "INSERT INTO FUNDAMENTAL VALUES(" + \
        to_string ( fundCount ) + ", '" + \
        ip->funda.getMnemonic() + "', '" + \
        ip->funda.getName() + "', '" + \
        ip->funda.getSector() + "', '" + \
        ip->funda.getIndustry() + "', '" + \
        ip->funda.getPE() + "', '" + \
        ip->funda.getEPS() + "', '" + \
        ip->funda.getDivYield() + "', '" + \
        to_string ( ip->funda.getShares() ) + "', '" + \
        ip->funda.getDPS() + "', '" + \
        ip->funda.getPEG() + "', '" + \
        ip->funda.getPtS() + "', '" + \
        ip->funda.getPtB() + "');";

  runSql ( sql );

  fundCount++;
}

void Sqlite::addPriceData ( InstrumentPrices *ip )
{

  string price_table_name = ip->rdi.getMnemonic();
  LOG ( "Creating Price Table for " << price_table_name );

  sql = "CREATE TABLE " + price_table_name + "(\n"
        + price_table_name + " INT PRIMARY              KEY         NOT NULL,\n" \
        "\t\tMNEMONIC                                   TEXT        NOT NULL,\n" \
        "\t\tDATE                                       TEXT        NOT NULL,\n" \
        "\t\tOPEN                                       REAL        NOT NULL,\n" \
        "\t\tHIGH                                       REAL        NOT NULL,\n" \
        "\t\tLOW                                        REAL        NOT NULL,\n" \
        "\t\tCLOSE                                      REAL        NOT NULL,\n" \
        "\t\tVOLUME                                     INTEGER     NOT NULL,\n" \
        "\t\tEMAFast                                    REAL        NOT NULL,\n" \
        "\t\tEMASlow                                    REAL        NOT NULL,\n" \
        "\t\tMACD                                       REAL        NOT NULL\n);";

  runSql ( sql );

  sql = "INSERT INTO " + price_table_name + " (" + price_table_name + ",MNEMONIC, DATE, OPEN, HIGH, LOW, CLOSE, VOLUME, EMAFast, EMASlow, MACD) VALUES\n";

  map<string, CalendarDayInstrumentPrice *> *cdipMapPtr = ip->pa.getCdipMapPointer();

  for ( auto i = cdipMapPtr->rbegin(); i != cdipMapPtr->rend(); ++i ) {
    //(*cdipMapSqlite)[i->first]->printCalendarDayInstrumentPrice();

    sql += "\t(" + \
           to_string ( priceTableCount ) + ", '" + \
           i->second->getMnemonic() + "', '" + \
           i->second->getDate() + "', " + \
           to_string ( i->second->getOpen() ) + ", " + \
           to_string ( i->second->getHigh() ) + ", " + \
           to_string ( i->second->getLow() ) + ", " + \
           to_string ( i->second->getClose() ) + ", " + \
           to_string ( i->second->getVolume() ) + ", " + \
           to_string ( i->second->getEMAFast() ) + ", " + \
           to_string ( i->second->getEMASlow() ) + ", " + \
           to_string ( i->second->getMACD() ) + "),\n"; //WATCH out for extra ' characters on strings

    priceTableCount++;

    //break;
  }

  //Replace last ",\n" with a ";"
  sql.pop_back();
  sql.pop_back();
  sql += ";";

//        sql = "INSERT INTO " + price_table_name + " VALUES(" + to_string(priceTableCount) + ", '" + cdipMapSqlite[i->first].getMnemonic() + "', '" + cdipMapSqlite[i->first].getDate() + "', " + to_string(cdipMapSqlite[i->first].getOpen()) + ", " + to_string(cdipMapSqlite[i->first].getHigh()) + ", " + to_string(cdipMapSqlite[i->first].getLow()) + ", " + to_string(cdipMapSqlite[i->first].getClose()) + ", " + to_string(cdipMapSqlite[i->first].getVolume()) + ");";

  runSql ( sql );

  //priceTableCount++;
}

/*
void Sqlite::addTechnical()
{
DLOG("Running addTechnical()");
    string percentagePriceChangeSql = "";

    for (auto const& priceChangeElement : taSqlite.getPercentageChanges())
    {
        // Accessing KEY from element
        int numberOfDays = priceChangeElement.first;
        double percentageChange = priceChangeElement.second;
        LOG("numberOfDays " << numberOfDays << "percentageChange " << percentageChange);

        //TODO Can I dynamically modify the schema if additional days are added?
        percentagePriceChangeSql += to_string(priceChangeElement.second) + "," ;
    }

    LOG("Percentage Change " << percentagePriceChangeSql);

    double PPO = 0.0;

//Get the last value in the cdipMap as the PPO
    for (auto rit = cdipMapSqlite.rbegin(); rit != cdipMapSqlite.rend(); ++rit)
    {
        PPO = rit->second.getPPO();
  break;
    }

    sql = "INSERT INTO TECHNICAL VALUES(" + \
            to_string(technicalCount) + ",'" + \
            rdiSqlite.getMnemonic() + "'," + \

            to_string(taSqlite.getMarketCap()) + "," + \
            to_string(PPO) + ");";

cdipMapSqlite.end().printCalendarDayInstrumentPrice();

runSql(sql);

exit(0);

    technicalCount++;
}
*/

void Sqlite::runSql ( const string &inSql )
{
  DLOG ( "Running runSql" );

  returnCode = sqlite3_exec ( db, inSql.c_str(), NULL, NULL, &zErrMsg );
  LOG ( "Return code " << returnCode );
  //LOG( << zErrMsg); // BUG DOES NOT PRINT OUT ERR MSG

  if ( returnCode ) {
    ELOG ( << inSql );
    ELOG ( ":Error running sql " << sqlite3_errmsg ( db ) );
    exit ( 1 );
  }
  else {
    LOG ( "sql ran successfully" );
    LOG ( << inSql );
  }
}

void Sqlite::closeDB()
{
  //Swap MARKET_SEGMENT_CODE with human readable description
  //TODO Can this be done in a single update

  sql = "UPDATE REFDATA SET MARKET_SEGMENT_CODE = REPLACE(MARKET_SEGMENT_CODE,'SET1', 'FTSE100');";
  runSql ( sql );

  sql = "UPDATE REFDATA SET MARKET_SEGMENT_CODE = REPLACE(MARKET_SEGMENT_CODE,'STMM', 'FTSE250');";
  runSql ( sql );

  sql = "UPDATE REFDATA SET MARKET_SEGMENT_CODE = REPLACE(MARKET_SEGMENT_CODE,'SSMM', 'SmallCap');";
  runSql ( sql );

  sql = "UPDATE REFDATA SET MARKET_SEGMENT_CODE = REPLACE(MARKET_SEGMENT_CODE,'SET3', 'Other');";
  runSql ( sql );

  sql = "UPDATE REFDATA SET MARKET_SEGMENT_CODE = REPLACE(MARKET_SEGMENT_CODE,'AMSM', 'AIM');";
  runSql ( sql );

  LOG ( "Closing database" );
  sqlite3_close ( db );
}
}
