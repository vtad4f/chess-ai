

#include "BitBoard.h"
#include <algorithm> // std::copy, std::fill


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
///           Default to everything captured, no castle, no en passant,
///           default queen promotion, but all promotions set to false,
///           default to no castles available, and it is white's turn
///
////////////////////////////////////////////////////////////////////////////////
BitBoard::BitBoard()
   : array{0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
           0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
           0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
           0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
           0x00, 0x00, 0x00, 0x00}
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (from array)
///
////////////////////////////////////////////////////////////////////////////////
BitBoard::BitBoard(const uint8_t other[BITBOARD_ARRAY_LEN])
{
   std::copy(other, other + BITBOARD_ARRAY_LEN, array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Copy Constructor
///
////////////////////////////////////////////////////////////////////////////////
BitBoard::BitBoard(const BitBoard& other)
{
   std::copy(other.array, other.array + BITBOARD_ARRAY_LEN, array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Assignment operator
///
////////////////////////////////////////////////////////////////////////////////
BitBoard& BitBoard::operator = (const BitBoard& other)
{
   std::copy(other.array, other.array + BITBOARD_ARRAY_LEN, array);
   return *this;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Compare this bit board with another
///
////////////////////////////////////////////////////////////////////////////////
bool BitBoard::operator == (const BitBoard& other)
{
   for (int i = 0; i < BITBOARD_ARRAY_LEN; ++i)
   {
      if (array[i] != other.array[i])
      {
         return false;
      }
   }
   return true;
}

