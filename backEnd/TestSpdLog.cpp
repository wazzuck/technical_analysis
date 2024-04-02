#include "spdlog/spdlog.h"

//Full tutorial at https://github.com/gabime/spdlog/wiki/1.-QuickStart

using namespace spdlog;

int main()
{
    //Use the default logger (stdout, multi-threaded, colored)
    info("Hello, {}!", "Info");
    
    set_level(level::debug);
    
    debug("Hello, {}!", "Debug");
}
