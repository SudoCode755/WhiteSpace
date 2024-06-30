#pragma once

#include <core.fwd.hpp>

#include <span>

namespace stack_manip {

static constexpr std::string_view IMP_TAG("[STACK MANIP]");

std::span<const InstructionParsingData> get_operations();
} // namespace stack_manip