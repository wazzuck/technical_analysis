#pragma once

#include <iostream>

using std::cout;

//TODO Colourise output

#define FLOG(x) cout << "FATAL : " x << "\n"
#define ELOG(x) cout << "ERROR : " x << "\n"
#define WLOG(x) cout << "WARN : " x << "\n"
#define LOG(x) cout << "INFO : " x << "\n"
#define DLOG(x) cout << "DEBUG : " x << "\n"
#define TLOG(x) cout << "TRACE : " x << "\n"
