#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Pawns move upward on the board (from the perspective of the
///           player they belong to. Normally they move one space at a time,
///           but the first move they make can be two spaces.
///           
///           Pawns attack diagonally 1 square, again in the upward direction.
///           
///           There is also a special move called En Passant, where a pawn can
///           capture an opponents pawn that just moved two spaces, by moving
///           behind it.
///
////////////////////////////////////////////////////////////////////////////////
class Pawn : public Piece
{
public:
   static const int VALUE = 1;
   
   Pawn(const uint8_t* bitBoard, bool black, int pawn_i);
   virtual ~Pawn();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual bool Captured() const override;
   virtual void GetActions(int64_t moveMask, std::set<Action, std::greater<Action> >& actions) const override;
   
   virtual void Move(uint8_t* bitBoard, const Action& action) const override;
   virtual void SetCaptured(uint8_t* bitBoard) const override;
   
   uint64_t EnPassantMask() const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   
protected:
   bool Promoted() const;
   uint8_t PromotedType() const;
   
   uint64_t __MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions) const;
   
   int m_PromotedIndex;
   uint8_t m_PromotedMask;
   bool m_MoveDownward;
};

