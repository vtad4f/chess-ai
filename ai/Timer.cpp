

#include "Timer.h"
#include "Settings.h"
#include "io/Error.h"
#include "io/Debug.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a static timer instance
///
////////////////////////////////////////////////////////////////////////////////
Timer& Timer::Instance()
{
   static Timer timer;
   return timer;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor (initialize start time)
///
////////////////////////////////////////////////////////////////////////////////
Timer::Timer()
   : m_SecondsInGame(0.0)
   , m_SecondsThisTurn(0.0)
   , m_Start(std::chrono::steady_clock::now())
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Reset the start time used in the 'Elapsed' calculation
///
///   @param remaining_s  Time remaining for the player in seconds
///
////////////////////////////////////////////////////////////////////////////////
void Timer::Restart(double remaining_s)
{
   // Initialize the total seconds in the game
   if (m_SecondsInGame == 0.0)
   {
      m_SecondsInGame = remaining_s;
   }
   
   // If there is a positive seconds limit setting, use it as is
   static Settings& settings = Settings::Instance();
   if (settings.seconds_limit >= 0.0)
   {
      m_SecondsThisTurn = settings.seconds_limit;
   }
   else
   {
      // Ignore a negative seconds limit setting and calculate a percent
      if (remaining_s / m_SecondsInGame > .95)
      {
         // For the first 5% of the game, make somewhat fast moves
         m_SecondsThisTurn = 0.0075 * m_SecondsInGame;
      }
      else if (remaining_s / m_SecondsInGame > .6)
      {
         // For the next 35% of the game, take our time
         m_SecondsThisTurn = 0.02 * m_SecondsInGame;
      }
      else if (remaining_s / m_SecondsInGame > .2)
      {
         // For the middle 40% - 80% of the game, take a little less time
         m_SecondsThisTurn = 0.01 * m_SecondsInGame;
      }
      else if (remaining_s / m_SecondsInGame > .05)
      {
         // For the next 80% - 95% of the game, move pretty fast
         m_SecondsThisTurn = 0.005 * m_SecondsInGame;
      }
      else
      {
         // For the last 5% of the game, move as soon as we pass a certain depth
         m_SecondsThisTurn = 0.000001;
         settings.min_depth_limit = 4; // Should be able to get to this quickly
      }
   }
   
   m_Start = std::chrono::steady_clock::now();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the number of seconds that have elapsed since the timer was
///           created or since the last restart
///
////////////////////////////////////////////////////////////////////////////////
double Timer::Elapsed() const
{
   return std::chrono::duration<double>(
      std::chrono::steady_clock::now() - m_Start).count();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If elapsed exceeds the limit specified in settings, throw
///           an exception
///
////////////////////////////////////////////////////////////////////////////////
void Timer::CheckOutOfTime(int depth)
{
   if (m_SecondsThisTurn > 0.0)
   {
      if (Elapsed() >= m_SecondsThisTurn)
      {
         throw OutOfTimeException();
      }
   }
}

