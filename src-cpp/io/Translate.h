#pragma once

#include <cstdint>
#include <string>
#include <utility> // std::pair


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This class is used to translate between different 
///           representations of a position or positions on the board.
///           
///           A position value [0, 63] starts with a value of 0 in the bottom-
///                                    right corner. Then increments upward,
///            +-----   -----+         then right.
///            | 7 15   55 63|         
///            | 6 14   54 62|         This is consistent with the direction
///               .......              the values in algebraic notion increase.
///            | 1  9   49 27|         
///            | 0  8   48 56|         For a bit map of a position, each number
///            +-----   -----+         on the left would correspond to a bit.
///
////////////////////////////////////////////////////////////////////////////////
class Translate
{
public:
   static std::string MaskToStr(uint64_t mask, bool labeled = false);
   static uint64_t PosToMask(int pos);
   static int ColRowToPos(int col, int row);
   static int FileRankToPos(const std::string& file, int rank);
   static std::pair<std::string, int> PosToFileRank(int pos);
   static int AlgebraicStrToPos(const std::string& algebraic);
   static std::string PosToAlgebraicStr(int pos);
   static std::string PromotionIntToStr(int promotion);
   static uint8_t PromotionStrToInt(const std::string& promotion);
   static double CountActiveBits(uint64_t mask);
};

