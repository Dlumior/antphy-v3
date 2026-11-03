#pragma once

#include "antphy/model/instance.hpp"
#include "antphy/solution/solution.hpp"

namespace antphy {

/// Línea base: vecino más cercano por tiempo de viaje.
/// Llena un vehículo a la vez; nunca viola capacidad ni duración máxima. Los
/// puntos que no caben en la flota quedan en `Solution::unserved`.
///
/// Sirve como referencia mínima para el ACO y para normalizar objetivos.
[[nodiscard]] Solution solve_nearest_neighbor(const Instance& instance);

} // namespace antphy
