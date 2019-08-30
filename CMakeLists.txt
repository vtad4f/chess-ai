

cmake_minimum_required(VERSION 3.14)

set (SRC
   ai/Action.cpp
   ai/Action.h
   ai/AiHelper.cpp
   ai/AiHelper.h
   ai/AiPlayer.cpp
   ai/AiPlayer.h
   ai/HeuristicValue.cpp
   ai/HeuristicValue.h
   ai/HistoryTable.cpp
   ai/HistoryTable.h
   ai/Node.cpp
   ai/Node.h
   ai/Pondering.cpp
   ai/Pondering.h
   ai/Settings.cpp
   ai/Settings.h
   ai/State.cpp
   ai/State.h
   ai/TerminalException.cpp
   ai/TerminalException.h
   ai/Timer.cpp
   ai/Timer.h
   
   board/BitBoard.cpp
   board/BitBoard.h
   board/Board.cpp
   board/Board.h
   
   io/Debug.cpp
   io/Debug.h
   io/Error.cpp
   io/Error.h
   io/Parser.cpp
   io/Parser.h
   io/Translate.cpp
   io/Translate.h
   
   pieces/Bishop.cpp
   pieces/Bishop.h
   pieces/King.cpp
   pieces/King.h
   pieces/Knight.cpp
   pieces/Knight.h
   pieces/Pawn.cpp
   pieces/Pawn.h
   pieces/Piece.cpp
   pieces/Piece.h
   pieces/Queen.cpp
   pieces/Queen.h
   pieces/Rook.cpp
   pieces/Rook.h
   
   # test/BitBoardTester.cpp
   # test/BitBoardTester.h
   # test/BoardTester.cpp
   # test/BoardTester.h
   # test/main.cpp
   # test/ParserTester.cpp
   # test/ParserTester.h
   # test/TranslateTester.cpp
   # test/TranslateTester.h
   
   main.cpp
)

set (proj chess-ai)
project(${proj})
add_executable(${proj} ${SRC})
include_directories(${proj} . )
set_target_properties(${proj} PROPERTIES COMPILE_OPTIONS "-Wall" "--std=c++11" "-g")

