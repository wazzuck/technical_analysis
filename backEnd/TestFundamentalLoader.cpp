#include <iostream>
#include <filesystem>
#include <map>
#include "Fundamental.h"
#include "FundamentalLoader.h"
#include "Logger.h"
//#include <fstream> //for reading in a file

using namespace std;
using namespace eod;
namespace fs = filesystem;

int main()
{
	LOG("Testing FundamentalLoader Class");

    string path = "/home/neville/data/";
    
    string todayDate="20200213";
 
    string startDateFolder = path + "/" + todayDate;

    string fundamentalList = startDateFolder + "/Fundamentals_LSE.txt";
    
    FundamentalLoader fl;
    
    map<string,Fundamental> testFundamentalMap;
    
    testFundamentalMap = fl.getFundamentalData(fundamentalList);
    
    testFundamentalMap["BATS"].printFundamental();
    
    return 0;
}
