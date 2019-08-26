#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A queen can move or capture diagonally, horizontally, or
///           vertically in any direction. It is essentially a bishop and rook
///           combined.
///
////////////////////////////////////////////////////////////////////////////////
class Queen : public Piece
{
public:
   static const int VALUE = 9;
   
   Queen(const uint8_t* bitBoard, bool black);
   virtual ~Queen();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   static uint64_t __MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   
protected:
};

