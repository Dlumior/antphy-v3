#pragma once

#include "antphy/model/instance.hpp"

#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace antphy {

/// Una ruta: sale del almacén, visita `stops` en orden y vuelve al almacén.
/// El almacén no se incluye en `stops`.
struct Route {
    std::size_t vehicle_type{0};
    std::vector<NodeIndex> stops;
};

/// Valores de los objetivos de una solución. El regreso al almacén SÍ se cuenta.
struct Objectives {
    double distance_m{0.0};      ///< Distancia total recorrida.
    double makespan_s{0.0};      ///< Duración de la ruta más larga.
    double urgency_penalty{0.0}; ///< ∑ u_i · t_i, con t_i = hora de llegada (en horas).
};

/// Métricas por ruta, calculadas al evaluar.
struct RouteMetrics {
    double load{0.0};
    double distance_m{0.0};
    double duration_s{0.0};
};

struct Solution {
    std::string solver;
    std::optional<std::uint64_t> seed;
    std::vector<Route> routes;
    std::vector<NodeIndex> unserved;
};

/// Resultado de evaluar una solución contra una instancia.
struct Evaluation {
    Objectives objectives;
    std::vector<RouteMetrics> routes;
};

/// Calcula los objetivos sin comprobar factibilidad.
[[nodiscard]] Evaluation evaluate(const Instance& instance, const Solution& solution);

/// Comprueba que la solución respete todas las restricciones:
/// cada punto atendido a lo sumo una vez, todo punto atendido o listado como no
/// atendido, capacidad, duración máxima y tamaño de flota.
[[nodiscard]] std::expected<void, std::string> validate(const Instance& instance,
                                                        const Solution& solution);

} // namespace antphy
