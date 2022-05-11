#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "../base/Log.h"
#include <unistd.h>
#include "TimerManager.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    TcpConnection::TcpConnection(EventLoop* loop,int fd,sockaddr* addr,socklen_t len)
    :m_loop(loop)
    ,m_sp_channel(new Channel(fd,loop))
    ,m_addr(addr)
    ,m_addr_len(len)
    ,m_state(Connecting)
    ,m_timer(nullptr)
    {
        m_sp_channel->SetReadHandle(std::bind(&TcpConnection::ReadHandle,this));
        m_sp_channel->SetWriteHandle(std::bind(&TcpConnection::WriteHandle,this));
        m_sp_channel->SetErrorHandle(std::bind(&TcpConnection::ErrorHandle,this));
    }

    TcpConnection::~TcpConnection()
    {
        LOG_DEBUG(logger)<<"TcpConnection is destoyed!";
        assert(m_state==DisConnected);
    }

    void TcpConnection::ConnectionEstablished()
    {
        SetState(Connected);
        m_sp_channel->EnableReading();
        m_connection_callBack(shared_from_this());

        //添加定时器
        m_timer = new Timer(std::bind(&TcpConnection::CloseHandle,shared_from_this()),(int64_t)(time(0)+3));
        m_loop->QueueInLoop(std::bind(&EventLoop::AddTimer,m_loop,m_timer));
    }

    void TcpConnection::ConnectionDestroyed()
    {
        m_loop->assertInLoopThread();
        if(m_state==Connected)
        {
            SetState(DisConnected);
            m_sp_channel->DisableAll();
        }
    }

    void TcpConnection::ReadHandle()
    {
        int read_bytes = m_read_buff.ReadableBytes();
        LOG_DEBUG(logger)<<"readable bytes is: "<<read_bytes;
        LOG_DEBUG(logger)<<"reading...";
        m_loop->assertInLoopThread();
        if(!m_sp_channel->IsETTrigger())
        {
           LOG_DEBUG(logger)<<"LT Model";
           //int n = read(m_sp_channel->Getfd(),m_read_buff+m_read_index,READ_BUFF_SIZE-m_read_index);
           int err;
           int n = m_read_buff.ReadFd(m_sp_channel->Getfd(),&err);
           if(n==0)
           {
              LOG_ERROR(logger)<<"close fd: "<<m_sp_channel->Getfd();
              m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
              CloseHandle();//关闭之前是不是要考虑把发送缓冲区中的数据发送出去？？？，应该关闭读端，继续监视写时间，写完之后自动关闭写端
              
              return;
           }
           else if(n<0)
           {
               if(err!=EAGAIN&&err!=EWOULDBLOCK)
               {
                    LOG_ERROR(logger)<<"Error In Read fd = "<<m_sp_channel->Getfd();
                    m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
                    ErrorHandle();//错误处理之前是不是要考虑把发送缓冲区中的数据发送出去？？？
               }
           }
        }
        else
        {
            LOG_DEBUG(logger)<<"ET Model";
            while(1)
            {
               //int n = read(m_sp_channel->Getfd(),m_read_buff+m_read_index,READ_BUFF_SIZE-m_read_index);
               int err;
               int n = m_read_buff.ReadFd(m_sp_channel->Getfd(),&err);
               if(n==0)
               {
                  LOG_DEBUG(logger)<<"close fd: "<<m_sp_channel->Getfd();
                  m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
                  CloseHandle();
                  return;
               }
               else if(n<0)
               {
                   if(err==EAGAIN||err==EWOULDBLOCK)
                   {
                       break;
                   }
                   else
                   {
                       LOG_DEBUG(logger)<<"Error In Read fd = "<<m_sp_channel->Getfd();
                       m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
                       ErrorHandle();
                   }
               }
            }
        }

        //更新定时器
        m_timer->SetTime(m_timer->GetTime()+3);
        m_loop->QueueInLoop(std::bind(&EventLoop::UpdateTimer,m_loop,m_timer));

        read_bytes = m_read_buff.ReadableBytes();
        LOG_DEBUG(logger)<<"readable bytes is: "<<read_bytes;
        std::string data = m_read_buff.ToString();
        m_message_callBack(shared_from_this(),m_read_buff);
        LOG_DEBUG(logger)<<"readed data is: "<<data;
        LOG_DEBUG(logger)<<"reading finish!";
        std::string response = m_write_buff.ToString();
        LOG_DEBUG(logger)<<"data in write_buff is: "<<response;
    }

    void TcpConnection::WriteHandle()
    {
        m_loop->assertInLoopThread();
        int remain = m_write_buff.ReadableBytes();
        LOG_DEBUG(logger)<<"writable bytes is: "<<remain;
        LOG_DEBUG(logger)<<"writing...";
        if(!m_sp_channel->IsETTrigger())
        {
            LOG_DEBUG(logger)<<"LT Model";
           int err;
           int nwrite = m_write_buff.WriteFd(m_sp_channel->Getfd(),&err);
           LOG_DEBUG(logger)<<"Writed byytes is "<<nwrite;
           if(nwrite<0)
           {
               if(err!=EAGAIN&&err!=EWOULDBLOCK)
               {
                    LOG_DEBUG(logger)<<"Error In Write fd = "<<m_sp_channel->Getfd();
               }
           }

        }
        else
        {
            while(1)
            {
               int err;
               int nwrite = m_write_buff.WriteFd(m_sp_channel->Getfd(),&err);
               if(nwrite<0)
               {
                   if(errno==EAGAIN||errno==EWOULDBLOCK)
                   {
                       break;
                   }
                   else
                   {
                       LOG_DEBUG(logger)<<"Error In Read fd = "<<m_sp_channel->Getfd();
                   }
               }
            }
        }
        if(m_write_buff.ReadableBytes()<=0)
        {
            //数据发送完，取消对写事件的监视
            m_sp_channel->DisableWriting();
            if(m_state==State::DisConnecting)
            {
                //如果之前关闭了读端：说明还有响应消息没发送出去，此时完成发送，关闭连接
                CloseHandle();
            }
        }
    }

    void TcpConnection::CloseHandle()
    {
        if(m_state==DisConnected)
        {
            return;
        }
        m_loop->assertInLoopThread();
        if(m_write_buff.Size()>0)
        {
            //还有完整响应报文待发送
            m_sp_channel->DisableReading();
            SetState(DisConnecting);
        }
        else
        {
            m_sp_channel->DisableAll();
            SetState(DisConnected);
        }
        LOG_DEBUG(logger)<<"TcpConnection's CloseCallback";
        m_close_callBack(shared_from_this());
        LOG_DEBUG(logger)<<"TcpConnection's CloseCallback Done";
    }

    void TcpConnection::ErrorHandle()
    {
        //错误处理：输出出错信息
        LOG_ERROR(logger)<<"Channel's revent is error!";
        CloseHandle();
    }

    void TcpConnection::Send(std::string &msg)
    {
        m_write_buff.Append(msg);
        if(!m_sp_channel->IsWriting())
        {
            m_sp_channel->EnableWriting();
        }
    }

    void TcpConnection::Send(Buffer &buf)
    {
        //有完整的相应消息被放入发送缓冲区，开始监视可写事件
        LOG_INFO(logger)<<"push to write_buff!";
        m_write_buff.Swap(buf);
        if(!m_sp_channel->IsWriting())
        {
            LOG_INFO(logger)<<"starting to listen writing event!";
            m_sp_channel->EnableWriting();
        }
    }


}