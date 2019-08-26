

#include "AiHelper.h"
#include "Pondering.h"
#include "TerminalException.h"
#include "Timer.h"
#include "Settings.h"
#include "io/Error.h"
#include "io/Debug.h"
#include <cstdlib>
#include <map>
#include <sstream>
#include <vector>


static const HVal TERMINAL_VAL = 1000; // > largest heuristic value
static const HVal ERROR_VAL    = 2000; // > terminal values
static const HVal INFINITE     = 3000; // > all other values

std::function<HVal(const MyNode&)> AiHelper::s_Heuristic = AiHelper::GoodHeuristic;
int AiHelper::s_DepthLimit = 0;
std::pair<HVal, Action> AiHelper::s_BestAction;
std::deque<Action> AiHelper::s_LastTwoMoves;


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  A heuristic to weigh a node by the value of a captured and/or
///           promoted pieces
/// 
////////////////////////////////////////////////////////////////////////////////
HVal AiHelper::LegacyHeuristic(const MyNode& node)
{
   return HVal(node.MaterialValueDelta());
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  A heuristic to weigh a node by (1st) the value of a captured
///           and/or promoted piece this turn and (2nd) the number of moves
///           available
/// 
////////////////////////////////////////////////////////////////////////////////
HVal AiHelper::GoodHeuristic(const MyNode& node)
{
   return HVal(node.MaterialValueDelta(), node.NumMovesDelta());
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Pick a random valid action
///
///   @param state  Get the valid actions for this state
///             
///   @return  A random action for the state
///
////////////////////////////////////////////////////////////////////////////////
Action AiHelper::Random(const State& state)
{
   std::set<Action, std::greater<Action> > actions;
   state.GetValidActions(actions);
   ASSERT(!actions.empty());
   int i = rand() % actions.size();
   const Action* pAction = nullptr;
   for (const Action& action : actions)
   {
      if (i == 0)
      {
         pAction = &action;
      }
      --i;
   }
   ASSERT_NE(nullptr, pAction);
   return *pAction;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Iterative Deepening Depth Limited Mini Max
///
///   @param state  The starting state for the search
///   @param L  The depth limit for the search (++ with recursive call)
///             
///             Starts at 1, because the chess framework will detect same turn
///             game-ending states for us. I.e. this function doesn't get called
///             if the game is over at depth 0.
///             
///   @return  The highest-scoring node. Note that we don't get to the point
///            of returning if a solution is found
///
////////////////////////////////////////////////////////////////////////////////
Action AiHelper::ID_DL_MiniMax(const State& state, int L)
{
   static const Settings& settings = Settings::Instance();
   s_DepthLimit = L;
   debug::Print("depth limit = " + std::to_string(L));
   
   bool outOfTime = false;
   try
   {
      MyNode node(state);
      std::pair<HVal, Action> action = GetMaxAction_Root(node, -INFINITE, INFINITE);
      
      // If we are only using even depths we won't keep the retrieved action
      // unless it is terminal
      if (!settings.even_depths_only || L % 2 == 0 || s_BestAction.first >= TERMINAL_VAL)
      {
         s_BestAction = action;
      }
   }
   catch (const TerminalException&)
   {
      EXIT("Expected at least one action!");
   }
   catch (const OutOfTimeException&)
   {
      outOfTime = true;
      if (L > MIN_DEPTH_LIMIT)
      {
         std::ostringstream oss;
         oss << "Out of time (" << Timer::Instance().Elapsed() << "s)! Taking best action from previous depth";
         debug::Print(oss.str());
      }
      else
      {
         EXIT("Ran out of time before finding any action!");
      }
   }
   
   if (!Pondering::Instance().Running())
   {
      if (outOfTime || s_BestAction.first >= TERMINAL_VAL || (settings.max_depth_limit > 0 && L >= settings.max_depth_limit))
      {
         // Track moves to avoid 3 move repetition draw
         s_LastTwoMoves.push_front(s_BestAction.second);
         if (s_LastTwoMoves.size() > 2)
         {
            s_LastTwoMoves.pop_back();
         }
         return s_BestAction.second;
      }
   }
   return ID_DL_MiniMax(state, L + 1);
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Get the action with the max heuristic value for this depth
///           
///           This root version of GetMaxAction avoids a 3-move repetition
///           draw and quits early if it encounters a terminal state
/// 
////////////////////////////////////////////////////////////////////////////////
std::pair<HVal, Action> AiHelper::GetMaxAction_Root(MyNode& node, HVal alpha, HVal beta)
{
   // Quit if this is as far as we go
   if (AtDepthLimit(node))
   {
      HVal leaf_val = s_Heuristic(node);
      return std::make_pair(leaf_val, node.GetAction());
   }
   
   // Get children
   std::vector<MyNode> successors;
   node.GetSuccessors(successors);
   
   // Check things like how much time we have left
   MaybeQuitEarly();
   
   // Iterate over successor nodes
   std::pair<HVal, MyNode*> max = std::make_pair(-INFINITE, nullptr);
   std::map<HVal, std::vector<MyNode*> > sorted;
   for (MyNode& successor : successors)
   {
      // Filter top-level moves to avoid a 3 move repetition draw
      if (!node.GetParent() && s_LastTwoMoves.size() >= 2 && successors.size() >= 2)
      {
         if (successor.GetAction() == s_LastTwoMoves.back())
         {
            continue;
         }
      }
      
      // Store the value
      HVal rollup = GetMinActionWrapper(successor, alpha, beta);
      if (rollup > max.first)
      {
         max.first = rollup;
         max.second = &successor;
      }
      sorted[rollup].push_back(&successor); // For debug printing
      
      // Quit if we found a terminal state
      if (!Pondering::Instance().Running() && rollup >= TERMINAL_VAL)
      {
         break;
      }
      
      // Alpha beta pruning?
      static const Settings& settings = Settings::Instance();
      if (settings.alpha_beta)
      {
         if (rollup >= beta)
         {
            if (rollup != max.first) { ++successor; } // Update history table
            break; // Fail High - Prune!
         }
         if (rollup > alpha)
         {
            alpha = rollup;
         }
      }
   }
   ASSERT_NE(nullptr, max.second);
   DebugPrint(node, sorted, max);
   ++(*max.second); // Update history table
   return std::make_pair(max.first, max.second->GetAction());
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Get the action with the max heuristic value for this depth
/// 
////////////////////////////////////////////////////////////////////////////////
std::pair<HVal, Action> AiHelper::GetMaxAction(MyNode& node, HVal alpha, HVal beta)
{
   // Quit if this is as far as we go
   if (AtDepthLimit(node))
   {
      HVal leaf_val = s_Heuristic(node);
      return std::make_pair(leaf_val, node.GetAction());
   }
   
   // Get children
   std::vector<MyNode> successors;
   node.GetSuccessors(successors);
   
   // Check things like how much time we have left
   MaybeQuitEarly();
   
   // Iterate over successor nodes
   std::pair<HVal, MyNode*> max = std::make_pair(-INFINITE, nullptr);
   std::map<HVal, std::vector<MyNode*> > sorted;
   for (MyNode& successor : successors)
   {
      // Store the value
      HVal rollup = GetMinActionWrapper(successor, alpha, beta);
      if (rollup > max.first)
      {
         max.first = rollup;
         max.second = &successor;
      }
      sorted[rollup].push_back(&successor); // For debug printing
      
      // Alpha beta pruning?
      static const Settings& settings = Settings::Instance();
      if (settings.alpha_beta)
      {
         if (rollup >= beta)
         {
            if (rollup != max.first) { ++successor; } // Update history table
            break; // Fail High - Prune!
         }
         if (rollup > alpha)
         {
            alpha = rollup;
         }
      }
   }
   ASSERT_NE(nullptr, max.second);
   DebugPrint(node, sorted, max);
   ++(*max.second); // Update history table
   return std::make_pair(max.first, max.second->GetAction());
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  We don't want to just use the GetMaxAction return value as-is.
///           Adjust the sign depending on depth. Account for terminal states.
/// 
////////////////////////////////////////////////////////////////////////////////
HVal AiHelper::GetMaxActionWrapper(MyNode& node, HVal alpha, HVal beta)
{
   try
   {
      return GetMaxAction(node, alpha, beta).first;
   }
   catch (const CheckmateException&)
   {
      return TERMINAL_VAL * node.Sign();
   }
   catch (const StalemateException&)
   {
      return -TERMINAL_VAL * node.Sign();
   }
   catch(const Error& e) // Shouldn't happen
   {
      debug::Print(e.what());
      return -ERROR_VAL * node.Sign();
   }
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Get the action with the max heuristic value for this depth
/// 
////////////////////////////////////////////////////////////////////////////////
std::pair<HVal, Action> AiHelper::GetMinAction(MyNode& node, HVal alpha, HVal beta)
{
   // Quit if this is as far as we go
   if (AtDepthLimit(node))
   {
      HVal leaf_val = s_Heuristic(node);
      return std::make_pair(leaf_val, node.GetAction());
   }
   
   // Get children
   std::vector<MyNode> successors;
   node.GetSuccessors(successors);
   
   // Check things like how much time we have left
   MaybeQuitEarly();
   
   // Iterate over successor nodes
   std::pair<HVal, MyNode*> min = std::make_pair(INFINITE, nullptr);
   std::map<HVal, std::vector<MyNode*> > sorted;
   for (MyNode& successor : successors)
   {
      // Store the value
      HVal rollup = GetMaxActionWrapper(successor, alpha, beta);
      if (rollup < min.first)
      {
         min.first = rollup;
         min.second = &successor;
      }
      sorted[rollup].push_back(&successor); // For debug printing
      
      // Alpha beta pruning?
      static const Settings& settings = Settings::Instance();
      if (settings.alpha_beta)
      {
         if (rollup <= alpha)
         {
            if (rollup != min.first) { ++successor; } // Update history table
            break; // Fail Low - Prune!
         }
         if (rollup < beta)
         {
            beta = rollup;
         }
      }
   }
   ASSERT_NE(nullptr, min.second);
   DebugPrint(node, sorted, min);
   ++(*min.second); // Update history table
   return std::make_pair(min.first, min.second->GetAction());
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  We don't want to just use the GetMinAction return value as-is.
///           Account for terminal states. Adjust the sign depending on depth.
/// 
////////////////////////////////////////////////////////////////////////////////
HVal AiHelper::GetMinActionWrapper(MyNode& node, HVal alpha, HVal beta)
{
   try
   {
      return GetMinAction(node, alpha, beta).first;
   }
   catch (const CheckmateException&)
   {
      return TERMINAL_VAL * node.Sign();
   }
   catch (const StalemateException&)
   {
      return -TERMINAL_VAL * node.Sign();
   }
   catch(const Error& e) // Shouldn't happen
   {
      debug::Print(e.what());
      return -ERROR_VAL * node.Sign();
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If we are pondering or have a time limit, we might need to quit
///           before finishing
///
////////////////////////////////////////////////////////////////////////////////
void AiHelper::MaybeQuitEarly()
{
   static const Settings& settings = Settings::Instance();
   if (s_DepthLimit > settings.min_depth_limit)
   {
      Timer::Instance().CheckOutOfTime(s_DepthLimit);
   }
   Pondering::Instance().CheckDonePondering();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Check the depth limit, and maybe additional depths if we are
///           past the normal limit and in a non-quiescent state
///
////////////////////////////////////////////////////////////////////////////////
bool AiHelper::AtDepthLimit(const MyNode& node)
{
   int depth = node.Depth();
   return (depth >= s_DepthLimit && Quiescent(node.GetAction())) ||
           depth >= NonQDepthLimit();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  We are considering a quiescent state one where there the last
///           action did not involve capturing or promoting a pawn
///
///           We are not looking for check because it is cheaper to consider
///           only the action that got us to this state than to evaluate the
///           state itself
///
////////////////////////////////////////////////////////////////////////////////
bool AiHelper::Quiescent(const Action& action)
{
   return !action.captured && !action.promoted;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the non-quiescent depth limit
///
////////////////////////////////////////////////////////////////////////////////
int AiHelper::NonQDepthLimit()
{
   const Settings& settings = Settings::Instance();
   return s_DepthLimit + settings.quiescent;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Print the node's parent action, its successor actions (sorted)
///           and the the final choice
///
////////////////////////////////////////////////////////////////////////////////
void AiHelper::DebugPrint(const MyNode& node,
                          const std::map<HVal, std::vector<MyNode*> >& sorted,
                          const std::pair<HVal, MyNode*>& minmax)
{
   if (ShouldPrint(node))
   {
      // First print the parent action
      if (node.GetParent())
      {
         debug::PrintAction(node.GetAction(), "parent = ");
      }
      else
      {
         debug::Print("parent = NULL");
      }
      
      // Then the successor actions
      for (auto pair : sorted)
      {
         for (auto node : pair.second)
         {
            debug::PrintAction(node->GetAction(), pair.first);
         }
      }
      
      // Then the successor that was selected
      debug::Print(node.Depth() % 2 == 0 ? "max = " : "min = ");
      debug::PrintAction(minmax.second->GetAction(), minmax.first);
      debug::Print("-------------------------");
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Only print if verbose and this is the search for the current max
///           depth limit.
///
///           In general we only want to print details for the root and its
///           successors, but we make a special exception for very verbose
///           mode with a single turn limit.
///
////////////////////////////////////////////////////////////////////////////////
bool AiHelper::ShouldPrint(const MyNode& node)
{
   static const Settings& settings = Settings::Instance();
   if (settings.verbose && s_DepthLimit == settings.max_depth_limit)
   {
      if (!node.GetParent()) // Usually just care about the root
      {
         return true;
      }
      else if (settings.very_verbose && settings.turn_limit == 1 && settings.max_depth_limit <= 3)
      {
         return true;
      }
   }
   return false;
}

