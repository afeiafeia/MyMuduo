#include "../net/EventLoop.h"
#include "../Thread.h"
#include "../Log.h"
using namespace afa;
afa::Logger::ptr curLog = LOG_ROOT();

void callback()
{
  LOG_DEBUG(curLog)<<"callback()";
  //printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
}
void threadFunc()
{
  LOG_DEBUG(curLog)<<"WorkThreadInfor";

  assert(EventLoop::GetEventLoopOfCurThread() == NULL);
  EventLoop loop;
  assert(EventLoop::GetEventLoopOfCurThread() == &loop);
  loop.RunInLoop(callback);
  loop.Loop();
}
int main()
{
    LOG_DEBUG(curLog)<<"MainThreadInfor";
    EventLoop loop;
    assert(EventLoop::GetEventLoopOfCurThread() == &loop);
    Thread thread(threadFunc);

    thread.Start();

    loop.Loop();
}