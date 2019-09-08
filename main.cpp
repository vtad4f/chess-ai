

#include "ai/AiPlayer.h"
#include <iostream>
#include <string>


int main(int argc, char **argv)
{
   int actual_argc = argc - 1;
   if (actual_argc < 2)
   {
      std::cerr << "Usage:  " << argv[0] << " <fen> <turn_limit_s>" << std::endl;
      return 1;
   }
   
   AiPlayer player;
   player.Init();
   player.MyTurn(argv[1], atof(argv[2]));
   return 0;
}

