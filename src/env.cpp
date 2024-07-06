#include "env.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace data {

static std::vector<int64_t>                 data_stack;
static std::unordered_map<int64_t, int64_t> data_heap;

void stack_push(const int64_t value)
{
  data_stack.emplace_back(value);
}

int64_t stack_top()
{
  if (data_stack.empty())
  {
    throw std::runtime_error("empty stack");
  }
  return data_stack.back();
}

int64_t stack_pop()
{
  if (data_stack.empty())
  {
    throw std::runtime_error("empty stack");
  }
  auto const val = data_stack.back();
  data_stack.pop_back();
  return val;
}

void heap_store(const int64_t address, const int64_t value)
{
  data_heap[address] = value;
}

int64_t heap_retrieve(const int64_t address)
{
  auto it = data_heap.find(address);
  if (it == data_heap.end())
  {
    throw std::runtime_error("headp address not found");
  }
  return it->second;
}

} // namespace data

namespace instruction {

static std::unordered_map<int64_t, std::size_t> labels;
static std::vector<std::size_t>                 call_stack;
static bool                                     terminated = false;

void label_add(const int64_t label, const std::size_t address)
{
  if(labels.insert({label,address}).second == false)
  {
    throw std::runtime_error("same label used twice");
  }
}

std::size_t label_get(const int64_t label)
{
  auto it = labels.find(label);
  if (it == labels.end())
  {
    throw std::runtime_error("label not found");
  }
  return it->second;
}

void return_address_push(const std::size_t address)
{
  call_stack.push_back(address);
}

std::size_t return_address_pop()
{
  if (call_stack.empty())
  {
    throw std::runtime_error("not in subroutine");
  }
  auto const val = call_stack.back();
  call_stack.pop_back();
  return val;
}

bool get_terminated()
{
  return terminated;
}

void set_terminated()
{
  terminated = true;
}

} // namespace instruction

namespace io_settings {

static std::reference_wrapper<std::istream> input_stream(std::cin);
static std::reference_wrapper<std::ostream> output_stream(std::cout);

void set_output_stream(std::ostream& ostream)
{
  output_stream = std::ref(ostream);
}

void set_input_stream(std::istream& istream)
{
  input_stream = std::ref(istream);
}

std::ostream& get_output_stream()
{
  return output_stream;
}

std::istream& get_input_stream()
{
  return input_stream;
}

} // namespace io_settings
