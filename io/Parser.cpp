

#include "Parser.h"
#include "Error.h"


// What is the max number of each piece we could see?
static constexpr int MAX_KINGS   = 1;
static constexpr int MAX_QUEENS  = 1 + 8; // promoted?
static constexpr int MAX_ROOKS   = 2 + 8; // promoted?
static constexpr int MAX_BISHOPS = 2 + 8; // promoted?
static constexpr int MAX_KNIGHTS = 2 + 8; // promoted?
static constexpr int MAX_PAWNS   = 8;

// Provide constants to tell if pawns have been promoted
static constexpr int Q_PROMOTE_THRESHOLD = 1;
static constexpr int RBN_PROMOTE_THRESHOLD = 2;

// Track current file/rank when parsing pieces
static constexpr int LEFT   = 'a'; // min file
static constexpr int RIGHT  = 'h'; // max file
static constexpr int TOP    = 8;   // max rank
static constexpr int BOTTOM = 1;   // min rank

////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Parser::Parser(const std::string& fen, const Options& options)
   : m_Fen(fen)
   , m_BitBoard()
   , m_Bytes(m_BitBoard.array)
   , m_BlackCount()
   , m_WhiteCount()
   , m_File(LEFT) // min file
   , m_Rank(TOP) // max rank
   , m_Section()
   , m_Options(options)
{
   for (auto c : m_Fen)
   {
      switch(c)
      {
         case 'k':
            ParseKing(m_BlackCount, BLACK_START) ||
            ParseCastle(c);
            break;
         case 'q':
            ParseQueen(m_BlackCount, BLACK_START, BLACK_PROMOTED) ||
            ParseCastle(c);
            break;
         case 'r': ParseRook(m_BlackCount, BLACK_START, BLACK_PROMOTED); break;
         case 'b':
            ParseBishop(m_BlackCount, BLACK_START, BLACK_PROMOTED) ||
            ParseBlack() ||
            ParseFile(c);
            break;
         case 'n': ParseKnight(m_BlackCount, BLACK_START, BLACK_PROMOTED); break;
         case 'p': ParsePawn(m_BlackCount, BLACK_START, BLACK_PROMOTED); break;
         
         case 'K':
            ParseKing(m_WhiteCount, WHITE_START) ||
            ParseCastle(c);
            break;
         case 'Q':
            ParseQueen(m_WhiteCount, WHITE_START, WHITE_PROMOTED) ||
            ParseCastle(c);
            break;
         case 'R': ParseRook(m_WhiteCount, WHITE_START, WHITE_PROMOTED); break;
         case 'B': ParseBishop(m_WhiteCount, WHITE_START, WHITE_PROMOTED); break;
         case 'N': ParseKnight(m_WhiteCount, WHITE_START, WHITE_PROMOTED); break;
         case 'P': ParsePawn(m_WhiteCount, WHITE_START, WHITE_PROMOTED); break;
         
         case '/': ParseRowDivider(); break;
         case ' ': ParseSectionDivider(); break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': ParseNumber(c - '0'); break;
         case '-': ParseNoCastleOrEnPassant(); break;
         case 'a':
         case 'c':
         case 'd':
         case 'e':
         case 'f':
         case 'g':
         case 'h': ParseFile(c); break;
         case 'w': ParseWhite(); break;
         default: EXIT("Unknown case"); break;
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the bit board parsed form the fen string
///
////////////////////////////////////////////////////////////////////////////////
BitBoard Parser::GetBitBoard() const
{
   return m_BitBoard;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a king
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseKing(PlayerPieceCount& count, int start_i)
{
   if (m_Section.id == Section::BOARD)
   {
      ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
      
      ++count.kings;
      ASSERT_LE(count.kings, MAX_KINGS);
      
      int i = start_i + K_INDEX;
      m_Bytes[i] |= GetPos() << POS_BITSHIFT;
      m_Bytes[i] &= ~BIG_CAPTURE_MASK; // not captured
      
      ++m_File; // Increment file for next piece
   }
   return m_Section.id == Section::BOARD;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a queen (maybe from promotion)
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseQueen(PlayerPieceCount& count, int start_i, int promoted_i)
{
   if (m_Section.id == Section::BOARD)
   {
      ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
      
      count.queens += 1;
      ASSERT_LE(count.queens, MAX_QUEENS);
      
      // If too many queens, this must be a promoted pawn
      if (count.queens > Q_PROMOTE_THRESHOLD)
      {
         AddPromotedPawn(count, start_i, promoted_i, PROMOTED_TO_Q);
      }
      else // Else assume just a usual queen
      {
         int i = start_i + Q_INDEX;
         m_Bytes[i] |= GetPos() << POS_BITSHIFT;
         m_Bytes[i] &= ~BIG_CAPTURE_MASK; // not captured
      }
      ++m_File; // Increment file for next piece
   }
   return m_Section.id == Section::BOARD;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a rook (maybe from promotion)
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseRook(PlayerPieceCount& count, int start_i, int promoted_i)
{
   ASSERT_EQ(Section::BOARD, m_Section.id);
   ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
   
   count.rooks += 1;
   ASSERT_LE(count.rooks, MAX_ROOKS);
   
   // If too many rooks, this must be a promoted pawn
   if (count.rooks > RBN_PROMOTE_THRESHOLD)
   {
      AddPromotedPawn(count, start_i, promoted_i, PROMOTED_TO_R);
   }
   else // Else assume just a usual rook
   {
      int i = start_i + (count.rooks == 1 ? R1_INDEX : R2_INDEX);
      m_Options.Swap(i);
      m_Bytes[i] |= GetPos() << POS_BITSHIFT;
      m_Bytes[i] &= ~BIG_CAPTURE_MASK; // not captured
   }
   ++m_File; // Increment file for next piece
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a bishop (maybe from promotion)
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseBishop(PlayerPieceCount& count, int start_i, int promoted_i)
{
   if (m_Section.id == Section::BOARD)
   {
      ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
      
      count.bishops += 1;
      ASSERT_LE(count.bishops, MAX_BISHOPS);
      
      // If too many bishops, this must be a promoted pawn
      if (count.bishops > RBN_PROMOTE_THRESHOLD)
      {
         AddPromotedPawn(count, start_i, promoted_i, PROMOTED_TO_B);
      }
      else // Else assume just a usual bishop
      {
         int i = start_i + (count.bishops == 1 ? B1_INDEX : B2_INDEX);
         m_Options.Swap(i);
         m_Bytes[i] |= GetPos() << POS_BITSHIFT;
         m_Bytes[i] &= ~BIG_CAPTURE_MASK; // not captured
      }
      ++m_File; // Increment file for next piece
   }
   return m_Section.id == Section::BOARD;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a knight (maybe from promotion)
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseKnight(PlayerPieceCount& count, int start_i, int promoted_i)
{
   ASSERT_EQ(Section::BOARD, m_Section.id);
   ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
   
   count.knights += 1;
   ASSERT_LE(count.knights, MAX_KNIGHTS);
   
   // If too many knights, this must be a promoted pawn
   if (count.knights > RBN_PROMOTE_THRESHOLD)
   {
      AddPromotedPawn(count, start_i, promoted_i, PROMOTED_TO_N);
   }
   else // Else assume just a usual knight
   {
      int i = start_i + (count.knights == 1 ? N1_INDEX : N2_INDEX);
      m_Options.Swap(i);
      m_Bytes[i] |= GetPos() << POS_BITSHIFT;
      m_Bytes[i] &= ~BIG_CAPTURE_MASK; // not captured
   }
   ++m_File; // Increment file for next piece
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a promoted pawn (called from inside ParseQeen, Rook,
///           Bishop, Knight methods)
///
////////////////////////////////////////////////////////////////////////////////
void Parser::AddPromotedPawn(PlayerPieceCount& count, int start_i, int promoted_i,
                             uint8_t promotedTo)
{
   ++count.pawns; // This queen is a promoted pawn
   ASSERT_LE(count.pawns, MAX_PAWNS);
   
   int i = start_i + (count.pawns - 1) * 2;
   
   m_Bytes[i] &= ~PAWN_CAPTURE_MASK; // not captured
   
   m_Bytes[i + 1] |= (GetPos() << POS_BITSHIFT) | promotedTo;
   
   uint8_t promoted_mask = 1 << (count.pawns - 1);
   m_Bytes[promoted_i] |= promoted_mask; // promoted
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse a normal pawn (not promoted)
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParsePawn(PlayerPieceCount& count, int start_i, int promoted_i)
{
   ASSERT_EQ(Section::BOARD, m_Section.id);
   ASSERT_LE(m_File, RIGHT); // Confirm valid value for file
   
   count.pawns += 1;
   ASSERT_LE(count.pawns, MAX_PAWNS);
   
   int i = start_i + (count.pawns - 1) * 2;
   m_Options.Swap(i, true);
   
   m_Bytes[i] &= ~PAWN_CAPTURE_MASK; // not captured
   
   m_Bytes[i + 1] |= GetPos() << POS_BITSHIFT;
   
   uint8_t promoted_mask = 1 << (count.pawns - 1);
   m_Bytes[promoted_i] &= ~promoted_mask; // not promoted
   
   ++m_File; // Increment file for next piece
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Move down a row and start at the left
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseRowDivider()
{
   --m_Rank;
   ASSERT_GE(m_Rank, BOTTOM);
   m_File = LEFT; // reset to lhs
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Increment the section number with casting
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseSectionDivider()
{
   ASSERT_IN_RANGE(m_Section.id, Section::BOARD, Section::FULL_MOVE);
   m_Section.id += 1;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  For the first portion of the fen, a number would indicate how
///           many columns in the row to skip
///
///           For the en passant portion of the fen, this number is the rank
///           of the en passant target pos
///
///   @param n  The number parsed from the fen
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseNumber(int n)
{
   if (m_Section.id == Section::BOARD)
   {
      m_File += n;
   }
   else if (m_Section.id == Section::EN_PASSANT)
   {
      m_Rank = n;
      m_Bytes[SPECIAL] |= (GetPos() << POS_BITSHIFT) | EN_PASSANT_MASK;
   }
   else if (m_Section.id == Section::HALF_MOVE ||
            m_Section.id == Section::FULL_MOVE)
   {
      // Don't care
   }
   else
   {
      EXIT("Didn't expect a number in this section");
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  It is black's turn
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseBlack()
{
   if (m_Section.id == Section::COLOR)
   {
      m_Bytes[SPECIAL] |= BLACKS_TURN_MASK;
   }
   return m_Section.id == Section::COLOR;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  It is white's turn
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseWhite()
{
   ASSERT_EQ(Section::COLOR, m_Section.id);
   m_Bytes[SPECIAL] &= ~BLACKS_TURN_MASK;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  King/queen side castle is available for each color
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseCastle(char c)
{
   if (m_Section.id == Section::CASTLE)
   {
      switch (c)
      {
      case 'q': m_Bytes[CASTLE_INDEX] |= R1_CASTLE_MASK << BLACK_CASTLE_BITSHIFT; break;
      case 'k': m_Bytes[CASTLE_INDEX] |= R2_CASTLE_MASK << BLACK_CASTLE_BITSHIFT; break;
      case 'Q': m_Bytes[CASTLE_INDEX] |= R1_CASTLE_MASK << WHITE_CASTLE_BITSHIFT; break;
      case 'K': m_Bytes[CASTLE_INDEX] |= R2_CASTLE_MASK << WHITE_CASTLE_BITSHIFT; break;
      default: EXIT("Unknown case"); break;
      }
   }
   return m_Section.id == Section::CASTLE;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  There is no en passant (unset the bit)
///
////////////////////////////////////////////////////////////////////////////////
void Parser::ParseNoCastleOrEnPassant()
{
   if (m_Section.id == Section::EN_PASSANT)
   {
      m_Bytes[SPECIAL] &= ~EN_PASSANT_MASK; // no en passant
   }
   else if (m_Section.id == Section::CASTLE)
   {
      uint8_t castle_mask = 0;
      castle_mask |= R1_CASTLE_MASK << BLACK_CASTLE_BITSHIFT;
      castle_mask |= R2_CASTLE_MASK << BLACK_CASTLE_BITSHIFT;
      castle_mask |= R1_CASTLE_MASK << WHITE_CASTLE_BITSHIFT;
      castle_mask |= R2_CASTLE_MASK << WHITE_CASTLE_BITSHIFT;
      m_Bytes[CASTLE_INDEX] &= ~castle_mask; // no castle
   }
   else
   {
      EXIT("Didn't expect a \'-\' in this section");
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Parse the file portion of the en passant position
///
///   @param c  A char from the fen (interpreted as the first digit in the
///             en passant location)
///
///   @return  True if this function should be used to parse this char from
///            the fen string (else another function should interpret it)
///
////////////////////////////////////////////////////////////////////////////////
bool Parser::ParseFile(char c)
{
   if (m_Section.id == Section::EN_PASSANT)
   {
      m_File = c; // for en passant
   }
   return m_Section.id == Section::EN_PASSANT;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the current position using file and rank
///
////////////////////////////////////////////////////////////////////////////////
uint8_t Parser::GetPos() const
{
   uint8_t pos = (m_File - LEFT) * 8 + (m_Rank - 1);
   ASSERT_IN_RANGE(pos, 0, 64);
   return pos;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Parser::Options::Options(bool swap_b_r, bool swap_b_b, bool swap_b_n,
                         bool swap_w_r, bool swap_w_b, bool swap_w_n)
   : swap_b_r(swap_b_r)
   , swap_b_b(swap_b_b)
   , swap_b_n(swap_b_n)
   , swap_w_r(swap_w_r)
   , swap_w_b(swap_w_b)
   , swap_w_n(swap_w_n)
   , swap_b_pawn({0, 0, 0, 0, 0, 0, 0, 0})
   , swap_w_pawn({0, 0, 0, 0, 0, 0, 0, 0})
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Provide a way to swap the order pieces are placed in the
///           bit board
///
////////////////////////////////////////////////////////////////////////////////
void Parser::Options::Swap(int& i, bool pawn) const
{
   if (pawn)
   {
      switch (i)
      {
         case (BLACK_START +  0): i += (swap_b_pawn[0] * 2); break;
         case (BLACK_START +  2): i += (swap_b_pawn[1] * 2); break;
         case (BLACK_START +  4): i += (swap_b_pawn[2] * 2); break;
         case (BLACK_START +  6): i += (swap_b_pawn[3] * 2); break;
         case (BLACK_START +  8): i += (swap_b_pawn[4] * 2); break;
         case (BLACK_START + 10): i += (swap_b_pawn[5] * 2); break;
         case (BLACK_START + 12): i += (swap_b_pawn[6] * 2); break;
         case (BLACK_START + 14): i += (swap_b_pawn[7] * 2); break;
         
         case (WHITE_START +  0): i += (swap_w_pawn[0] * 2); break;
         case (WHITE_START +  2): i += (swap_w_pawn[1] * 2); break;
         case (WHITE_START +  4): i += (swap_w_pawn[2] * 2); break;
         case (WHITE_START +  6): i += (swap_w_pawn[3] * 2); break;
         case (WHITE_START +  8): i += (swap_w_pawn[4] * 2); break;
         case (WHITE_START + 10): i += (swap_w_pawn[5] * 2); break;
         case (WHITE_START + 12): i += (swap_w_pawn[6] * 2); break;
         case (WHITE_START + 14): i += (swap_w_pawn[7] * 2); break;
         
         default: EXIT("Unknown Case"); break;
      }
   }
   else
   {
      switch (i)
      {
         case (BLACK_START + R1_INDEX): if (swap_b_r) { i += 2; } break;
         case (BLACK_START + R2_INDEX): if (swap_b_r) { i -= 2; } break;
         case (BLACK_START + B1_INDEX): if (swap_b_b) { i += 2; } break;
         case (BLACK_START + B2_INDEX): if (swap_b_b) { i -= 2; } break;
         case (BLACK_START + N1_INDEX): if (swap_b_n) { i += 2; } break;
         case (BLACK_START + N2_INDEX): if (swap_b_n) { i -= 2; } break;
         
         case (WHITE_START + R1_INDEX): if (swap_w_r) { i += 2; } break;
         case (WHITE_START + R2_INDEX): if (swap_w_r) { i -= 2; } break;
         case (WHITE_START + B1_INDEX): if (swap_w_b) { i += 2; } break;
         case (WHITE_START + B2_INDEX): if (swap_w_b) { i -= 2; } break;
         case (WHITE_START + N1_INDEX): if (swap_w_n) { i += 2; } break;
         case (WHITE_START + N2_INDEX): if (swap_w_n) { i -= 2; } break;
         
         default: EXIT("Unknown Case"); break;
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Parser::PlayerPieceCount::PlayerPieceCount()
   : kings(0)
   , queens(0)
   , rooks(0)
   , bishops(0)
   , knights(0)
   , pawns(0)
   , total(0)
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Parser::Section::Section()
   : id(Section::BOARD)
{
   
}

