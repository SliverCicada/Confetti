#pragma once
#ifndef _SRC_BASE_TEMPLATE_H_
#define _SRC_BASE_TEMPLATE_H_

#include "attribution.h"


//assert(ability::ErrCode == int)
template <typename ErrEnumType>
inline ability::ErrCode ErrEnumToCode(ErrEnumType ErrEnum)
{   return static_cast<ability::ErrCode>(ErrEnum);   }

template <typename ErrEnumType>
inline ErrEnumType ErrCodeToEnum(ability::ErrCode ErrCode)
{   return static_cast<ErrEnumType>(ErrCode);   }

//定时器基类
template <typename CounterSize = uint16_t, typename GoSpeedType = uint8_t>
class TimerBase
{
    protected:
    TimerBase() = delete;
    TimerBase(  CounterSize cd_time,
                CounterSize cd_left = cd_time,
                bool go_flag = true,
                GoSpeedType go_speed = static_cast<GoSpeedType>((uint8_t)'\001'))
            :cd_time_(cd_time)
            ,cd_left_(cd_left)
            ,go_flag_(go_flag == true ? (uint8_t)'\001' : (uint8_t)'\000')
            ,go_speed_(go_speed) {}
    TimerBase(const TimerBase& timer_base)
            :cd_time_(timer_base.cd_time_)
            ,cd_left_(timer_base.cd_left_)
            ,go_flag_(timer_base.go_flag_)
            ,go_speed_(timer_base.go_speed_) {}
    ~TimerBase() = default;

    protected:
    //取决于g0_flag_, 决定是否流逝一个speed
    void Go() { if(go_flag_.Bool())
                    cd_left_ = cd_left_ > go_speed ? cd_left_ - go_speed_ : 0; }
    //将go_flag设置为false 
    void Stop() { go_flag_.Reset(); }
    //将go_flag设置为true
    void Start() { go_flag_.Set(); }
    //获取当前状态
    bool IsGoing() { return go_flag_.Bool(); }

    protected:
    //设置流逝速度
    virtual void SetSpeed(GoSpeedType go_speed) { go_speed_ = go_speed; }
    //设置定时器的总cd
    virtual void SetCdTime(CounterSize cd_time) {cd_time_ = cd_time; }

    protected:
    //定时器总cd
    CounterSize cd_time_;
    //定时器剩余cd
    CounterSize cd_left_;
    //流逝速度
    GoSpeedType go_speed_;
    //正常工作与否
    bool8_t go_flag_;
};

//_PART_INTERFACE_
//计时器组件，模板类型为回调函数类型
template <typename CallBackFuncType
        , typename ParentCounterSizeType = uint16_t
        , typename ParentGoSpeedType = uint8_t>
class Timer
        :   virtual public TimerBase<ParentCounterSizeType, ParentGoSpeedType>
{
    public:
    Timer() = delete;
    Timer(uint32_t cd_time, uint32_t cd_left, CallBackFuncType call_back_func)
        :TimerBase(cd_time, cd_left)
        ,call_back_func_(call_back_func) {}
    Timer(const Timer& timer)
        :TimerBase(timer)
        ,call_back_func_(timer.call_back_func_) {}
    Timer(Timer&& timer) = default;
    ~Timer() = default;

    public:
    //是否归零
    bool Ready() { return cd_left_ == 0U;   }

    void ResetFunc(CallBackFuncType func) { call_back_func_ = func; }

    protected:
    CallBackFuncType call_back_func_;
};








#endif //_SRC_BASE_TEMPLATE_H_