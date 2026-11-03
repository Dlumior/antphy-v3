#pragma once

#include "antphy/model/instance.hpp"

namespace antphy::test {

/// Almacén + 4 puntos en línea recta a 10, 20, 30 y 40 km; 1 km = 60 s.
inline Instance line_instance(double capacity = 10.0, std::size_t vehicles = 2) {
    Instance inst;
    inst.name = "linea";
    inst.depot = NodeIndex{0};
    inst.nodes = {
        {.id = "D", .name = "almacén", .demand = 0, .urgency = 0},
        {.id = "A", .name = "", .demand = 4, .urgency = 1.0},
        {.id = "B", .name = "", .demand = 4, .urgency = 0.5},
        {.id = "C", .name = "", .demand = 4, .urgency = 0.5},
        {.id = "E", .name = "", .demand = 4, .urgency = 0.0},
    };
    inst.vehicles = {
        {.id = "camion", .capacity = capacity, .count = vehicles, .max_duration_s = {}}};
    const std::size_t n = inst.nodes.size();
    inst.distance_m = SquareMatrix<double>(n);
    inst.duration_s = SquareMatrix<double>(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            const double km =
                10.0 * (i > j ? static_cast<double>(i - j) : static_cast<double>(j - i));
            inst.distance_m(i, j) = km * 1000.0;
            inst.duration_s(i, j) = km * 60.0;
        }
    }
    return inst;
}

} // namespace antphy::test
