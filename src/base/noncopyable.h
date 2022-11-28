/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    noncopyable.h
 * @brief   不可拷贝的对象的基类
 * @author  zhangafei
 * @date    2022-09-15
 * @email   1763029328@qq.com
 */
#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H
namespace afa
{
    //继承至该类的子类，都不能进行拷贝构造和赋值运算
    class noncopyable
    {
    protected:
        noncopyable(){}
        ~noncopyable(){}
    public:
        noncopyable(const noncopyable &rhs)=default;
        noncopyable& operator=(const noncopyable &rhs)=default;
    };
}
#endif
