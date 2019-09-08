#pragma once

#include <cstdint>


// When iterating over pieces for a color, expect them at these indexes
constexpr int  K_INDEX = 0;
constexpr int  Q_INDEX = 2;
constexpr int R1_INDEX = 4;
constexpr int R2_INDEX = 6;
constexpr int B1_INDEX = 8;
constexpr int B2_INDEX = 10;
constexpr int N1_INDEX = 12;
constexpr int N2_INDEX = 14;

// These are the starting indexes for the major sections in the bit board
constexpr int BLACK_START        = 0;
constexpr int WHITE_START        = 16;
constexpr int BLACK_PROMOTED     = 32;
constexpr int WHITE_PROMOTED     = 33;
constexpr int SPECIAL            = 34;
constexpr int CASTLE_INDEX       = 35;
constexpr int BITBOARD_ARRAY_LEN = 36;

// These are bit-shifts to apply to a byte in the bit board
constexpr int POS_BITSHIFT          = 2;
constexpr int BLACK_CASTLE_BITSHIFT = 6;
constexpr int WHITE_CASTLE_BITSHIFT = 4;

// These are the masks to apply to a byte in the bit board
constexpr uint8_t CLEAR_POS_MASK    = 0b00000011;
constexpr uint8_t BIG_CAPTURE_MASK  = 0b00000010;
constexpr uint8_t PAWN_CAPTURE_MASK = 0b00000001;
constexpr uint8_t PAWN_PROMOTE_MASK = 0b00000011;
constexpr uint8_t EN_PASSANT_MASK   = 0b00000010;
constexpr uint8_t BLACKS_TURN_MASK  = 0b00000001;
constexpr uint8_t R1_CASTLE_MASK    = 0b00000010;
constexpr uint8_t R2_CASTLE_MASK    = 0b00000001;

// Possible values for what a pawn could be promoted to
constexpr uint8_t PROMOTED_TO_Q = 0;
constexpr uint8_t PROMOTED_TO_R = 1;
constexpr uint8_t PROMOTED_TO_B = 2;
constexpr uint8_t PROMOTED_TO_N = 3;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Thus far this bit board is only used for memory optimization,
///           by minimizing the size of the state data to be copied.
///           
///           In future versions we should attempt to store useful bit masks,
///           etc. to reduce the necessary run time processing.
///           
///           For now, store the minimal state:
///            
///            (24 bytes)
///               32 pieces * 2^6 positions = 196 bits 
///               
///            (4 bytes)
///               32 pieces captured = 32 bits (4 bytes)
///               
///            (6 bytes)
///               16 pawns promoted? = 16 bits (2 bytes)
///               16 pawns * 2^2 promotion types = 32 bits (4 bytes)
///               
///            (2 bytes)
///               en passant pos 2^6 + en passant? black's turn? = 8 bits 
///               2 * (king and r1 can castle + king and r2) = 4 bits
///               padding = 4 bits
///               
////////////////////////////////////////////////////////////////////////////////
struct BitBoard
{
   BitBoard();
   BitBoard(const uint8_t other[BITBOARD_ARRAY_LEN]);
   BitBoard(const BitBoard& other);
   BitBoard& operator = (const BitBoard& other);
   bool operator == (const BitBoard& other);
   
   uint8_t array[BITBOARD_ARRAY_LEN];
   
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///   @brief  This is a bit-field representation of the bit board array
   ///
   ///           Unfortunately the compiler can pick and choose how to arrange
   ///           everything. Despite this struct having the same size as the
   ///           array member variable below it, there is not a one-to-one
   ///           relation between the bits in each.
   ///               
   /////////////////////////////////////////////////////////////////////////////
   struct Fields
   {
      // black
      uint8_t k_pos          : 6; // king
      uint8_t k_p1_captured  : 2;
      uint8_t p1_pos         : 6; // pawn
      uint8_t p1_new_type    : 2;
      
      
      uint8_t q_pos          : 6; // queen
      uint8_t q_p2_captured  : 2;
      uint8_t p2_pos         : 6; // pawn
      uint8_t p2_new_type    : 2;
      
      
      uint8_t r1_pos         : 6; // rook
      uint8_t r1_p3_captured : 2;
      uint8_t p3_pos         : 6; // pawn
      uint8_t p3_new_type    : 2;
      
      
      uint8_t r2_pos         : 6; // rook
      uint8_t r2_p4_captured : 2;
      uint8_t p4_pos         : 6; // pawn
      uint8_t p4_new_type    : 2;
      
      
      uint8_t b1_pos         : 6; // bishop
      uint8_t b1_p5_captured : 2;
      uint8_t p5_pos         : 6; // pawn
      uint8_t p5_new_type    : 2;
      
      
      uint8_t b2_pos         : 6; // bishop
      uint8_t b2_p6_captured : 2;
      uint8_t p6_pos         : 6; // pawn
      uint8_t p6_new_type    : 2;
      
      
      uint8_t n1_pos         : 6; // knight
      uint8_t n1_p7_captured : 2;
      uint8_t p7_pos         : 6; // pawn
      uint8_t p7_new_type    : 2;
      
      
      uint8_t n2_pos         : 6; // knight
      uint8_t n2_p8_captured : 2;
      uint8_t p8_pos         : 6; // pawn
      uint8_t p8_new_type    : 2;
      
      
      // white
      uint8_t K_POS          : 6; // king
      uint8_t K_P1_CAPTURED  : 2;
      uint8_t P1_POS         : 6; // pawn
      uint8_t P1_NEW_TYPE    : 2;
      
      
      uint8_t Q_POS          : 6; // queen
      uint8_t Q_P2_CAPTURED  : 2;
      uint8_t P2_POS         : 6; // pawn
      uint8_t P2_NEW_TYPE    : 2;
      
      
      uint8_t R1_POS         : 6; // rook
      uint8_t R1_P3_CAPTURED : 2;
      uint8_t P3_POS         : 6; // pawn
      uint8_t P3_NEW_TYPE    : 2;
      
      
      uint8_t R2_POS         : 6; // rook
      uint8_t R2_P4_CAPTURED : 2;
      uint8_t P4_POS         : 6; // pawn
      uint8_t P4_NEW_TYPE    : 2;
      
      
      uint8_t B1_POS         : 6; // bishop
      uint8_t B1_P5_CAPTURED : 2;
      uint8_t P5_POS         : 6; // pawn
      uint8_t P5_NEW_TYPE    : 2;
      
      
      uint8_t B2_POS         : 6; // bishop
      uint8_t B2_P6_CAPTURED : 2;
      uint8_t P6_POS         : 6; // pawn
      uint8_t P6_NEW_TYPE    : 2;
      
      
      uint8_t N1_POS         : 6; // knight
      uint8_t N1_P7_CAPTURED : 2;
      uint8_t P7_POS         : 6; // pawn
      uint8_t P7_NEW_TYPE    : 2;
      
      
      uint8_t N2_POS         : 6; // knight
      uint8_t N2_P8_CAPTURED : 2;
      uint8_t P8_POS         : 6; // pawn
      uint8_t P8_NEW_TYPE    : 2;
      
      
      // black / white
      uint8_t p_promoted; // Ignore 'new_type' if not promoted
      uint8_t P_PROMOTED;
      
      
      // black / neutral / white
      uint8_t en_passant_pos : 6; // track last move for en passant
      uint8_t en_passant     : 1; // en passant available?
      uint8_t blacks_turn    : 1; // turn player is black?
      uint8_t k_r1_r2_castle : 2; // track if pieces have moved for castle
      uint8_t K_R1_R2_CASTLE : 2;
      uint8_t padding        : 4;
   };
};

