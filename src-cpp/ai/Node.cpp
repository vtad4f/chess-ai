

#include "Node.h"
#include "HistoryTable.h"
#include "Settings.h"
#include "io/Error.h"
#include "io/Debug.h"


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Copy constructor
///        
///   @param other  The other node
/// 
////////////////////////////////////////////////////////////////////////////////
MyNode::MyNode(const MyNode& other)
   : m_State(other.m_State)
   , m_pParent(other.m_pParent)
   , m_Action(other.m_Action)
   , m_Depth(other.m_Depth)
   , m_MaterialValueDelta(other.m_MaterialValueDelta)
   , m_NumMovesDelta(other.m_NumMovesDelta)
{

}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Construct a node that still has its parent's state
///        
///   @param state  The initial state
/// 
////////////////////////////////////////////////////////////////////////////////
MyNode::MyNode(const State& state)
   : m_State(state)
   , m_pParent(NULL)
   , m_Action()
   , m_Depth(0)
   , m_MaterialValueDelta(0)
   , m_NumMovesDelta(0)
{

}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Construct a node that still has its parent's state
///        
///   @param state  The parent state
///   @param parent  A reference to the parent node
///   @param action  Apply this to the parent state to actuate
/// 
////////////////////////////////////////////////////////////////////////////////
MyNode::MyNode(const State& state, const MyNode* parent, const Action& action)
   : m_State(state)
   , m_pParent(parent)
   , m_Action(action)
   , m_Depth(parent ? parent->m_Depth + 1: 0)
   , m_MaterialValueDelta(parent ? parent->m_MaterialValueDelta : 0)
   , m_NumMovesDelta(parent ? parent->m_NumMovesDelta : 0)
{
   if (m_pParent)
   {
      m_MaterialValueDelta += m_State.ApplyAction(m_Action) * Sign();
   }
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Pre-increment operator 
///           (to update the node's action's history table value)
/// 
////////////////////////////////////////////////////////////////////////////////
MyNode& MyNode::operator ++()
{
   static const Settings& settings = Settings::Instance();
   if (settings.history_table)
   {
      ++HistoryTable::Instance()[&m_Action];
   }
   return *this;
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Check an actuated state for the actions that could be applied
///           to it, then construct nodes from those actions
///                 
///   @param nodes  Populated with the list of successor nodes
/// 
////////////////////////////////////////////////////////////////////////////////
void MyNode::GetSuccessors(std::vector<MyNode>& nodes)
{
   std::set<Action, std::greater<Action> > actions;
   m_State.GetValidActions(actions); // Get actions
   m_NumMovesDelta += static_cast<int>(actions.size()) * -Sign(); // Opposite sign for parent
   for (const Action& action : actions) // Create a node for each action
   {
      try
      {
         nodes.emplace_back(m_State, this, action);
      }
      catch (const Error& e)
      {
         std::deque<MyNode> parents;
         BackTrace(parents);
         for(MyNode& node : parents) { debug::PrintAction(node.GetAction()); }
         debug::PrintAction(action);
         debug::Print(e.what());
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Recursively traverse the parents of this node to retrieve a
///           back-trace of nodes, minus the initial state
///           
///   @param nodes  Populated with the list of nodes leading to and including
///                 the current node, minus the initial state node
///
////////////////////////////////////////////////////////////////////////////////
void MyNode::BackTrace(std::deque<MyNode>& nodes) const
{
   if (!m_pParent)
   {
      return;
   }
   nodes.emplace_front(*this);
   m_pParent->BackTrace(nodes);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Provide public access to the node's state
///
////////////////////////////////////////////////////////////////////////////////
const State& MyNode::GetState() const
{
   return m_State;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Provide public access to the node's parent
///
////////////////////////////////////////////////////////////////////////////////
const MyNode* MyNode::GetParent() const
{
   return m_pParent;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Provide public access to the node's action
///
////////////////////////////////////////////////////////////////////////////////
const Action& MyNode::GetAction() const
{
   return m_Action;
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Get the depth
///
////////////////////////////////////////////////////////////////////////////////
int MyNode::Depth() const
{
   return m_Depth;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get the difference in material value before/after the action
///           was applied for this node
///
///   @return  The material value delta (value of captured piece this turn +
///            new value of promoted piece - 1)
///
////////////////////////////////////////////////////////////////////////////////
int MyNode::MaterialValueDelta() const
{
   return m_MaterialValueDelta;
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Get the number of moves for this player
/// 
////////////////////////////////////////////////////////////////////////////////
int MyNode::NumMovesDelta() const
{
   return m_NumMovesDelta;
}


////////////////////////////////////////////////////////////////////////////////
/// 
///   @brief  Negative if node depth is even
/// 
////////////////////////////////////////////////////////////////////////////////
int MyNode::Sign() const
{
   return (m_Depth % 2 == 0) ? -1 : 1;
}

