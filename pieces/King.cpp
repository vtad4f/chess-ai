

#include "King.h"
#include "board/BitBoard.h"
#include "io/Error.h"


// Black is on top. White is on bottom.

// Check the 3 (left) or 2 (right) squares for pieces in the way
static constexpr uint64_t BLACK_R1_CASTLE_PIECES_MASK = 0x0000000080808000;
static constexpr uint64_t BLACK_R2_CASTLE_PIECES_MASK = 0x0080800000000000;
static constexpr uint64_t WHITE_R1_CASTLE_PIECES_MASK = 0x0000000001010100;
static constexpr uint64_t WHITE_R2_CASTLE_PIECES_MASK = 0x0001010000000000;

// Check the two squares the king has to move through for attacks
static constexpr uint64_t BLACK_R1_CASTLE_THREAT_MASK = 0x0000000080800000;
static constexpr uint64_t BLACK_R2_CASTLE_THREAT_MASK = 0x0080800000000000;
static constexpr uint64_t WHITE_R1_CASTLE_THREAT_MASK = 0x0000000001010000;
static constexpr uint64_t WHITE_R2_CASTLE_THREAT_MASK = 0x0001010000000000;

static constexpr uint64_t BLACK_R1_CASTLE_KING_POS = 0x0000000000800000;
static constexpr uint64_t BLACK_R2_CASTLE_KING_POS = 0x0080000000000000;
static constexpr uint64_t WHITE_R1_CASTLE_KING_POS = 0x0000000000010000;
static constexpr uint64_t WHITE_R2_CASTLE_KING_POS = 0x0001000000000000;

static constexpr uint64_t R1R2_CASTLE_MASK = R1_CASTLE_MASK | R2_CASTLE_MASK;

static constexpr int TWO_SPACES_HORIZONTAL = 16;
static constexpr int ONE_SPACE_HORIZONTAL  = 8;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
King::King(const uint8_t* bitBoard, bool black)
   : Piece(bitBoard,     (black ? BLACK_START                 : WHITE_START) + K_INDEX     )
   , m_CastleBitshift    (black ? BLACK_CASTLE_BITSHIFT       : WHITE_CASTLE_BITSHIFT      )
   , m_R1CastlePiecesMask(black ? BLACK_R1_CASTLE_PIECES_MASK : WHITE_R1_CASTLE_PIECES_MASK)
   , m_R2CastlePiecesMask(black ? BLACK_R2_CASTLE_PIECES_MASK : WHITE_R2_CASTLE_PIECES_MASK)
   , m_R1CastleThreatMask(black ? BLACK_R1_CASTLE_THREAT_MASK : WHITE_R1_CASTLE_THREAT_MASK)
   , m_R2CastleThreatMask(black ? BLACK_R2_CASTLE_THREAT_MASK : WHITE_R2_CASTLE_THREAT_MASK)
   , m_R1CastleKingPos   (black ? BLACK_R1_CASTLE_KING_POS    : WHITE_R1_CASTLE_KING_POS   )
   , m_R2CastleKingPos   (black ? BLACK_R2_CASTLE_KING_POS    : WHITE_R2_CASTLE_KING_POS   )
   , m_R1Index(          (black ? BLACK_START                 : WHITE_START) + R1_INDEX    )
   , m_R2Index(          (black ? BLACK_START                 : WHITE_START) + R2_INDEX    )
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
King::~King()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the piece type as a string
///
////////////////////////////////////////////////////////////////////////////////
std::string King::Type() const
{
   return "King";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Don't try to get the value of a king. Infinite would be
///           suitable, but really it shouldn't be compared to other pieces.
///
////////////////////////////////////////////////////////////////////////////////
int King::Value() const
{
   EXIT("Shouldn't be trying to retrieve the king's value");
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move the piece to its new position
///
////////////////////////////////////////////////////////////////////////////////
void King::Move(uint8_t* bitBoard, const Action& action) const
{
   Piece::Move(bitBoard, action);
   
   // Account for castling
   if (action.end_pos < action.start_pos)
   {
      // Move r1 if we castled left
      if (action.end_pos + TWO_SPACES_HORIZONTAL == action.start_pos)
      {
         bitBoard[m_R1Index] &= CLEAR_POS_MASK;
         bitBoard[m_R1Index] |= (action.end_pos + ONE_SPACE_HORIZONTAL) << POS_BITSHIFT;
      }
   }
   else
   {
      // Move r2 if we castled right
      if (action.start_pos + TWO_SPACES_HORIZONTAL == action.end_pos)
      {
         bitBoard[m_R2Index] &= CLEAR_POS_MASK;
         bitBoard[m_R2Index] |= (action.start_pos + ONE_SPACE_HORIZONTAL) << POS_BITSHIFT;
      }
   }
   
   // Clear castle flags
   if (CastleAvailable())
   {
      bitBoard[CASTLE_INDEX] &= ~(R1R2_CASTLE_MASK << m_CastleBitshift);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Set the state of this piece to captured
///
////////////////////////////////////////////////////////////////////////////////
void King::SetCaptured(uint8_t* bitBoard) const
{
   EXIT("King shouldn't be captured");
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king has moved or either rook has moved
///
////////////////////////////////////////////////////////////////////////////////
bool King::CastleAvailable() const
{
   return (m_BitBoard[CASTLE_INDEX] >> m_CastleBitshift) & R1R2_CASTLE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king has moved or rook 1 has moved
///
////////////////////////////////////////////////////////////////////////////////
bool King::RlCastleAvailable() const
{
   return (m_BitBoard[CASTLE_INDEX] >> m_CastleBitshift) & R1_CASTLE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king has moved or rook 2 has moved
///
////////////////////////////////////////////////////////////////////////////////
bool King::R2CastleAvailable() const
{
   return (m_BitBoard[CASTLE_INDEX] >> m_CastleBitshift) & R2_CASTLE_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Represent all the moves a king can make as a bit mask
///
////////////////////////////////////////////////////////////////////////////////
uint64_t King::MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const
{
   if (Captured() || maskOptions.blockableKingAttack || maskOptions.skipKing)
   {
      return 0;
   }
   
   const uint64_t posMask = PosMask();
   uint64_t moveMask = 0;
   
   const int row = Row();
   const int col = Col();
   
   bool not_top    = row < TOP_ROW;
   bool not_bottom = row > BOTTOM_ROW;
   bool not_left   = col > LEFT_COL;
   bool not_right  = col < RIGHT_COL;
   
   if (not_left)
   {
      if (not_top)
      {
         moveMask |= posMask >> (8 - 1); // left 1, up 1
      }
      moveMask |= posMask >> 8; // left 1
      if (not_bottom)
      {
         moveMask |= posMask >> (8 + 1); // left 1, down 1
      }
   }
   if (not_top)
   {
      moveMask |= posMask << 1; // up 1
   }
   if (not_bottom)
   {
      moveMask |= posMask >> 1; // down 1
   }
   if (not_right)
   {
      if (not_top)
      {
         moveMask |= posMask << (8 + 1); // right 1, up 1
      }
      moveMask |= posMask << 8; // right 1
      if (not_bottom)
      {
         moveMask |= posMask << (8 - 1); // right 1, down 1
      }
   }
   
   // These 'moves' include guarded pieces, so our king won't be entering
   // check by capturing something
   moveMask &= ~playerMasks.myKingsDangerSquares;
   
   // Exclude spaces already occupied by our pieces (unless told to include
   // the pieces we are guarding)
   if (!maskOptions.guard)
   {
      moveMask &= ~playerMasks.myPieces;
   }
   
   // We can castle if the stars align :)
   if (!(posMask & playerMasks.myKingsDangerSquares)) // Don't castle in check
   {
      uint64_t piecesMask = playerMasks.myPieces | playerMasks.theirPieces;
      
      if (RlCastleAvailable() &&
         !(m_R1CastlePiecesMask & piecesMask) && 
         !(m_R1CastleThreatMask & playerMasks.myKingsDangerSquares))
      {
         moveMask |= m_R1CastleKingPos;
      }
      if (R2CastleAvailable() &&
         !(m_R2CastlePiecesMask & piecesMask) &&
         !(m_R2CastleThreatMask & playerMasks.myKingsDangerSquares))
      {
         moveMask |= m_R2CastleKingPos;
      }
   }
   return moveMask;
}

