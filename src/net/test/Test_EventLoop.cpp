/*
 * @Author: your name
 * @Date: 2022-04-17 18:52:01
 * @LastEditTime: 2022-04-18 13:25:58
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /home/zaf/MyTinyMuduo/net/test/Test_EventLoop.cpp
 */
#include "../EventLoop.h"
#include "../EventLoopThreadPool.h"
#include "../../base/Thread.h"
#include "../../base/CurrentThread.h"
#include <sys/syscall.h>
#include "../../base/Log.h"
#include <unistd.h>
using namespace afa;
Logger::Ptr curLog = LOG_ROOT();

void callback()
{
  LOG_DEBUG(curLog)<<"callback()";
  //printf("callback(): pid = %d, tid = %d\n", CurrentThread::tid(), CurrentThread::tid());
}

int main()
{
    LOG_DEBUG(curLog)<<"MainThreadInfor";
    EventLoop loop;
    assert(EventLoop::GetEventLoopOfCurThread() == &loop);
    EventLoopThreadPool pool(&loop,3);
    pool.Start();
    //for(int i=0;i<2;i++)
    //{
    //  EventLoop* cur = pool.GetNextLoop();
    //  cur->RunInLoop(callback);
    //}
    sleep(3);
    for(int i=0;i<6;i++)
    {
      EventLoop* cur = pool.GetNextLoop();
      if(cur!=&loop)
      {
        cur->Quit();
      }
    }

    //loop.Loop();
}