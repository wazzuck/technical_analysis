# Prevent GNU Make from deleting intermediate files on error
DELETE_ON_ERROR:=

CXX=g++
#CXX=clang++
RM=rm -f
LDFLAGS=-lsqlite3 -DSQLITE_THREADSAFE=1 -lta_lib
#CPPFLAGS=-std=c++20 -ggdb3 -O0 -Wall -Wextra -pedantic -fsanitize=address -static-libasan -fno-omit-frame-pointer -fsanitize-address-use-after-scope
CPPFLAGS=-std=c++20 -ggdb3 -O0 -Wall -Wextra -pedantic
ObjectCompilerCommand=$(CXX) $(CPPFLAGS) -o $@ $? -c
ExecutableCompilerCommand=$(CXX) $(CPPFLAGS) -o $@ $? $(LDFLAGS)

Eod.exe: EodDate.o RefDataInstrument.o RefDataLoader.o Fundamental.o FundamentalLoader.o ToolKit.o CalendarDayInstrumentPrice.o Technical.o InstrumentPrices.o PriceAnalysis.o PriceLoader.o TechnicalAnalysis.o Sqlite.o Eod.cpp
	$(ExecutableCompilerCommand)
	-./$@

all: Eod.exe TestInstrument.exe TestPriceLoader.exe TestCalendarDayInstrumentPrice.exe TestToolKit.exe TestEodDate.exe TestInstrumentPrices.exe TestSqlite.exe

EodDate.o: EodDate.cpp
	$(ObjectCompilerCommand)

TestEodDate.exe: EodDate.o TestEodDate.cpp
	$(ExecutableCompilerCommand)
	-./$@

ToolKit.o: ToolKit.cpp
	$(ObjectCompilerCommand)

TestToolKit.exe: ToolKit.o TestToolKit.cpp
	$(ExecutableCompilerCommand)
	-./$@

RefDataInstrument.o: RefDataInstrument.cpp
	$(ObjectCompilerCommand)

TestRefDataInstrument.exe: ToolKit.o RefDataInstrument.o TestRefDataInstrument.cpp
	$(ExecutableCompilerCommand)
	-./$@

RefDataLoader.o: RefDataInstrument.o RefDataLoader.cpp
	$(ObjectCompilerCommand)

TestRefDataLoader.exe: RefDataInstrument.o RefDataLoader.o ToolKit.o TestRefDataLoader.cpp
	$(ExecutableCompilerCommand)
	-./$@

CalendarDayInstrumentPrice.o: CalendarDayInstrumentPrice.cpp
	$(ObjectCompilerCommand)

TestCalendarDayInstrumentPrice.exe: CalendarDayInstrumentPrice.o ToolKit.o TestCalendarDayInstrumentPrice.cpp
	$(ExecutableCompilerCommand)
	-./$@

PriceLoader.o: CalendarDayInstrumentPrice.o ToolKit.o PriceLoader.cpp
	$(ObjectCompilerCommand)

TestPriceLoader.exe: CalendarDayInstrumentPrice.o ToolKit.o PriceLoader.o TestPriceLoader.cpp
	$(ExecutableCompilerCommand)
	-./$@

Fundamental.o: ToolKit.o Fundamental.cpp
	$(ObjectCompilerCommand)

TestFundamental.exe: ToolKit.o Fundamental.o TestFundamental.cpp
	$(ExecutableCompilerCommand)
	-./$@

FundamentalLoader.o: Fundamental.o ToolKit.o FundamentalLoader.cpp
	$(ObjectCompilerCommand)

TestFundamentalLoader.exe: FundamentalLoader.o Fundamental.o ToolKit.o TestFundamentalLoader.cpp
	$(ExecutableCompilerCommand)
	-./$@

InstrumentPrices.o: PriceLoader.o InstrumentPrices.cpp
	$(ObjectCompilerCommand)

TestInstrumentPrices.exe: PriceAnalysis.o RefDataInstrument.o Fundamental.o InstrumentPrices.o Technical.o CalendarDayInstrumentPrice.o ToolKit.o TestInstrumentPrices.cpp
	$(ExecutableCompilerCommand)
	-./$@

Technical.o: Technical.cpp
	$(ObjectCompilerCommand)

TestTechnical.exe: Technical.o TestTechnical.cpp
	$(ExecutableCompilerCommand)
	-./$@

PriceAnalysis.o: ToolKit.o Technical.o CalendarDayInstrumentPrice.o PriceAnalysis.cpp
	$(ObjectCompilerCommand)

TestPriceAnalysis.exe: ToolKit.o PriceAnalysis.o InstrumentPrices.o Technical.o Fundamental.o RefDataInstrument.o CalendarDayInstrumentPrice.o TestPriceAnalysis.cpp
	$(ExecutableCompilerCommand)
	-./$@

TechnicalAnalysis.o: TechnicalAnalysis.cpp
	$(ObjectCompilerCommand)
	
TestTechnicalAnalysis.exe: EodDate.o RefDataInstrument.o Fundamental.o PriceLoader.o InstrumentPrices.o PriceAnalysis.o ToolKit.o TechnicalAnalysis.o CalendarDayInstrumentPrice.o Technical.o TestTechnicalAnalysis.cpp
	$(ExecutableCompilerCommand)
	-./$@

Sqlite.o: RefDataInstrument.o TechnicalAnalysis.o Sqlite.cpp
	$(ObjectCompilerCommand)

TestSqlite.exe: Sqlite.o RefDataInstrument.o Fundamental.o InstrumentPrices.o CalendarDayInstrumentPrice.o TechnicalAnalysis.o ToolKit.o TestSqlite.cpp
	$(ExecutableCompilerCommand)
	-./$@
	sqlite3 /dev/shm/test.db 'SELECT * FROM VOD;'
	sqlite3 test.db 'SELECT * FROM REFDATA;'
	#sqlite3 test.db 'SELECT * FROM FUNDAMENTAL;'

TestTALib.exe: TestTALib.cpp 
	$(ExecutableCompilerCommand)
	-./$@

Scratch.exe: ScratchMain.cpp CalendarDayInstrumentPrice.o ToolKit.o
	$(ExecutableCompilerCommand)
	-./$@

Scratch_new.exe: Scratch_new.cpp
	$(ExecutableCompilerCommand)
	-./$@

GetData:
	~/vunderland/eod/scripts/pull_data.sh

Deploy:
	~/vunderland/eod/scripts/deploy.sh

clean:
	$(RM) *.gch *.o *.exe *.db *journal *.out *.log
