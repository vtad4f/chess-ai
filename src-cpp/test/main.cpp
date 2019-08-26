

#include "tests/BitBoardTester.h"
#include "tests/BoardTester.h"
#include "tests/ParserTester.h"
#include "tests/TranslateTester.h"
#include "ai/Settings.h"
#include "board/BitBoard.h"
#include "io/Error.h"
#include <cstdlib>
#include <ctime>

#include <iostream>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Main execution for test utility
///
////////////////////////////////////////////////////////////////////////////////
int main()
{
   srand(time(NULL));
   try
   {
      Settings::Instance().test = true;
      TranslateTester::RunTests();
      BitBoardTester::RunTests();
      ParserTester::RunTests();
      BoardTester::RunTests();
      std::cout << "SUCCESS - All tests passed." << std::endl;
   }
   catch (const Error& e)
   {
      std::cerr << e.what() << std::endl;
      return 1;
   }
   return 0;
}

