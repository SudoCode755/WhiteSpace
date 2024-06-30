#pragma once

#include <core.fwd.hpp>

#include <span>

namespace heap_access {

static constexpr std::string_view IMP_TAG("[HEAP]");

std::span<const InstructionParsingData> get_operations();

} // namespace heap_access