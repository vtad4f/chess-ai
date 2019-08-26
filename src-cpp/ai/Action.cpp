

#include "Action.h"
#include "HistoryTable.h"
#include "Settings.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "board/BitBoard.h"
#include "io/Translate.h"
#include "io/Error.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
SimpleAction::SimpleAction(int start_pos, int end_pos, bool promoted, int promoted_type)
   : start_pos(start_pos)
   , captured(false) // Set in Board::MovePiece
   , promoted(promoted)
   , end_pos(end_pos)
   , promoted_type(promoted_type)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Copy constructor
///
////////////////////////////////////////////////////////////////////////////////
SimpleAction::SimpleAction(const SimpleAction& other)
   : start_pos(other.start_pos)
   , captured(false) // Set in Board::MovePiece
   , promoted(other.promoted)
   , end_pos(other.end_pos)
   , promoted_type(other.promoted_type)
{
   
}





////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Action::Action()
   : SimpleAction(0, 0, false, 0)
   , piece_index(UNKNOWN_INDEX)
   , history(HistoryTable::Instance()[this])
   , random(rand())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (created by piece)
///
////////////////////////////////////////////////////////////////////////////////
Action::Action(int start_pos, int end_pos, int piece_index, bool promoted, int promoted_type)
   : SimpleAction(start_pos, end_pos, promoted, promoted_type)
   , piece_index(piece_index)
   , history(HistoryTable::Instance()[this])
   , random(rand())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (used to translate from Move)
///
////////////////////////////////////////////////////////////////////////////////
Action::Action(const std::string& from_file, int from_rank, const std::string& to_file, int to_rank, const std::string& promotion)
   : SimpleAction(
      Translate::FileRankToPos(from_file, from_rank),
      Translate::FileRankToPos(to_file, to_rank),
      !promotion.empty(),
      !promotion.empty() ? Translate::PromotionStrToInt(promotion) : 0
   )
   , piece_index(UNKNOWN_INDEX)
   , history(HistoryTable::Instance()[this])
   , random(rand())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (used for unit testing)
///
////////////////////////////////////////////////////////////////////////////////
Action::Action(const std::string& from, const std::string& to, const std::string& promotion)
   : SimpleAction(
      Translate::AlgebraicStrToPos(from),
      Translate::AlgebraicStrToPos(to),
      !promotion.empty(),
      !promotion.empty() ? Translate::PromotionStrToInt(promotion) : 0
   )
   , piece_index(UNKNOWN_INDEX)
   , history(HistoryTable::Instance()[this])
   , random(rand())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Copy constructor
///
////////////////////////////////////////////////////////////////////////////////
Action::Action(const Action& other)
   : SimpleAction(other)
   , piece_index(other.piece_index)
   , history(other.history)
   , random(other.random)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Assignment operator
///
////////////////////////////////////////////////////////////////////////////////
Action& Action::operator = (const Action& other)
{
   start_pos     = other.start_pos;
   captured      = other.captured;
   promoted      = other.promoted;
   end_pos       = other.end_pos;
   promoted_type = other.promoted_type;
   piece_index   = other.piece_index;
   history       = other.history;
   random        = other.random;
   return *this;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Comparison (equality) operator
///
////////////////////////////////////////////////////////////////////////////////
bool Action::operator == (const Action& other) const
{
   return start_pos == other.start_pos &&
          end_pos == other.end_pos &&
          promoted_type == other.promoted_type;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Comparison (greater than) operator
///
////////////////////////////////////////////////////////////////////////////////
bool Action::operator > (const Action& other) const
{
   static const Settings& settings = Settings::Instance();
   if (settings.test)
   {
      return (start_pos >  other.start_pos) ||
             (start_pos == other.start_pos && end_pos >  other.end_pos) ||
             (start_pos == other.start_pos && end_pos == other.end_pos && promoted_type > other.promoted_type);
   }
   else
   {
      // Reverse sign because we want the largest values first
      return (history >  other.history) ||
             (history == other.history && random > other.random);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If a piece was promoted in this action, get its new value minus
///           the original value
///
////////////////////////////////////////////////////////////////////////////////
int Action::PromotionValueDelta() const
{
   if (promoted)
   {
      switch (promoted_type)
      {
         case PROMOTED_TO_Q: return  Queen::VALUE - Pawn::VALUE;
         case PROMOTED_TO_R: return   Rook::VALUE - Pawn::VALUE;
         case PROMOTED_TO_B: return Bishop::VALUE - Pawn::VALUE;
         case PROMOTED_TO_N: return Knight::VALUE - Pawn::VALUE;
         default: EXIT("Unknown case");
      }
   }
   return 0;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Copy constructor
///
////////////////////////////////////////////////////////////////////////////////
FileRankAction::FileRankAction(const Action& action)
   : start_pos(Translate::PosToFileRank(action.start_pos))
   , end_pos(Translate::PosToFileRank(action.end_pos))
   , promotion(action.promoted ? Translate::PromotionIntToStr(action.promoted_type) : "")
{
   
}

