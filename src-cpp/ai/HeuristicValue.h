#pragma once


/////////////////////////////////////////////////////////////////////////////
///
///   @brief  This struct consolidates heuristic values for the sake of
///           comparison
///
/////////////////////////////////////////////////////////////////////////////
struct HVal
{
   HVal(int val = 0);
   HVal(int first, int second);
   HVal& operator = (const HVal& other);
   bool operator == (const HVal& other) const;
   bool operator <  (const HVal& other) const;
   bool operator >  (const HVal& other) const;
   bool operator != (const HVal& other) const;
   bool operator <= (const HVal& other) const;
   bool operator >= (const HVal& other) const;
   HVal operator *  (const HVal& other) const;
   HVal operator -  () const;
   
   int first;
   int second;
};

