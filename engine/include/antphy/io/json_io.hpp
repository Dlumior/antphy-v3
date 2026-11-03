#pragma once

#include "antphy/model/instance.hpp"
#include "antphy/solution/solution.hpp"

#include <expected>
#include <string>
#include <string_view>

namespace antphy::io {

/// Lee una instancia en el formato de contracts/schemas/instance.schema.json.
[[nodiscard]] std::expected<Instance, std::string> parse_instance(std::string_view json_text);

/// Serializa una solución en el formato de contracts/schemas/solution.schema.json.
[[nodiscard]] std::string to_json(const Instance& instance, const Solution& solution,
                                  int indent = 2);

} // namespace antphy::io
