#include "antphy/solution/solution.hpp"

#include <algorithm>
#include <format>

namespace antphy {
namespace {

constexpr double kSecondsPerHour = 3600.0;
constexpr double kEpsilon = 1e-9;

} // namespace

Evaluation evaluate(const Instance& instance, const Solution& solution) {
    Evaluation eval;
    eval.routes.reserve(solution.routes.size());
    const std::size_t depot = instance.depot.value;

    for (const Route& route : solution.routes) {
        RouteMetrics metrics;
        std::size_t prev = depot;
        for (const NodeIndex stop : route.stops) {
            metrics.distance_m += instance.distance_m(prev, stop.value);
            metrics.duration_s += instance.duration_s(prev, stop.value);
            metrics.load += instance.nodes[stop.value].demand;
            eval.objectives.urgency_penalty +=
                instance.nodes[stop.value].urgency * (metrics.duration_s / kSecondsPerHour);
            prev = stop.value;
        }
        if (!route.stops.empty()) {
            metrics.distance_m += instance.distance_m(prev, depot);
            metrics.duration_s += instance.duration_s(prev, depot);
        }
        eval.objectives.distance_m += metrics.distance_m;
        eval.objectives.makespan_s = std::max(eval.objectives.makespan_s, metrics.duration_s);
        eval.routes.push_back(metrics);
    }
    return eval;
}

std::expected<void, std::string> validate(const Instance& instance, const Solution& solution) {
    const std::size_t n = instance.size();
    const std::size_t depot = instance.depot.value;
    std::vector<int> seen(n, 0);
    std::vector<std::size_t> used_per_type(instance.vehicles.size(), 0);

    const Evaluation eval = evaluate(instance, solution);

    for (std::size_t r = 0; r < solution.routes.size(); ++r) {
        const Route& route = solution.routes[r];
        if (route.vehicle_type >= instance.vehicles.size()) {
            return std::unexpected(std::format("ruta {}: tipo de vehículo inexistente", r));
        }
        const VehicleType& vehicle = instance.vehicles[route.vehicle_type];
        if (++used_per_type[route.vehicle_type] > vehicle.count) {
            return std::unexpected(
                std::format("se usan más vehículos '{}' de los disponibles", vehicle.id));
        }
        for (const NodeIndex stop : route.stops) {
            if (stop.value >= n) {
                return std::unexpected(std::format("ruta {}: nodo fuera de rango", r));
            }
            if (stop.value == depot) {
                return std::unexpected(
                    std::format("ruta {}: el almacén no debe estar en stops", r));
            }
            ++seen[stop.value];
        }
        if (eval.routes[r].load > vehicle.capacity + kEpsilon) {
            return std::unexpected(std::format("ruta {}: carga {} supera la capacidad {}", r,
                                               eval.routes[r].load, vehicle.capacity));
        }
        if (vehicle.max_duration_s &&
            eval.routes[r].duration_s > *vehicle.max_duration_s + kEpsilon) {
            return std::unexpected(std::format("ruta {}: duración {} s supera el máximo {} s", r,
                                               eval.routes[r].duration_s, *vehicle.max_duration_s));
        }
    }

    for (const NodeIndex u : solution.unserved) {
        if (u.value >= n || u.value == depot) {
            return std::unexpected("nodo no atendido inválido");
        }
        ++seen[u.value];
    }

    for (std::size_t i = 0; i < n; ++i) {
        if (i == depot) {
            continue;
        }
        if (seen[i] != 1) {
            return std::unexpected(std::format("el nodo {} aparece {} veces (debe ser 1)",
                                               instance.nodes[i].id, seen[i]));
        }
    }
    return {};
}

} // namespace antphy
