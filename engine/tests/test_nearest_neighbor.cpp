#include "antphy/solvers/nearest_neighbor.hpp"
#include "fixtures.hpp"

#include <catch2/catch_test_macros.hpp>

using antphy::test::line_instance;

TEST_CASE("Vecino más cercano produce soluciones válidas", "[nearest_neighbor]") {
    const auto inst = line_instance(/*capacity=*/10.0, /*vehicles=*/2);
    const auto sol = antphy::solve_nearest_neighbor(inst);
    REQUIRE(antphy::validate(inst, sol).has_value());
    REQUIRE(sol.routes.size() == 2);
    REQUIRE(sol.unserved.empty());
}

TEST_CASE("Si la flota no alcanza, los puntos quedan como no atendidos", "[nearest_neighbor]") {
    const auto inst = line_instance(/*capacity=*/8.0, /*vehicles=*/1);
    const auto sol = antphy::solve_nearest_neighbor(inst);
    REQUIRE(antphy::validate(inst, sol).has_value());
    REQUIRE(sol.unserved.size() == 2);
}

TEST_CASE("Se respeta la duración máxima de ruta", "[nearest_neighbor]") {
    auto inst = line_instance(/*capacity=*/100.0, /*vehicles=*/4);
    inst.vehicles[0].max_duration_s = 50.0 * 60.0; // 50 min: alcanza para ir y volver de 20 km
    const auto sol = antphy::solve_nearest_neighbor(inst);
    REQUIRE(antphy::validate(inst, sol).has_value());
    REQUIRE_FALSE(sol.unserved.empty());
}
