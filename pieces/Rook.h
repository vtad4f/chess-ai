#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Rooks can move and attack horizontally and vertically in any
///           direction.
///
////////////////////////////////////////////////////////////////////////////////
class Rook : public Piece
{
public:
   static const int VALUE = 5;
   
   Rook(const uint8_t* bitBoard, bool black, int rook_i);
   virtual ~Rook();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual void Move(uint8_t* bitBoard, const Action& action) const override;
   virtual void SetCaptured(uint8_t* bitBoard) const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   static uint64_t __MoveMask(uint64_t posMask, int row, int col, const PlayerMasks& playerMasks, const MaskOptions& maskOptions);
   
protected:
   bool CastleAvailable() const;
   
   int m_CastleBitshift;
   uint8_t m_CastleMask;
};

