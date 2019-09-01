

#include "BoardTester.h"
#include "ai/Settings.h"
#include "io/Parser.h"
#include "io/Translate.h"
#include "io/Debug.h"
#include "io/Error.h"
#include <sstream>
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Run all the tests
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::RunTests()
{
   test_MoveSequence1();
   test_MoveSequence2();
   test_MoveSequence3();
   test_MoveSequence4();
   test_MoveSequence_SetEnPassant();
   test_MoveSequence_Castle_r1();
   test_MoveSequence_Castle_r2();
   test_MoveSequence_Castle_R1();
   test_MoveSequence_Castle_R2();
   test_Fen0_PawnMoves();
   test_Fen1_PawnMoves();
   test_Fen2_KingMoves();
   test_Fen3_PawnMoves();
   test_Fen4_BishopMoves();
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Execute a series of moves to reproduce this failure...
///           http://vis.siggame.io/?log=http%3A%2F%2Fgame.siggame.io%3A3080%2Fgamelog%2F2018.04.01.04.03.21.840-Chess-Vince_555)
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence1()
{
   static const std::string fen1 = "2bqnk1r/r1p1pp1p/7b/1p1pn1Pp/p3B3/PP2PP2/2PP1NRP/R1B1K3 w Q -"; Action a12("e4", "d3"); // white bishop down-left
   static const std::string fen2 = "2bqnk1r/r1p1pp1p/7b/1p1pn1Pp/p7/PP1BPP2/2PP1NRP/R1B1K3 b Q -";  Action a23("e5", "d3"); // black knight down-left, captures white bishop
   static const std::string fen3 = "2bqnk1r/r1p1pp1p/7b/1p1p2Pp/p7/PP1nPP2/2PP1NRP/R1B1K3 w Q -";   Action a34("f2", "d3"); // white knight up-left, captures black knight
   static const std::string fen4 = "2bqnk1r/r1p1pp1p/7b/1p1p2Pp/p7/PP1NPP2/2PP2RP/R1B1K3 b Q -";    Action a45("f7", "f6"); // black pawn down
   static const std::string fen5 = "2bqnk1r/r1p1p2p/5p1b/1p1p2Pp/p7/PP1NPP2/2PP2RP/R1B1K3 w Q -";   Action a56("d3", "b4"); // white knight up-left (**This is where it stops matching)
   static const std::string fen6 = "2bqnk1r/r1p1p2p/5p1b/1p1p2Pp/pN6/PP2PP2/2PP2RP/R1B1K3 b Q -";   Action a67("f8", "f7"); // black king down
   static const std::string fen7 = "2bqn2r/r1p1pk1p/5p1b/1p1p2Pp/pN6/PP2PP2/2PP2RP/R1B1K3 w Q -";   
   
   Parser::Options options(false, false, false, false, true, false); // swap white bishop 1&2
   
   MyState state(fen1);
   MyState state2(fen2);
   MyState state3(fen3, options);
   MyState state4(fen4, options);
   options.swap_b_pawn[2] += 1;
   options.swap_b_pawn[3] -= 1;
   MyState state5(fen5, options);
   MyState state6(fen6, options);
   MyState state7(fen7, options);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   state.ApplyAction(a23);
   ASSERT_EQ(state.m_BitBoard, state3.m_BitBoard);
   
   state.ApplyAction(a34);
   ASSERT_EQ(state.m_BitBoard, state4.m_BitBoard);
   
   state.ApplyAction(a45);
   ASSERT_EQ(state.m_BitBoard, state5.m_BitBoard);
   
   state.ApplyAction(a56);
   ASSERT_EQ(state.m_BitBoard, state6.m_BitBoard);
   
   state.ApplyAction(a67);
   ASSERT_EQ(state.m_BitBoard, state7.m_BitBoard);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Execute a series of moves to reproduce this failure...
///           http://vis.siggame.io/?log=http%3A%2F%2Fgame.siggame.io%3A3080%2Fgamelog%2F2018.04.03.00.23.40.195-Chess-Vince_691
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence2()
{
   const std::string fen1 = "5k2/2r2bb1/8/7K/p7/P3N3/2p3rn/R7 w - -"; Action a12("h5", "h4"); // white king down
   const std::string fen2 = "5k2/2r2bb1/8/8/p6K/P3N3/2p3rn/R7 b - -";
   
   // Settings::Instance().verbose = true;
   
   MyState state(fen1);
   MyState state2(fen2);
   
   std::set<Action, std::greater<Action> > actions;
   std::set<Action, std::greater<Action> > actions2 = GetActions(state2);
   
   state.ApplyAction(a12);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions2);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  After the last move there was an std::exception ...
///           http://vis.siggame.io/?log=http%3A%2F%2Fgame.siggame.io%3A3080%2Fgamelog%2F2018.04.03.03.01.31.613-Chess-Vince_763
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence3()
{
   const std::string fen1 = "r1b2br1/p1N3pp/3k3n/Ppp1N2P/2P1n2K/1P4PR/8/7q b - -";   Action a12("g7", "g5"); // black pawn down
   const std::string fen2 = "r1b2br1/p1N4p/3k3n/Ppp1N1pP/2P1n2K/1P4PR/8/7q w - g6";
   
   // Settings::Instance().verbose = true;
   
   MyState state(fen1);
   MyState state2(fen2);
   
   std::set<Action, std::greater<Action> > actions;
   std::set<Action, std::greater<Action> > actions2 = GetActions(state2);
   
   state.ApplyAction(a12);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions2);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  The last action taken is invalid...
///           http://vis.siggame.io/?log=http%3A%2F%2Fgame.siggame.io%3A3080%2Fgamelog%2F2018.04.08.18.06.32.712-Chess-Vince_968
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence4()
{
   const std::string fen1 = "r6r/p1p1k1pp/2p2P2/3p4/1P2P1b1/8/PP3PPP/R1B1K2R b KQ - 0 1"; Action a12("e7", "f7"); // Black king right
   const std::string fen2 = "r6r/p1p2kpp/2p2P2/3p4/1P2P1b1/8/PP3PPP/R1B1K2R w KQ - 1 2";  Action a23("f2", "f4"); // White pawn up 2
   const std::string fen3 = "r6r/p1p2kpp/2p2P2/3p4/1P2PPb1/8/PP4PP/R1B1K2R b KQ f3 0 2";  Action a34("a8", "e8"); // Black rook right
   const std::string fen4 = "4r2r/p1p2kpp/2p2P2/3p4/1P2PPb1/8/PP4PP/R1B1K2R w KQ - 1 3";  Action a45("f6", "g7"); // White pawn up-right (captures black pawn)
   const std::string fen5 = "4r2r/p1p2kPp/2p5/3p4/1P2PPb1/8/PP4PP/R1B1K2R b KQ - 0 3";    Action a56("a7", "a5"); // Black pawn down 2
   const std::string fen6 = "4r2r/2p2kPp/2p5/p2p4/1P2PPb1/8/PP4PP/R1B1K2R w KQ a6 0 4";   Action a67("g7", "f8", "q"); // Illegal action
   
   // Settings::Instance().verbose = true;
   
   MyState state(fen1);
   MyState state2(fen2);
   MyState state3(fen3);
   MyState state4(fen4);
   MyState state5(fen5);
   MyState state6(fen6);
   
   std::set<Action, std::greater<Action> > actions;
   std::set<Action, std::greater<Action> > actions2 = GetActions(state2);
   std::set<Action, std::greater<Action> > actions3 = GetActions(state3);
   std::set<Action, std::greater<Action> > actions4 = GetActions(state4);
   std::set<Action, std::greater<Action> > actions5 = GetActions(state5);
   std::set<Action, std::greater<Action> > actions6 = GetActions(state6);
   
   state.ApplyAction(a12);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions2);
   
   state.ApplyAction(a23);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions3);
   
   state.ApplyAction(a34);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions4);
   
   state.ApplyAction(a45);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions5);
   
   state.ApplyAction(a56);
   actions = GetActions(state);
   ASSERT_EQUAL_ACTIONS(actions, actions6);
   
   ASSERT(actions.find(a67) == actions.end());
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Confirm state matches after applying en passsant
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence_SetEnPassant()
{
   const std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";   Action a12("a2", "a4"); // white pawn up
   const std::string fen2 = "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3";
   
   MyState state(fen1);
   MyState state2(fen2);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Confirm state matches after castling with black rook 1
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence_Castle_r1()
{
   const std::string fen1 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq -"; Action a12("e8", "c8"); // black king castle left
   const std::string fen2 = "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ -";
   
   MyState state(fen1);
   MyState state2(fen2);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   Settings::Instance().verbose = false;
}

////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Confirm state matches after castling with black rook 2
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence_Castle_r2()
{
   const std::string fen1 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq -"; Action a12("e8", "g8"); // black king castle right
   const std::string fen2 = "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ -";
   
   MyState state(fen1);
   MyState state2(fen2);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Confirm state matches after castling with white rook 1
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence_Castle_R1()
{
   const std::string fen1 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq -"; Action a12("e1", "c1"); // white king castle left
   const std::string fen2 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq -";
   
   MyState state(fen1);
   MyState state2(fen2);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Confirm state matches after castling with white rook 2
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_MoveSequence_Castle_R2()
{
   const std::string fen1 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq -"; Action a12("e1", "g1"); // white king castle left
   const std::string fen2 = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq -";
   
   MyState state(fen1);
   MyState state2(fen2);
   
   // Settings::Instance().verbose = true;
   
   state.ApplyAction(a12);
   ASSERT_EQ(state.m_BitBoard, state2.m_BitBoard);
   
   Settings::Instance().verbose = false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Expect the bottom-left black pawn to not move to a1
///           http://vis.siggame.io/?log=http%3A%2F%2Fgame.siggame.io%3A3080%2Fgamelog%2F2018.04.03.00.32.59.744-Chess-Vince_721
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_Fen0_PawnMoves()
{
   static const std::string FEN = "2Rb2k1/B7/2B5/2P5/P7/8/1p1K4/6N1 b - -";
   
   MyState state(FEN);
   std::set<Action, std::greater<Action> > actions = GetActions(state);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a later-game fen (just a few pieces) and check:
///           - The collection of parsed pieces
///           - The move masks for all the pieces
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_Fen1_PawnMoves()
{
   static const std::string FEN = "8/3Pk3/8/3R4/N2B3P/7K/8/3r4 w KQkq -";
   
   MyState state(FEN);
   std::set<Action, std::greater<Action> > actions = GetActions(state);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a later-game fen (just a few pieces) and check:
///           - The collection of parsed pieces
///           - The move masks for all the pieces
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_Fen2_KingMoves()
{
   static const std::string FEN = "B7/6k1/8/5P2/8/1B1KB3/8/8 b - -";
   
   MyState state(FEN);
   std::set<Action, std::greater<Action> > actions = GetActions(state);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a later-game fen (just a few pieces) and check:
///           - The collection of parsed pieces
///           - The move masks for all the pieces
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_Fen3_PawnMoves()
{
   static const std::string FEN = "8/5k2/8/8/8/2b5/3P4/4K3 w - -";
   
   MyState state(FEN);
   std::set<Action, std::greater<Action> > actions = GetActions(state);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Get a fen and check:
///           - The collection of parsed pieces
///           - The move masks for all the pieces
///
////////////////////////////////////////////////////////////////////////////////
void BoardTester::test_Fen4_BishopMoves()
{
   static const std::string FEN = "r1Q1kr2/3bp3/2B2p1b/pN4Pp/P2p4/n1pP4/RP1B3P/4KRN1 b - -";
   
   MyState state(FEN);
   std::set<Action, std::greater<Action> > actions = GetActions(state);
}


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Seed the static board with the state's bit board then retrieve
///           actions
///
////////////////////////////////////////////////////////////////////////////////
std::set<Action, std::greater<Action> > BoardTester::GetActions(const MyState& state)
{
   std::set<Action, std::greater<Action> > actions;
   MyState::s_Board.GetPiecesAndMasks(state.m_BitBoard.array);
   MyState::s_Board.GetTurnPlayerMoves(actions);
   return actions;
}

