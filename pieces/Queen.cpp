

#include "Queen.h"
#include "board/BitBoard.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Queen::Queen(const uint8_t* bitBoard, bool black)
   : Piece(bitBoard, (black ? BLACK_START : WHITE_START) + Q_INDEX)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Queen::~Queen()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string Queen::Type() const
{
   return "Queen";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the value for the queen
///
////////////////////////////////////////////////////////////////////////////////
int Queen::Value() const
{
   return Queen::VALUE;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a bishop can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Queen::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   return Captured() ? 0 : __MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a queen can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Queen::__MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   uint64_t moveMask = 0;
   
   int i;
   int j;
   
   // Normally we don't move through pieces, but if the options tell us to
   // we have to count them in at least one case
   int through;
   
   // Left
   through = 0;
   for (i = col - 1; i >= LEFT_COL; --i)
   {
      uint64_t left = posMask >> ((col - i) * 8); // left 1, 2, ...
      if (!ApplyPosToMask(moveMask, left, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the left?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Right
   through = 0;
   for (i = col + 1; i <= RIGHT_COL; ++i)
   {
      uint64_t right = posMask << ((i - col) * 8); // right 1, 2, ...
      if (!ApplyPosToMask(moveMask, right, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the right?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Up
   through = 0;
   for (i = row + 1; i <= TOP_ROW; ++i)
   {
      uint64_t up = posMask << (i - row); // up 1, 2, ...
      if (!ApplyPosToMask(moveMask, up, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king above?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Down
   through = 0;
   for (i = row - 1; i >= BOTTOM_ROW; --i)
   {
      uint64_t down = posMask >> (row - i); // down 1, 2, ...
      if (!ApplyPosToMask(moveMask, down, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king below?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Top-Left
   through = 0;
   for (i = row + 1, j = col - 1; i <= TOP_ROW && j >= LEFT_COL; ++i, --j)
   {
      uint64_t top_left = posMask >> ((col - j) * 8 - (i - row)); // top-left 1, 2, ...
      if (!ApplyPosToMask(moveMask, top_left, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the top-left?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Bottom-Left
   through = 0;
   for (i = row - 1, j = col - 1; i >= BOTTOM_ROW && j >= LEFT_COL; --i, --j)
   {
      uint64_t bottom_left = posMask >> ((col - j) * 8 + (row - i)); // bottom-left 1, 2, ...
      if (!ApplyPosToMask(moveMask, bottom_left, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the bottom-left?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Top-Right
   through = 0;
   for (i = row + 1, j = col + 1; i <= TOP_ROW && j <= RIGHT_COL; ++i, ++j)
   {
      uint64_t top_right = posMask << ((j - col) * 8 + (i - row)); // top-right 1, 2, ...
      if (!ApplyPosToMask(moveMask, top_right, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the top-right?
   if (FindBlockableKingAttack(moveMask, playerMasks, maskOptions))
   {
      return moveMask;
   }
   
   // Bottom-Right
   through = 0;
   for (i = row - 1, j = col + 1; i >= BOTTOM_ROW && j <= RIGHT_COL; --i, ++j)
   {
      uint64_t bottom_right = posMask << ((j - col) * 8 - (row - i)); // bottom-right 1, 2, ...
      if (!ApplyPosToMask(moveMask, bottom_right, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the bottom-right?
   FindBlockableKingAttack(moveMask, playerMasks, maskOptions);
   
   return moveMask;
}

