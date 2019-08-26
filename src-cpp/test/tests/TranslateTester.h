#pragma once


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A class for testing translation
///
////////////////////////////////////////////////////////////////////////////////
class TranslateTester
{
public:
   static void RunTests();
   
protected:
   static void MakeBitMask(int row_start = 7, int row_end = 0, int col_start = 0, int col_end = 7);
   static void test_MaskToStr();
};

