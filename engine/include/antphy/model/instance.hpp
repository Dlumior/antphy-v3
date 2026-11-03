#pragma once

#include "antphy/model/matrix.hpp"

#include <cstddef>
#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace antphy {

/// Índice de un nodo dentro de la instancia (0 … n-1). Tipo fuerte para no
/// confundirlo con otros enteros (vehículos, iteraciones, etc.).
struct NodeIndex {
    std::size_t value{0};
    friend constexpr bool operator==(NodeIndex, NodeIndex) = default;
    friend constexpr auto operator<=>(NodeIndex, NodeIndex) = default;
};

/// Punto de atención (o el almacén).
struct Node {
    std::string id;
    std::string name;
    double demand{0.0};
    double urgency{0.0}; ///< Severidad u_i ∈ [0,1], ya calculada a partir del escenario.
};

/// Tipo de vehículo; `count` unidades idénticas disponibles.
struct VehicleType {
    std::string id;
    double capacity{0.0};
    std::size_t count{1};
    std::optional<double> max_duration_s; ///< Duración máxima de ruta, incluido el regreso.
};

/// Datos de entrada del motor. Inmutable una vez construida y validada.
struct Instance {
    std::string name;
    NodeIndex depot{};
    std::vector<Node> nodes;
    std::vector<VehicleType> vehicles;
    SquareMatrix<double> distance_m;
    SquareMatrix<double> duration_s;

    [[nodiscard]] std::size_t size() const noexcept { return nodes.size(); }

    /// Suma de demandas de todos los puntos distintos del almacén.
    [[nodiscard]] double total_demand() const noexcept;

    /// Comprueba la coherencia interna (tamaños, rangos, ids únicos).
    [[nodiscard]] std::expected<void, std::string> validate() const;
};

} // namespace antphy
