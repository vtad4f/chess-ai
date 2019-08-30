#pragma once

#include "board/Board.h"
#include "io/Parser.h"
#include "board/BitBoard.h"
#include "Action.h"
#include <string>
#include <set>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This exception class is intended to be thrown with a cached
///           message in the event of an error
///
////////////////////////////////////////////////////////////////////////////////
class State
{
public:
   State(const std::string& fen, const Parser::Options& options = Parser::Options());
   explicit State(const State& other);
   
   void GetValidActions(std::set<Action, std::greater<Action> >& actions) const;
   int ApplyAction(Action& action, bool forceRefresh = false);
   void SwapTurnPlayer();
   void Refresh(const std::string& fen = "");
   
protected:
   static Board s_Board;
   BitBoard m_BitBoard;
};

