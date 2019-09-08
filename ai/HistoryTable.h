#pragma once

#include <cstdint>

// forward declaration
struct SimpleAction;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This table contains counts of the number of times each action /
///           node is selected by a search or causes a prune
///
////////////////////////////////////////////////////////////////////////////////
class HistoryTable
{
public:
   static HistoryTable& Instance();
   void Reset();
   
   uint64_t& operator[](SimpleAction* pAction);
   const uint64_t& operator[](SimpleAction* pAction) const;
   
protected:
   HistoryTable();
   
   static constexpr int ARRAY_LEN = 64 * 64 * 4;
   uint64_t array[64][64][4]; // 125 KB
};

