#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Bishops move and attack diagonally in any direction.
///           They can never move to a different colored square.
///
////////////////////////////////////////////////////////////////////////////////
class Bishop : public Piece
{
public:
   static const int VALUE = 3;
   
   Bishop(const uint8_t* bitBoard, bool black, int bishop_i);
   virtual ~Bishop();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   static uint64_t __MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   
protected:
};

