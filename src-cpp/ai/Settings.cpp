

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
   ASSERT(seconds_limit || test);
}

