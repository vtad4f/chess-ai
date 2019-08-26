

#include "ai/AiPlayer.h"


int main(int argc, char **argv)
{
   std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
   double time_remaining_s = 15 * 60.0;
   int current_turn = 1;
   
   AiPlayer player;
   player.Init();
   player.MyTurn(FEN, time_remaining_s, current_turn);
   return 0;
}

