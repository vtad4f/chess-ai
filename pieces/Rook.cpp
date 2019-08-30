

#include "Rook.h"
#include "board/BitBoard.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Rook::Rook(const uint8_t* bitBoard, bool black, int rook_i)
   : Piece(bitBoard, (black ? BLACK_START : WHITE_START) + (rook_i == 0 ? R1_INDEX : R2_INDEX))
   , m_CastleBitshift(black ? BLACK_CASTLE_BITSHIFT : WHITE_CASTLE_BITSHIFT)
   , m_CastleMask(rook_i == 0 ? R1_CASTLE_MASK : R2_CASTLE_MASK)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Rook::~Rook()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string Rook::Type() const
{
   return "Rook";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the value for the rook
///
////////////////////////////////////////////////////////////////////////////////
int Rook::Value() const
{
   return Rook::VALUE;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move the piece to its new position
///
////////////////////////////////////////////////////////////////////////////////
void Rook::Move(uint8_t* bitBoard, const Action& action) const
{
   Piece::Move(bitBoard, action);
   if (CastleAvailable())
   {
      bitBoard[CASTLE_INDEX] &= ~(m_CastleMask << m_CastleBitshift);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Set the state of this piece to captured
///
////////////////////////////////////////////////////////////////////////////////
void Rook::SetCaptured(uint8_t* bitBoard) const
{
   Piece::SetCaptured(bitBoard);
   if (CastleAvailable())
   {
      bitBoard[CASTLE_INDEX] &= ~(m_CastleMask << m_CastleBitshift);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king has moved or this rook has moved
///
////////////////////////////////////////////////////////////////////////////////
bool Rook::CastleAvailable() const
{
   return (m_BitBoard[CASTLE_INDEX] >> m_CastleBitshift) & m_CastleMask;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a bishop can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Rook::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   return Captured() ? 0 : __MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a rook can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Rook::__MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   uint64_t moveMask = 0;
   
   int i;
   
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
   FindBlockableKingAttack(moveMask, playerMasks, maskOptions);
   
   return moveMask;
}

