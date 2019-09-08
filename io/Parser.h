#pragma once

#include "board/BitBoard.h"
#include <string>
#include <vector>


class Parser
{
public:
   
   // Helper class for unit testing
   struct Options
   {
      Options(bool swap_b_r = false, bool swap_b_b = false, bool swap_b_n = false,
              bool swap_w_r = false, bool swap_w_b = false, bool swap_w_n = false);
      void Swap(int& i, bool pawn = false) const;
      
      const bool swap_b_r;
      const bool swap_b_b;
      const bool swap_b_n;
      const bool swap_w_r;
      const bool swap_w_b;
      const bool swap_w_n;
      std::vector<int> swap_b_pawn;
      std::vector<int> swap_w_pawn;
   };
   
   Parser(const std::string& fen, const Options& options = Options());
   BitBoard GetBitBoard() const;
   
protected:
   
   // Helper class for counting pieces
   struct PlayerPieceCount
   {
      PlayerPieceCount();
      
      int kings;
      int queens;
      int rooks;
      int bishops;
      int knights;
      int pawns;
      int total;
   };
   
   // Helper class to track section of fen being parsed
   struct Section
   {
      Section();
      
      static const int BOARD      = 0;
      static const int COLOR      = 1; // active player
      static const int CASTLE     = 2;
      static const int EN_PASSANT = 3;
      static const int HALF_MOVE  = 4; // half moves since pawn capture/advance
      static const int FULL_MOVE  = 5; // starts at 1, incremented after black move
      
      int id;
   };
   
   bool ParseKing  (PlayerPieceCount& count, int start_i);
   bool ParseQueen (PlayerPieceCount& count, int start_i, int promoted_i);
   void ParseRook  (PlayerPieceCount& count, int start_i, int promoted_i);
   bool ParseBishop(PlayerPieceCount& count, int start_i, int promoted_i);
   void ParseKnight(PlayerPieceCount& count, int start_i, int promoted_i);
   void ParsePawn  (PlayerPieceCount& count, int start_i, int promoted_i);
   void ParseRowDivider();
   void ParseSectionDivider();
   void ParseNumber(int n);
   bool ParseBlack();
   void ParseWhite();
   bool ParseCastle(char c);
   void ParseNoCastleOrEnPassant();
   bool ParseFile(char c);
   
   void AddPromotedPawn(PlayerPieceCount& count, int start_i, int promoted_i,
                        uint8_t promotedTo);
   uint8_t GetPos() const;
   
   const std::string m_Fen; // Forsyth Edwards Notation
   BitBoard m_BitBoard;
   uint8_t* m_Bytes; // A pointer to the bit board array
   
   PlayerPieceCount m_BlackCount;
   PlayerPieceCount m_WhiteCount;

   char m_File;
   int m_Rank;
   
   Section m_Section;
   
   Options m_Options;
};

