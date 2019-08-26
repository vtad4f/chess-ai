

#include "Translate.h"
#include "Error.h"
#include "board/BitBoard.h"
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Translate the bit-mask to a string for printing
///
////////////////////////////////////////////////////////////////////////////////
std::string Translate::MaskToStr(uint64_t mask, bool labeled)
{
   std::stringstream ss;
   ss << (labeled ? " abcdefgh " : "+--------+") << "\n";
   for(int row = 7; row >= 0; --row)
   {
      ss << (labeled ? std::to_string(row + 1) : "|");
      for(int col = 0; col < 8; ++col)
      {
         bool bit_set = (mask >> ColRowToPos(col, row) & 1);
         ss << (labeled ? (bit_set ? "o" : ".") : (bit_set ? "1" : " "));
      }
      ss << (labeled ? std::to_string(row + 1) : "|") << "\n";
   }
   ss << (labeled ? " abcdefgh " : "+--------+");
   return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the 64 bit mask for the position
///
////////////////////////////////////////////////////////////////////////////////
uint64_t Translate::PosToMask(int pos)
{
   ASSERT_IN_RANGE(pos, 0, 64);
   return 1ULL << pos;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Translate col and row indexes to a position
///
////////////////////////////////////////////////////////////////////////////////
int Translate::ColRowToPos(int col, int row)
{
   return (col * 8) + row;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the position from a rank and file string
///
////////////////////////////////////////////////////////////////////////////////
int Translate::FileRankToPos(const std::string& file, int rank)
{
   ASSERT(!file.empty());
   ASSERT_IN_RANGE(file[0], 'a', 'h' + 1);
   ASSERT_IN_RANGE(rank, 1, 8 + 1);
   return (file[0] - 'a') * 8 + (rank - 1);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Split the algebraic notation string apart, into pair with 
///           distinct file and rank
///
////////////////////////////////////////////////////////////////////////////////
std::pair<std::string, int> Translate::PosToFileRank(int pos)
{
   ASSERT_IN_RANGE(pos, 0, 64);
   std::string algebra = PosToAlgebraicStr(pos);
   return std::make_pair(algebra.substr(0, 1), std::atoi(algebra.substr(1).c_str()));
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the position from the algebraic notation representation
///
////////////////////////////////////////////////////////////////////////////////
int Translate::AlgebraicStrToPos(const std::string& algebraic)
{
   ASSERT_EQ(2, algebraic.size());
   char file = algebraic[0];
   int rank = algebraic[1] - '0';
   ASSERT_IN_RANGE(file, 'a', 'h' + 1);
   ASSERT_IN_RANGE(rank, 1, 8 + 1);
   return (file - 'a') * 8 + (rank - 1);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the algebraic notation representation of the position
///
////////////////////////////////////////////////////////////////////////////////
std::string Translate::PosToAlgebraicStr(int pos)
{
   ASSERT_IN_RANGE(pos, 0, 64);
   static const std::string FILE = "abcdefgh";
   return FILE[pos / 8] + std::to_string((pos % 8) + 1);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the string corresponding to the promotion provided
///
////////////////////////////////////////////////////////////////////////////////
std::string Translate::PromotionIntToStr(int promotion)
{
   ASSERT_IN_RANGE(promotion, 0, 4);
   static const std::string NAME[4] = {"Queen", "Rook", "Bishop", "Knight"};
   return NAME[promotion];
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the string corresponding to the promotion provided
///
////////////////////////////////////////////////////////////////////////////////
uint8_t Translate::PromotionStrToInt(const std::string& promotion)
{
   if (promotion == "Queen")
   {
      return PROMOTED_TO_Q;
   }
   else if (promotion == "Rook")
   {
      return PROMOTED_TO_R;
   }
   else if (promotion == "Bishop")
   {
      return PROMOTED_TO_B;
   }
   else if (promotion == "Knight")
   {
      return PROMOTED_TO_N;
   }
   else
   {
      EXIT("Unknown case");
   }
   return 0;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Count the number of active bits in the mask
///
////////////////////////////////////////////////////////////////////////////////
double Translate::CountActiveBits(uint64_t mask)
{
   int n_active_bits = 0;
   if (mask)
   {
      for (int pos = 0; pos < 64; ++pos)
      {
         if ((mask >> pos) & 1) // Position present in bit-mask?
         {
            ++n_active_bits;
         }
      }
   }
   return n_active_bits;
}

