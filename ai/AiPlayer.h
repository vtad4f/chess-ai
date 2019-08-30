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
   void GameOver();
   void MyTurn(const std::string& fen, double turn_limit_s);
   
private:
   void _RefreshState(State& state, const std::string& fen);
};

