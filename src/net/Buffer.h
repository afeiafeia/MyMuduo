#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <string>
namespace afa
{

    class Buffer
    {
    public:
        static const ssize_t KPrepend = 4;
        static const ssize_t KinitSize = 1024;

        static const ssize_t KTmpBuffSize = 65535;
    private:
        std::vector<char> m_buff;
        ssize_t           m_read_index;
        ssize_t           m_write_index;

    public:
        Buffer();
        ~Buffer();

        size_t ReadableBytes() const 
        {
            return m_write_index-m_read_index;
        }

        void Swap(Buffer &rhs)
        {
            m_buff.swap(rhs.m_buff);
            std::swap(m_read_index,rhs.m_read_index);
            std::swap(m_write_index,rhs.m_write_index);
        }

        size_t Size()const
        {
            return m_buff.size();
        }

        size_t Capacity()const
        {
            return m_buff.capacity();
        }
        size_t WritableBytes() const
        {
            return Capacity()-m_write_index;
        }

        size_t PrependableBytes() const
        {
            return m_read_index;
        }

        const char* Peek() const
        {
            return &m_buff[0]+m_read_index;
        }

        int32_t GetHeader32();

        int64_t GetHeader64();

        size_t ReadFd(int fd,int *err);

        size_t WriteFd(int fd,int *err);

        std::string ToString()const;
        
        void AppendInt32(int32_t num);
        void Append(const char* data,size_t len);

        void Append(const std::string &msg);

        void Prepend(const char* data,size_t len);

        void PrependInt32(int32_t num);
        
        std::string RetrieveAllAsString();
        void Retrieve(size_t len);

    private:
        std::string RetrieveAsString(size_t len);

        

        void Resize(size_t len);

    };
}


#endif