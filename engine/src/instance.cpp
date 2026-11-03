#include "antphy/model/instance.hpp"

#include <format>
#include <unordered_set>

namespace antphy {

double Instance::total_demand() const noexcept {
    double total = 0.0;
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        if (i != depot.value) {
            total += nodes[i].demand;
        }
    }
    return total;
}

std::expected<void, std::string> Instance::validate() const {
    const std::size_t n = nodes.size();
    if (n == 0) {
        return std::unexpected("la instancia no tiene nodos");
    }
    if (depot.value >= n) {
        return std::unexpected(std::format("depot={} fuera de rango (n={})", depot.value, n));
    }
    if (vehicles.empty()) {
        return std::unexpected("la instancia no tiene vehículos");
    }
    if (distance_m.size() != n || duration_s.size() != n) {
        return std::unexpected(std::format("las matrices deben ser {}x{}", n, n));
    }

    std::unordered_set<std::string> ids;
    for (const Node& node : nodes) {
        if (node.id.empty()) {
            return std::unexpected("hay un nodo sin id");
        }
        if (!ids.insert(node.id).second) {
            return std::unexpected(std::format("id de nodo duplicado: {}", node.id));
        }
        if (node.demand < 0.0) {
            return std::unexpected(std::format("demanda negativa en {}", node.id));
        }
        if (node.urgency < 0.0 || node.urgency > 1.0) {
            return std::unexpected(std::format("urgencia fuera de [0,1] en {}", node.id));
        }
    }

    for (const VehicleType& v : vehicles) {
        if (v.capacity <= 0.0 || v.count == 0) {
            return std::unexpected(
                std::format("vehículo {} con capacidad o cantidad inválida", v.id));
        }
    }

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (distance_m(i, j) < 0.0 || duration_s(i, j) < 0.0) {
                return std::unexpected(std::format("valor negativo en matrices ({},{})", i, j));
            }
        }
    }
    return {};
}

} // namespace antphy
