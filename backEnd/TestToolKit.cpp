#include <iostream>
#include <regex>

#include "ToolKit.h"
#include "Logger.h"

using namespace std;
using namespace EodToolKit;

int main()
{   
    
	LOG("Testing ToolKit Class");

	//string line="OCDO,OCADO GROUP PLC,ORD 2P,GB00B3MBS747,SHRS,Shares,Consumer Staples ,\"Personal Care, Drug and Grocery Stores \",21/07/2010,United Kingdom,GBX,MAIN MARKET,Premium Equity Commercial Companies,SET1,FE10";
	//string line="PPHC,\"PUBLIC POLICY HOLDING COMPANY, INC.\",\"COM SHS USD0.001 (DI) (REG S, CAT3)\",US7444301094,SHRS,Shares,Industrials,Industrial Goods and Services,16/12/2021,United States,GBX,AIM,,ASX1,AMSR";

    //LOG( << line);
    
	ToolKit tk;
    
	//vector<string> splitLine = tk.split(line, ',');

    
	//tk.displayVector(splitLine);

    // Test removeSpaces tool
    // TODO Where Services is appended twice
    string s = "Financial Services "; 
    //LOG("Testing removeSpaces" << tk.removeSpaces(s));

	string date = "2023-10-11";
	//LOG("Testing removeChar " << tk.removeChar(date, '-'));

	string str = "CARR'S";
	LOG("Testing removePunctuation " << tk.removePunctuation(str));

    return 0;
}
