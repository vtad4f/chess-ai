

#include "AiPlayer.h"
#include "AiHelper.h"
#include "Pondering.h"
#include "Timer.h"
#include "Settings.h"
#include "io/Error.h"
#include "io/Debug.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the name of the AI
///
///   @return  The name of the AI
///
////////////////////////////////////////////////////////////////////////////////
std::string AiPlayer::GetName() const
{
   return "VolcanicCounter";
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the name of the AI
///
////////////////////////////////////////////////////////////////////////////////
void AiPlayer::Init()
{
   unsigned int seed = static_cast<unsigned int>(time(NULL));
   std::cerr << "srand(" << seed << ")" << std::endl;
   srand(seed);
   
   // Parse args (TODO)
   static const std::string verboseStr   = ""; // get_setting("verbose");
   static const std::string randomStr    = ""; // get_setting("random");
   static const std::string alphaBStr    = ""; // get_setting("alpha_beta");
   static const std::string htableStr    = ""; // get_setting("history_table");
   static const std::string ponderingStr = ""; // get_setting("pondering");
   static const std::string sLimitStr    = ""; // get_setting("seconds_limit");
   static const std::string qLimitStr    = ""; // get_setting("quiescent");
   static const std::string dLimitStr    = ""; // get_setting("depth_limit");
   static const std::string whichAiStr   = ""; // get_setting("which_ai");
   static const std::string evenOnlyStr  = ""; // get_setting("even_depths_only");
   
   // Initialize settings
   static Settings& settings = Settings::Instance();
   
   static const int verboseInt = verboseStr.empty() ? -1 : std::stoi(verboseStr);
   settings.silent           = (verboseInt == 0);
   settings.verbose          = (verboseInt >= 1);
   settings.very_verbose     = (verboseInt >= 2);
   settings.random           = randomStr.empty()    ?  0 : std::stoi(randomStr);
   settings.alpha_beta       = alphaBStr.empty()    ?  1 : std::stoi(alphaBStr);
   settings.history_table    = htableStr.empty()    ?  1 : std::stoi(htableStr);
   settings.pondering        = ponderingStr.empty() ?  0 : std::stoi(ponderingStr);
   settings.seconds_limit    = sLimitStr.empty()    ? -1 : std::stod(sLimitStr);
   settings.quiescent        = qLimitStr.empty()    ?  2 : std::stoi(qLimitStr);
   settings.max_depth_limit  = dLimitStr.empty()    ?  0 : std::stoi(dLimitStr);
   settings.even_depths_only = evenOnlyStr.empty()  ?  1 : std::stoi(evenOnlyStr);
   
   settings.min_depth_limit = 2; // Must exceed this before a move can run out of time
   settings.test = false; // Set to true when unit testing
   
   // Validate settings
   try
   {
     settings.Validate();
     
     switch(whichAiStr.empty() ? 2 : std::stoi(whichAiStr))
     {
     case 1: AiHelper::s_Heuristic = AiHelper::LegacyHeuristic; break;
     case 2: AiHelper::s_Heuristic = AiHelper::GoodHeuristic; break;
     default: EXIT("Unknown case"); break;
     }
   }
   catch (const Error& e)
   {
      std::cerr << e.what() << std::endl;
      std::exit(1);
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Called when the game ends
///
////////////////////////////////////////////////////////////////////////////////
void AiPlayer::GameOver()
{
   std::cerr << "ended!" << std::endl;
   Pondering::Instance().Stop();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Called when it's your turn
///
////////////////////////////////////////////////////////////////////////////////
void AiPlayer::MyTurn(const std::string& fen, double seconds_limit)
{
   try
   {
      // Stop pondering
      Pondering::Instance().Stop();
      
      // Get the initial state and apply opponent's move
      static State state(fen);
      _RefreshState(state, fen);
      
      // Reset time limit
      static Settings& settings = Settings::Instance();
      settings.seconds_limit = seconds_limit;
      Timer::Instance().Restart();
      
      // Pick a move to make
      Action action = settings.random ? AiHelper::Random(state) : AiHelper::ID_DL_MiniMax(state);
      debug::PrintAction(action);
      
      // Apply the move
      state.ApplyAction(action, true);
      Pondering::Instance().Start(state); // Start pondering
      // _ApplyActionToGame(action); // Apply my move
   }
   catch (const Error& e)
   {
      std::cerr << e.what() << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Refresh the game state with a new fen string
///
///   @param state  This is the state to apply the fen to
///
////////////////////////////////////////////////////////////////////////////////
void AiPlayer::_RefreshState(State& state, const std::string& fen)
{
   static bool beforeFirstMove = true;
   static std::string last_fen;
   if (!beforeFirstMove)
   {
      ASSERT_NE(last_fen, fen);
      state.Refresh(fen);
   }
   if (beforeFirstMove)
   {
      beforeFirstMove = false;
   }
   last_fen = fen;
}

