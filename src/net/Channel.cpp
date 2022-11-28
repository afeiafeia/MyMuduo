
#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>
#include "../base/Log.h"

namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    const int Channel::s_readEvent = EPOLLIN|EPOLLPRI;
    const int Channel::s_writeEvent = EPOLLOUT;
    const int Channel::s_noEvent = 0;



    Channel::Channel(int fd,EventLoop* loop)
    :m_fd(fd)
    ,m_loop(loop)
    ,m_events(0)
    ,m_state(ChannelState::UnRegistered)
    {
        //LOG_DEBUG(logger)<<"Channel with fd "<<m_fd<<" is created!";
    }

    Channel::~Channel()
    {
        LOG_DEBUG(logger)<<"Channel with fd "<<m_fd<<" is destroyed!";
        close(m_fd);
    }

    void Channel::ReadHandler()
    {
        if(m_readHandler)
        m_readHandler();
    }
    void Channel::WriteHandler()
    {
        if(m_writeHandler)
        m_writeHandler();
    }
    void Channel::ErrorHandler()
    {
        if(m_errorHandler)
        m_errorHandler();
    }

    void Channel::HandleEvent()
    {
        if((m_revents&EPOLLHUP)&&!(m_revents&EPOLLIN))
        {
            m_revents = 0;
            return ;
        }
        else if(m_revents&(EPOLLIN|EPOLLPRI|EPOLLRDHUP))
        {
            LOG_DEBUG(logger)<<"ReadHandle()!";
            ReadHandler();
        }
        else if(m_revents&EPOLLOUT)
        {
            WriteHandler();
        }
        else if(m_revents&EPOLLERR)
        {
            if(m_errorHandler)
            {
                ErrorHandler();
            }
            return;
        }
    }

    void Channel::Update()
    {
        m_loop->UpdateChannel(this);
    }
    void Channel::Remove()
    {
        m_loop->RemoveChannel(this);
    }

    void Channel::SetETTrigger()
    {
        m_events|EPOLLET;
    }
}