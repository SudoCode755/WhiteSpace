#include "utils.hpp"

#include <stdexcept>

bool equal_to_token(const std::string&     script,
                    const std::size_t      program_counter,
                    const std::string_view token)
{
  return script.compare(program_counter, token.size(), token) == 0;
}

std::pair<int64_t, std::size_t> parse_int(const std::string& script,
                                          const std::size_t  program_counter)
{
  if (script[program_counter] == '\n')
  {
    throw std::runtime_error("invalid parse: needs sign value");
  }
  int64_t       num      = 0;
  const int64_t sign     = script[program_counter] == '\t' ? -1 : 1;
  std::size_t   num_size = 1;
  while ((program_counter + num_size < script.size())
         && script[program_counter + num_size] != '\n')
  {
    num *= 2;
    num += ((script[program_counter + num_size] == '\t') ? 1 : 0);
    ++num_size;
  }
  if (program_counter + num_size == script.size())
  {
    throw std::runtime_error("invalid parse: does not end with  '\\n' ");
  }
  num *= sign;
  return std::make_pair(num, num_size + 1);
}