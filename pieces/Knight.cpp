

#include "Knight.h"
#include "board/BitBoard.h"
#include "io/Error.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Knight::Knight(const uint8_t* bitBoard, bool black, int knight_i)
   : Piece(bitBoard, (black ? BLACK_START : WHITE_START) + (knight_i == 0 ? N1_INDEX : N2_INDEX))
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Knight::~Knight()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string Knight::Type() const
{
   return "Knight";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the value for the knight
///
////////////////////////////////////////////////////////////////////////////////
int Knight::Value() const
{
   return Knight::VALUE;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a knight can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Knight::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   return Captured() ? 0 : __MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a knight can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Knight::__MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   if (maskOptions.blockableKingAttack)
   {
      return 0;
   }
   
   uint64_t moveMask = 0;
   
   bool not_top     = row < TOP_ROW;
   bool not_top2    = row < TOP_ROW - 1;
   bool not_bottom  = row > BOTTOM_ROW;
   bool not_bottom2 = row > BOTTOM_ROW + 1;
   bool not_left    = col > LEFT_COL;
   bool not_left2   = col > LEFT_COL + 1;
   bool not_right   = col < RIGHT_COL;
   bool not_right2  = col < RIGHT_COL - 1;
   
   if (not_left2)
   {
      if (not_top)
      {
         moveMask |= posMask >> ((8 * 2) - 1); // left 2, up 1
      }
      if (not_bottom)
      {
         moveMask |= posMask >> ((8 * 2) + 1); // left 2, down 1
      }
   }
   
   if (not_left)
   {
      if (not_top2)
      {
         moveMask |= posMask >> (8 - 2); // left 1, up 2
      }
      if (not_bottom2)
      {
         moveMask |= posMask >> (8 + 2); // left 1, down 2
      }
   }
   
   if (not_right)
   {
      if (not_top2)
      {
         moveMask |= posMask << (8 + 2); // right 1, up 2
      }
      if (not_bottom2)
      {
         moveMask |= posMask << (8 - 2); // right 1, down 2
      }
   }
   
   if (not_right2)
   {
      if (not_top)
      {
         moveMask |= posMask << ((8 * 2) + 1); // right 2, up 1
      }
      if (not_bottom)
      {
         moveMask |= posMask << ((8 * 2) - 1); // right 2, down 1
      }
   }
   
   // Exclude spaces already occupied by our pieces (unless told to include
   // the pieces we are guarding)
   if (!maskOptions.guard)
   {
      moveMask &= ~playerMasks.myPieces;
   }
   return moveMask;
}

