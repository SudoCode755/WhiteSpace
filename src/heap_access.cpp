#include "heap_access.hpp"

#include <core.hpp>
#include <program_state.hpp>

#include <array>
#include <cstddef>
#include <optional>

namespace {

enum class HeapOp
{
  STORE,
  RETRIEVE
};

static constexpr std::string_view STORE_TOKEN(" ");
static constexpr std::string_view RETRIEVE_TOKEN("\t");

static constexpr std::string_view STORE_TAG("[STORE]");
static constexpr std::string_view RETRIEVE_TAG("[RETRIEVE]");

std::optional<std::size_t> heap_store(ProgramState& state)
{
  const int64_t value   = state.data_memory.stack_pop();
  const int64_t address = state.data_memory.stack_pop();
  state.data_memory.heap_store(address, value);
  return std::nullopt;
}

std::optional<std::size_t> heap_retrieve(ProgramState& state)
{
  state.data_memory.stack_push(state.data_memory.heap_retrieve(state.data_memory.stack_pop()));
  return std::nullopt;
}

InstructionParseResult get_heap_store_fn(const std::string&, const std::size_t, const std::size_t)
{
  return {
      .instruction_fn = heap_store,
  };
}

InstructionParseResult get_heap_retrieve_fn(const std::string&, const std::size_t, const std::size_t)
{
  return {
      .instruction_fn = heap_retrieve,
  };
}

template<HeapOp op>
constexpr InstructionParsingData generate_op_data()
{
  if constexpr (op == HeapOp::STORE)
  {
    return {
        .token              = STORE_TOKEN,
        .tag                = STORE_TAG,
        .instruction_gen_fn = get_heap_store_fn,
    };
  }
  else if constexpr (op == HeapOp::RETRIEVE)
  {
    return {
        .token              = RETRIEVE_TOKEN,
        .tag                = RETRIEVE_TAG,
        .instruction_gen_fn = get_heap_retrieve_fn,
    };
  }
}

static constexpr std::array<const InstructionParsingData, 2> operations_data{
    generate_op_data<HeapOp::STORE>(),
    generate_op_data<HeapOp::RETRIEVE>(),
};
} // namespace

namespace heap_access {
std::span<const InstructionParsingData> get_operations()
{
  return operations_data;
}
} // namespace heap_access