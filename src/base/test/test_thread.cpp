#include <iostream>
#include <unistd.h>
#include "Thread.h"
#include "Log.h"
#include "Logger.h"
#include "Semaphore.h"
using namespace afa;
Logger::Ptr log = LOG_ROOT();

Semaphore semOdd(0);
Semaphore semEven(0);
int num = 0;
void odd()
{
    for(int i=1;i<101;i+=2)
    {
        //semOdd.Wait();
        std::cout<<i<<std::endl;
        //semEven.Post();
    }
}

void even()
{
    for(int i=2;i<=100;i+=2)
    {
        //semEven.Wait();
        std::cout<<i<<std::endl;
        //semOdd.Post();
    }
}
void fun()
{

    for(int i=0;i<500;i++)
    {
        LOG_DEBUG(log)<<"ThreadFun";
        //std::cout<<"thread1.Start()!"<<std::endl;
    }
}

void fun2()
{
    for(int i=0;i<500;i++)
    {
        LOG_DEBUG(log)<<"ThreadFun2";
        //std::cout<<"thread2.Start()!"<<std::endl;
    }
}
int main()
{
    Thread threadOdd(odd,"OddPrintf");
    Thread threadEven(even,"EvenPrintf");

    threadEven.Start();
    threadOdd.Start();


    //semOdd.Post();
    //std::cout<<"thread2.Start()!"<<std::endl;
    //for(int i=0;i<500;i++)
    //LOG_DEBUG(log)<<"Main";
    //std::cout<<"main!"<<std::endl;

    threadOdd.Join();
    threadEven.Join();

    std::cout<<"Finished!"<<std::endl;
}