

#include "State.h"
#include "TerminalException.h"
#include "Settings.h"
#include "io/Error.h"
#include "io/Debug.h"


Board State::s_Board;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
State::State(const std::string& fen, const Parser::Options& options)
   : m_BitBoard(Parser(fen, options).GetBitBoard())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Copy Constructor
///
////////////////////////////////////////////////////////////////////////////////
State::State(const State& other)
   : m_BitBoard(other.m_BitBoard)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Iterate over all the pieces for the current player. Consolidate
///           the list of available actions for all those pieces.
///
////////////////////////////////////////////////////////////////////////////////
void State::GetValidActions(std::set<Action, std::greater<Action> >& actions) const
{
   s_Board.GetPiecesAndMasks(m_BitBoard.array); // Reset s_Board
   s_Board.GetTurnPlayerMoves(actions);
   if (actions.empty()) // If no actions, this state is terminal
   {
      if (s_Board.InCheck())
      {
         throw CheckmateException();
      }
      else
      {
         throw StalemateException();
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Apply the action to the collection of pieces
///
///   @return  The material value delta (value of captured piece this turn +
///            new value of promoted piece - 1)
///
////////////////////////////////////////////////////////////////////////////////
int State::ApplyAction(Action& action, bool forceRefresh)
{
   bool unknownIndex = (action.piece_index == Action::UNKNOWN_INDEX);
   
   // Refresh the board pieces?
   if (unknownIndex || forceRefresh)
   {
      s_Board.GetPiecesAndMasks(m_BitBoard.array); // Reset s_Board
   }
   
   // Find the index (if this action wasn't constructed by a Piece)
   if (unknownIndex)
   {
      action.piece_index = s_Board.GetPieceIndex(action.start_pos);
   }
   
   int captureVal = s_Board.MovePiece(m_BitBoard.array, action);
   
   // Debug printing...
   static const Settings& settings = Settings::Instance();
   static bool everyOther = true;
   if (settings.verbose && ((settings.random && unknownIndex && everyOther) || settings.test))
   {
      if (settings.test) { debug::PrintAction(action); }
      s_Board.GetPiecesAndMasks(m_BitBoard.array); // Reset s_Board
      s_Board.PrintPieceMasks();
   }
   everyOther = !everyOther; // Skip printing when we apply their move
   
   return captureVal + action.PromotionValueDelta();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Swap white/black as the turn player
///
////////////////////////////////////////////////////////////////////////////////
void State::SwapTurnPlayer()
{
   s_Board.SwapTurnPlayer(m_BitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Reset the bit board with the new fen
///
////////////////////////////////////////////////////////////////////////////////
void State::Refresh(const std::string& fen)
{
   if (!fen.empty())
   {
      m_BitBoard = Parser(fen).GetBitBoard();
   }
   s_Board.GetPiecesAndMasks(m_BitBoard.array); // Reset s_Board
}

