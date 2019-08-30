

#include "Bishop.h"
#include "board/BitBoard.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Bishop::Bishop(const uint8_t* bitBoard, bool black, int bishop_i)
   : Piece(bitBoard, (black ? BLACK_START : WHITE_START) + (bishop_i == 0 ? B1_INDEX : B2_INDEX))
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Bishop::~Bishop()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string Bishop::Type() const
{
   return "Bishop";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the value for the bishop
///
////////////////////////////////////////////////////////////////////////////////
int Bishop::Value() const
{
   return Bishop::VALUE;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a bishop can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Bishop::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   return Captured() ? 0 : __MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a bishop can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Bishop::__MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   uint64_t moveMask = 0;
   
   int i;
   int j;
   
   // Normally we don't move through pieces, but if the options tell us to
   // we have to count them in at least one case
   int through;
   
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
      uint64_t top_right = posMask << ((j - col) * 8 - (row - i)); // bottom-right 1, 2, ...
      if (!ApplyPosToMask(moveMask, top_right, through, playerMasks, maskOptions))
      {
         break;
      }
   }
   
   // Attacking a king to the bottom-right?
   FindBlockableKingAttack(moveMask, playerMasks, maskOptions);
   
   return moveMask;
}

