#ifndef EPOLL_POLLER_H
#define EPOLL_POLLER_H

#include <vector>
#include <memory>
#include <sys/epoll.h>
#include "Poller.h"
namespace afa
{
    class EventLoop;
    class Channel;
    class EpollPoller:public Poller
    {
    public:
    private:
        int m_epollfd;
        std::vector<epoll_event> m_vctEvents;

    public:
        EpollPoller(EventLoop* loop);
        ~EpollPoller();

        void UpdateChannel(Channel* pChannel);

        void Poll(std::vector<Channel*> &oActiveChannel);
        
    private:
        void RemoveChannel(Channel* pChannel);

        void EpollAdd(Channel* pChannel,int timeout=5);
        void EpollDel(Channel* pChannel,int timeout=5);
        void EpollMod(Channel* pChannel);

        

    };
}


#endif