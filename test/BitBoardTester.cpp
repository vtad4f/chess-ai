

#include "BitBoardTester.h"
#include "board/Board/BitBoard.h"
#include "io/Error.h"
#include <bitset>
#include <cstdlib>
#include <iostream>


uint8_t BitBoardTester::m_Random = rand();


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Run all the tests
///
////////////////////////////////////////////////////////////////////////////////
void BitBoardTester::RunTests()
{
   test_SizeOf();
   test_ArrayToFields();
   test_RandomValues();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Expect a certain size for the bit board and its members
///
////////////////////////////////////////////////////////////////////////////////
void BitBoardTester::test_SizeOf()
{
   BitBoard bitBoard;
   ASSERT_EQ(36, sizeof(bitBoard));
   ASSERT_EQ(36, sizeof(bitBoard.array));
   ASSERT_EQ(36, sizeof(BitBoard::Fields));
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Expect an array to yield a specific struct in the BitBoard union
///
////////////////////////////////////////////////////////////////////////////////
void BitBoardTester::test_ArrayToFields()
{
   const uint8_t TEST_ARRAY[BITBOARD_ARRAY_LEN] = {
      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // 0101...
      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
      0x55, 0x55, 0x55, 0x55
   };
   BitBoard bitBoard(TEST_ARRAY);
   BitBoard::Fields* pFields = reinterpret_cast<BitBoard::Fields*>(bitBoard.array);
   
   // black big piece pos
   ASSERT_EQ(21, pFields->k_pos);
   ASSERT_EQ(21, pFields->q_pos);
   ASSERT_EQ(21, pFields->r1_pos);
   ASSERT_EQ(21, pFields->r2_pos);
   ASSERT_EQ(21, pFields->b1_pos);
   ASSERT_EQ(21, pFields->b2_pos);
   ASSERT_EQ(21, pFields->n1_pos);
   ASSERT_EQ(21, pFields->n2_pos);
   
   // black big piece captured bool
   ASSERT_EQ(false, pFields->k_p1_captured  & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->q_p2_captured  & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->r1_p3_captured & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->r2_p4_captured & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->b1_p5_captured & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->b2_p6_captured & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->n1_p7_captured & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->n2_p8_captured & BIG_CAPTURE_MASK);
   
   // black pawn captured bool
   ASSERT_EQ(true, pFields->k_p1_captured  & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->q_p2_captured  & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->r1_p3_captured & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->r2_p4_captured & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->b1_p5_captured & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->b2_p6_captured & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->n1_p7_captured & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->n2_p8_captured & PAWN_CAPTURE_MASK);
   
   // black pawn pos
   ASSERT_EQ(21, pFields->p1_pos);
   ASSERT_EQ(21, pFields->p2_pos);
   ASSERT_EQ(21, pFields->p3_pos);
   ASSERT_EQ(21, pFields->p4_pos);
   ASSERT_EQ(21, pFields->p5_pos);
   ASSERT_EQ(21, pFields->p6_pos);
   ASSERT_EQ(21, pFields->p7_pos);
   ASSERT_EQ(21, pFields->p8_pos);
   
   // black pawn new type from promotion
   ASSERT_EQ(1, pFields->p1_new_type);
   ASSERT_EQ(1, pFields->p2_new_type);
   ASSERT_EQ(1, pFields->p3_new_type);
   ASSERT_EQ(1, pFields->p4_new_type);
   ASSERT_EQ(1, pFields->p5_new_type);
   ASSERT_EQ(1, pFields->p6_new_type);
   ASSERT_EQ(1, pFields->p7_new_type);
   ASSERT_EQ(1, pFields->p8_new_type);
   
   // black pawn promoted bool
   ASSERT_EQ(0b00000000, pFields->p_promoted & (1 << 7));
   ASSERT_EQ(0b01000000, pFields->p_promoted & (1 << 6));
   ASSERT_EQ(0b00000000, pFields->p_promoted & (1 << 5));
   ASSERT_EQ(0b00010000, pFields->p_promoted & (1 << 4));
   ASSERT_EQ(0b00000000, pFields->p_promoted & (1 << 3));
   ASSERT_EQ(0b00000100, pFields->p_promoted & (1 << 2));
   ASSERT_EQ(0b00000000, pFields->p_promoted & (1 << 1));
   ASSERT_EQ(0b00000001, pFields->p_promoted & (1 << 0));
   
   // white big piece pos
   ASSERT_EQ(21, pFields->K_POS);
   ASSERT_EQ(21, pFields->Q_POS);
   ASSERT_EQ(21, pFields->R1_POS);
   ASSERT_EQ(21, pFields->R2_POS);
   ASSERT_EQ(21, pFields->B1_POS);
   ASSERT_EQ(21, pFields->B2_POS);
   ASSERT_EQ(21, pFields->N1_POS);
   ASSERT_EQ(21, pFields->N2_POS);
   
   // white big piece captured bool
   ASSERT_EQ(false, pFields->K_P1_CAPTURED  & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->Q_P2_CAPTURED  & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->R1_P3_CAPTURED & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->R2_P4_CAPTURED & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->B1_P5_CAPTURED & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->B2_P6_CAPTURED & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->N1_P7_CAPTURED & BIG_CAPTURE_MASK);
   ASSERT_EQ(false, pFields->N2_P8_CAPTURED & BIG_CAPTURE_MASK);
   
   // white pawn captured bool
   ASSERT_EQ(true, pFields->K_P1_CAPTURED  & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->Q_P2_CAPTURED  & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->R1_P3_CAPTURED & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->R2_P4_CAPTURED & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->B1_P5_CAPTURED & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->B2_P6_CAPTURED & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->N1_P7_CAPTURED & PAWN_CAPTURE_MASK);
   ASSERT_EQ(true, pFields->N2_P8_CAPTURED & PAWN_CAPTURE_MASK);
   
   // white pawn pos
   ASSERT_EQ(21, pFields->P1_POS);
   ASSERT_EQ(21, pFields->P2_POS);
   ASSERT_EQ(21, pFields->P3_POS);
   ASSERT_EQ(21, pFields->P4_POS);
   ASSERT_EQ(21, pFields->P5_POS);
   ASSERT_EQ(21, pFields->P6_POS);
   ASSERT_EQ(21, pFields->P7_POS);
   ASSERT_EQ(21, pFields->P8_POS);
   
   // white pawn new type from promotion
   ASSERT_EQ(1, pFields->P1_NEW_TYPE);
   ASSERT_EQ(1, pFields->P2_NEW_TYPE);
   ASSERT_EQ(1, pFields->P3_NEW_TYPE);
   ASSERT_EQ(1, pFields->P4_NEW_TYPE);
   ASSERT_EQ(1, pFields->P5_NEW_TYPE);
   ASSERT_EQ(1, pFields->P6_NEW_TYPE);
   ASSERT_EQ(1, pFields->P7_NEW_TYPE);
   ASSERT_EQ(1, pFields->P8_NEW_TYPE);
   
   // white pawn promoted bool
   ASSERT_EQ(0b00000000, pFields->P_PROMOTED & (1 << 7));
   ASSERT_EQ(0b01000000, pFields->P_PROMOTED & (1 << 6));
   ASSERT_EQ(0b00000000, pFields->P_PROMOTED & (1 << 5));
   ASSERT_EQ(0b00010000, pFields->P_PROMOTED & (1 << 4));
   ASSERT_EQ(0b00000000, pFields->P_PROMOTED & (1 << 3));
   ASSERT_EQ(0b00000100, pFields->P_PROMOTED & (1 << 2));
   ASSERT_EQ(0b00000000, pFields->P_PROMOTED & (1 << 1));
   ASSERT_EQ(0b00000001, pFields->P_PROMOTED & (1 << 0));
   
   // black and white last move and castling
   ASSERT_EQ(21,    pFields->en_passant_pos);
   // ASSERT_EQ(false, pFields->en_passant);
   // ASSERT_EQ(true,  pFields->blacks_turn);
   ASSERT_EQ(false, pFields->k_r1_r2_castle & R1_CASTLE_MASK);
   ASSERT_EQ(true,  pFields->k_r1_r2_castle & R2_CASTLE_MASK);
   ASSERT_EQ(false, pFields->K_R1_R2_CASTLE & R1_CASTLE_MASK);
   ASSERT_EQ(true,  pFields->K_R1_R2_CASTLE & R2_CASTLE_MASK);
   ASSERT_EQ(5,     pFields->padding);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Make a few edits to the fields member of a bit board and confirm
///           the values match
///
////////////////////////////////////////////////////////////////////////////////
void BitBoardTester::test_RandomValues()
{
   const uint8_t TEST_ARRAY[BITBOARD_ARRAY_LEN] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00
   };
   BitBoard bitBoard(TEST_ARRAY);
   BitBoard::Fields* pFields = reinterpret_cast<BitBoard::Fields*>(bitBoard.array);
   
   // first array assignment
   // bitBoard.array[0] = NewRand(64) << 2;
   // ASSERT_EQ(LastRand(), pFields->k_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[0] >> 2);
   
   // black big piece pos
   pFields->k_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->k_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[0] >> 2);
   pFields->q_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->q_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[2] >> 2);
   pFields->r1_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->r1_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[4] >> 2);
   pFields->r2_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->r2_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[6] >> 2);
   pFields->b1_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->b1_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[8] >> 2);
   pFields->b2_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->b2_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[10] >> 2);
   pFields->n1_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->n1_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[12] >> 2);
   pFields->n2_pos = NewRand(64);
   ASSERT_EQ(LastRand(), pFields->n2_pos);
   // ASSERT_EQ(LastRand(), bitBoard.array[14] >> 2);
   
   // black big piece captured
   pFields->k_p1_captured = NewRand(64);
   ASSERT_EQ(LastRand() & BIG_CAPTURE_MASK,
             pFields->k_p1_captured & BIG_CAPTURE_MASK);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Make a new random number and cache it
///
///   @param mod  The value to mod the random number by
///
////////////////////////////////////////////////////////////////////////////////
uint8_t BitBoardTester::NewRand(uint8_t mod)
{
   return m_Random = rand() % mod;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the last random value cached
///
///   @return  The last random number returned by NewRand
///
////////////////////////////////////////////////////////////////////////////////
uint8_t BitBoardTester::LastRand()
{
   return m_Random;
}

