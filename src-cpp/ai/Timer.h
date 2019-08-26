#pragma once

#include <chrono>
#include <exception>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This exception is raised if we run out of time for the turn
///
////////////////////////////////////////////////////////////////////////////////
class OutOfTimeException : public std::exception
{
   
};


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This class can is used to measure elapsed time
///
////////////////////////////////////////////////////////////////////////////////
class Timer
{
public:
   static Timer& Instance();
   Timer();
   
   void Restart(double time_remaining_ns = 0.0);
   double Elapsed() const;
   void CheckOutOfTime(int depth);
   
protected:
   double m_SecondsInGame;
   double m_SecondsThisTurn;
   std::chrono::time_point<std::chrono::steady_clock> m_Start;
};

