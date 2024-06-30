#include "arithmetic.hpp"

#include <core.hpp>
#include <env.hpp>
#include <utils.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <array>

namespace {

enum class ArithmeticOp
{
  ADD,
  SUB,
  MUL,
  DIV,
  MOD
};

static constexpr std::string_view ADD_TOKEN("  ");
static constexpr std::string_view SUB_TOKEN(" \t");
static constexpr std::string_view MUL_TOKEN(" \n");
static constexpr std::string_view DIV_TOKEN("\t ");
static constexpr std::string_view MOD_TOKEN("\t\t");

static constexpr std::string_view ADD_TAG("[ADD]");
static constexpr std::string_view SUB_TAG("[SUB]");
static constexpr std::string_view MUL_TAG("[MUL]");
static constexpr std::string_view DIV_TAG("[DIV]");
static constexpr std::string_view MOD_TAG("[MOD]");

template<ArithmeticOp op>
std::optional<std::size_t> generic_arithmetic_op()
{
  const int64_t second_operand = data::stack_pop();
  const int64_t first_operand  = data::stack_pop();
  if constexpr (op == ArithmeticOp::ADD)
  {
    data::stack_push(first_operand + second_operand);
  }
  else if constexpr (op == ArithmeticOp::SUB)
  {
    data::stack_push(first_operand - second_operand);
  }
  else if constexpr (op == ArithmeticOp::MUL)
  {
    data::stack_push(first_operand * second_operand);
  }
  else if constexpr (op == ArithmeticOp::DIV)
  {
    data::stack_push(first_operand / second_operand);
  }
  else if constexpr (op == ArithmeticOp::MOD)
  {
    const int64_t result = first_operand % second_operand;
    data::stack_push((first_operand * second_operand < 0)
                         ? result + second_operand
                         : result);
  }
  return std::nullopt;
}

template<ArithmeticOp op>
InstructionParseResult get_arithmetic_fn(const std::string&,
                                         const std::size_t,
                                         const std::size_t)
{
  return {
      .instruction_fn = generic_arithmetic_op<op>,
  };
}

template<ArithmeticOp op>
constexpr InstructionParsingData generate_op_data()
{
  if constexpr (op == ArithmeticOp::ADD)
  {
    return {
        .token              = ADD_TOKEN,
        .tag                = ADD_TAG,
        .instruction_gen_fn = get_arithmetic_fn<op>,
    };
  }
  else if constexpr (op == ArithmeticOp::SUB)
  {
    return {
        .token              = SUB_TOKEN,
        .tag                = SUB_TAG,
        .instruction_gen_fn = get_arithmetic_fn<op>,
    };
  }
  else if constexpr (op == ArithmeticOp::MUL)
  {
    return {
        .token              = MUL_TOKEN,
        .tag                = MUL_TAG,
        .instruction_gen_fn = get_arithmetic_fn<op>,
    };
  }
  else if constexpr (op == ArithmeticOp::DIV)
  {
    return {
        .token              = DIV_TOKEN,
        .tag                = DIV_TAG,
        .instruction_gen_fn = get_arithmetic_fn<op>,
    };
  }
  else if constexpr (op == ArithmeticOp::MOD)
  {
    return {
        .token              = MOD_TOKEN,
        .tag                = MOD_TAG,
        .instruction_gen_fn = get_arithmetic_fn<op>,
    };
  }
}

static constexpr std::array<const InstructionParsingData, 5> operations_data{
    generate_op_data<ArithmeticOp::ADD>(),
    generate_op_data<ArithmeticOp::SUB>(),
    generate_op_data<ArithmeticOp::MUL>(),
    generate_op_data<ArithmeticOp::DIV>(),
    generate_op_data<ArithmeticOp::MOD>(),
};

} // namespace

namespace arithmetic {
std::span<const InstructionParsingData> get_operations()
{
  return operations_data;
}
} // namespace arithmetic