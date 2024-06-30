#include "io_ops.hpp"

#include <core.hpp>
#include <env.hpp>

#include <cstddef>
#include <cstdint>
#include <array>

namespace {

enum class IO_Op
{
  CHAR_OUT,
  INT_OUT,
  CHAR_IN,
  INT_IN
};

static constexpr std::string_view CHAR_OUT_TOKEN("  ");
static constexpr std::string_view INT_OUT_TOKEN(" \t");
static constexpr std::string_view CHAR_IN_TOKEN("\t ");
static constexpr std::string_view INT_IN_TOKEN("\t\t");

static constexpr std::string_view CHAR_OUT_TAG("[CHAR OUT]");
static constexpr std::string_view INT_OUT_TAG("[INT OUT]");
static constexpr std::string_view CHAR_IN_TAG("[CHAR IN]");
static constexpr std::string_view INT_IN_TAG("[INT IN]");

template<typename T>
std::optional<std::size_t> out_op()
{
  io_settings::get_output_stream() << static_cast<T>(data::stack_pop());
  return std::nullopt;
}

template<typename T>
std::optional<std::size_t> in_op()
{
  T input;
  io_settings::get_input_stream() >> input;
  data::heap_store(data::stack_pop(), input);
  return std::nullopt;
}

template<typename T>
InstructionParseResult get_out_fn(const std::string&,
                                  const std::size_t,
                                  const std::size_t)
{
  return {
      .instruction_fn = out_op<T>,
  };
}

template<typename T>
InstructionParseResult get_in_fn(const std::string&,
                                 const std::size_t,
                                 const std::size_t)
{
  return {
      .instruction_fn = in_op<T>,
  };
}

template<IO_Op op>
constexpr InstructionParsingData generate_op_data()
{
  if constexpr (op == IO_Op::CHAR_OUT)
  {
    return {
        .token              = CHAR_OUT_TOKEN,
        .tag                = CHAR_OUT_TAG,
        .instruction_gen_fn = get_out_fn<char>,
    };
  }
  else if constexpr (op == IO_Op::INT_OUT)
  {
    return {
        .token              = INT_OUT_TOKEN,
        .tag                = INT_OUT_TAG,
        .instruction_gen_fn = get_out_fn<int64_t>,
    };
  }
  else if constexpr (op == IO_Op::CHAR_IN)
  {
    return {
        .token              = CHAR_IN_TOKEN,
        .tag                = CHAR_IN_TAG,
        .instruction_gen_fn = get_in_fn<char>,
    };
  }
  else if constexpr (op == IO_Op::INT_IN)
  {
    return {
        .token              = INT_IN_TOKEN,
        .tag                = INT_IN_TAG,
        .instruction_gen_fn = get_in_fn<int64_t>,
    };
  }
}

static constexpr std::array<const InstructionParsingData, 4> operations_data{
    generate_op_data<IO_Op::CHAR_OUT>(),
    generate_op_data<IO_Op::INT_OUT>(),
    generate_op_data<IO_Op::CHAR_IN>(),
    generate_op_data<IO_Op::INT_IN>(),
};
} // namespace

namespace io_ops {
std::span<const InstructionParsingData> get_operations()
{
  return operations_data;
}
} // namespace io_ops