

#include "HeuristicValue.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
HVal::HVal(int val)
   : first(val)
   , second(val)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (from values)
///
////////////////////////////////////////////////////////////////////////////////
HVal::HVal(int first, int second)
   : first(first)
   , second(second)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Assignment operator
///
////////////////////////////////////////////////////////////////////////////////
HVal& HVal::operator = (const HVal& other)
{
   first = other.first;
   second = other.second;
   return *this;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Comparison operators
///
////////////////////////////////////////////////////////////////////////////////
bool HVal::operator == (const HVal& other) const { return first == other.first && second == other.second; }
bool HVal::operator <  (const HVal& other) const { return (first < other.first) || (first == other.first && second < other.second); }
bool HVal::operator >  (const HVal& other) const { return (first > other.first) || (first == other.first && second > other.second); }
bool HVal::operator != (const HVal& other) const { return !(*this == other); }
bool HVal::operator <= (const HVal& other) const { return *this < other || *this == other; }
bool HVal::operator >= (const HVal& other) const { return *this > other || *this == other; }


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Arithmetic operators
///
////////////////////////////////////////////////////////////////////////////////
HVal HVal::operator * (const HVal& other) const { return HVal(first * other.first, second * other.second); }
HVal HVal::operator - () const { return HVal(-first, -second); }

