

#include "TranslateTester.h"
#include "io/Translate.h"
#include <bitset>
#include <iomanip>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Run all the tests
///
////////////////////////////////////////////////////////////////////////////////
void TranslateTester::RunTests()
{
   // Generate castle bit-masks
   // MakeBitMask(7, 7, 2, 3);
   // MakeBitMask(7, 7, 5, 6);
   // MakeBitMask(0, 0, 2, 3);
   // MakeBitMask(0, 0, 5, 6);
   
   // MakeBitMask(7, 7, 2, 2);
   // MakeBitMask(7, 7, 6, 6);
   // MakeBitMask(0, 0, 2, 2);
   // MakeBitMask(0, 0, 6, 6);
   
   // test_MaskToStr();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Quick sandbox for making bit masks
///
////////////////////////////////////////////////////////////////////////////////
void TranslateTester::MakeBitMask(int row_start, int row_end, int col_start, int col_end)
{
   uint64_t bitMask = 0;
   for(int row = row_start; row >= row_end; --row)
   {
      for(int col = col_start; col < (col_end + 1); ++col)
      {
         int pos = Translate::ColRowToPos(col, row);
         bitMask |= Translate::PosToMask(pos);
      }
   }
   std::cout << "mask = 0x" << std::setfill('0') << std::setw(16) << std::hex << bitMask << std::endl;
   std::cout << "mask = " << std::bitset<64>(bitMask) << std::endl;
   std::cout << Translate::MaskToStr(bitMask, true) << '\n' << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Run all the tests
///
////////////////////////////////////////////////////////////////////////////////
void TranslateTester::test_MaskToStr()
{
   for(int row = 7; row >= 0; --row)
   {
      for(int col = 0; col < 8; ++col)
      {
         int pos = Translate::ColRowToPos(col, row);
         uint64_t mask = Translate::PosToMask(pos);
         
         std::cout << "pos = " << pos << std::endl;
         std::cout << "mask = " << std::bitset<64>(mask) << std::endl;
         std::cout << Translate::MaskToStr(mask) << '\n' << std::endl;
      }
   }
   std::cout << std::endl;
}

