#include <iostream>
#include <vector>

#include "Fundamental.h"
#include "Logger.h"
#include "ToolKit.h"

using namespace std;
using namespace eod;
using namespace EodToolKit;

int main()
{
	LOG("Testing Fundamental Class");

	Fundamental fund;

//Note that the data is TAB delimited
	//string instrument = "BP.A	BP Plc			0	0	0	0	0	0	0	0";
	string instrument = "CARR	Carr's Group Plc			12.56	12.9	0	898210	0	0	36.23	171.13";

	ToolKit tk;

	fund.setValues(tk.split(instrument, '\t'));

	LOG( << fund.getMnemonic());

	fund.printFundamental();

	return 0;
}
