#pragma once

#include <iostream>
#include <map>
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A base Action class used to store a piece's position movement and
///           promotion value
///
////////////////////////////////////////////////////////////////////////////////
struct SimpleAction
{
   SimpleAction(int start_pos, int end_pos, bool promoted = false, int promoted_type = 0);
   SimpleAction(const SimpleAction& other);
   
   uint8_t start_pos     : 6;
   uint8_t captured      : 1;
   uint8_t promoted      : 1;
   uint8_t end_pos       : 6;
   uint8_t promoted_type : 2;
};

////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A more complex action class, with a history table sorting
///           mechanism
///
////////////////////////////////////////////////////////////////////////////////
struct Action : public SimpleAction
{
   static constexpr int UNKNOWN_INDEX = -1;
   
   Action();
   Action(int start_pos, int end_pos, int piece_index, bool promoted = false, int promoted_type = 0);
   Action(const std::string& from_file, int from_rank, const std::string& to_file, int to_rank, const std::string& promotion = "");
   Action(const std::string& from, const std::string& to, const std::string& promotion = "");
   Action(const Action& other);
   Action& operator = (const Action& other);
   bool operator == (const Action& other) const;
   bool operator > (const Action& other) const;
   
   int PromotionValueDelta() const;
   
   int piece_index;
   uint64_t history;
   int random;
};


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A class used to store the move and potential promotion
///
///           Use an explicit file and rank to represent a position
///
////////////////////////////////////////////////////////////////////////////////
struct FileRankAction
{
   FileRankAction(const Action& action);
   
   std::pair<std::string, int> start_pos;
   std::pair<std::string, int> end_pos;
   std::string promotion;
};

