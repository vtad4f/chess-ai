

#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "ai/Settings.h"
#include "io/Translate.h"
#include "board/BitBoard.h"
#include "io/Error.h"
#include "io/Debug.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Pawn::Pawn(const uint8_t* bitBoard, bool black, int pawn_i)
   : Piece(bitBoard, (black ? BLACK_START : WHITE_START) + (pawn_i * 2) + 1)
   , m_PromotedIndex(black ? BLACK_PROMOTED : WHITE_PROMOTED)
   , m_PromotedMask(1 << pawn_i)
   , m_MoveDownward(black)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Pawn::~Pawn()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string Pawn::Type() const
{
   if (Promoted())
   {
      switch (PromotedType())
      {
         case PROMOTED_TO_Q: return "Promoted Queen";
         case PROMOTED_TO_R: return "Promoted Rook";
         case PROMOTED_TO_B: return "Promoted Bishop";
         case PROMOTED_TO_N: return "Promoted Knight";
         default: EXIT("Unknown case"); return "";
      }
   }
   else
   {
      return "Pawn";
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the value for the pawn (possibly promoted)
///
////////////////////////////////////////////////////////////////////////////////
int Pawn::Value() const
{
   if (Promoted())
   {
      switch (PromotedType())
      {
         case PROMOTED_TO_Q: return Queen::VALUE;
         case PROMOTED_TO_R: return Rook::VALUE;
         case PROMOTED_TO_B: return Bishop::VALUE;
         case PROMOTED_TO_N: return Knight::VALUE;
         default: EXIT("Unknown case"); return 0;
      }
   }
   else
   {
      return Pawn::VALUE;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the pawn has been promoted
///
////////////////////////////////////////////////////////////////////////////////
bool Pawn::Promoted() const
{
   return m_BitBoard[m_PromotedIndex] & m_PromotedMask;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  What type of piece has the pawn been promoted to?
///
////////////////////////////////////////////////////////////////////////////////
uint8_t Pawn::PromotedType() const
{
   return m_BitBoard[m_PosIndex] & PAWN_PROMOTE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see the piece is captured
///
////////////////////////////////////////////////////////////////////////////////
bool Pawn::Captured() const
{
   return m_BitBoard[m_PosIndex - 1] & PAWN_CAPTURE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Push actions for every move in the bit-mask
///
////////////////////////////////////////////////////////////////////////////////
void Pawn::GetActions(int64_t moveMask, std::set<Action, std::greater<Action> >& actions) const
{
   ASSERT(!Captured());
   if (Promoted())
   {
      Piece::GetActions(moveMask, actions);
   }
   else
   {
      for (int targetPos = 0; targetPos < 64; ++targetPos)
      {
         if ((moveMask >> targetPos) & 1) // Position present in bit-mask?
         {
            int row = targetPos % (TOP_ROW + 1);
            if (row == TOP_ROW || row == BOTTOM_ROW)
            {
               actions.emplace(Pos(), targetPos, Index(), true, PROMOTED_TO_Q);
               actions.emplace(Pos(), targetPos, Index(), true, PROMOTED_TO_R);
               actions.emplace(Pos(), targetPos, Index(), true, PROMOTED_TO_B);
               actions.emplace(Pos(), targetPos, Index(), true, PROMOTED_TO_N);
            }
            else
            {
               actions.emplace(Pos(), targetPos, Index());
            }
         }
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move the piece to its new position
///
////////////////////////////////////////////////////////////////////////////////
void Pawn::Move(uint8_t* bitBoard, const Action& action) const
{
   Piece::Move(bitBoard, action);
   
   // If just a normal pawn that advanced two, set the en passant pos
   if(!Promoted())
   {
      if (action.end_pos + 2 == action.start_pos) // Move downward
      {
         bitBoard[SPECIAL] |= (action.end_pos + 1) << POS_BITSHIFT;
         bitBoard[SPECIAL] |= EN_PASSANT_MASK;
      }
      else if (action.start_pos + 2 == action.end_pos) // Move upward
      {
         bitBoard[SPECIAL] |= (action.start_pos + 1) << POS_BITSHIFT;
         bitBoard[SPECIAL] |= EN_PASSANT_MASK;
      }
   }
   
   // Did the pawn just get promoted this turn?
   if (action.promoted)
   {
      bitBoard[m_PromotedIndex] |= m_PromotedMask;
      bitBoard[m_PosIndex] |= action.promoted_type;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Set the state of this piece to captured
///
////////////////////////////////////////////////////////////////////////////////
void Pawn::SetCaptured(uint8_t* bitBoard) const
{
   bitBoard[m_PosIndex - 1] |= PAWN_CAPTURE_MASK;
   
   // Clear the pos mask if testing (b/c easier to match with parsed fen)
   static Settings& settings = Settings::Instance();
   if (settings.test)
   {
      bitBoard[m_PosIndex] &= CLEAR_POS_MASK;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a mask for the en passant position (if there is one)
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Pawn::EnPassantMask() const
{
   return (m_BitBoard[SPECIAL] & EN_PASSANT_MASK) ? Translate::PosToMask(m_BitBoard[SPECIAL] >> POS_BITSHIFT) : 0;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a bishop can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Pawn::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   if (Captured())
   {
      return 0;
   }
   else if (Promoted())
   {
      switch (PromotedType())
      {
         case PROMOTED_TO_Q: return  Queen::__MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
         case PROMOTED_TO_R: return   Rook::__MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
         case PROMOTED_TO_B: return Bishop::__MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
         case PROMOTED_TO_N: return Knight::__MoveMask(PosMask(), Row(), Col(), playerMasks, maskOptions);
         default: EXIT("Unknown case"); return 0;
      }
   }
   else
   {
      return Pawn::__MoveMask(playerMasks, maskOptions);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a pawn can make as a bit mask
///           - Consider en passant
///           - Consider that the pawn could be promoted
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Pawn::__MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   if (maskOptions.blockableKingAttack)
   {
      return 0;
   }
   
   const uint64_t posMask = PosMask();
   const uint64_t emptySpaceMask = ~playerMasks.myPieces & ~playerMasks.theirPieces;
   uint64_t moveMask = 0;
   uint64_t captureMask = 0;
   
   const int row = Row();
   const int col = Col();
   
   // Skip these if we are only looking for king-threatening moves
   if (!maskOptions.throughKing)
   {
      // Move 1 space?
      if (m_MoveDownward)
      {
         moveMask |= posMask >> 1; // down 1
      }
      else
      {
         moveMask |= posMask << 1; // up 1
      }
      moveMask &= emptySpaceMask; // Filter out occupied spaces
      
      // Move 2 spaces?
      if (moveMask)
      {
         if (m_MoveDownward)
         {
            if (row == TOP_PAWN_ROW)
            {
               moveMask |= posMask >> 2; // down 2
            }
         }
         else
         {
            if (row == BOTTOM_PAWN_ROW)
            {
               moveMask |= posMask << 2; // up 2
            }
         }
      }
      moveMask &= emptySpaceMask; // Filter out occupied spaces
   }
   
   // Capture?
   if (m_MoveDownward)
   {
      if (col > LEFT_COL)
      {
         captureMask |= posMask >> (8 + 1); // left 1, down 1
      }
      if (col < RIGHT_COL)
      {
         captureMask |= posMask << (8 - 1); // right 1, down 1
      }
   }
   else
   {
      if (col > LEFT_COL)
      {
         captureMask |= posMask >> (8 - 1); // left 1, up 1
      }
      if (col < RIGHT_COL)
      {
         captureMask |= posMask << (8 + 1); // right 1, up 1
      }
   }
   
   // A pawn can only move diagonally if there is an opponent's piece in the
   // square or if en passant is available. However, if we are directed to
   // look for pieces that pawn could guard, don't bother filtering...
   if (!maskOptions.guard)
   {
      captureMask &= (playerMasks.theirPieces | EnPassantMask());
   }
   
   return (moveMask | captureMask);
}

