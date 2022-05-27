#ifndef CALLBACK_H
#define CALLBACK_H
#include <functional>
#include <memory>

namespace afa
{
    class TcpConnection;
    class Buffer;
    typedef std::function<void(const std::shared_ptr<TcpConnection>)> CloseCallBack;
    typedef std::function<void (const std::shared_ptr<TcpConnection>)> ConnectionCallback;
    typedef std::function<void (const std::shared_ptr<TcpConnection>,Buffer&)> MessageCallback;
}

#endif