#include <WhiteSpace.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(WHITESPACE, COUNTING_SAMPLE)
{
  const std::string script = "     \t\n__Put_a_1_on_the_stack"
                             "\n   \t    \t\t\n__Set_a_Label_at_this_point"
                             " \n __Duplicate_the_top_stack_item"
                             "\t\n \t__Output_the_current_value"
                             "   \t \t \n__Put_10_(newline)_on_the_stack..."
                             "\t\n  __...and_output_the_newline"
                             "   \t\n__Put_a_1_on_the_stack"
                             "\t   __Addition._This_increments_our_current_value."
                             " \n __Duplicate_that_value_so_we_can_test_it"
                             "   \t \t\t\n__Push_11_onto_the_stack"
                             "\t  \t__SUB_So_if_we've_reached_the_end,_we_have_a_zero_on_the_stack."
                             "\n\t  \t   \t \t\n__If_we_have_a_zero,_jump_to_the_end"
                             "\n \n \t    \t\t\n__Jump_to_the_start"
                             "\n   \t   \t \t\n__Set_the_end_label"
                             " \n\n__Discard_our_accumulator,_to_be_tidy"
                             "\n\n\n__Finish";

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);
  ASSERT_EQ(output.str(), "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");
}

TEST(WHITESPACE, HELLO_WORLD_WITH_COMMENTS)
{
  const std::string script = "   \t  \t   \n:Push_+1001000=72='H'_onto_the_stack;"
                             "\t\n  :Output_'H';"
                             "   \t\t  \t \t\n:Push_+1100101=101='e'_onto_the_stack;"
                             "\t\n  :Output_'e';"
                             "   \t\t \t\t  \n:+1101100=108='l';"
                             " \n :Duplicate_'l';"
                             "\t\n  :Output_'l';"
                             "\t\n  :Output_'l';"
                             "     \t\t \t\t\t\t\n:+1101111=111='o';"
                             "\t\n  :Output_'o';"
                             "     \t \t\t  \n:+101100=44=','"
                             "\t\n  :Output_',';"
                             "     \t     \n:+100000=32=Space"
                             "\t\n  :Output_'SPACE';"
                             "     \t\t\t \t\t\t\n:+1110111=119='w'"
                             "\t\n  :Output_'w';"
                             "     \t\t \t\t\t\t\n:+1101111=111='o'"
                             "\t\n  :Output_'o';"
                             "     \t\t\t  \t \n:+1110010=114='r'"
                             "\t\n  :Output_'r';"
                             "     \t\t \t\t  \n:+1101100=108='l'"
                             "\t\n  :Output_'l';"
                             "     \t\t  \t  \n=+1100100=100='d'"
                             "\t\n  :Output_'d';"
                             "     \t    \t\n:+100001=33='!'"
                             "\t\n  :Output_'!';"
                             "\n\n\n:End_the_program;";

  std::ostringstream output;
  std::istringstream script_in(script);
  run_script(script_in, output);
  ASSERT_EQ(output.str(), "Hello, world!");
}
