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
   void Init();
   void MyTurn(const std::string& fen, double remaining_s);
   
private:
   void _RefreshState(State& state, const std::string& fen);
};

