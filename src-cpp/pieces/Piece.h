#pragma once

#include "ai/Action.h"
#include <cstdint>
#include <string>
#include <set>

static constexpr int TOP_ROW         = 7;
static constexpr int TOP_PAWN_ROW    = 6;
static constexpr int BOTTOM_PAWN_ROW = 1;
static constexpr int BOTTOM_ROW      = 0;

static constexpr int LEFT_COL  = 0;
static constexpr int RIGHT_COL = 7;


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A simple base class for pieces
///
////////////////////////////////////////////////////////////////////////////////
class Piece
{
public:
   
   // Helper class to store all the masks needed to generate piece move masks
   struct PlayerMasks
   {
      PlayerMasks(uint64_t myPieces = 0, uint64_t theirPieces = 0, uint64_t theirMoves = 0,
                  uint64_t theirKing = 0, uint64_t myKingsDangerSquares = 0);
      void Clear();
      uint64_t myPieces;
      uint64_t theirPieces;
      uint64_t theirMoves;
      uint64_t theirKing;
      uint64_t myKingsDangerSquares;
   };
   
   // Helper class to store options used when generating move masks
   struct MaskOptions
   {
      MaskOptions(bool guard = false, bool throughKing = false, bool blockableKingAttack = false,
                  bool throughNotKingOnce = false, bool skipKing = false);
      bool guard;
      bool throughKing;
      bool blockableKingAttack;
      bool throughNotKingOnce;
      bool skipKing;
   };
   
   Piece(const uint8_t* bitBoard, int posIndex);
   virtual ~Piece();
   
   int Index() const;
   int Pos() const;
   uint64_t PosMask() const;
   
   virtual std::string Type() const = 0;   
   virtual int Value() const = 0;
   
   // pawn should override these
   virtual bool Captured() const;
   virtual void GetActions(int64_t moveMask, std::set<Action, std::greater<Action> >& actions) const;
   
   virtual void Move(uint8_t* bitBoard, const Action& action) const;
   virtual void SetCaptured(uint8_t* bitBoard) const;
   
   virtual uint64_t EnPassantMask() const;
   
   // every piece overrides this (should return 0 if captured)
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const = 0;
   
protected:
   int Row() const;
   int Col() const;
   
   static bool ApplyPosToMask(uint64_t& moveMask, uint64_t posMask, int& through, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   static bool FindBlockableKingAttack(uint64_t& moveMask, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   
   const uint8_t* m_BitBoard;
   const int m_PosIndex;
};

