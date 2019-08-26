
#include "pieces/Bishop.h"
#include "pieces/King.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "Board.h"
#include "BitBoard.h"
#include "Error.h"
#include "Debug.h"


static constexpr int NUM_R_B_N = 2; // number of rooks/bishops/knights
static constexpr int NUM_PAWNS = 8;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Board::Board()
   : m_BitBoard(NULL)
   , m_MyPieces()
   , m_TheirPieces()
   , m_Masks()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Destructor
///
////////////////////////////////////////////////////////////////////////////////
Board::~Board()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move the piece specified in the action
///
///   @return  The value of the captured piece if there was one
///
////////////////////////////////////////////////////////////////////////////////
int Board::MovePiece(uint8_t* bitBoard, Action& action)
{
   int capture_val = 0;
   
   // Get the piece that moved
   auto piece = m_MyPieces.all[action.piece_index];
   
   // Get a mask for the square that gets captured...
   uint64_t captureMask = Translate::PosToMask(action.end_pos);
   if (captureMask == piece->EnPassantMask())
   {
      if (BlacksTurn()) // Moved downward
      {
         captureMask <<= 1; // So the pawn to capture is above end_pos
      }
      else // Moved upward
      {
         captureMask >>= 1; // So the pawn to capture is below end_pos
      }
   }
   
   // Capture their piece?
   if (captureMask & m_TheirPieces.pos_mask)
   {
      for (auto targetPiece : m_TheirPieces.all)
      {
         if (captureMask == targetPiece.second->PosMask())
         {
            action.captured = true;
            targetPiece.second->SetCaptured(bitBoard);
            capture_val = targetPiece.second->Value();
            break;
         }
      }
      ASSERT_GT(capture_val, 0);
   }
   
   // Move my piece
   piece->Move(bitBoard, action);
   
   // Switch turn player
   SwapTurnPlayer(bitBoard);
   
   return capture_val;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Swap white/black as the turn player
///
////////////////////////////////////////////////////////////////////////////////
void Board::SwapTurnPlayer(uint8_t* bitBoard)
{
   if (bitBoard[SPECIAL] & BLACKS_TURN_MASK)
   {
      bitBoard[SPECIAL] &= ~BLACKS_TURN_MASK;
   }
   else
   {
      bitBoard[SPECIAL] |= BLACKS_TURN_MASK;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king is in check
///
////////////////////////////////////////////////////////////////////////////////
bool Board::InCheck() const
{
   return m_MyPieces.king->PosMask() & m_Masks.myMasks.theirMoves;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if the king is in check
///
////////////////////////////////////////////////////////////////////////////////
int Board::GetPieceIndex(int pos) const
{
   uint64_t posMask = Translate::PosToMask(pos);
   for (auto piece : m_MyPieces.all)
   {
      if (piece.second->PosMask() == posMask)
      {
         return piece.second->Index();
      }
   }
   debug::Print("-------------------------");
   PrintPieceMasks();
   debug::PrintPos(pos);
   EXIT("Couldn't find piece by position");
   return -1;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print a mask of all of my pieces side-by side with the
///           opponent's pieces
///
////////////////////////////////////////////////////////////////////////////////
void Board::PrintPieceMasks() const
{
   if (BlacksTurn()) // Always print white's pieces first
   {
      debug::PrintMasks(m_TheirPieces.pos_mask, m_MyPieces.pos_mask, true);
   }
   else
   {
      debug::PrintMasks(m_MyPieces.pos_mask, m_TheirPieces.pos_mask, true);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the actions available to the turn player
///
////////////////////////////////////////////////////////////////////////////////
void Board::GetTurnPlayerMoves(std::set<Action, std::greater<Action> >& actions) const
{
   if (InCheck())
   {
      // First just grab all the pieces the king can move to
      uint64_t myKingMoveMask = m_MyPieces.king->MoveMask(m_Masks.myMasks);
      m_MyPieces.king->GetActions(myKingMoveMask, actions);
      
      // If multiple threats, the king has to move
      ASSERT(!m_Masks.threats.empty());
      if (m_Masks.threats.size() <= 1)
      {
         auto threat = m_Masks.threats.back();
         uint64_t threatPosMask = threat->PosMask();
         
         // Can it be captured?
         static const Piece::MaskOptions skipKingOpt(false, false, false, false, true); // Skip king (already have king actions)
         for (auto piece : m_MyPieces.all)
         {
            if (uint64_t captureMask = piece.second->MoveMask(m_Masks.myMasks, skipKingOpt) & threatPosMask)
            {
               DontMoveIntoCheck(piece.second->PosMask(), captureMask);
               if (captureMask)
               {
                  piece.second->GetActions(captureMask, actions);
               }
            }
         }
         
         // Can we put something in its way?
         static const Piece::MaskOptions blockOpt(false, false, true, false, true); // Blockable attack against king
         if (uint64_t blockableMask = threat->MoveMask(m_Masks.theirMasks, blockOpt))
         {
            for (auto piece : m_MyPieces.all)
            {
               if (uint64_t blockMask = piece.second->MoveMask(m_Masks.myMasks) & blockableMask)
               {
                  DontMoveIntoCheck(piece.second->PosMask(), blockMask);
                  if (blockMask)
                  {
                     piece.second->GetActions(blockMask, actions);
                  }
               }
            }
         }
      }
   }
   else
   {
      // Check all the pieces for actions
      for (auto piece : m_MyPieces.all)
      {
         if (uint64_t moveMask = piece.second->MoveMask(m_Masks.myMasks))
         {
            DontMoveIntoCheck(piece.second->PosMask(), moveMask);
            if (moveMask)
            {
               piece.second->GetActions(moveMask, actions);
            }
         }
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If the piece's position is in the threat mask, we have to be
///           careful where we move it. Else we will illegally place our king
///           in check by moving this piece.
///
///           Note that the king piece already has internal safeguards that
///           prevent it from moving into check.
///
////////////////////////////////////////////////////////////////////////////////
void Board::DontMoveIntoCheck(uint64_t posMask, uint64_t& moveMask) const
{
   if (posMask & m_Masks.threatsIfMoveMask)
   {
      for (auto threatMask : m_Masks.threatsIfMove)
      {
         if (posMask & threatMask)
         {
            moveMask &= threatMask;
         }
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get all the pieces on the board
///
////////////////////////////////////////////////////////////////////////////////
void Board::GetPiecesAndMasks(const uint8_t* bitBoard)
{
   m_BitBoard = bitBoard;
   bool blacksTurn = BlacksTurn();
   GetPieces(blacksTurn ? m_MyPieces : m_TheirPieces, true);
   GetPieces(blacksTurn ? m_TheirPieces : m_MyPieces, false);
   GetMasks();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check to see if it is black's turn
///
////////////////////////////////////////////////////////////////////////////////
bool Board::BlacksTurn() const
{
   return (m_BitBoard[SPECIAL] & BLACKS_TURN_MASK);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get all the move masks the players' pieces need to decide how to
///           move
///
////////////////////////////////////////////////////////////////////////////////
void Board::GetMasks()
{
   m_Masks.Clear();
   
   // Get my king position
   uint64_t myKingPosMask = m_MyPieces.king->PosMask();
   
   // Keep theirs simple - just accounting for piece positions
   m_Masks.theirMasks = Piece::PlayerMasks(m_TheirPieces.pos_mask, m_MyPieces.pos_mask, 0, myKingPosMask, 0);
   
   // Consolidate opponent move masks
   uint64_t theirMoves = 0;
   for (auto piece : m_TheirPieces.all)
   {
      theirMoves |= piece.second->MoveMask(m_Masks.theirMasks);
   }
   
   // Prep search options...
   static const Piece::MaskOptions firstTwoOpts(true, true); // Guarded pieces, move through my king
   static const Piece::MaskOptions lastTwoOpts(false, false, true, true); // Attacks against king if move through one piece
   
   // Their moves + their guarded pieces + spaces behind my king if Q/R/B on other side
   uint64_t myKingsDangerSquares = 0;
   for (auto piece : m_TheirPieces.all)
   {
      myKingsDangerSquares |= piece.second->MoveMask(m_Masks.theirMasks, firstTwoOpts);
   }
   
   // My set of masks is more complex - mostly because of check
   m_Masks.myMasks = Piece::PlayerMasks(m_MyPieces.pos_mask, m_TheirPieces.pos_mask, theirMoves, 0, myKingsDangerSquares);
   
   // Find all the threats to our king if we move a piece
   m_Masks.threatsIfMoveMask = 0;
   for (auto piece : m_TheirPieces.all)
   {
      if (uint64_t moveMask = piece.second->MoveMask(m_Masks.theirMasks, lastTwoOpts))
      {
         m_Masks.threatsIfMove.push_back(moveMask | piece.second->PosMask());
         m_Masks.threatsIfMoveMask |= m_Masks.threatsIfMove.back();
      }
   }
   
   // Find all the pieces threatening our king
   if (InCheck())
   {
      for (auto piece : m_TheirPieces.all)
      {
         if (piece.second->MoveMask(m_Masks.theirMasks) & myKingPosMask)
         {
            m_Masks.threats.push_back(piece.second);
         }
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get all the player's pieces on the board
///
///   @param pieces  Populate this struct of pieces for a particular player
///   @param black  If true use the indexes in the bit board for black pieces.
///                 Else use the indexes for white pieces.
///
////////////////////////////////////////////////////////////////////////////////
void Board::GetPieces(PlayerPieces& pieces, bool black) const
{
   pieces.Clear();
   
   // King
   pieces.king = std::make_shared<King>(m_BitBoard, black);
   pieces.all[pieces.king->Index()] = pieces.king;
   
   // Queen
   std::shared_ptr<Queen> queen = std::make_shared<Queen>(m_BitBoard, black);
   pieces.all[queen->Index()] = queen;
   
   for(int i = 0; i < NUM_R_B_N; ++i)
   {
      // Rook
      std::shared_ptr<Rook> rook = std::make_shared<Rook>(m_BitBoard, black, i);
      pieces.all[rook->Index()] = rook;
      
      // Bishop
      std::shared_ptr<Bishop> bishop = std::make_shared<Bishop>(m_BitBoard, black, i);
      pieces.all[bishop->Index()] = bishop;
      
      // Knight
      std::shared_ptr<Knight> knight = std::make_shared<Knight>(m_BitBoard, black, i);
      pieces.all[knight->Index()] = knight;
   }
   
   for(int i = 0; i < NUM_PAWNS; ++i)
   {
      // Pawn
      std::shared_ptr<Pawn> pawn = std::make_shared<Pawn>(m_BitBoard, black, i);
      pieces.all[pawn->Index()] = pawn;
   }
   
   // Position mask
   pieces.pos_mask = 0;
   for (auto piece : pieces.all)
   {
      pieces.pos_mask |= piece.second->PosMask();
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Clear the collection of pieces
///
////////////////////////////////////////////////////////////////////////////////
void Board::PlayerPieces::Clear()
{
   king.reset();
   all.clear();
   pos_mask = 0;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Clear the collection of masks/threats
///
////////////////////////////////////////////////////////////////////////////////
void Board::Masks::Clear()
{
   myMasks.Clear();
   theirMasks.Clear();
   threatsIfMove.clear();
   threatsIfMoveMask = 0;
   threats.clear();
}

