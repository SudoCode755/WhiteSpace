#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(STACK_MANIP, PUSH)
{
  const std::string script = "   \t  \t   \n" // 72
                             "\t\n \t"        // int out
                             "\n\n\n";

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "72");
}

TEST(STACK_MANIP, DUPLICATE)
{
  const std::string script = "   \t  \t   \n" // 72
                             " \n "           // duplicate
                             "\t\n \t"        // int out
                             "\t\n \t"        // int out
                             "\n\n\n";        // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "7272");
}

TEST(STACK_MANIP, SWAP)
{
  const std::string script = "   \t  \t   \n"   // 72
                             "   \t\t \t\t  \n" // 108
                             " \n\t"            // swap
                             "\t\n \t"          // int out
                             "\t\n \t"          // int out
                             "\n\n\n";          // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "72108");
}

TEST(STACK_MANIP, POP)
{
  const std::string script = "   \t  \t   \n"   // 72
                             "   \t\t \t\t  \n" // 108
                             " \n\n"            // pop
                             "\t\n \t"          // int out
                             "\n\n\n";          // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "72");
}