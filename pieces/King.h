#pragma once

#include "Piece.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Kings can move or attack one square in any direction.
///           
///           A king can make a special move called castling, where it moves
///           adjacent to a rook and then the rook is moved to the opposite
///           side of the king. A castle isn't allowed if the king is in check,
///           would castle through check, into check, or if the king or rook
///           have moved already.
///           
///           A king is in check if it is currently threatened by another
///           piece. 
///           
///           Checkmate occurs when a king is unable to move and is in check
///           and no other move can be made to eliminate the check.
///           
///           Stalemate occurs when a king is unable to move and there is no
///           other move available.
///
////////////////////////////////////////////////////////////////////////////////
class King : public Piece
{
public:
   King(const uint8_t* bitBoard, bool black);
   virtual ~King();
   
   virtual std::string Type() const override;
   virtual int Value() const override;
   
   virtual void Move(uint8_t* bitBoard, const Action& action) const override;
   virtual void SetCaptured(uint8_t* bitBoard) const override;
   
   virtual uint64_t MoveMask(const PlayerMasks& playerMasks, const MaskOptions& maskOptions = MaskOptions()) const override;
   
protected:
   bool CastleAvailable() const;
   bool RlCastleAvailable() const;
   bool R2CastleAvailable() const;
   
   int m_CastleBitshift;
   uint64_t m_R1CastlePiecesMask; // Don't castle if pieces are in the way
   uint64_t m_R2CastlePiecesMask;
   uint64_t m_R1CastleThreatMask; // Don't castle through check
   uint64_t m_R2CastleThreatMask;
   uint64_t m_R1CastleKingPos; // The destination square for the king when castling
   uint64_t m_R2CastleKingPos;
   uint64_t m_R1Index; // King will move the rook when it castles
   uint64_t m_R2Index;
};

