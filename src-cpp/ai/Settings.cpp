

#include "Settings.h"
#include "io/Error.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Access the static AI Settings
///
////////////////////////////////////////////////////////////////////////////////
Settings& Settings::Instance()
{
   static Settings settings;
   return settings;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Assignment operator
///
////////////////////////////////////////////////////////////////////////////////
Settings& Settings::operator = (const Settings& other)
{
   silent           = other.silent;
   verbose          = other.verbose;
   very_verbose     = other.very_verbose;
   random           = other.random;
   alpha_beta       = other.alpha_beta;
   history_table    = other.history_table;
   pondering        = other.pondering;
   seconds_limit    = other.seconds_limit;
   quiescent        = other.quiescent;
   min_depth_limit  = other.min_depth_limit;
   max_depth_limit  = other.max_depth_limit;
   decrement_dl     = other.decrement_dl;
   turn_limit       = other.turn_limit;
   which_ai         = other.which_ai;
   even_depths_only = other.even_depths_only;
   test             = other.test;
   return *this;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  When we first initialize the settings, we want to validate them.
///           Otherwise we can end up with some crazy output or behavior.
///
////////////////////////////////////////////////////////////////////////////////
void Settings::Validate()
{
   ASSERT_GE(max_depth_limit, 0);
   ASSERT_GE(seconds_limit, -1);
   ASSERT(!decrement_dl || max_depth_limit > 1);
   ASSERT(!random || !decrement_dl);
   ASSERT_GE(turn_limit, 0);
   ASSERT(seconds_limit || turn_limit || test);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This should be placed at the top of AI::run_turn
///
///   @param current_turn  An incrementing turn count (starts at 0)
///
////////////////////////////////////////////////////////////////////////////////
void Settings::UpdateBeforeTurn(int current_turn)
{
   DecrementDL(current_turn);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  When this method is called from Settings::UpdateAfterTurn, the
///           depth limit will decrease by 1 after each turn. This essentially
///           gives us a "stack trace" of best moves that keeps resetting.
///
////////////////////////////////////////////////////////////////////////////////
void Settings::DecrementDL(int current_turn)
{
   static const int ORIGINAL_MAX = max_depth_limit;
   if (decrement_dl)
   {
      max_depth_limit = ORIGINAL_MAX - current_turn;
      while (max_depth_limit < 1) // Don't drop below 1
      {
         max_depth_limit += ORIGINAL_MAX;
      }
   }
}

