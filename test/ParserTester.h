#pragma once

#include "board/Board/BitBoard.h"
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A class for testing the parser
///
////////////////////////////////////////////////////////////////////////////////
class ParserTester
{
public:
   static void RunTests();
   
protected:
   static void PrintPositions(int last2bits);
   
   static void test_StartingFen();
   static void test_CastleFen();
   static void test_PromotedQueensFen();
   // TODO - test rook promotion
   static void test_PromotedBishopsFen();
   static void test_PromotedKnightsFen();
   static void test_EnPassantFen();
   static void test_LateGameFen1();
   // TODO - test en passant
   // TODO - test invalid fen strings
   
   static void AssertEq(const std::string& testName,
                        const uint8_t expected[BITBOARD_ARRAY_LEN],
                        const uint8_t actual[BITBOARD_ARRAY_LEN]);
};

