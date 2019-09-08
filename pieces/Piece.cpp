

#include "Piece.h"
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
Piece::Piece(const uint8_t* bitBoard, int posIndex)
   : m_BitBoard(bitBoard)
   , m_PosIndex(posIndex)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Piece::~Piece()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the unique index for the piece in the bit board
///
////////////////////////////////////////////////////////////////////////////////
int Piece::Index() const
{
   return m_PosIndex;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece's [0, 64) position
///
////////////////////////////////////////////////////////////////////////////////
int Piece::Pos() const
{
   ASSERT(!Captured());
   return m_BitBoard[m_PosIndex] >> POS_BITSHIFT;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Translate the piece's position into a bit-mask if the piece
///           is not captured
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Piece::PosMask() const
{
   return Captured() ? 0 : Translate::PosToMask(Pos());
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see the piece is captured
///
////////////////////////////////////////////////////////////////////////////////
bool Piece::Captured() const
{
   return m_BitBoard[m_PosIndex] & BIG_CAPTURE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Push actions for every move in the bit-mask
///
////////////////////////////////////////////////////////////////////////////////
void Piece::GetActions(int64_t moveMask, std::set<Action, std::greater<Action> >& actions) const
{
   ASSERT(!Captured());
   for (int targetPos = 0; targetPos < 64; ++targetPos)
   {
      if ((moveMask >> targetPos) & 1) // Position present in bit-mask?
      {
         actions.emplace(Pos(), targetPos, Index());
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move the piece to its new position
///
////////////////////////////////////////////////////////////////////////////////
void Piece::Move(uint8_t* bitBoard, const Action& action) const
{
   ASSERT(!Captured());
   
   // Apply the position
   bitBoard[m_PosIndex] &= CLEAR_POS_MASK;
   bitBoard[m_PosIndex] |= action.end_pos << POS_BITSHIFT;
   
   // Clear the en passant pos mask and flag
   bitBoard[SPECIAL] &= CLEAR_POS_MASK;
   bitBoard[SPECIAL] &= ~EN_PASSANT_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Set the state of this piece to captured
///
////////////////////////////////////////////////////////////////////////////////
void Piece::SetCaptured(uint8_t* bitBoard) const
{
   bitBoard[m_PosIndex] |= BIG_CAPTURE_MASK;
   
   // Clear the pos mask if testing (b/c easier to match with parsed fen)
   static Settings& settings = Settings::Instance();
   if (settings.test)
   {
      bitBoard[m_PosIndex] &= CLEAR_POS_MASK;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Doesn't make sense for anything but a pawn, so just return 0
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Piece::EnPassantMask() const
{
   return 0;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  The the row index for the current position
///
////////////////////////////////////////////////////////////////////////////////
int Piece::Row() const
{
   return Pos() % (TOP_ROW + 1);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  The the col index for the current position
///
////////////////////////////////////////////////////////////////////////////////
int Piece::Col() const
{
   return Pos() / (RIGHT_COL + 1);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Apply the position to the moves bit-mask. If 'guard' is
///           specified, apply it even if the pos is also occupied by another
///           one of our pieces.
///
///   @return  true if the position didn't match one of my pieces or their
///            pieces or if this is their king and we are attacking through it
///
////////////////////////////////////////////////////////////////////////////////
bool Piece::ApplyPosToMask(uint64_t& moveMask, uint64_t posMask, int& through, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   bool myPiece = (posMask & playerMasks.myPieces);
   bool theirPiece = (posMask & playerMasks.theirPieces);
   bool theirKing = (posMask & playerMasks.theirKing);
   bool throughKing = (theirKing && maskOptions.throughKing);
   bool throughNotKing = (theirPiece && !theirKing && maskOptions.throughNotKingOnce && ++through <= 1);
   
   if (!myPiece || maskOptions.guard)
   {
      moveMask |= posMask;
   }
   return (!myPiece && !theirPiece) || throughKing || throughNotKing;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If we are looking for a king attack that can be blocked (i.e. an
///           attack from a Queen / Rook / Bishop)...
///           
///           Omit the target king from the bit mask if we find it, but reset
///           the bit mask if we don't find it.
///
///   @return  true if we found the attack bitmap we are looking for
///
////////////////////////////////////////////////////////////////////////////////
bool Piece::FindBlockableKingAttack(uint64_t& moveMask, const PlayerMasks& playerMasks, const MaskOptions& maskOptions)
{
   if (maskOptions.blockableKingAttack)
   {
      if (moveMask & playerMasks.theirKing)
      {
         moveMask &= ~playerMasks.theirKing;
         return true;
      }
      else
      {
         moveMask = 0;
      }
   }
   return false;
}


/////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor - Just a set of masks used to get moves for pieces
///
/////////////////////////////////////////////////////////////////////////////
Piece::PlayerMasks::PlayerMasks(uint64_t myPieces, uint64_t theirPieces, uint64_t theirMoves,
                                uint64_t theirKing, uint64_t myKingsDangerSquares)
   : myPieces(myPieces)
   , theirPieces(theirPieces)
   , theirMoves(theirMoves)
   , theirKing(theirKing)
   , myKingsDangerSquares(myKingsDangerSquares)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Clear the masks
///
////////////////////////////////////////////////////////////////////////////////
void Piece::PlayerMasks::Clear()
{
   myPieces = 0;
   theirPieces = 0;
   theirMoves = 0;
   theirKing = 0;
   myKingsDangerSquares = 0;
}


/////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor - Just a set options used to get moves for pieces
///
/////////////////////////////////////////////////////////////////////////////
Piece::MaskOptions::MaskOptions(bool guard, bool throughKing, bool blockableKingAttack,
                                bool throughNotKingOnce, bool skipKing)
   : guard(guard)
   , throughKing(throughKing)
   , blockableKingAttack(blockableKingAttack)
   , throughNotKingOnce(throughNotKingOnce)
   , skipKing(skipKing)
{
   
}

