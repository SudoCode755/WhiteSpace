#pragma once

#include <core.fwd.hpp>

#include <span>

namespace io_ops {

static constexpr std::string_view IMP_TAG("[I/O]");

std::span<const InstructionParsingData> get_operations();

} // namespace io_ops