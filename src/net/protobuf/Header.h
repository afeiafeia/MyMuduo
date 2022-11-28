#ifndef HEADER_H
#define HEADER_H

#define MAX_CMD_LEN  64
namespace afa{
    struct Header{
        int  Magic;  //魔数
        int  MsgId;  //消息编号
        int  Length; //消息实体长度
        char CMD[MAX_CMD_LEN];//调用方法名称
    };
}


#endif