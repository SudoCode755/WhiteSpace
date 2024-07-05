#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(HEAP_ACEESS, STORE_RETRIEVE)
{
  const std::string script = "   \t\n"       // push 1
                             "   \t  \t \n"  // push 18
                             "\t\t "         // value 18 is stored at the address 1
                             "   \t\n"       // push 1
                             "\t\t\t"        // retrive value stored at 1 -> 18
                             "\t\n \t"       // int out
                             "   \t\n"       // push 1, stack: 1
                             "   \t\n"       // push 1
                             "   \t  \t\t\n" // push 19
                             "\t\t "         // 19 stored at address 1
                             "   \t\n"       // push 1
                             "\t\t\t"        // retrive value stored at 1 -> 19
                             "\t\n \t"       // int out
                             "\n\n\n";       // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "1819");
}
