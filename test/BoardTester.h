#pragma once

#include "ai/State.h"
#include "board/Board.h"


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  A class for testing the board
///
////////////////////////////////////////////////////////////////////////////////
class BoardTester
{
public:
   static void RunTests();
   
protected:
   static void test_MoveSequence1();
   static void test_MoveSequence2();
   static void test_MoveSequence3();
   static void test_MoveSequence4();
   static void test_MoveSequence_SetEnPassant();
   static void test_MoveSequence_Castle_r1();
   static void test_MoveSequence_Castle_r2();
   static void test_MoveSequence_Castle_R1();
   static void test_MoveSequence_Castle_R2();
   static void test_Fen0_PawnMoves();
   static void test_Fen1_PawnMoves();
   static void test_Fen2_KingMoves();
   static void test_Fen3_PawnMoves();
   static void test_Fen4_BishopMoves();
   
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///   @brief  Just need access to the protected members;
   ///
   /////////////////////////////////////////////////////////////////////////////
   class MyState : public State
   {
   public:
      MyState(const std::string& fen, const Parser::Options& options = Parser::Options()) : State(fen, options) { }
      using State::s_Board;
      using State::m_BitBoard;
   };
   
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///   @brief  Just need access to the protected members;
   ///
   /////////////////////////////////////////////////////////////////////////////
   class MyBoard : public Board
   {
   public:
      using Board::m_MyPieces;
      using Board::m_TheirPieces;
      using Board::m_Masks;
   };
   
   
   static std::set<Action, std::greater<Action> > GetActions(const MyState& state);
};


#define ASSERT_EQUAL_ACTIONS(actionsA, actionsB) ({ \
   if (actionsA != actionsB)                        \
   {                                                \
      for (auto action : actionsA)                  \
      {                                             \
         debug::PrintAction(action);                \
      }                                             \
      std::cout << "----------------" << std::endl; \
      for (auto action : actionsB)                  \
      {                                             \
         debug::PrintAction(action);                \
      }                                             \
   }                                                \
                                                    \
   ASSERT_EQ(actionsA, actionsB);                   \
})

