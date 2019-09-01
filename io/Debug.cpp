

#include "Debug.h"
#include "ai/Settings.h"
#include "board/BitBoard.h"
#include <sstream>


namespace debug
{

////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the value
///
////////////////////////////////////////////////////////////////////////////////
void Print(int val, const std::string& prefix_msg)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cerr << prefix_msg << val << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the string
///
////////////////////////////////////////////////////////////////////////////////
void Print(const std::string& msg)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cerr << msg << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the position
///
////////////////////////////////////////////////////////////////////////////////
void PrintPos(int pos)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cerr << Translate::PosToAlgebraicStr(pos) << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the mask
///
////////////////////////////////////////////////////////////////////////////////
void PrintMask(uint64_t mask, bool labeled)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::ostringstream oss;
      if (labeled)
      {
         oss << '\n';
      }
      oss << Translate::MaskToStr(mask, labeled) << '\n';
      if (labeled)
      {
         oss << '\n';
      }
      std::cerr << oss.str() << std::flush;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the masks side by side
///
////////////////////////////////////////////////////////////////////////////////
void PrintMasks(uint64_t mask1, uint64_t mask2, bool labeled)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::ostringstream oss;
      if (labeled)
      {
         oss << '\n';
      }
      std::string mask1_str = Translate::MaskToStr(mask1, labeled);
      std::string mask2_str = Translate::MaskToStr(mask2, labeled);
      size_t i = 0;
      size_t i2 = 0;
      for(; i < mask1_str.size(); ++i)
      {
         if (mask1_str[i] == '\n')
         {
            oss << ' ' << mask2_str.substr(i2, i - i2) << '\n';
            i2 = i + 1;
         }
         else
         {
            oss << mask1_str[i];
         }
      }
      oss << ' ' << mask2_str.substr(i2, i - i2) << '\n';
      if (labeled)
      {
         oss << '\n';
      }
      std::cerr << oss.str() << std::flush;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the piece
///
////////////////////////////////////////////////////////////////////////////////
void PrintPiece(const Piece& piece)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cerr << (piece.Captured() ? "Captured " : "") << piece.Type() << std::endl;
      PrintMask(piece.PosMask());
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print a message followed by an action
///
////////////////////////////////////////////////////////////////////////////////
void PrintAction(const Action& action, const std::string& prefix_msg)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cout << prefix_msg
                << Translate::PosToAlgebraicStr(action.start_pos)
                << Translate::PosToAlgebraicStr(action.end_pos)
                << (action.promoted ? Translate::PromotionIntToStr(action.promoted_type) : "") << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the action and the associated heuristic value
///
////////////////////////////////////////////////////////////////////////////////
void PrintAction(const Action& action, const HVal& h_val)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::ostringstream oss;
      oss << "(" << std::to_string(h_val.first) << ", " << std::to_string(h_val.second)  << ") ";
      PrintAction(action, oss.str());
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the action and the associated heuristic value
///
////////////////////////////////////////////////////////////////////////////////
void PrintAction(const Action& action, int h_val)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::ostringstream oss;
      oss << "(" << std::setw(2) << std::to_string(h_val) << ") ";
      PrintAction(action, oss.str());
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the action and the associated heuristic value
///
////////////////////////////////////////////////////////////////////////////////
void PrintAction(const Action& action, double h_val)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::ostringstream oss;
      oss << "(" << std::to_string(h_val) << ") ";
      PrintAction(action, oss.str());
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the bit board in hex
///
////////////////////////////////////////////////////////////////////////////////
void PrintBitBoard(const uint8_t* bitBoard)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      for (int i = BLACK_START; i < WHITE_START; ++i)
      {
         PrintByteAsHex(bitBoard[i]);
      }
      std::cerr << ' ' << std::flush;
      for (int i = WHITE_START; i < BLACK_PROMOTED; ++i)
      {
         PrintByteAsHex(bitBoard[i]);
      }
      std::cerr << ' ' << std::flush; PrintByteAsHex(bitBoard[BLACK_PROMOTED]);
      std::cerr << ' ' << std::flush; PrintByteAsHex(bitBoard[WHITE_PROMOTED]);
      std::cerr << ' ' << std::flush; PrintByteAsHex(bitBoard[SPECIAL       ]);
      std::cerr << ' ' << std::flush; PrintByteAsHex(bitBoard[CASTLE_INDEX  ]);
      std::cerr << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the byte as a 2-digit hex value
///
////////////////////////////////////////////////////////////////////////////////
void PrintByteAsHex(uint8_t byte)
{
   const Settings& settings = Settings::Instance();
   if (!settings.silent)
   {
      std::cerr << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << std::flush;
   }
}

} // end namespace debug