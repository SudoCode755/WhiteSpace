#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>

TEST(ARITHMETIC, ADD)
{
  const std::string script = "   \t\n"  // 1
                             "   \t \n" // 2
                             "\t   "    // ADD
                             "\t\n \t"  // int out
                             "\n\n\n";  // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "3");
}

TEST(ARITHMETIC, SUB)
{
  const std::string script = "   \t\n"  // 1
                             "   \t \n" // 2
                             "\t  \t"   // SUB
                             "\t\n \t"  // int out
                             "\n\n\n";  // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "-1");
}

TEST(ARITHMETIC, MUL)
{
  const std::string script = "   \t\t\n" // 3
                             "   \t \n"  // 2
                             "\t  \n"    // MUL
                             "\t\n \t"   // int out
                             "\n\n\n";   // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "6");
}

TEST(ARITHMETIC, DIV_NO_TRUNC)
{
  const std::string script = "   \t  \n" // 4
                             "   \t \n"  // 2
                             "\t \t "    // DIV
                             "\t\n \t"   // int out
                             "\n\n\n";   // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "2");
}

TEST(ARITHMETIC, DIV_TRUNC)
{
  const std::string script = "   \t\t\n" // 3
                             "   \t \n"  // 2
                             "\t \t "    // DIV
                             "\t\n \t"   // int out
                             "\n\n\n";   // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "1");
}

TEST(ARITHMETIC, MOD_POS)
{
  const std::string script = "   \t\t\t\n" // 7
                             "   \t\t\n"   // 3
                             "\t \t\t"     // MOD
                             "\t\n \t"     // int out
                             "\n\n\n";     // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "1");
}

TEST(ARITHMETIC, MOD_NEG_FIRST)
{
  const std::string script = "  \t\t\t\t\n" // -7
                             "   \t\t\n"    // 3
                             "\t \t\t"      // MOD
                             "\t\n \t"      // int out
                             "\n\n\n";      // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "2");
}

TEST(ARITHMETIC, MOD_NEG_SECOND)
{
  const std::string script = "   \t\t\t\n" // 7
                             "  \t\t\t\n"  // -3
                             "\t \t\t"     // MOD
                             "\t\n \t"     // int out
                             "\n\n\n";     // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "-2");
}

TEST(ARITHMETIC, MOD_NEG_BOTH)
{
  const std::string script = "  \t\t\t\t\n" // -7
                             "  \t\t\t\n"   // -3
                             "\t \t\t"      // MOD
                             "\t\n \t"      // int out
                             "\n\n\n";      // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "-1");
}

TEST(ARITHMETIC, EMPTY_STACK)
{
  const std::string script = "  \t\t\t\n" // -3
                             "\t \t\t"    // MOD
                             "\t\n \t"    // int out
                             "\n\n\n";    // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);

  EXPECT_THROW(
      {
        try
        {
          run_script(script_in, output);
        }
        catch (const std::runtime_error& e)
        {
          EXPECT_STREQ("[ARITHMETIC][MOD] executing instruction 1: empty stack", e.what());
          throw;
        }
      },
      std::runtime_error);
}
