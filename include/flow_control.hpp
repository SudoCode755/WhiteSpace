#pragma once

#include <core.fwd.hpp>

#include <span>

namespace flow_control {

static constexpr std::string_view IMP_TAG("[FLOW CONTROL]");

std::span<const InstructionParsingData> get_operations();

} // namespace flow_control