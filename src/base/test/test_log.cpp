#include "Logger.h"
#include "LogManager.h"
#include "Log.h"
#include "LogAppender.h"
#include "LogFileAppender.h"
#include <unistd.h>
char logString[] = "This is Log Test!\n";
int main()
{
    std::string name("TestSylarLog");
    afa::Logger::Ptr TestLog = LOG_NAME(name);

    for(int i=0;i<10;i++)
    {
        LOG_DEBUG(TestLog)<<"ThisIsTestLog";
    }

    afa::Logger::Ptr rootLoog = LOG_ROOT();
    for(int i=0;i<10;i++)
    {
        LOG_DEBUG(rootLoog)<<"This is a testLog of root";
    }
    return 0;
}