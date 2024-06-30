#include "whiteSpace.hpp"

#include <arithmetic.hpp>
#include <core.hpp>
#include <env.hpp>
#include <flow_control.hpp>
#include <heap_access.hpp>
#include <io_ops.hpp>
#include <stack_manip.hpp>
#include <utils.hpp>

#include <array>
#include <cstddef>
#include <cstring>
#include <istream>
#include <iterator>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

namespace {

enum class IMP
{
  STACK_MANIP,
  ARITHMETIC,
  HEAP_ACCESS,
  FLOW_CONTROL,
  IO_OPS
};

static constexpr std::string_view STACK_MANIP_TOKEN(" ");
static constexpr std::string_view ARITHMETIC_TOKEN("\t ");
static constexpr std::string_view HEAP_ACCESS_TOKEN("\t\t");
static constexpr std::string_view FLOW_CONTROL_TOKEN("\n");
static constexpr std::string_view IO_OPS_TOKEN("\t\n");

template<IMP imp>
constexpr decltype(auto) get_imp_parse_data()
{
  if constexpr (imp == IMP::STACK_MANIP)
  {
    return std::make_pair(stack_manip::IMP_TAG, stack_manip::get_operations);
  }
  else if constexpr (imp == IMP::ARITHMETIC)
  {
    return std::make_pair(arithmetic::IMP_TAG, arithmetic::get_operations);
  }
  else if constexpr (imp == IMP::HEAP_ACCESS)
  {
    return std::make_pair(heap_access::IMP_TAG, heap_access::get_operations);
  }
  else if constexpr (imp == IMP::FLOW_CONTROL)
  {
    return std::make_pair(flow_control::IMP_TAG, flow_control::get_operations);
  }
  else if constexpr (imp == IMP::IO_OPS)
  {
    return std::make_pair(io_ops::IMP_TAG, io_ops::get_operations);
  }
}

struct Instruction
{
  std::string_view    imp_tag;
  std::string_view    op_tag;
  InstructionCallable instruction_fn;
};

template<IMP imp_code>
constexpr std::pair<std::optional<Instruction>, std::size_t>
    try_parse(const std::string& script,
              const std::size_t  script_index,
              const std::size_t  instruction_index)
{
  const auto&& [imp_tag, get_operation_parsers]
      = get_imp_parse_data<imp_code>();

  std::string_view parsed_op_tag;
  auto const       operation_parsers = get_operation_parsers();
  try
  {
    for (auto const& [op_token, op_tag, op_func]: operation_parsers)
    {
      if (equal_to_token(script, script_index, op_token))
      {
        parsed_op_tag = op_tag;
        auto&& [instruction_fn, argument_size]
            = op_func(script,
                      script_index + op_token.size(),
                      instruction_index);
        if constexpr (imp_code == IMP::FLOW_CONTROL)
        {
          if (op_token == "  ")
          {
            instruction_fn();
            return {
                std::nullopt,
                script_index + op_token.size() + argument_size,
            };
          }
        }
        return {
            Instruction{
                        .imp_tag        = imp_tag,
                        .op_tag         = op_tag,
                        .instruction_fn = instruction_fn,
                        },
            script_index + op_token.size() + argument_size,
        };
      }
    }
  }
  catch (std::runtime_error& e)
  {
    std::string message(imp_tag);
    message += parsed_op_tag;
    message += " parsing instruction ";
    message += std::to_string(instruction_index);
    message += ": ";
    message += message += e.what();
    throw std::runtime_error(message);
  }

  std::string message(imp_tag);
  message += "[ERROR] unknown operation parsing instruction ";
  message += std::to_string(instruction_index);
  throw std::invalid_argument(message);

  return {std::nullopt, -1};
}

using ParserFn
    = std::pair<std::optional<Instruction>, std::size_t> (*)(const std::string&,
                                                             const std::size_t,
                                                             const std::size_t);

template<IMP imp>
constexpr std::pair<std::string_view, ParserFn> get_imp_data()
{
  if constexpr (imp == IMP::STACK_MANIP)
  {
    return std::make_pair(STACK_MANIP_TOKEN, try_parse<imp>);
  }
  else if constexpr (imp == IMP::ARITHMETIC)
  {
    return std::make_pair(ARITHMETIC_TOKEN, try_parse<imp>);
  }
  else if constexpr (imp == IMP::HEAP_ACCESS)
  {
    return std::make_pair(HEAP_ACCESS_TOKEN, try_parse<imp>);
  }
  else if constexpr (imp == IMP::FLOW_CONTROL)
  {
    return std::make_pair(FLOW_CONTROL_TOKEN, try_parse<imp>);
  }
  else if constexpr (imp == IMP::IO_OPS)
  {
    return std::make_pair(IO_OPS_TOKEN, try_parse<imp>);
  }
}

constexpr static std::array<std::pair<std::string_view, ParserFn>, 5>
    imp_parsers = {get_imp_data<IMP::STACK_MANIP>(),
                   get_imp_data<IMP::ARITHMETIC>(),
                   get_imp_data<IMP::HEAP_ACCESS>(),
                   get_imp_data<IMP::FLOW_CONTROL>(),
                   get_imp_data<IMP::IO_OPS>()};

std::size_t try_execute(const Instruction& instruction,
                        const std::size_t  program_counter)
{
  try
  {
    const auto next_jump_location_opt = instruction.instruction_fn();
    return next_jump_location_opt.value_or(program_counter + 1);
  }
  catch (std::runtime_error& e)
  {
    std::string message(instruction.imp_tag);
    message += instruction.op_tag;
    message += " executing instruction ";
    message += std::to_string(program_counter);
    message += ": ";
    message += e.what();
    throw std::runtime_error(message);
  }
  return -1;
}

std::vector<Instruction> parse_script(const std::string& script)
{
  std::size_t              index = 0;
  std::vector<Instruction> instructions;
  while (index < script.size())
  {
    for (const auto& [imp_token, parser_fn]: imp_parsers)
    {
      if (equal_to_token(script, index, imp_token))
      {
        auto&& [opt_instruction, next_index]
            = parser_fn(script, index + imp_token.size(), instructions.size());
        index = next_index;
        if (opt_instruction.has_value())
        {
          instructions.emplace_back(opt_instruction.value());
        }
        break;
      }
    }
  }
  return instructions;
}

std::string filter_chars(std::istream& script_in)
{
  std::string script;
  std::copy_if(std::istreambuf_iterator<char>(script_in),
               std::istreambuf_iterator<char>(),
               std::back_inserter(script),
               [&](char c) { return std::strchr(" \t\n", c) != nullptr; });
  return script;
}

} // namespace

void run_script(std::istream& script, std::ostream& output, std::istream& input)
{
  io_settings::set_output_stream(output);
  io_settings::set_input_stream(input);
  std::vector<Instruction> instructions    = parse_script(filter_chars(script));
  std::size_t              program_counter = 0;
  while (program_counter < instructions.size())
  {
    program_counter
        = try_execute(instructions[program_counter], program_counter);
  }
  if (! instruction::get_terminated())
  {
    throw std::runtime_error("Prgroma terminated incorrectly "
                             "[FLOW][EXIT](\"\\n\\n\\n\") was never executed");
  }
}
