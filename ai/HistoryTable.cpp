

#include "HistoryTable.h"
#include "Action.h"
#include <cstring>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Access the static AI HistoryTable
///
////////////////////////////////////////////////////////////////////////////////
HistoryTable& HistoryTable::Instance()
{
   static HistoryTable historyTable;
   return historyTable;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Reset the history table to all 0s
///
///           We may want to do this from time to time because we don't track
///           the type of piece in the history table
///
////////////////////////////////////////////////////////////////////////////////  
void HistoryTable::Reset()
{
   memset(array, 0, sizeof(array[0][0][0]) * ARRAY_LEN);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Bracket operator
///
////////////////////////////////////////////////////////////////////////////////  
uint64_t& HistoryTable::operator[](SimpleAction* pAction)
{
   return array[pAction->start_pos][pAction->end_pos][pAction->promoted_type];
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Const bracket operator
///
////////////////////////////////////////////////////////////////////////////////
const uint64_t& HistoryTable::operator[](SimpleAction* pAction) const
{
   return array[pAction->start_pos][pAction->end_pos][pAction->promoted_type];
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
HistoryTable::HistoryTable()
   : array{{{0}}}
{
   
}

