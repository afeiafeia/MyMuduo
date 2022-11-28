#ifndef EPOLL_POLLER_H
#define EPOLL_POLLER_H

#include <vector>
#include <memory>
#include <sys/epoll.h>
#include "Poller.h"
#include "../base/TimeStamp.h"
namespace afa
{
    class EventLoop;
    class Channel;
    class EpollPoller:public Poller
    {
    public:
    private:
        int m_epollfd;
        std::vector<epoll_event> m_vctEvents;//用于epoll_wait的参数，存放返回的就绪的事件
        int32_t                  m_maxFd;//所监视的最大描述符


    public:
        EpollPoller(EventLoop* loop);
        ~EpollPoller();

        void UpdateChannel(Channel* pChannel);

        void Poll(std::vector<Channel*> &oActiveChannel,const TimeStamp &time);
        
    private:
        void RemoveChannel(Channel* pChannel);

        void EpollAdd(Channel* pChannel,int timeout=5);
        void EpollDel(Channel* pChannel,int timeout=5);
        void EpollMod(Channel* pChannel);

        

    };
}


#endif