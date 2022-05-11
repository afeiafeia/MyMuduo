#include "TimerHeap.h"
#include "../base/MutexLockGuard.h"
#include <ctime>
namespace afa
{
    TimerHeap::TimerHeap()
    :m_lock()
    {

    }
    TimerHeap::~TimerHeap()
    {

    }


    void TimerHeap::AddTimer(Timer* iAddTimer)
    {
        m_vctTimer.push_back(iAddTimer);
        int curIndex = (int)m_vctTimer.size()-1;//最后一个元素的下标 
        int fatherIndex = (curIndex-1)/2;//父节点的下标

        //过期时间最小（最近要过期）的节点上浮
        while(curIndex>0&&m_vctTimer[curIndex]->GetTime()<m_vctTimer[fatherIndex]->GetTime())
        {
            std::swap(m_vctTimer[curIndex],m_vctTimer[fatherIndex]);
            curIndex =fatherIndex;
            fatherIndex = (curIndex-1)/2;
        }
    }

    void TimerHeap::DelTimer()
    {
        int64_t now = time(NULL);
        
        while(!m_vctTimer.empty()&&m_vctTimer[0]->GetTime()<now)
        {
            PopTimer();
        }

    }

    void TimerHeap::PopTimer()
    {
        if(m_vctTimer.empty())
        {
            return;
        }
        std::swap(m_vctTimer[0],m_vctTimer.back());
        int lastIndex = m_vctTimer.size()-2;
        int fatherIndex = 0;
        int childIndex = 1;
        while(childIndex<=lastIndex)
        {
            if(childIndex<lastIndex&&m_vctTimer[childIndex]->GetTime()>m_vctTimer[childIndex+1]->GetTime())
            {
                childIndex++;
            }
            if(m_vctTimer[childIndex]->GetTime()<m_vctTimer[fatherIndex]->GetTime())
            {
                std::swap(m_vctTimer[childIndex],m_vctTimer[fatherIndex]);
                fatherIndex = childIndex;
                childIndex = fatherIndex*2+1;
            }
            else
            {
                break;
            }
        }
        //被删除的元素放到容器的末尾
        Timer* back = m_vctTimer.back();
        delete back;
        m_vctTimer.pop_back();
    }

    void TimerHeap::UpdateTimer(Timer* timer)
    {
        int64_t expire = timer->GetTime();
        int index = Search(timer);
        if(index<0)
        {
            return;
        }
        int last_index = (int)m_vctTimer.size()-1;
        int father_index = (index-1)>>1;
        while(father_index>=0)
        {
            if(timer->GetTime()<m_vctTimer[father_index]->GetTime())
            {
                std::swap(m_vctTimer[index],m_vctTimer[father_index]);
                index = father_index;
            }
            else
            {
                break;
            }
            father_index = (index-1)>>1;
        }
        int child_index = (index<<1)+1;
        while(child_index<last_index)
        {
            if(child_index<last_index-1&&m_vctTimer[child_index]->GetTime()>m_vctTimer[child_index+1]->GetTime())
            {
                child_index++;
            }
            if(m_vctTimer[index]->GetTime()>m_vctTimer[child_index]->GetTime())
            {
                std::swap(m_vctTimer[index],m_vctTimer[child_index]);
                index = child_index;
            }
            else
            {
                break;
            }
            child_index = (index<<1)+1;
        }

    }

    int TimerHeap::Search(Timer* timer)
    {
        for(int i=0;i<m_vctTimer.size();i++)
        {
            if(m_vctTimer[i]==timer)
            {
                return i;
            }
        }
        return -1;
    }

    void TimerHeap::EraseTimer(Timer* timer)
    {
        int index = Search(timer);
        if(index<0)
        {
            return ; 
        }
        timer->SetTime(0);
        UpdateTimer(timer);
        PopTimer();

    }


}