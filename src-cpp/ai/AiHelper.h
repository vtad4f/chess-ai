#pragma once

#include "Node.h"
#include "HeuristicValue.h"
#include <functional>
#include <queue>
#include <utility> // std::pair


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Since there is already an AI class defined, this is where I will
///           place all of my AI code (to be called from the other class)
///
////////////////////////////////////////////////////////////////////////////////
class AiHelper
{
public:
   static HVal LegacyHeuristic(const MyNode& node);
   static HVal GoodHeuristic(const MyNode& node);
   static std::function<HVal(const MyNode&)> s_Heuristic;
   
   static Action Random(const State& state);
   static Action ID_DL_MiniMax(const State& state, int L = MIN_DEPTH_LIMIT);
   
protected:
   static constexpr int MIN_DEPTH_LIMIT = 1;
   
   static std::pair<HVal, Action> GetMaxAction_Root(MyNode& node, HVal alpha, HVal beta);
   static std::pair<HVal, Action> GetMaxAction(MyNode& node, HVal alpha, HVal beta);
   static std::pair<HVal, Action> GetMinAction(MyNode& node, HVal alpha, HVal beta);
   
   static HVal GetMaxActionWrapper(MyNode& node, HVal alpha, HVal beta);
   static HVal GetMinActionWrapper(MyNode& node, HVal alpha, HVal beta);
   
   static void MaybeQuitEarly();
   static bool AtDepthLimit(const MyNode& node);
   static bool Quiescent(const Action& action);
   static int NonQDepthLimit();
   
   static void DebugPrint(const MyNode& node,
                          const std::map<HVal, std::vector<MyNode*> >& sorted,
                          const std::pair<HVal, MyNode*>& minmax);
   static bool ShouldPrint(const MyNode& node);
   
   static int s_DepthLimit;
   static std::pair<HVal, Action> s_BestAction;
   static std::deque<Action> s_LastTwoMoves;
};

