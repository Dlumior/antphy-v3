#include "antphy/solvers/nearest_neighbor.hpp"

#include <limits>
#include <optional>

namespace antphy {

Solution solve_nearest_neighbor(const Instance& instance) {
    const std::size_t n = instance.size();
    const std::size_t depot = instance.depot.value;

    Solution solution;
    solution.solver = "nearest_neighbor";

    std::vector<bool> pending(n, true);
    pending[depot] = false;
    std::size_t remaining = n - 1;

    for (std::size_t type = 0; type < instance.vehicles.size() && remaining > 0; ++type) {
        const VehicleType& vehicle = instance.vehicles[type];
        for (std::size_t unit = 0; unit < vehicle.count && remaining > 0; ++unit) {
            Route route{.vehicle_type = type, .stops = {}};
            double load = 0.0;
            double elapsed = 0.0;
            std::size_t current = depot;

            while (true) {
                std::optional<std::size_t> best;
                double best_time = std::numeric_limits<double>::infinity();
                for (std::size_t j = 0; j < n; ++j) {
                    if (!pending[j] || load + instance.nodes[j].demand > vehicle.capacity) {
                        continue;
                    }
                    const double t = instance.duration_s(current, j);
                    if (vehicle.max_duration_s &&
                        elapsed + t + instance.duration_s(j, depot) > *vehicle.max_duration_s) {
                        continue;
                    }
                    if (t < best_time) {
                        best_time = t;
                        best = j;
                    }
                }
                if (!best) {
                    break;
                }
                route.stops.push_back(NodeIndex{*best});
                pending[*best] = false;
                --remaining;
                load += instance.nodes[*best].demand;
                elapsed += best_time;
                current = *best;
            }

            if (!route.stops.empty()) {
                solution.routes.push_back(std::move(route));
            }
        }
    }

    for (std::size_t i = 0; i < n; ++i) {
        if (pending[i]) {
            solution.unserved.push_back(NodeIndex{i});
        }
    }
    return solution;
}

} // namespace antphy
