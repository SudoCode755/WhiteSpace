#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(FLOW, JUMP_FORWARD)
{
  const std::string script = "   \t\n"     // 1
                             "\n \n\t\t\n" // Jump to \t\t
                             "   \t \n"    // 2
                             "\n  \t\t\n"  // MARK \t\t
                             "\t\n \t"     // int out
                             "\n\n\n";     // EXIT
  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "1");
}

TEST(FLOW, JUMP_IF_ZERO)
{
  const std::string script = "   \t \n"      // 2
                             "\n  \t\t\n"    // MARK \t\t
                             " \n "          // duplicate
                             "\t\n \t"       // int out
                             "   \t\n"       // 1
                             "\t  \t"        // SUB
                             " \n "          // duplicate
                             "\n\t \t\t\t\n" // Jump to \t\t\t if top is 0
                             "\n \n\t\t\n"   // Jump to \t\t
                             "\n  \t\t\t\n"  // MARK \t\t\t
                             "\t\n \t"       // int out
                             "\n\n\n";       // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "210");
}

TEST(FLOW, JUMP_IF_NEGATIVE)
{
  const std::string script = "   \t \n"       // 2
                             "\n  \t\t\n"     // MARK \t\t
                             " \n "           // duplicate
                             "\t\n \t"        // int out
                             "   \t\n"        // 1
                             "\t  \t"         // SUB
                             " \n "           // duplicate
                             "\n\t\t\t\t\t\n" // Jump to \t\t\t if top < 0
                             "\n \n\t\t\n"    // Jump to \t\t
                             "\n  \t\t\t\n"   // MARK \t\t\t
                             "\n\n\n";        // EXIT

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "210");
}

TEST(FLOW, SUBROUTINE)
{
  const std::string script = "\n \t\t\t\t\n"  // CALL \t\t\t
                             "   \t\n"        // 1
                             "\t\n \t"        // int out
                             "\n\n\n"         // EXIT
                             "\n  \t\t\t\n"   // MARK \t\t\t
                             "   \t  \t   \n" // 72
                             "\t\n \t"        // int out
                             "\n\t\n";        // RETURN

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);

  ASSERT_EQ(output.str(), "721");
}

TEST(FLOW, JUMP_LABEL_NOT_FOUND)
{
  const std::string script = "\n \n\t\t\n" // Jump to \t\t
                             "   \t \n"    // 2
                             "\t\n \t"     // int out
                             "\n\n\n";     // EXIT

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
          EXPECT_STREQ("[FLOW CONTROL][JUMP] executing instruction 0: label not found", e.what());
          throw;
        }
      },
      std::runtime_error);
}

TEST(FLOW, DOUBLE_LABEL)
{
  const std::string script = "\n  \t\t\n"
                             "   \t \n"    // 2
                             "\n\n\n"      // EXIT  // MARK \t\t
                             "\n  \t\t\n"; // MARK \t\t

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
          EXPECT_STREQ("[FLOW CONTROL][MARK] parsing instruction 3: same label used twice", e.what());
          throw;
        }
      },
      std::runtime_error);
}

TEST(FLOW, NO_SUBROUTINE)
{
  const std::string script = "\n\t\n"; // RETURN

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
          EXPECT_STREQ("[FLOW CONTROL][RETURN] executing instruction 0: not in subroutine", e.what());
          throw;
        }
      },
      std::runtime_error);
}