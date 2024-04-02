#include <regex>
#include <iostream>
#include <filesystem>
#include <fstream> //for reading in a file
#include "ToolKit.h"
#include "FundamentalLoader.h"
#include "Logger.h"

using namespace std;
namespace fs = filesystem;
using namespace EodToolKit;

namespace eod
{
	FundamentalLoader::FundamentalLoader()
	{
        map<string,Fundamental> fundamentalMap;
	}

    map<string,Fundamental> FundamentalLoader::loadFundamental(map<string,Fundamental> fundamentalMap, string fundamentalAbsoluteFileName)
    {
        LOG("Reading fundamentalList " << fundamentalAbsoluteFileName);
        
        if (fs::exists(fundamentalAbsoluteFileName)) {
            LOG("fundamentalAbsoluteFileName directory " << fundamentalAbsoluteFileName << " exists");
        }
        else {
            ELOG(<< fundamentalAbsoluteFileName << " doesn't exist");
            exit(1);
        }

        ifstream inputStream(fundamentalAbsoluteFileName);

        string line = "";

        int count = 0;

        regex r("^,,,");

        count=0;

        ToolKit tk;

            while (getline (inputStream, line))
            {
                //DLOG(<< line);
                if ( line == "" )
                {
                    LOG("Reached end of file");
                    break;
                }
                else
                {
                    //ignore header
                    if(count > 0)
                    {
                            line.pop_back(); //Remove end of line char
                            vector<string> x = tk.split(line, '\t');
                            //TODO Is a vector really the best way to do this considering I know the number of elements?

                            Fundamental temp;
                            temp.setValues(x);
                            
                            string mnemonic = temp.getMnemonic();
                            
                            fundamentalMap[mnemonic] = temp;
                            //fundamentalMap[mnemonic].printFundamental();
                    }
                }
                count++;
            }
            
        inputStream.close();
        return fundamentalMap;
    }

    map<string,Fundamental> FundamentalLoader::getFundamentalData(string fundamentalAbsoluteFileName)
    {
        return loadFundamental(fundamentalMap, fundamentalAbsoluteFileName);
    }

	/*Number of elements function
     * 
            cout << "Number of instruments in reference data " << refDataMap.size() << endl;
    */

	//PrintInstrument fucntion
}
