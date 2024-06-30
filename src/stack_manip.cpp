#include "stack_manip.hpp"

#include <core.hpp>
#include <env.hpp>
#include <utils.hpp>

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <array>

namespace {

enum class StackOp
{
  PUSH,
  DUPLICATE,
  SWAP,
  POP
};

static constexpr std::string_view PUSH_TOKEN(" ");
static constexpr std::string_view DUPLICATE_TOKEN("\n ");
static constexpr std::string_view SWAP_TOKEN("\n\t");
static constexpr std::string_view POP_TOKEN("\n\n");

static constexpr std::string_view PUSH_TAG("[PUSH]");
static constexpr std::string_view DUPLICATE_TAG("[DUPLICATE]");
static constexpr std::string_view SWAP_TAG("[SWAP]");
static constexpr std::string_view POP_TAG("[POP]");

std::optional<std::size_t> push_op(const int64_t num)
{
  data::stack_push(num);
  return std::nullopt;
}

std::optional<std::size_t> duplicate_op()
{
  data::stack_push(data::stack_top());
  return std::nullopt;
}

std::optional<std::size_t> swap_op()
{
  const int64_t old_top = data::stack_pop();
  const int64_t new_top = data::stack_pop();
  data::stack_push(old_top);
  data::stack_push(new_top);
  return std::nullopt;
}

std::optional<std::size_t> pop_op()
{
  data::stack_pop();
  return std::nullopt;
}

InstructionParseResult get_push_fn(const std::string& script,
                                   const std::size_t  program_counter,
                                   const std::size_t)
{
  const auto [num, num_size] = parse_int(script, program_counter);
  return {
      .instruction_fn = std::bind(push_op, num),
      .argument_size  = num_size,
  };
}

InstructionParseResult get_duplicate_fn(const std::string&,
                                        const std::size_t,
                                        const std::size_t)
{
  return {
      .instruction_fn = duplicate_op,
  };
}

InstructionParseResult get_swap_fn(const std::string&,
                                   const std::size_t,
                                   const std::size_t)
{
  return {
      .instruction_fn = swap_op,
  };
}

InstructionParseResult get_pop_fn(const std::string&,
                                  const std::size_t,
                                  const std::size_t)
{
  return {
      .instruction_fn = pop_op,
  };
}

template<StackOp op>
constexpr InstructionParsingData generate_op_data()
{
  if constexpr (op == StackOp::PUSH)
  {
    return {
        .token              = PUSH_TOKEN,
        .tag                = PUSH_TAG,
        .instruction_gen_fn = get_push_fn,
    };
  }
  else if constexpr (op == StackOp::DUPLICATE)
  {
    return {
        .token              = DUPLICATE_TOKEN,
        .tag                = DUPLICATE_TAG,
        .instruction_gen_fn = get_duplicate_fn,
    };
  }
  else if constexpr (op == StackOp::SWAP)
  {
    return {
        .token              = SWAP_TOKEN,
        .tag                = SWAP_TAG,
        .instruction_gen_fn = get_swap_fn,
    };
  }
  else if constexpr (op == StackOp::POP)
  {
    return {
        .token              = POP_TOKEN,
        .tag                = POP_TAG,
        .instruction_gen_fn = get_pop_fn,
    };
  }
}

static constexpr std::array<const InstructionParsingData, 4> operations_data{
    generate_op_data<StackOp::PUSH>(),
    generate_op_data<StackOp::DUPLICATE>(),
    generate_op_data<StackOp::SWAP>(),
    generate_op_data<StackOp::POP>(),
};

} // namespace

namespace stack_manip {
std::span<const InstructionParsingData> get_operations()
{
  return operations_data;
}
} // namespace stack_manip