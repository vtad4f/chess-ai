#pragma once

#include "State.h"
#include "Settings.h"

#include <atomic>
#include <exception>
#include <thread>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This exception will be thrown to stop the pondering thread
///
////////////////////////////////////////////////////////////////////////////////
class DonePonderingException : public std::exception
{
   
};


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This class wraps a thread used for thinking through moves
///           on the opponent's turn
///
////////////////////////////////////////////////////////////////////////////////
class Pondering
{
public:
   static Pondering& Instance();
   
   void Start(State& state);
   void Stop();
   
   bool Running() const;
   void CheckDonePondering() const;
   
protected:
   Pondering();
   static void Run(State* pState);
   
   std::atomic<bool> m_Continue;
   std::atomic<bool> m_Running;
   std::thread m_Thread;
   State* m_pState;
   Settings m_Settings;
};

