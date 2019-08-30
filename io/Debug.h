#pragma once

#include "ai/HeuristicValue.h"
#include "pieces/Piece.h"
#include "Translate.h"
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <utility> // std::pair


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A collection of functions to make debugging faster / easier
///
////////////////////////////////////////////////////////////////////////////////
namespace debug
{

void Print(int val, const std::string& prefix_msg = "");
void Print(const std::string& msg);
void PrintPos(int pos);
void PrintMask(uint64_t mask, bool labeled = false);
void PrintMasks(uint64_t mask1, uint64_t mask2, bool labeled = false);
void PrintPiece(const Piece& piece);
void PrintAction(const Action& action, const std::string& prefix_msg = "");
void PrintAction(const Action& action, const HVal& h_val);
void PrintAction(const Action& action, int h_val);
void PrintAction(const Action& action, double h_val);
void PrintBitBoard(const uint8_t* bitBoard);
void PrintByteAsHex(uint8_t byte);

} // end namespace debug

