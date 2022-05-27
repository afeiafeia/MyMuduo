#ifndef POLLER_H
#define POLLER_H
#include <vector>
#include <map>
#include "../base/TimeStamp.h"
namespace afa
{
    class EventLoop;
    class Channel;
    /**
     * @brief  IO复用基类
     * @author afei
     * @date   2022-03-27
     */
    class Poller
    {
    public:
    protected:
        std::map<int,Channel*> mapChannel;//io复用函数返回的是文件描述符，此map的作用是找到对应的Channel
        EventLoop* m_loop;
    private:
        //std::vector<Channel*> m_vctChannels;

    public:
        Poller(){};
        Poller(EventLoop* loop)
        :m_loop(loop){};
        
        virtual ~Poller(){};

        /**
         * @brief  IO复用函数
         * @author afei
         * @date   2022-03-27
         */
        virtual void Poll(std::vector<Channel*> &oActiveChannel,const TimeStamp &time)=0;

        /**
         * @brief  更新对于Channel的监视状态，添加进注册、移除或者修改
         * @param  pChannel         
         * @author afei
         * @date   2022-03-27
         */
        virtual void UpdateChannel(Channel* spChannel)=0;

        /**
         * @brief  移除相应的Channel
         * @param  pChannel         
         * @author afei
         * @date   2022-03-27
         */
        virtual void RemoveChannel(Channel* spChannel)=0;

    };
}

#endif