#include <iostream>
#include "../Thread.h"
#include "../Log.h"
#include "../Logger.h"
using namespace afa;
Logger::Ptr log = LOG_ROOT();

void fun()
{
    for(int i=0;i<10;i++)
    {
        LOG_DEBUG(log)<<"ThreadFun";
    }
}
int main()
{
    Thread thread(fun,"TestThread");

    thread.Start();

    thread.Join();
}