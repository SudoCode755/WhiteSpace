#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

class DataMemory
{
public:
  DataMemory()                        = default;
  DataMemory(DataMemory&&)            = default;
  DataMemory& operator=(DataMemory&&) = default;

  DataMemory(const DataMemory&)            = delete;
  DataMemory& operator=(const DataMemory&) = delete;

  void    stack_push(const int64_t value);
  int64_t stack_top();
  int64_t stack_pop();
  void    heap_store(const int64_t address, const int64_t value);
  int64_t heap_retrieve(const int64_t address);

private:
  std::vector<int64_t>                 data_stack;
  std::unordered_map<int64_t, int64_t> data_heap;
};

class InstructionMemory
{
public:
  InstructionMemory()                               = default;
  InstructionMemory(InstructionMemory&&)            = default;
  InstructionMemory& operator=(InstructionMemory&&) = default;

  InstructionMemory(const InstructionMemory&)            = delete;
  InstructionMemory& operator=(const InstructionMemory&) = delete;

  void                      label_add(const int64_t label, const std::size_t address);
  std::size_t               label_get(const int64_t label);
  void                      return_address_push(const std::size_t address);
  [[nodiscard]] std::size_t return_address_pop();

private:
  std::unordered_map<int64_t, std::size_t> m_labels;
  std::vector<std::size_t>                 m_call_stack;
};

class IOSettings
{
public:
  IOSettings()                        = default;
  IOSettings(IOSettings&&)            = default;
  IOSettings& operator=(IOSettings&&) = default;

  IOSettings(const IOSettings&)            = delete;
  IOSettings& operator=(const IOSettings&) = delete;

  void          set_output_stream(std::ostream& ostream);
  void          set_input_stream(std::istream& istream);
  std::ostream& get_output_stream();
  std::istream& get_input_stream();

private:
  std::reference_wrapper<std::istream> m_input_stream  = std::cin;
  std::reference_wrapper<std::ostream> m_output_stream = std::cout;
};

struct ProgramState
{
  ProgramState()                          = default;
  ProgramState(ProgramState&&)            = default;
  ProgramState& operator=(ProgramState&&) = default;

  ProgramState(const ProgramState&)            = delete;
  ProgramState& operator=(const ProgramState&) = delete;

  DataMemory        data_memory;
  InstructionMemory instruction_memory;
  IOSettings        io_settings;

  void set_terminated();
  bool get_terminated();

private:
  bool m_terminated = false;
};