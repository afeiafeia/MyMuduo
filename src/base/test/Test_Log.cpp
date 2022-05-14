#include "../Logger.h"
#include "../LogManager.h"
#include "../Log.h"
#include "../LogAppender.h"
#include "../LogFileAppender.h"
#include <unistd.h>
char logString[] = "This is Log Test!\n";
int main()
{
    //afa::AsyncLogger loger("./LogTest.txt",2);
    //loger.Start();
    //sleep(4);
    //for(int i=0;i<10;++i)
    //{
    //    loger.Append(logString,strlen(logString));
    //}

    std::string name("TestSylarLog");
    afa::Logger::Ptr TestLog = LOG_NAME(name);
    //afa::LogAppender::ptr append(new afa::FileLogAppender("./TestSylarLog"));
    //TestLog->AddAppender(append);
    for(int i=0;i<10;i++)
    {
        //afa::LogEvent::ptr event(new afa::LogEvent(afa::LogLevel::DEBUG, __FILE__,__LINE__,afa::CurrentThread::tid(),0,time(0),std::string(afa::CurrentThread::name())));
        //afa::LogEventWarp warp(TestLog,event);
        //warp.GetSS() << "TestLog";
        LOG_DEBUG(TestLog)<<"ThisIsTestLog";
    }
    //loger.stop();
    return 0;
}