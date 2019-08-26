#pragma once


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This struct provides easy access to the global settings
///           retrieved in ai.cpp
///
////////////////////////////////////////////////////////////////////////////////
struct Settings
{
   static Settings& Instance();
   Settings& operator = (const Settings& other);
   
   void Validate();
   void UpdateBeforeTurn(int current_turn);
   void DecrementDL(int current_turn);
   
   bool silent;
   bool verbose;
   bool very_verbose;
   bool random;
   bool alpha_beta;
   bool history_table;
   bool pondering;
   double seconds_limit;
   int quiescent;
   int min_depth_limit;
   int max_depth_limit;
   bool decrement_dl;
   int turn_limit;
   int which_ai;
   bool even_depths_only;
   bool test; // set only if unit testing
};

