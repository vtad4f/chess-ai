#pragma once

#include "State.h"
#include "Action.h"
#include <queue>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  A node used in AI searches, to store a particular state and
///           associated action / parent node
/// 
////////////////////////////////////////////////////////////////////////////////
class MyNode
{
public:
   explicit MyNode(const MyNode& other);
   explicit MyNode(const State& state);
   MyNode(const State& state, const MyNode* parent, const Action& action);
   MyNode& operator ++();
   
   void GetSuccessors(std::vector<MyNode>& nodes);
   void BackTrace(std::deque<MyNode>& nodes) const;
   
   const State& GetState() const;
   const MyNode* GetParent() const;
   const Action& GetAction() const;
   int Depth() const;
   int MaterialValueDelta() const;
   int NumMovesDelta() const;
   int Sign() const;
   
protected:
   State       m_State;
   const MyNode* m_pParent;
   Action      m_Action;
   int         m_Depth;
   int         m_MaterialValueDelta;
   int         m_NumMovesDelta;
};

