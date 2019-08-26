#pragma once

#include "ai/State.h"
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This is the top-level of the 
///
////////////////////////////////////////////////////////////////////////////////
class AiPlayer
{
public:
   std::string GetName() const;
   void Init();
   void GameUpdated();
   void GameOver();
   void MyTurn();
   
private:
   void _RefreshState(State& state);
   void _ApplyLastMoveToState(State& state);
   void _ApplyActionToGame(const FileRankAction& action);
};

