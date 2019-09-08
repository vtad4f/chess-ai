#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A knight can move or attach two spaces either horizontally or
///           vertically, and then one space to the left or right (from the
///           perspective of the direction it just moved).
///
////////////////////////////////////////////////////////////////////////////////
class Knight : public Piece
{
public:
   static const int VALUE = 3;
   
   Knight(const uint8_t* bitBoard, bool black, int knight_i);
   virtual ~Knight();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   static uint64_t __MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   
protected:
};

