#include "CodeC.h"
#include <endian.h>
#include <google/protobuf/descriptor.h>
#include "../../base/Log.h"

namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    ProtobufCodeC::ProtobufCodeC()
    :m_error_cb(DefaultErrorCallBack)
    {

    }


    ProtobufCodeC::ProtobufCodeC(MessageCallBack &msg_callback)
    :m_message_cb(msg_callback)
    ,m_error_cb(DefaultErrorCallBack)
    {

    }

    ProtobufCodeC::~ProtobufCodeC()
    {

    }
    void ProtobufCodeC::DefaultErrorCallBack(SP_TcpConnection &sp_conn)
    {
        LOG_ERROR(logger)<<"Error in OnMessage()!";
        
    }
    void ProtobufCodeC::Send(SP_TcpConnection &sp_conn,const google::protobuf::Message &msg)
    {
        //msg应该是一个RpcMessage,所有的其它类型Message被放入RpcMessage
        //收到消息后，先通过RpcChannel的OnMessage获取RpcMessag对象并用OutBuffer中的内容初始化它；
        //然后调用OnRpcMessage获取其内的具体Message对象以及以该对象为参数所调用的函数
        LOG_INFO(logger)<<"构造Message并发送";
        Buffer buff;
        std::string type_name = msg.GetTypeName();
        int32_t name_len = type_name.size();

        std::string content = msg.SerializeAsString();

        int32_t len = s_header_len+s_header_len+static_cast<int32_t>(type_name.size())+static_cast<int32_t>(content.size());
        buff.AppendInt32(name_len);
        buff.Append(type_name);
        buff.Append(content);
        buff.PrependInt32(len);//总长度
        uint32_t test_len;//总长度
        memmove((void*)&test_len,(void*)(buff.Peek()),sizeof(int32_t));
        test_len = be32toh((uint32_t)test_len);
        LOG_INFO(logger)<<"报文长度是："<<test_len;
        LOG_INFO(logger)<<"Message 的TypeName："<<type_name;
        LOG_INFO(logger)<<"Message 的TypeName的长度是："<<name_len;
        sp_conn->Send(buff);
    }

    void ProtobufCodeC::OnMessage(SP_TcpConnection &sp_conn,Buffer &buff)
    {
        LOG_INFO(logger)<<"解析报文...";
        while(buff.ReadableBytes()>=s_min_msg_len+s_header_len)
        {
            //使用循环的目的：如果缓冲区中有多条消息，可以一直处理直到处理完或者剩下的是不完整消息
            uint32_t len;//总长度
            memcpy(&len,buff.Peek(),sizeof(int32_t));
            len = be32toh(len);
            LOG_INFO(logger)<<"长度: "<<len;
            if(len>s_max_msg_len||len<s_min_msg_len)
            {
                LOG_ERROR(logger)<<"报文长度不在有效范围内";
                LOG_ERROR(logger)<<"报文长度是："<<len;
                m_error_cb(sp_conn);
                break;
            }
            else if(buff.ReadableBytes()>=static_cast<size_t>(len))
            {
                LOG_DEBUG(logger)<<"存在完整消息";
                //至少有一个完整消息
                //LOG_DEBUG(logger)<<"Data's length: "<<len;
                //LOG_DEBUG(logger)<<std::string(buff.Peek(),len);
                SP_Message sp_msg = Parse(buff.Peek()+s_header_len,len-s_header_len);
                if(sp_msg&&m_message_cb)
                {
                    LOG_DEBUG(logger)<<"解析成功";
                    m_message_cb(sp_conn,sp_msg);
                    buff.Retrieve(len);
                }
                else
                {
                    LOG_ERROR(logger)<<"解析失败";
                    m_error_cb(sp_conn);
                    break;
                }
            }
            else
            {
                //是一个不完整消息
                LOG_ERROR(logger)<<"待解析消息不完整";
                break;
            }
        }
    }

    SP_Message ProtobufCodeC::Parse(const char* buf,size_t len)
    {
        SP_Message sp_message;
        //获取名字长度
        int32_t name_len;
        memcpy(&name_len,buf,sizeof(int32_t));
        name_len = be32toh(name_len);
        LOG_DEBUG(logger)<<"名字长度："<<name_len;
        //获取名字
        std::string name(buf+s_header_len,buf+s_header_len+name_len);
        LOG_DEBUG(logger)<<"名字："<<name;
        google::protobuf::Message* cur_msg = CreateMessage(name);
        if(cur_msg)
        {
            std::string content(buf+s_header_len+name_len,len-s_header_len-name_len);
            cur_msg->ParseFromString(content);
            sp_message.reset(cur_msg);
        }
        return sp_message;
    }

    google::protobuf::Message* ProtobufCodeC::CreateMessage(std::string type_name)
    {
        google::protobuf::Message* cur_msg = nullptr;
        const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

        if(descriptor)
        {
            const google::protobuf::Message* prototype = 
            google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
            if(prototype)
            {
                cur_msg = prototype->New();
            }
        }
        return cur_msg;
    }
}