#pragma once

#include <cstdint>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A class for testing the bit board
///
////////////////////////////////////////////////////////////////////////////////
class BitBoardTester
{
public:
   static void RunTests();
   
protected:
   static void test_SizeOf();
   static void test_ArrayToFields();
   static void test_RandomValues();
   
   static uint8_t NewRand(uint8_t mod);
   static uint8_t LastRand();
   
   static uint8_t m_Random;
};

