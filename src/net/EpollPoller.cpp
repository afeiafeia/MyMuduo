#include "EpollPoller.h"
#include "Channel.h"
#include "../base/Log.h"
#include <assert.h>
#include <unistd.h>
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    EpollPoller::EpollPoller(EventLoop* loop)
    :Poller(loop)
    ,m_epollfd(epoll_create(1))
    ,m_maxFd(16)
    {
        m_vctEvents.reserve(m_maxFd);
    }

    EpollPoller::~EpollPoller()
    {
        LOG_DEBUG(logger)<<"~EpollPoller()";
        close(m_epollfd);
    }

    void EpollPoller::EpollAdd(Channel* pChannel,int timeout)
    {
        //timeout用来设置定时器

        int fd = pChannel->Getfd();
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = pChannel->GetEvents();
        int error = epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,&ev);
        if(error<0)
        {
            LOG_ERROR(logger)<<"epoll_add fd = "<<fd;
        }
        else
        {
            LOG_DEBUG(logger)<<"add fd = "<<fd<<" to epoll: "<<m_epollfd<<"!";
        }
    }

    void EpollPoller::EpollDel(Channel* pChannel,int timeout)
    {
        int fd = pChannel->Getfd();
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = pChannel->GetEvents();
        if((epoll_ctl(m_epollfd,EPOLL_CTL_DEL,fd,&ev))<0)
        {
            LOG_ERROR(logger)<<"epoll_del fd = "<<fd;
        }
        else
        {
            LOG_DEBUG(logger)<<"delete fd = "<<fd<<" from epoll: "<<m_epollfd<<"!";
        }
    }

    void EpollPoller::EpollMod(Channel* pChannel)
    {
        int fd = pChannel->Getfd();
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = pChannel->GetEvents();
        if((epoll_ctl(m_epollfd,EPOLL_CTL_MOD,fd,&ev))<0)
        {
            LOG_ERROR(logger)<<"epoll_mod fd = "<<fd;
        }
    }

    void EpollPoller::UpdateChannel(Channel* pChannel)
    {
        Channel::ChannelState state = pChannel->GetState();
        if(state==Channel::ChannelState::UnRegistered)
        {
            //-1表示之前该套接字不在监听范围，2表示从之前的监听范围移除
            int fd = pChannel->Getfd();
            //如果括号内表达式为false，将输出错误消息程序退出，如果为true，继续往后执行
            assert(mapChannel.find(fd)==mapChannel.end());
            mapChannel[fd] = pChannel;
            EpollAdd(pChannel);
            pChannel->SetState(Channel::ChannelState::Registered);
            if(m_maxFd<fd)
            {
                m_maxFd = fd;
                m_vctEvents.reserve(m_maxFd);
            }
            LOG_DEBUG(logger)<<"EpollPoller add channel with fd = "<<pChannel->Getfd();
        }
        else
        {
            //修改已经存在的文件描述符的监听事件
            int fd = pChannel->Getfd();
            assert(mapChannel.find(fd)!=mapChannel.end());
            assert(mapChannel[fd]==pChannel);
            assert(pChannel->GetState()==Channel::ChannelState::Registered);
            if(pChannel->IsNoEvent())
            {
                RemoveChannel(pChannel);
            }
            else
            {
                EpollMod(pChannel);
            }
        }
    }

    void EpollPoller::RemoveChannel(Channel* pChannel)
    {
       
        if(pChannel==nullptr)
        {
            return;
        }
        LOG_DEBUG(logger)<<"EpollPoller remove channel with fd = "<<pChannel->Getfd();
        int fd = pChannel->Getfd();
        if(mapChannel.find(fd)==mapChannel.end())
        {
            return;
        }
        mapChannel.erase(fd);
        EpollDel(pChannel);
        pChannel->SetState(Channel::ChannelState::UnRegistered);
    }

    void EpollPoller::Poll(std::vector<Channel*> &oActiveChannel,const TimeStamp &timeout)
    {
        int time0 = timeout.GetTime();
        int num = epoll_wait(m_epollfd,&*m_vctEvents.begin(),m_maxFd,time0);
        LOG_DEBUG(logger)<<num <<"events is ready!";
        for(int i=0;i<num;i++)
        {
            int fd = m_vctEvents[i].data.fd;
            LOG_DEBUG(logger)<<fd<<"is ready!";
            assert(mapChannel.find(fd)!=mapChannel.end());
            Channel* pChannel = mapChannel[fd];
            __uint32_t curEvent = m_vctEvents[i].events;
            pChannel->SetRevents(curEvent);
            oActiveChannel.push_back(pChannel);
        }
    }




}