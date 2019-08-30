

#include "ParserTester.h"
#include "io/Parser.h"
#include "board/Board/BitBoard.h"
#include "io/Error.h"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Run all the tests
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::RunTests()
{
   // PrintPositions(0); // Useful when setting up a test
   test_StartingFen();
   test_CastleFen();
   test_PromotedQueensFen();
   test_PromotedBishopsFen();
   test_PromotedKnightsFen();
   test_EnPassantFen();
   test_LateGameFen1();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Useful method for seeing [0, 64) board positions in hex
///
///   @param last2bits  Set the last two bits (since the pos value is shifted
///                     left by 2)
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::PrintPositions(int last2bits)
{
   for(int row = 7; row >= 0; --row)
   {
      for(int col = 0; col < 8; ++col)
      {
         std::cout << std::hex << std::setw(2)
                   << (((col*8 + row)<< 2) + last2bits) << ' ';
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check parsing the typical starting board for chess
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_StartingFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x18, 0x7C, 0x38, 0x1C, 0x58, 0xFC, 0x78,
      0x5C, 0x98, 0xBC, 0xB8, 0x3C, 0xD8, 0xDC, 0xF8,
      0x80, 0x04, 0x60, 0x24, 0x00, 0x44, 0xE0, 0x64,
      0x40, 0x84, 0xA0, 0xA4, 0x20, 0xC4, 0xC0, 0xE4,
      0x00, 0x00, 0x00, 0xF0
   };
   
   static const std::string FEN = 
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check parsing the typical starting board for chess
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_CastleFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x18, 0x7C, 0x38, 0x1C, 0x58, 0xFC, 0x78,
      0x5C, 0x98, 0xBC, 0xB8, 0x3C, 0xD8, 0xDC, 0xF8,
      0x80, 0x04, 0x60, 0x24, 0x00, 0x44, 0xE0, 0x64,
      0x40, 0x84, 0xA0, 0xA4, 0x20, 0xC4, 0xC0, 0xE4,
      0x00, 0x00, 0x00, 0xA0
   };
   
   static const std::string FEN = 
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qq - 8 5";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check a board with the maximum number of promoted queens
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_PromotedQueensFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x58, 0xBC, 0x98, 0x02, 0x34, 0x02, 0x74,
      0x02, 0x10, 0x02, 0x50, 0x02, 0x2C, 0x02, 0x08,
      0x80, 0xD0, 0xF4, 0xAC, 0x02, 0xEC, 0x02, 0x88,
      0x02, 0xC8, 0x02, 0x64, 0x02, 0xA4, 0x02, 0x40,
      0xFF, 0xFF, 0x00, 0x00
   };
   
   static const std::string FEN = 
      "4kq2/2q1q3/1q1q3Q/q1q3Q1/1q3Q1Q/q3Q1Q1/3Q1Q2/2Q1K3 w - -";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check a board with several promoted bishops
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_PromotedBishopsFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x9A, 0x02, 0x36, 0x02, 0x76, 0x02, 0x52,
      0xBD, 0x00, 0x59, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x80, 0xCA, 0x02, 0x66, 0x02, 0xA6, 0x02, 0x42,
      0xAD, 0x00, 0x89, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x0F, 0x0F, 0x01, 0x00
   };
   
   static const std::string FEN = 
      "4kb2/2b1b3/1b1b4/2b5/5B2/4B1B1/3B1B2/2B1K3 b - -";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check a board with several promoted knights
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_PromotedKnightsFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x9B, 0x02, 0x37, 0x02, 0x77, 0x02, 0x53,
      0x03, 0x00, 0x03, 0x00, 0xBD, 0x00, 0x59, 0x00,
      0x80, 0xCB, 0x02, 0x67, 0x02, 0xA7, 0x02, 0x43,
      0x03, 0x00, 0x03, 0x00, 0xAD, 0x00, 0x89, 0x00,
      0x0F, 0x0F, 0x00, 0x00
   };
   
   static const std::string FEN = 
      "4kn2/2n1n3/1n1n4/2n5/5N2/4N1N1/3N1N2/2N1K3 w - -";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check a board with a pawn that could be captured with en passant
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_EnPassantFen()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0x9C, 0x18, 0x7C, 0x38, 0x1C, 0x78, 0xFC, 0xB8,
      0x5C, 0xD8, 0xBC, 0xF8, 0x3C, 0x94, 0xDC, 0x50,
      0x80, 0x70, 0x60, 0x04, 0x00, 0x24, 0xE0, 0x44,
      0x40, 0x84, 0xA0, 0xA4, 0x20, 0xC4, 0xC0, 0xE4,
      0x00, 0x00, 0x56, 0xF0
   };
   
   static const std::string FEN = 
      "rnbqkbnr/pp1p1ppp/4p3/2pP4/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 3";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}



////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check a board with a pawn that could be captured with en passant
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::test_LateGameFen1()
{
   static const uint8_t EXPECTED[BITBOARD_ARRAY_LEN] = {
      0xB9, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x49, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x80, 0x64, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x00, 0x00, 0x00, 0x00
   };
   
   static const std::string FEN = "8/5k2/8/8/8/2b5/3P4/4K3 w - -";
      
   BitBoard bitBoard = Parser(FEN).GetBitBoard();
   
   AssertEq(__func__, EXPECTED, bitBoard.array);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Throw Error if array contents don't match
///
///   @param testName  The name of the test
///   @param expected  The expected array contents
///   @param actual  The actual array contents
///
////////////////////////////////////////////////////////////////////////////////
void ParserTester::AssertEq(const std::string& testName,
                            const uint8_t expected[BITBOARD_ARRAY_LEN],
                            const uint8_t actual[BITBOARD_ARRAY_LEN])
{
   std::stringstream ss;
   for(int i = 0; i < BITBOARD_ARRAY_LEN; ++i)
   {
      if (expected[i] != actual[i])
      {
         if (ss.rdbuf()->in_avail())
         {
            ss << '\n';
         }
         ss << testName << " (i=" << std::setw(2) << i << "): Expected "
            << std::bitset<8>(expected[i]) << " == "
            << std::bitset<8>(actual[i]) << std::flush;
      }
   }
   if (ss.rdbuf()->in_avail())
   {
      throw Error(ss.str());
   }
}

