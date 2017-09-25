#ifndef __NO_COPYALBE_H__
#define __NO_COPYALBE_H__

/*
定义一个不能被继承的类
*/
namespace GakkiNet
{

class NoCopyAble
{
    public:
        NoCopyAble() = default;
        ~NoCopyAble() = default;
    private:
        NoCopyAble& operator=(const NoCopyAble& rhs) ;
        NoCopyAble(const NoCopyAble& rhs) ;
};

}

#endif