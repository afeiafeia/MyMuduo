/*
 * @Author: your name
 * @Date: 2022-04-17 18:52:01
 * @LastEditTime: 2022-04-18 14:03:45
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /home/zaf/MyTinyMuduo/net/Channel.cpp
 */
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
        LOG_DEBUG(logger)<<"Channel with fd "<<m_fd<<" is created!";
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