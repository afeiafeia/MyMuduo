#ifndef CHANNEL_H
#define CHANNEL_H
#include <functional>
#include <sys/epoll.h>
#include <memory>

namespace afa
{
    class EventLoop;
    class Channel
    {
    public:
        typedef std::function<void()> CallBack;
        enum ChannelState
        {
            Registered=0,
            UnRegistered
        };

    public:
        enum TriggerModel
        {
            LT=0,
            ET
        };

    private:
        int        m_fd;
        EventLoop* m_loop;     //所属的事件循环
        __uint32_t m_events;   //监视事件
        __uint32_t m_revents;  //就绪事件

        ChannelState m_state;  //标记描述符是否已注册到epoll
        

        static const int s_readEvent;
        static const int s_writeEvent;
        static const int s_noEvent;

        //回调函数
        //对于一个套接字，处理三种事件：可读、可写、出错
        //不对其注册close事件，close由出错事件的回调函数进行处理
        CallBack m_readHandler;
        CallBack m_writeHandler;
        CallBack m_errorHandler;
    public:
        Channel(int fd,EventLoop* loop);

        ~Channel();

        /**
         * @brief  注册可读事件
         * @author afei
         * @date   2022-03-27
         */
        void EnableReading()
        {
            m_events |= s_readEvent;
            Update();
        }

        /**
         * @brief  取消对可读事件的监视
         * @author afei
         * @date   2022-04-10
         */
        void DisableReading()
        {
            m_events &= ~s_readEvent;
            Update();
        }

        /**
         * @brief  注册可写事件
         * @author afei
         * @date   2022-03-27
         */
        void EnableWriting()
        {
            m_events |= s_writeEvent;
            Update();
        }
        /**
         * @brief  取消对可写事件的监视
         * @author afei
         * @date   2022-04-10
         */
        void DisableWriting()
        {
            m_events &= ~s_writeEvent;
            Update();
        }

        void DisableAll()
        {
            m_events=s_noEvent;
            Update();
        }

        bool IsNoEvent() const 
        {
            return m_events==s_noEvent;
        }

        bool IsReading() const
        {
            return m_events&s_readEvent;
        }

        bool IsWriting() const 
        {
            return m_events&s_writeEvent;
        }

        void SetReadHandle(CallBack readHandle)
        {
            m_readHandler = readHandle;
        }

        void SetWriteHandle(CallBack writeHandle)
        {
            m_writeHandler = writeHandle;
        }

        void SetErrorHandle(CallBack errorHandler)
        {
            m_errorHandler = errorHandler;
        }


        void ReadHandler();
        void WriteHandler();
        void ErrorHandler();

        /**
         * @brief  事件处理函数
         * @author afei
         * @date   2022-03-27
         */
        void HandleEvent();

        void SetEvents(__uint32_t event)
        {
            m_events = event;
        }

        void SetRevents(__uint32_t revent)
        {
            m_revents = revent;
        }
        __uint32_t GetEvents()
        {
            return m_events;
        }

        int Getfd() const
        {
            return m_fd;
        }

        ChannelState GetState() const
        {
            return m_state;
        }


        void SetState(ChannelState state)
        {
            m_state = state;
        }

        void Remove();

        void SetETTrigger();

        bool IsETTrigger()
        {
            return m_events&EPOLLET;
        }

    private:
        /**
         * @brief  更新所监视的事件
         * @author afei
         * @date   2022-03-27
         */
        void Update();








        


    };
}




#endif
