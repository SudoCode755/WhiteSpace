#include "heap_access.hpp"

#include <core.hpp>
#include <env.hpp>

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

std::optional<std::size_t> heap_store()
{
  const int64_t value   = data::stack_pop();
  const int64_t address = data::stack_pop();
  data::heap_store(address, value);
  return std::nullopt;
}

std::optional<std::size_t> heap_retrieve()
{
  data::stack_push(data::heap_retrieve(data::stack_pop()));
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