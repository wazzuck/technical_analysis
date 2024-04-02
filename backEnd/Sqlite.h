#pragma once

#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

#include "InstrumentPrices.h"
/*
#include "RefDataInstrument.h"
#include "Fundamental.h"
#include "CalendarDayInstrumentPrice.h"
#include "TechnicalAnalysis.h"
*/
using namespace std;

namespace eod
{
	class Sqlite: public RefDataInstrument
	{
        public:
            Sqlite(string absoluteDatabaseFileName);
            void closeDB();
            //void createPriceTable();
            void addTechnical();
            void addReferenceData(InstrumentPrices*);
            void addFundamental(InstrumentPrices*);
            void addPriceData(InstrumentPrices*);

            //map <string, CalendarDayInstrumentPrice*>* cdipMapSqlite;

        private:
			void runSql(const string& inSql);
            sqlite3 *db;
            string sql;
            char *zErrMsg;
            int returnCode;
            int refCount;
            int fundCount;
            int priceTableCount;
            int technicalCount;
    };
}
