#pragma once

#include <core.fwd.hpp>

#include <span>

namespace arithmetic {

static constexpr std::string_view IMP_TAG("[ARITHMETIC]");

std::span<const InstructionParsingData> get_operations();

} // namespace arithmetic