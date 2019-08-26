#!/bin/bash

################################################################################
#
#  @brief  Print with color
#
################################################################################
function ColorPrint
{
   echo -e "\033[0;36m$@ \033[0m";
}

################################################################################
#
#  @brief  Function to find newest file in dir
#
#              https://stackoverflow.com/questions/4561895/how-to-recursively-
#              find-the-latest-modified-file-in-a-directory
#
################################################################################
function NewestInDir
{
   find "$1" -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d' '
}

################################################################################
#
#  @brief  Check for forced arg, build dir missing, or newer src
#
################################################################################
function ShouldMake
{
   if [[ "$@" == *'f'* || ! -d $BUILD_DIR ]]; then
      return $TRUE_
      
   elif [[ "$@" != *'.2'* ]]; then
      
      new_build_file=$(NewestInDir $BUILD_DIR)
      new_src_file=$(NewestInDir $SRC_DIR)
      
      if [[ "$@" == *'u'* ]]; then
         new_test_file=$(NewestInDir $TESTS_DIR)
         [[ $new_build_file -ot $new_src_file || \
            $new_build_file -ot $AI_CPP || \
            $new_build_file -ot $AI_HPP || \
            $new_build_file -ot $new_test_file || \
            $new_build_file -ot $TEST_MAIN ]] && return $TRUE_
      else
         [[ $TEMP_FILE -ot $new_src_file ]] && return $TRUE_
      fi
   fi
   return $FALSE_
}

################################################################################
#
#  @brief  Get session name
#
################################################################################
function GetSessionName
{
   if [[ -f $TEMP_FILE ]]; then # Read from file
      n=$(<$TEMP_FILE)
   else
      n=0
   fi
   session_name="$SESSION_BASE_NAME"_$n
}

################################################################################
#
#  @brief  Set session name
#
################################################################################
function SetNextSession
{
   echo -n $((n + 1)) > $TEMP_FILE # Write
}

################################################################################
#
#  @brief  Run with gdb or just normally
#
################################################################################
function RunCppClient
{
   newFen=$(echo -n "$fen" | sed -e 's/\//%2F/g' | sed -e 's/ /%20/g')
   
   if [[ "$@" == *'g'* ]]; then
      echo "file ./build/cpp-client"
      
      if [[ "$@" == *'.1'* ]]; then
         GetSessionName
         echo "run Chess -s game.siggame.io -r \"$session_name\" --aiSettings \"$ai_cfg\" --gameSettings \"fen=$newFen\""
         
      elif [[ "$@" == *'.2'* ]]; then
         GetSessionName
         SetNextSession
         echo "run Chess -s game.siggame.io -r \"$session_name\" --aiSettings \"$ai_cfg\""
         
      else
         GetSessionName
         SetNextSession
         echo "run Chess -s game.siggame.io -r \"$session_name\" --aiSettings \"$ai_cfg\" --gameSettings \"fen=$newFen\""
         echo "run Chess -s game.siggame.io -r \"$session_name\" --aiSettings \"$ai_cfg\""
      fi
      
      gdb
   else
      if [[ "$@" == *'.1'* ]]; then
         GetSessionName
         ./testRun "$session_name" --aiSettings "$ai_cfg" --gameSettings "fen=$newFen" &
      
      elif [[ "$@" == *'.2'* ]]; then
         GetSessionName
         SetNextSession
         ./testRun "$session_name" --aiSettings "$ai_cfg" &
      
      else
         GetSessionName
         SetNextSession
         ./testRun "$session_name" --aiSettings "$ai_cfg" --gameSettings "fen=$newFen" &
         ./testRun "$session_name" --aiSettings "$ai_cfg" &
      fi
   fi
}

BUILD_DIR='build'
SRC_DIR='games/chess'
TEMP_FILE='.temp'
SESSION_BASE_NAME='Vince'

TRUE_=0
FALSE_=3

# Pick a language
LANG='Joueur.cpp'
ColorPrint "cd $LANG"
cd $LANG

# Custom build?
if [[ "$@" == *'u'* ]]; then
   TEST_DIR='games/chess/my_test'
   ColorPrint "cd $TEST_DIR"
   cd $TEST_DIR
   SRC_DIR='../my_src'
   AI_CPP='../ai.cpp'
   AI_HPP='../ai.hpp'
   TESTS_DIR='tests'
   TEST_MAIN='main.cpp'
fi

# Optional cmd line arg to clean the build directory
if [[ "$@" == *'c'* ]] && [[ -d $BUILD_DIR ]]; then
   ColorPrint "rm -rf $BUILD_DIR"
   rm -rf $BUILD_DIR
fi

# Make if forced, build dir missing, or if newer src
if ShouldMake "$@"; then
   ColorPrint "make"
   make || exit $?
fi

# Maybe run a local test
if [[ "$@" == *'u'* ]]; then
   ColorPrint "./build/cpp-test"
   ./build/cpp-test
   
else # Otherwise run remotely
   
   # Prep AI settings
   verbose=''
   random=''
   alpha_beta=''
   history_table=''
   pondering=''
   seconds_limit=''
   quiescent=''
   depth_limit=''
   decrement_dl=''
   turn_limit=''
   which_ai=''
   even_depths_only=''
   if [[ "$@" == *'v0'*                   ]]; then verbose=0; fi
   if [[ "$@" == *'v1'* || "$@" == *'v'*  ]]; then verbose=1; fi
   if [[ "$@" == *'v2'* || "$@" == *'vv'* ]]; then verbose=2; fi
   if [[ "$@" == *'r'*   ]]; then random=1; fi
   if [[ "$@" == *'ab0'* ]]; then alpha_beta=0; fi
   if [[ "$@" == *'ab1'* ]]; then alpha_beta=1; fi
   if [[ "$@" == *'h0'*  ]]; then history_table=0; fi
   if [[ "$@" == *'h1'*  ]]; then history_table=1; fi
   if [[ "$@" == *'p0'*  ]]; then pondering=0; fi
   if [[ "$@" == *'p1'*  ]]; then pondering=1; fi
   if [[ "$@" == *'s%'*  ]]; then seconds_limit=-1; fi
   if [[ "$@" == *'s0'*  ]]; then seconds_limit=0; fi
   if [[ "$@" == *'s1'*  ]]; then seconds_limit=1; fi
   if [[ "$@" == *'s3'*  ]]; then seconds_limit=3; fi
   if [[ "$@" == *'s5'*  ]]; then seconds_limit=5; fi
   if [[ "$@" == *'s10'* ]]; then seconds_limit=10; fi
   if [[ "$@" == *'s60'* ]]; then seconds_limit=60; fi
   if [[ "$@" == *'q0'*  ]]; then quiescent=0; fi
   if [[ "$@" == *'q1'*  ]]; then quiescent=1; fi
   if [[ "$@" == *'q2'*  ]]; then quiescent=1; fi
   if [[ "$@" == *'d1'*  ]]; then depth_limit=1; fi
   if [[ "$@" == *'d2'*  ]]; then depth_limit=2; fi
   if [[ "$@" == *'d3'*  ]]; then depth_limit=3; fi
   if [[ "$@" == *'d4'*  ]]; then depth_limit=4; fi
   if [[ "$@" == *'d5'*  ]]; then depth_limit=5; fi
   if [[ "$@" == *'d6'*  ]]; then depth_limit=6; fi
   if [[ "$@" == *'d7'*  ]]; then depth_limit=7; fi
   if [[ "$@" == *'d8'*  ]]; then depth_limit=8; fi
   if [[ "$@" == *'d9'*  ]]; then depth_limit=9; fi
   if [[ "$@" == *'d10'* ]]; then depth_limit=10; fi
   if [[ "$@" == *'d11'* ]]; then depth_limit=11; fi
   if [[ "$@" == *'d12'* ]]; then depth_limit=12; fi
   if [[ "$@" == *'d--'* ]]; then decrement_dl=1; turn_limit=$depth_limit; fi
   if [[ "$@" == *'t1'*  ]]; then turn_limit=1; fi
   if [[ "$@" == *'t2'*  ]]; then turn_limit=2; fi
   if [[ "$@" == *'t3'*  ]]; then turn_limit=3; fi
   if [[ "$@" == *'t4'*  ]]; then turn_limit=4; fi
   if [[ "$@" == *'w1'*  ]]; then which_ai=1; fi
   if [[ "$@" == *'w2'*  ]]; then which_ai=2; fi
   if [[ "$@" == *'e0'*  ]]; then even_depths_only=0; fi
   if [[ "$@" == *'e1'*  ]]; then even_depths_only=1; fi
   verbose="verbose=$verbose"
   random="random=$random"
   alpha_beta="alpha_beta=$alpha_beta"
   history_table="history_table=$history_table"
   pondering="pondering=$pondering"
   seconds_limit="seconds_limit=$seconds_limit"
   quiescent="quiescent=$quiescent"
   depth_limit="depth_limit=$depth_limit"
   decrement_dl="decrement_dl=$decrement_dl"
   turn_limit="turn_limit=$turn_limit"
   which_ai="which_ai=$which_ai"
   even_depths_only="even_depths_only=$even_depths_only"
   ai_cfg=$verbose'&'$random'&'$alpha_beta'&'$history_table'&'$pondering'&'$seconds_limit'&'$quiescent'&'$depth_limit'&'$decrement_dl'&'$turn_limit'&'$which_ai'&'$even_depths_only
   
   
   # Get the fen
   fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" # default
   
   if [[ "$@" == *'-1'* ]]; then # en passant
      fen="rnbqkbn1/7r/1p1p3p/pPpPp1pP/P1P1PpP1/5P2/8/RNBQKBNR b KQq e3 0 11"
      
   elif [[ "$@" == *'-2'* ]]; then # castling
      # fen="r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" # all available
      # fen="rn2k1nr/p6p/8/8/8/8/P6P/RN2K1NR b KQkq - 0 1" # no castle moves available
      # fen="r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" # castling
      fen="r3k2r/8/8/4B3/4b3/8/8/R3K2R w KQkq - 0 1" # bishop but still castle
      
   elif [[ "$@" == *'-3'* ]]; then # in check
      # fen="rnbqkbnr/2pppQpp/1p6/p7/8/4P3/PPPP1PPP/RNB1KBNR b - - 0 3" # king must attack queen
      # fen="rnbqkbnr/ppppp1pp/5p2/7Q/8/4P3/PPPP1PPP/RNB1KBNR b - - 1 2" # Pawn must get in queen's way
      # fen="4k3/8/7B/8/8/3rqr2/8/4K3 w - - 0 1" # bishop must take queen
      # fen="4k3/8/8/8/4q3/8/3BKB2/3R1R2 w - - 0 1" # bishop must get in queen's way
      # fen="r1Q1kr2/3bp3/2B2p1b/pN4Pp/P2p4/n1pP4/RP1B3P/4KRN1 b - - 0 1" # bishop can't capture queen
      fen="8/8/1k6/7b/8/6p1/8/3r2K1 w - - 0 1" # king moves up
      
   elif [[ "$@" == *'-4'* ]]; then # not into check
      fen="4k3/4q3/8/8/8/P2p1p2/3PBP2/3PKP2 w - - 0 1"
      
   elif [[ "$@" == *'-5'* ]]; then # game 2 scenarios
      # fen="rn3bn1/1p2k1p1/p4p2/2p5/P5P1/8/RP1q3N/1bB1K2R w K - 0 1" # can't castle now
      # fen="1n1qkbn1/r1ppppp1/1p6/p7/4b1P1/P2B3N/1PPP1P1R/RNBK4 w - - 0 1" # should capture bishop
      # fen="rnbqkb1r/pppp1ppp/4pn2/8/4P3/3P1P2/PPP3PP/RNBQKBNR b KQkq - 0 3" # for max depth > 1, shouldn't capture pawn
      fen="rnb1nb1r/pp1pkppp/2p1p3/q3P1N1/5P2/3P1Q2/PPP3PP/RNB1KB1R w KQkq - 0 1" # get out of check
      
   elif [[ "$@" == *'-6'* ]]; then # game 3 scenarios
      # fen="r1b1k2r/1pp2ppp/2nb1n2/p3p3/2P1p3/P1Q3PP/1PKP1q2/RNB2BNR b kq - 0 1" # max depth 5, shouldn't capture king
      fen="rn2k2r/pp2pp1p/2pp2pb/7n/1q4bP/P1N1PQ2/1PPP1PP1/R1B1KBNR b KQkq - 0 1" # Trade queens? Lots of guarding...
   
   elif [[ "$@" == *'-7'* ]]; then # game 4 scenarios
      # fen="r6r/p1p1k1pp/2p2P2/3p4/1P2P1b1/8/PP3PPP/R1B1K2R b KQ - 0 1" # test w/ depth 6, NumMovesDelta
      fen="8/p4P1k/8/8/4bP2/1P2b3/1K1p2P1/3r4 w - - 0 1"
   fi
   
   # Run
   RunCppClient "$@"
fi

