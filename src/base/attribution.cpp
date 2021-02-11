#pragma once
#ifndef _SRC_BASE_ATTR_CPP_
#define _SRC_BASE_ATTR_CPP_

#include "attribution.h"

bool8_t::bool8_t(bool flag) 
        :   flag_(flag == true ? bool8_t::True : bool8_t::False) {}

bool8_t::bool8_t(uint32_t flag)
        :   flag_(flag > 0 ? bool8_t::True : bool8_t::False) {}

bool8_t::bool8_t(const bool8_t& flag)   :   flag_(flag.flag_) {}

inline bool8_t& bool8_t::operator=(const bool8_t& flag)
{   flag_ = flag.flag_; return *this;   }

inline bool8_t::operator bool()
{ return flag_ >= bool8_t::True ? true : false; }

inline bool bool8_t::Bool() { return flag_ >= bool8_t::True ? true : false; }

inline void bool8_t::Set() { flag_ = bool8_t::True; }

inline void bool8_t::Reset() { flag_ = bool8_t::False; }

inline constexpr Dirction DirectionBase::EnumToDir(const DirEnum dir_enum)
{
    using DirBase = DirectionBase;
    switch (dir_enum)
    {
        case DirEnum::NoDir     : return DirBase::NoDir;
        case DirEnum::East      : return DirBase::East;
        case DirEnum::EastNorth : return DirBase::EastNorth;
        case DirEnum::North     : return DirBase::North;
        case DirEnum::WestNorth : return DirBase::WestNorth;
        case DirEnum::West      : return DirBase::West;
        case DirEnum::WestSouth : return DirBase::WestSouth;
        case DirEnum::South     : return DirBase::South;
        case DirEnum::EastSouth : return DirBase::EastSouth;
        default:    return DirBase::NoDir;
    }
}

inline constexpr Dirction DirectionBase::operator [] (const DirEnum dir_enum)
{ return EnumToDir(dir_enum); }

inline constexpr Dirction DirectionBase::operator[] (uint32_t dir_index)
{ return EnumToDir(static_cast<DirEnum>(dir_index % (unsigned int)9)); }

std::pair<uint32_t, uint32_t> operator * 
    (const std::pair<uint32_t, uint32_t>& distance, uint32_t k)
{   return std::move(std::pair<uint32_t, uint32_t>(distance.first * k, 
                                                distance.second * k));  }
std::pair<uint32_t, uint32_t> operator * 
    (uint32_t k, const std::pair<uint32_t, uint32_t>& distance)
{   return std::move(std::pair<uint32_t, uint32_t>(distance.first * k,
                                                distance.second * k));  }

// vvv BASE_ABILITY_INTERFACE
inline void ability::AbilityBase::Refresh()
{   use_flag_ = bool8_t::True;  }

inline bool ability::AbilityBase::IsUsable()
{   return use_flag_.Bool();    }

inline void ability::AbilityBase::Use()
{   use_flag_ = bool8_t::False; }

// vvv MOVE_ABILITY_INTERFACE   
inline bool ability::Movable::IsMovable()
{   return use_flag_.Bool();    }

ability::Movable::MoveFail ability::Movable::MoveTo()
{   return ability::Movable::MoveFail::PURE_VIRTUAL_FUNC_CALL;  }

// vvv ATACK_ABILITY_INTERFACE
inline uint32_t ability::Attackable::GetDamage()
{   return static_cast<uint32_t>(damage_);  }

inline void ability::Attackable::DamageIncrease(uint32_t increment16)
{   
    decltype(damage_) change = static_cast<decltype(damage_)>(increment16);
    damage_ = change <= UINT16_MAX - damage_ ? damage_ + change : damage_;
}

inline void ability::Attackable::DamageDecrease(uint32_t decrement16)
{
    decltype(damage_) change = static_cast<decltype(damage_)>(decrement16);
    damage_ = change <= damage_ ? damage_ - change : damage_;
}

ability::Attackable::AttackFail Attack()
{   return ability::Attackable::AttackFail::PURE_VIRTUAL_FUNC_CALL; }

// vvv TREAT_ABILITY_INTERFACE
inline uint32_t ability::Treatable::GetTreatment()
{   return static_cast<uint32_t>(treatment_);   }

inline void ability::Treatable::TreatIncrease(uint32_t increment16)
{
    decltype(treatment_) change = static_cast<decltype(treatment_)>(increment16);
    treatment_ = change <= UINT16_MAX - treatment_ ? treatment_ + change : treatment_;
}

inline void ability::Treatable::TreatDecrease(uint32_t decrement16)
{
    decltype(treatment_) change = static_cast<decltype(treatment_)>(decrement16);
    treatment_ = change <= treatment_ ? treatment_ - change : treatment_;
}

ability::Treatable::TreatFail ability::Treatable::Treat()
{   return ability::Treatable::TreatFail::PURE_VIRTUAL_FUNC_CALL;   }

// vvv PPRODUCABLE_INTERFACE





// vvv OFFSET_PART
// parts::Offset::Offset() 
//         :offset_x_((uint8_t)'\000')
//         ,offset_y_((uint8_t)'\000') {}

parts::Offset::Offset(int x, int y)
        :offset_x_((uint8_t)((unsigned int)x % 64))
        ,offset_y_((uint8_t)((unsigned int)y % 64)) {}

parts::Offset::Offset(uint32_t x, uint32_t y)
        :offset_x_((uint8_t)(x % 64))
        ,offset_y_((uint8_t)(y % 64)) {}

parts::Offset::Offset(const Offset& offset)
        :offset_x_(offset.offset_x_)
        ,offset_y_(offset.offset_y_) {}

inline uint32_t parts::Offset::GetX()
{   return static_cast<uint32_t>(offset_x_);    }

inline uint32_t parts::Offset::GetY()
{   return static_cast<uint32_t>(offset_y_);    }

//parts::Offset::~Offset() {}

// vvv GPOSITION_PART
parts::GPosition::GPosition(int x, int y)
        :glo_x_(x)
        ,glo_y_(y) {}

parts::GPosition::GPosition(const GPosition& Gposition)
        :glo_x_(Gposition.glo_x_)
        ,glo_y_(Gposition.glo_y_) {}

int parts::GPosition::GetGloX() { return glo_x_; }

int parts::GPosition::GetGloY() { return glo_y_; }

bool parts::GPosition::IsInBlocks(const GPosition& check, 
                                int x_begin, int x_end, int y_begin, int y_end)
{
    if(x_end < x_begin || y_end < y_begin)
        return false;
    return x_begin <= check.glo_x_ && check.glo_x_ < x_end
        && y_begin <= check.glo_y_ && check.glo_x_ < y_end;
}

bool parts::GPosition::IsInBlocks(const GPosition& check 
                                , const GPosition& original
                                , int x_range, int y_range)
{
    return parts::GPosition::IsInBlocks(check, 
                                    original.glo_x_, original.glo_x_ + x_range,
                                    original.glo_y_, original.glo_y_ + y_range);
}

// vvv HEALTH_PART
parts::Health::Health(uint32_t hp_max16)
        :health_max_(hp_max16)
        ,current_hp_(health_max_) {}

parts::Health::Health(const Health& hp)
        :health_max_(hp.health_max_)
        ,current_hp_(hp.current_hp_) {}

uint32_t parts::Health::GetHp() { return current_hp_; }

uint32_t parts::Health::GetHpMax() { return health_max_; }

bool parts::Health::NeedHeal(bool under_half = false)
{   return under_half   ? current_hp_ < (health_max_ >> 2) : current_hp_ < health_max_; }

bool parts::Health::IsAlive() { return current_hp_ > 0 ? true : false;  }

void parts::Health::UnderAttack(uint32_t hurt_value)
{   current_hp_ = hurt_value >= current_hp_ ? (uint16_t)'\000' : hurt_value - current_hp_;   }

void parts::Health::OnHeal(uint32_t heal_value)
{   current_hp_ = heal_value >= (health_max_ - current_hp_) ? health_max_ : current_hp_ + heal_value;   }




parts::GPosition::~GPosition() {}


#endif //_SRC_BASE_ATTR_CPP_