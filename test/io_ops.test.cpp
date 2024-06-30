#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(IO_OPS, OUT_CHAR)
{
  const std::string script = "   \t  \t   \n" // PUSH 72 = H
                             "\t\n  "         // char out
                             "\n\n\n";

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "H");
}

TEST(IO_OPS, OUT_INT)
{
  const std::string script = "   \t  \t   \n" // PUSH 72 = H
                             "\t\n \t"        // int out
                             "\n\n\n";

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "72");
}

TEST(IO_OPS, IN_CHAR)
{
  const std::string script = "   \t\n" // PUSH 1
                             " \n "    // duplicate
                             "\t\n\t " // read char from input to heap address 1
                             "\t\t\t"  // retrive value from address 1
                             "\t\n  "  // char out
                             "\n\n\n"; // EXIT

  std::istringstream input("H");
  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output, input);

  ASSERT_EQ(output.str(), "H");
}

TEST(IO_OPS, IN_INT)
{
  const std::string script = "   \t\n"  // PUSH 1
                             " \n "     // duplicate
                             "\t\n\t\t" // read char to heap address-1
                             "\t\t\t"   // retrive value from address 1
                             "\t\n \t"  // char out
                             "\n\n\n";  // EXIT

  std::istringstream input("72");
  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output, input);

  ASSERT_EQ(output.str(), "72");
}
