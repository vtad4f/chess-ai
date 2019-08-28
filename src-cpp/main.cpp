

#include "ai/AiPlayer.h"


int main(int argc, char **argv)
{
   std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
   double seconds_limit = 8;
   
   AiPlayer player;
   player.Init();
   player.MyTurn(FEN, seconds_limit);
   return 0;
}

