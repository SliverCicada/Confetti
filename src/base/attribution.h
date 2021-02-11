#pragma once
#ifndef _SRC_BASE_ATTR_H_
#define _SRC_BASE_ATTR_H_

#include "template.h"
#include <stdint.h>
#include <utility>
#include <memory>

//#ifndef _SHIELD_RAW_DirectionBase
//ctrl+k ctrl+c
//ctrl+k ctrl+u
/*  abadon */
/* */

//用一字节存储flag
class bool8_t
{
    public:
    bool8_t() = delete;
    bool8_t(bool flag);
    //显式构造确保没有歧义
    explicit bool8_t(int flag);
    explicit bool8_t(uint32_t flag);
    
    bool8_t(const bool8_t& flag);
    //不含有内存分配的类和全部都是基础类型的类只能拷贝构造
    //bool8_t(bool8_t&& flag);
    ~bool8_t() = default;

    bool8_t& operator = (const bool8_t& flag);
    operator bool();

    public:
    //转换成bool类型
    bool Bool();

    //重置为true
    void Set();
    
    //重置为false
    void Reset();

    private:
    uint8_t flag_;
    
    public:
    static const constexpr uint8_t True = (uint8_t)'\001';   // 1U - 127U
    static const constexpr uint8_t False = (uint8_t)'\000';  // 0U
};
/**** to_string(bool_8_t); **/


//Distance表示两点间距离
using Distance = std::pair<uint32_t, uint32_t>;
using Dirction = std::pair<uint32_t, uint32_t>;

enum class DirEnum : uint8_t
{
    NoDir       = 0U,
    East        = 1U,
    EastNorth   = 2U,
    North       = 3U,
    WestNorth   = 4U,
    West        = 5U,
    WestSouth   = 6U,
    South       = 7U,
    EastSouth   = 8U,
};

//继承自std::派人<uint32_t, uint32_t>;
class DirectionBase
{
                                                    /*方向        X    Y */
    static const constexpr std::pair<uint32_t, uint32_t> NoDir     = {  0,   0  };
    static const constexpr std::pair<uint32_t, uint32_t> East      = {  1,   0  };
    static const constexpr std::pair<uint32_t, uint32_t> EastNorth = {  1,   1  };
    static const constexpr std::pair<uint32_t, uint32_t> North     = {  0,   1  };
    static const constexpr std::pair<uint32_t, uint32_t> WestNorth = { -1,   1  };
    static const constexpr std::pair<uint32_t, uint32_t> West      = { -1,   0  };
    static const constexpr std::pair<uint32_t, uint32_t> WestSouth = { -1,  -1  };
    static const constexpr std::pair<uint32_t, uint32_t> South     = {  0,  -1  };
    static const constexpr std::pair<uint32_t, uint32_t> EastSouth = {  1,  -1  };

    public:
    Dirction constexpr operator [](const DirEnum dir_enum);
    Dirction constexpr operator [](const uint32_t dir_index);
    
    private:
    Dirction constexpr EnumToDir(const DirEnum dir_enum);
};
//方向作为全局变量
extern const constexpr DirectionBase dirction = {};

//定义 k * dirction = distance || k * distance = distance
std::pair<uint32_t, uint32_t> operator * 
    (const std::pair<uint32_t, uint32_t>& distance, uint32_t k);
std::pair<uint32_t, uint32_t> operator * 
    (uint32_t k, const std::pair<uint32_t, uint32_t>& distance);

#define _ABILITY_INTERFACE_
namespace ability   {

_ABILITY_INTERFACE_
//用以记录ability是否被使用过
class AbilityBase 
{
    public:
    //使ability可用
    void Refresh();
    
    protected:
    //返回ability是否可用
    bool IsUsable();
    //在子类使用过能力后, 将标值置为不可用
    void Use();
    //可用与否的标值
    bool8_t use_flag_ = (uint8_t)'\001';    //true
};

using ErrCode = int;

_ABILITY_INTERFACE_
//移动接口
class Movable   
        :   virtual public AbilityBase
{
    public:
    //是否可以移动
    bool IsMovable();
    //获取移动cd
    //uint32_t GetMoveCd();
    //获取剩余移动cd
    //uint32_t GetMoveCdLeft();
    //移动失败枚举
    enum class MoveFail : ErrCode
    {
        //避免由于不明原因导致的父类纯虚函数调用
        PURE_VIRTUAL_FUNC_CALL = INT32_MAX,
        
        //to do
        SUCCESS = 0,
        ABILITY_USED = -1,
        //IN_CD = -2,

    };
    //移动接口, 默认返回PURE_VIRTUAL_FUNC_CALL
    virtual MoveFail MoveTo() = 0;  
};


_ABILITY_INTERFACE_
//攻击力及攻击接口
class Attackable    
        :   virtual public AbilityBase
{
    public:
    //获取攻击力
    uint32_t GetDamage();
    //增加攻击力, 16表示最大上界
    virtual void DamageIncrease(uint32_t increment16);
    //减少攻击力
    virtual void DamageDecrease(uint32_t decrement16);
    //攻击失败枚举
    enum class AttackFail : ErrCode
    {
        //避免由于不明原因导致的父类纯虚函数调用
        PURE_VIRTUAL_FUNC_CALL = INT32_MAX,
        //to de
        SUCCESS = 0,
        ABILITY_USED = -1,
        //IN_CD = -2,
    };
    //攻击接口, 默认返回纯虚函数调用
    virtual AttackFail Attack() = 0;

    protected:
    uint16_t damage_;
};

_ABILITY_INTERFACE_
//治疗（维修）及治疗（维修）接口
class Treatable    
        :   virtual public AbilityBase
{
    public:
    //获取治疗量
    uint32_t GetTreatment();
    //增加治疗量
    virtual void TreatIncrease(uint32_t increment16);
    //减少治疗量
    virtual void TreatDecrease(uint32_t decrement16);
    //治疗失败枚举
    enum class TreatFail : ErrCode
    {
        //避免由于不明原因导致的父类纯虚函数调用
        PURE_VIRTUAL_FUNC_CALL = INT32_MAX,
        //to do
        SUCCESS = 0,
        ABILITY_USED = -1,

    };
    //治疗接口
    virtual TreatFail Treat() = 0;

    protected:
    //治疗数值
    uint16_t treatment_;
};

//todo
_ABILITY_INTERFACE_
//生产其他单位的接口
template<typename PeoductType>
class Producible    
        :   virtual public Ability
        ,   virtual public TimerBase<uint32_t>
{
    public:
    virtual ProductType& Produce() = 0;
};


};  //namespace ability end


#define _PART_INTERFACE_    
//对象的组件
namespace parts {

_PART_INTERFACE_
//在房间内的偏移坐标对, 低四位x, 高四位y x,y <= [0, 63] 
class Offset 
{
    public:
    Offset() = delete;
    Offset(int x, int y);
    Offset(uint32_t x, uint32_t y);
    //Offset(uint32_t x, uint32_t y);
    //Offset(pair<int, int> x_y);
    //Offset(pair<uint8_t, uint8_t> x_y);
    //Offset(pair<uint32_t, uint32_t> x_y);
    //Offset(initializer_list<int> x_y);
    //Offset(initializer_list<uint32_t> x_y);
    Offset(const Offset& offset);
    Offset(Offset&& offset) = default;
    ~Offset() = default;

    public:
    /*低四位*/
    uint32_t GetX();
    /*高四位*/
    uint32_t GetY();

    protected:
    uint8_t offset_x_;
    uint8_t offset_y_;
};
using Pos = Offset;

_PART_INTERFACE_
//房间在世界中的坐标对
class GPosition 
{
    public:
    GPosition();
    GPosition(int x, int y);
    GPosition(const GPosition& Gposition);
    GPosition(GPosition&& Gposition) = default;
    ~GPosition() = default;

    public:
    /*获取房间横坐标*/
    int GetGloX();
    /*获取房间纵坐标*/
    int GetGloY();
    /*判断房间是否在指定的区域内, 起点坐标, 起点坐标X轴区间长度, 起点坐标Y轴区间长度*/
    static bool IsInBlocks(const GPosition& check, 
                            const GPosition& original, int x_range, int y_range);
    /*判断房间是否在指定的区域内, X轴起点, X轴结束点, Y轴起点, Y轴结束点*/
    static bool IsInBlocks(const GPosition& check,
                            int x_begin, int x_end, int y_begin, int y_end);

    protected:
    int64_t glo_x_;
    int64_t glo_y_; 
};
//using World = GPosition; 

_PART_INTERFACE_
//生命值组件
class Health {
    public:          
    Health() = delete;        
    Health(uint32_t hp_max16);
    Health(const Health& hp);
    Health(Health&& hp) = default;
    ~Health() = default;

    public:
    //under_half = false, 不满半血才返回true, 否则非满血返回true
    bool NeedHeal(bool under_half = false);

    bool IsAlive();

    uint32_t GetHpMax();

    uint32_t GetHp();

    public:
    virtual void UnderAttack(uint32_t hurt_value);
    
    virtual void OnHeal(uint32_t heal_value);

    protected:
    uint16_t health_max_;
    
    uint16_t current_hp_;
};
using HP = Health;

_PART_INTERFACE_
//占地面积组件
class CoverArea 
{
    private:
    //占地面积的枚举值, 边长只能为 1 - 7 整数
    enum class EnumCoverArea : uint8_t 
    {
        OneSqure = 1U,
        TwoSqure = 4U,
        ThreeSqure = 9U,
        FourSqure = 16U,
        FiveSqure = 25U,
        SixSqure  = 36U,
        SevenSqure = 49U,
    };
    
    protected:

    EnumCoverArea enum_cover_area_;
    
    Offset start_Gposition_;
    
    public:
    uint32_t GetCoverSize();

    Offset   GetStartGPosition();

    uint32_t GetSideLen();
};

}



#endif _SRC_BASE_ATTR_H_
