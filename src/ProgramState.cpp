#include "program_sate.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

bool ProgramState::get_terminated()
{
  return m_terminated;
}

void ProgramState::set_terminated()
{
  m_terminated = true;
}

void DataMemory::stack_push(const int64_t value)
{
  data_stack.emplace_back(value);
}

int64_t DataMemory::stack_top()
{
  if (data_stack.empty())
  {
    throw std::runtime_error("empty stack");
  }
  return data_stack.back();
}

int64_t DataMemory::stack_pop()
{
  if (data_stack.empty())
  {
    throw std::runtime_error("empty stack");
  }
  auto const val = data_stack.back();
  data_stack.pop_back();
  return val;
}

void DataMemory::heap_store(const int64_t address, const int64_t value)
{
  data_heap[address] = value;
}

int64_t DataMemory::heap_retrieve(const int64_t address)
{
  auto it = data_heap.find(address);
  if (it == data_heap.end())
  {
    throw std::runtime_error("headp address not found");
  }
  return it->second;
}

void InstructionMemory::label_add(const int64_t label, const std::size_t address)
{
  if (m_labels.insert({label, address}).second == false)
  {
    throw std::runtime_error("same label used twice");
  }
}

std::size_t InstructionMemory::label_get(const int64_t label)
{
  auto it = m_labels.find(label);
  if (it == m_labels.end())
  {
    throw std::runtime_error("label not found");
  }
  return it->second;
}

void InstructionMemory::return_address_push(const std::size_t address)
{
  m_call_stack.push_back(address);
}

std::size_t InstructionMemory::return_address_pop()
{
  if (m_call_stack.empty())
  {
    throw std::runtime_error("not in subroutine");
  }
  auto const val = m_call_stack.back();
  m_call_stack.pop_back();
  return val;
}

void IOSettings::set_output_stream(std::ostream& ostream)
{
  m_output_stream = std::ref(ostream);
}

void IOSettings::set_input_stream(std::istream& istream)
{
  m_input_stream = std::ref(istream);
}

std::ostream& IOSettings::get_output_stream()
{
  return m_output_stream;
}

std::istream& IOSettings::get_input_stream()
{
  return m_input_stream;
}
