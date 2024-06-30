#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>

namespace data {
void stack_push(const int64_t value);

int64_t stack_top();

int64_t stack_pop();

void heap_store(const int64_t address, const int64_t value);

int64_t heap_retrieve(const int64_t address);
} // namespace data

namespace instruction {

void set_terminated();

bool get_terminated();

void label_add(const int64_t label, const std::size_t address);

std::size_t label_get(const int64_t label);

void return_address_push(const std::size_t address);

[[nodiscard]] std::size_t return_address_pop();
} // namespace instruction

namespace io_settings {
void          set_output_stream(std::ostream& ostream);
void          set_input_stream(std::istream& istream);
std::ostream& get_output_stream();
std::istream& get_input_stream();
} // namespace io_settings
