#include "flow_control.hpp"

#include <core.hpp>
#include <program_state.hpp>
#include <utils.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace {

enum class FlowOp
{
  MARK,
  CALL,
  JUMP,
  JUMP_IF_ZERO,
  JUMP_IF_NEG,
  RETURN,
  EXIT
};

static constexpr std::string_view MARK_TOKEN("  ");
static constexpr std::string_view CALL_TOKEN(" \t");
static constexpr std::string_view JUMP_TOKEN(" \n");
static constexpr std::string_view JUMP_IF_ZERO_TOKEN("\t ");
static constexpr std::string_view JUMP_IF_NEG_TOKEN("\t\t");
static constexpr std::string_view RETURN_TOKEN("\t\n");
static constexpr std::string_view EXIT_TOKEN("\n\n");

static constexpr std::string_view MARK_TAG("[MARK]");
static constexpr std::string_view CALL_TAG("[CALL]");
static constexpr std::string_view JUMP_TAG("[JUMP]");
static constexpr std::string_view JUMP_IF_ZERO_TAG("[JUMP IF ZERO]");
static constexpr std::string_view JUMP_IF_NEG_TAG("[JUMP IF NEGATIVE]");
static constexpr std::string_view RETURN_TAG("[RETURN]");
static constexpr std::string_view EXIT_TAG("[EXIT]");

std::optional<std::size_t> mark_op(const int64_t label, const std::size_t instruction_index, ProgramState& state)
{
  state.instruction_memory.label_add(label, instruction_index);
  return std::nullopt;
}

std::optional<std::size_t> call_op(const int64_t label, const std::size_t instruction_index, ProgramState& state)
{
  state.instruction_memory.return_address_push(instruction_index + 1);
  return state.instruction_memory.label_get(label);
}

std::optional<std::size_t> jump_op(const int64_t label, ProgramState& state)
{
  return state.instruction_memory.label_get(label);
}

std::optional<std::size_t> jump_if_zero_op(const int64_t label, ProgramState& state)
{
  if (state.data_memory.stack_pop() != 0)
  {
    return std::nullopt;
  }
  return state.instruction_memory.label_get(label);
}

std::optional<std::size_t> jump_if_neg_op(const int64_t label, ProgramState& state)
{
  if (state.data_memory.stack_pop() >= 0)
  {
    return std::nullopt;
  }
  return state.instruction_memory.label_get(label);
}

std::optional<std::size_t> return_op(ProgramState& state)
{
  return state.instruction_memory.return_address_pop();
}

std::optional<std::size_t> exit_op(ProgramState& state)
{
  state.set_terminated();
  return -1;
}

InstructionParseResult get_mark_fn(const std::string& script,
                                   const std::size_t  script_index,
                                   const std::size_t  instruction_index)
{
  const auto [label, label_size] = parse_int(script, script_index);
  return {
      .instruction_fn = std::bind(mark_op, label, instruction_index, std::placeholders::_1),
      .argument_size  = label_size,
  };
}

InstructionParseResult get_call_fn(const std::string& script,
                                   const std::size_t  index,
                                   const std::size_t  instruction_index)
{
  const auto [label, label_size] = parse_int(script, index);
  return {
      .instruction_fn = std::bind(call_op, label, instruction_index, std::placeholders::_1),
      .argument_size  = label_size,
  };
}

InstructionParseResult get_jump_fn(const std::string& script, const std::size_t index, const std::size_t)
{
  const auto [label, label_size] = parse_int(script, index);
  return {
      .instruction_fn = std::bind(jump_op, label, std::placeholders::_1),
      .argument_size  = label_size,
  };
}

InstructionParseResult get_jump_if_zero_fn(const std::string& script, const std::size_t index, const std::size_t)
{
  const auto [label, label_size] = parse_int(script, index);
  return {
      .instruction_fn = std::bind(jump_if_zero_op, label, std::placeholders::_1),
      .argument_size  = label_size,
  };
}

InstructionParseResult get_jump_if_negative_fn(const std::string& script, const std::size_t index, const std::size_t)
{
  const auto [label, label_size] = parse_int(script, index);
  return {
      .instruction_fn = std::bind(jump_if_neg_op, label, std::placeholders::_1),
      .argument_size  = label_size,
  };
}

InstructionParseResult get_return_fn(const std::string&, const std::size_t, const std::size_t)
{
  return {
      .instruction_fn = return_op,
  };
}

InstructionParseResult get_exit_fn(const std::string&, const std::size_t, const std::size_t)
{
  return {
      .instruction_fn = exit_op,
  };
}

template<FlowOp op>
constexpr InstructionParsingData generate_op_data()
{
  if constexpr (op == FlowOp::MARK)
  {
    return {
        .token              = MARK_TOKEN,
        .tag                = MARK_TAG,
        .instruction_gen_fn = get_mark_fn,
    };
  }
  else if constexpr (op == FlowOp::CALL)
  {
    return {
        .token              = CALL_TOKEN,
        .tag                = CALL_TAG,
        .instruction_gen_fn = get_call_fn,
    };
  }
  else if constexpr (op == FlowOp::JUMP)
  {
    return {
        .token              = JUMP_TOKEN,
        .tag                = JUMP_TAG,
        .instruction_gen_fn = get_jump_fn,
    };
  }
  else if constexpr (op == FlowOp::JUMP_IF_ZERO)
  {
    return {
        .token              = JUMP_IF_ZERO_TOKEN,
        .tag                = JUMP_IF_ZERO_TAG,
        .instruction_gen_fn = get_jump_if_zero_fn,
    };
  }
  else if constexpr (op == FlowOp::JUMP_IF_NEG)
  {
    return {
        .token              = JUMP_IF_NEG_TOKEN,
        .tag                = JUMP_IF_NEG_TAG,
        .instruction_gen_fn = get_jump_if_negative_fn,
    };
  }
  else if constexpr (op == FlowOp::RETURN)
  {
    return {
        .token              = RETURN_TOKEN,
        .tag                = RETURN_TAG,
        .instruction_gen_fn = get_return_fn,
    };
  }
  else if constexpr (op == FlowOp::EXIT)
  {
    return {
        .token              = EXIT_TOKEN,
        .tag                = EXIT_TAG,
        .instruction_gen_fn = get_exit_fn,
    };
  }
}

static constexpr std::array<const InstructionParsingData, 7> operations_data{
    generate_op_data<FlowOp::MARK>(),
    generate_op_data<FlowOp::CALL>(),
    generate_op_data<FlowOp::JUMP>(),
    generate_op_data<FlowOp::JUMP_IF_ZERO>(),
    generate_op_data<FlowOp::JUMP_IF_NEG>(),
    generate_op_data<FlowOp::RETURN>(),
    generate_op_data<FlowOp::EXIT>(),
};

} // namespace

namespace flow_control {
std::span<const InstructionParsingData> get_operations()
{
  return operations_data;
}
} // namespace flow_control