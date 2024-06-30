#pragma once

#include "core.fwd.hpp"

struct InstructionParsingData
{
  std::string_view     token;
  std::string_view     tag;
  InstructionGenerator instruction_gen_fn;
};

struct InstructionParseResult
{
  InstructionCallable instruction_fn;
  std::size_t         argument_size = 0;
};