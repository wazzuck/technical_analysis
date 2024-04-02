#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string> 
#include <sstream>

using namespace std;

namespace eod
{
    class EodDate
    {
    public:
        EodDate();
        string getDate();
        string getNextDate();
        void setPreviousDate();
        void setDate(const string inDate);
    private:
        chrono::system_clock::time_point now_time_point;
        time_t mIn_time_t;
    };
}
