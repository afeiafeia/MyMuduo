#include "Buffer.h"

#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <endian.h>
#include "../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    Buffer::Buffer()
    :m_buff(KinitSize+KPrepend)
    ,m_read_index(KPrepend)
    ,m_write_index(KPrepend)
    {

    }
    Buffer::~Buffer()
    {
        m_read_index = KPrepend;
        m_write_index = KPrepend;
    }
    void Buffer::Retrieve(size_t len)
    {
        assert(len<=ReadableBytes());
        if(len<ReadableBytes())
        {
            m_read_index+=len;
        }
        else
        {
            //读取完
            m_read_index = KPrepend;
            m_write_index = KPrepend;
        }
    }

    std::string Buffer::RetrieveAsString(size_t len)
    {
        assert(len<=ReadableBytes());
        std::string res(Peek(),len);
        Retrieve(len);
        return res;
    }

    std::string Buffer::RetrieveAllAsString()
    {
        return RetrieveAsString(ReadableBytes());
    }

    void Buffer::AppendInt32(int32_t num)
    {
        if(sizeof(int32_t)>=WritableBytes())
        {
            Resize(sizeof(int32_t));
        }
        uint32_t cur = num;
        num = htobe32(cur);
        LOG_INFO(logger)<<"Buffer::AppendInt32:"<<cur;
        LOG_INFO(logger)<<"Buffer::AppendInt32:"<<num;
        memmove((void*)(&m_buff[0]+m_write_index),(void*)&num,sizeof(int32_t));
        int x = 0;
        memmove((void*)(&x),(void*)(&m_buff[0]+m_write_index),sizeof(int32_t));
        LOG_INFO(logger)<<"Buffer::AppendInt32:"<<x;
        x = be32toh(x);
        LOG_INFO(logger)<<"Buffer::AppendInt32:"<<x;
        m_write_index+=sizeof(int32_t);
    }

    void Buffer::Append(const char* data,size_t len)
    {
        if(len>=WritableBytes())
        {
            Resize(len);
        }
        memcpy(&m_buff[0]+m_write_index,data,len);
        m_write_index+=len;
    }

    void Buffer::Append(const std::string &msg)
    {
        Append(msg.c_str(),msg.size());
    }

    void Buffer::Prepend(const char* data,size_t len)
    {
        assert(len<=PrependableBytes());
        int offset = PrependableBytes()-len;
        memcpy(&m_buff[offset],data,len);
        m_read_index = offset;

    }

    void Buffer::PrependInt32(int32_t num)
    {
        num = htobe32(num);
        Prepend((const char*)&num,sizeof(int32_t));
    }

    void Buffer::PrependInt64(int64_t num)
    {
        num = htobe64(num);
        Prepend((const char*)&num,sizeof(int64_t));
    }

    int32_t Buffer::GetHeader32()
    {
        int32_t res;
        memcpy(&res,&m_buff[m_read_index],sizeof(int32_t));
        Retrieve(sizeof(int32_t));
        return be32toh(res);
        //return res;
    }

    int64_t Buffer::GetHeader64()
    {
        int64_t res;
        memcmp(&res,&m_buff[m_read_index],sizeof(int64_t));
        Retrieve(sizeof(int64_t));
        return be64toh(res);
    }


    void Buffer::Resize(size_t len) 
    {
        if(WritableBytes()+PrependableBytes()<len+KPrepend)
        {
            //重新分配内存
            m_buff.resize((m_buff.capacity()+len)*2);
        }
        else
        {
            //不重新分配内存，将数据迁移到前面
            memcpy(&m_buff[0]+KPrepend,&m_buff[0]+m_read_index,ReadableBytes());
            m_read_index = KPrepend;
            m_write_index = m_read_index+ReadableBytes();
        }
    }

    size_t Buffer::ReadFd(int fd,int* err)
    {
        char tmp_buff[KTmpBuffSize];
        int writeable_bytes =WritableBytes();
        struct iovec vec[2];
        vec[0].iov_base = &m_buff[0]+m_write_index;
        vec[0].iov_len  = writeable_bytes;
        vec[1].iov_base = tmp_buff;
        vec[1].iov_len  = KTmpBuffSize;

        int read_bytes = readv(fd,vec,2);
        if(read_bytes<0)
        {
            *err = errno;
        }
        else if(read_bytes<=writeable_bytes)
        {
            m_write_index+=read_bytes;
        }
        else
        {
            
            m_write_index+=writeable_bytes;
            Append(tmp_buff,read_bytes-writeable_bytes);
        }
        return read_bytes;
    }

    size_t Buffer::WriteFd(int fd,int* err)
    {
        int write_bytes = write(fd,Peek(),ReadableBytes());
        if(write_bytes>0)
        {
            Retrieve(write_bytes);
        }
        else
        {
            *err = errno;
        }
        return write_bytes;
    }

    std::string Buffer::ToString() const
    {
        std::string res(Peek(),ReadableBytes());
        return res;
    }

}