#pragma once

#include <string>
#include <string_view>

bool equal_to_token(const std::string&     script,
                    const std::size_t      program_counter,
                    const std::string_view token);

std::pair<int64_t, std::size_t> parse_int(const std::string& script,
                                          const std::size_t  program_counter);
