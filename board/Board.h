#pragma once

#include "pieces/Piece.h"
#include "ai/Action.h"
#include <map>
#include <memory>
#include <set>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This class represents the current state of the game
///
////////////////////////////////////////////////////////////////////////////////
class Board
{
public:
   Board();
   virtual ~Board();
   
   int MovePiece(uint8_t* bitBoard, Action& action);
   static void SwapTurnPlayer(uint8_t* bitBoard);
   
   bool InCheck() const;
   int GetPieceIndex(int pos) const;
   void PrintPieceMasks() const;
   
   void GetTurnPlayerMoves(std::set<Action, std::greater<Action> >& actions) const;
   void GetPiecesAndMasks(const uint8_t* bitBoard);
   
protected:
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///   @brief  Helper class to consolidate storage of pieces for a player
   ///
   /////////////////////////////////////////////////////////////////////////////
   struct PlayerPieces
   {
      void Clear();
      std::shared_ptr<Piece> king;
      std::map<int, std::shared_ptr<Piece> > all;
      uint64_t pos_mask; // position bit-mask
   };
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///   @brief  Helper class to store 
   ///
   /////////////////////////////////////////////////////////////////////////////
   struct Masks
   {
      void Clear();
      Piece::PlayerMasks myMasks;
      Piece::PlayerMasks theirMasks;
      std::vector<uint64_t> threatsIfMove; // Potential threats
      uint64_t threatsIfMoveMask;
      std::vector<std::shared_ptr<Piece> > threats; // Active threats
   };
   
   void DontMoveIntoCheck(uint64_t posMask, uint64_t& moveMask) const;
   bool BlacksTurn() const;
   void GetMasks();
   void GetPieces(PlayerPieces& pieces, bool black) const;
   
   const uint8_t* m_BitBoard; // Set only when GetPiecesAndMasks is called
   PlayerPieces m_MyPieces;
   PlayerPieces m_TheirPieces;
   Masks        m_Masks;
};

