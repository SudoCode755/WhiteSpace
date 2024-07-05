#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <string>

struct InstructionParsingData;
struct InstructionParseResult;

using InstructionCallable  = std::function<std::optional<std::size_t>()>;
using InstructionGenerator = InstructionParseResult (*)(const std::string&, const std::size_t, const std::size_t);