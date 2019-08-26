

#include "Pondering.h"
#include "AiHelper.h"
#include "Timer.h"
#include "HistoryTable.h"
#include "io/Error.h"
#include "io/Debug.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Access the static AI Pondering
///
////////////////////////////////////////////////////////////////////////////////
Pondering& Pondering::Instance()
{
   static Pondering pondering;
   return pondering;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Start the thread
///
////////////////////////////////////////////////////////////////////////////////
void Pondering::Start(State& state)
{
   static Settings& settings = Settings::Instance();
   if (settings.pondering && settings.history_table)
   {
      m_Settings = settings; // Backup the settings
      
      // Downgrade each stage of verbosity
      settings.silent = !settings.verbose;
      settings.verbose = settings.very_verbose;
      settings.very_verbose = false;
      
      // Keep looking as long as we can
      settings.max_depth_limit = 0;
      settings.seconds_limit = 0;
      Timer::Instance().Restart();
      
      debug::Print("----------------- Start pondering");
      
      state.SwapTurnPlayer(); // Analyze without the opponent's move
      state.Refresh();
      
      HistoryTable::Instance().Reset();
      
      m_Continue = true;
      m_Running = true;
      m_Thread = std::thread(Run, m_pState = &state); // Start the thread
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Stop the thread
///
////////////////////////////////////////////////////////////////////////////////
void Pondering::Stop()
{
   static Settings& settings = Settings::Instance();
   if (settings.pondering && settings.history_table && m_pState)
   {
      m_Continue = false;
      m_Thread.join(); // Stop the thread
      m_Running = false;
      
      m_pState->SwapTurnPlayer(); // Reverse previous swap
      m_pState->Refresh();
      
      debug::Print("----------------- Stop pondering");
      
      settings = m_Settings; // Restore the settings
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Is the pondering thread running?
///
////////////////////////////////////////////////////////////////////////////////
bool Pondering::Running() const
{
   return m_Running;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If Pondering::Stop has been called after Pondering::Start,
///           this method should throw an exception
///
////////////////////////////////////////////////////////////////////////////////
void Pondering::CheckDonePondering() const
{
   if (!m_Continue && m_Running)
   {
      throw DonePonderingException();
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Constructor
///
////////////////////////////////////////////////////////////////////////////////
Pondering::Pondering()
   : m_Continue(false)
   , m_Running(false)
   , m_Thread()
   , m_pState(nullptr)
   , m_Settings()
{
   
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Wrap ID_DL_MiniMax to catch the exeption raised when we are done
///           pondering
///
////////////////////////////////////////////////////////////////////////////////
void Pondering::Run(State* pState)
{
   try
   {
      ASSERT_NE(nullptr, pState);
      AiHelper::ID_DL_MiniMax(*pState);
   }
   catch (const DonePonderingException&)
   {
      // Done!
   }
   catch (const Error& e)
   {
      std::cerr << e.what() << std::endl;
   }
}

