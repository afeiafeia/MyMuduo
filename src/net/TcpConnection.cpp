#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "../base/Log.h"
#include <unistd.h>
#include "TimerManager.h"
#include <string.h>
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    TcpConnection::TcpConnection(EventLoop* loop,int fd,const InetAddress &address)
    :m_loop(loop)
    ,m_sp_channel(new Channel(fd,loop))
    ,m_state(Connecting)
    ,m_timer(nullptr)
    ,m_address(address)
    {
        m_sp_channel->SetReadHandle(std::bind(&TcpConnection::ReadHandle,this));
        m_sp_channel->SetWriteHandle(std::bind(&TcpConnection::WriteHandle,this));
        m_sp_channel->SetErrorHandle(std::bind(&TcpConnection::ErrorHandle,this));
        LOG_DEBUG(logger)<<"TcpConnection is created!";
    }

    TcpConnection::~TcpConnection()
    {
        LOG_DEBUG(logger)<<"TcpConnection is destoyed!";
        assert(m_state==DisConnected);
    }

    void TcpConnection::ConnectionEstablished()
    {
        LOG_DEBUG(logger)<<"Connected!";
        SetState(Connected);
        m_sp_channel->EnableReading();
        if(m_connection_callBack)
        {
            m_connection_callBack(shared_from_this());
        }

        //添加定时器：将在m_update_time时候后超时
        //m_timer = new Timer(std::bind(&TcpConnection::CloseHandle,shared_from_this()),TimeStamp::Now()+m_update_time,0);
        //m_loop->QueueInLoop(std::bind(&EventLoop::AddTimer,m_loop,m_timer));
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
              LOG_DEBUG(logger)<<"收到FIN";
              LOG_ERROR(logger)<<"close fd: "<<m_sp_channel->Getfd();
              m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
              CloseHandle();//关闭之前是不是要考虑把发送缓冲区中的数据发送出去？？？，应该关闭读端，继续监视写事件，写完之后自动关闭写端
              //m_first_fin++;
              //if(m_first_fin==2)
              //{
              //    CloseHandle();
              //}
              //return;
           }
           else if(n<0)
           {
               if(err!=EAGAIN&&err!=EWOULDBLOCK)
               {
                    LOG_ERROR(logger)<<"Error In Read fd = "<<m_sp_channel->Getfd();
                    LOG_DEBUG(logger)<<"errno is: "<<strerror(errno);
                    m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
                    ErrorHandle();//错误处理之前是不是要考虑把发送缓冲区中的数据发送出去？？？

                    return;
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
                       LOG_DEBUG(logger)<<"errno is: "<<strerror(errno);
                       m_loop->QueueInLoop(std::bind(&EventLoop::EraseTimer,m_loop,m_timer));
                       ErrorHandle();
                       return;
                   }
               }
            }
        }

        //更新定时器
        //m_timer->SetTime(m_timer->GetTime()+m_update_time);
        //m_loop->QueueInLoop(std::bind(&EventLoop::UpdateTimer,m_loop,m_timer));

        read_bytes = m_read_buff.ReadableBytes();
        LOG_DEBUG(logger)<<"readable bytes is: "<<read_bytes;
        std::string data = m_read_buff.ToString();
        LOG_DEBUG(logger)<<"readed data is: "<<data;
        SP_TcpConnection self = shared_from_this();
        if(m_message_callBack){
            m_message_callBack(self,m_read_buff);
        }
        //LOG_DEBUG(logger)<<"readed data is: "<<data;
        LOG_DEBUG(logger)<<"reading finish!";
        std::string response = m_write_buff.ToString();
        LOG_DEBUG(logger)<<"data in write_buff is: "<<response;
    }

    void TcpConnection::WriteHandle()
    {
        m_loop->assertInLoopThread();
        int remain = m_write_buff.ReadableBytes();
        LOG_DEBUG(logger)<<"writable bytes is: "<<remain;
        if(!m_sp_channel->IsETTrigger())
        {
            LOG_DEBUG(logger)<<"LT Model";
           int err;
           int nwrite = m_write_buff.WriteFd(m_sp_channel->Getfd(),&err);
           LOG_DEBUG(logger)<<"Writed bytes is "<<nwrite;
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
        if(m_write_buff.ReadableBytes()>0)
        {
            //还有完整响应报文待发送
            //不再监视可读事件，等到把发送缓冲区中的数据发送出去后就关闭连接
            m_sp_channel->DisableReading();
            SetState(DisConnecting);
        }
        else
        {
            m_sp_channel->DisableAll();
            SetState(DisConnected);
            LOG_DEBUG(logger)<<"TcpConnection's CloseCallback";
            if(m_close_callBack)
            {
                m_close_callBack(shared_from_this());
            }
            LOG_DEBUG(logger)<<"TcpConnection's CloseCallback Done";
        }
        
    }

    void TcpConnection::ErrorHandle()
    {
        //错误处理：输出出错信息
        LOG_ERROR(logger)<<"Channel's revent is error!";
        CloseHandle();
    }

    void TcpConnection::Send(std::string &msg)
    {
         //有完整的相应消息被放入发送缓冲区，开始监视可写事件
        LOG_INFO(logger)<<"push to write_buff!";
        m_write_buff.Append(msg);
        //m_write_buff.PrependInt32((int32_t)msg.size());
        if(!m_sp_channel->IsWriting())
        {
            LOG_INFO(logger)<<"starting to listen writing event!";
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