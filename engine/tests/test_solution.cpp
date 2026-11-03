#include "antphy/solution/solution.hpp"
#include "fixtures.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using antphy::NodeIndex;
using antphy::Route;
using antphy::Solution;
using antphy::test::line_instance;
using Catch::Approx;

TEST_CASE("La evaluación cuenta el regreso al almacén", "[solution]") {
    const auto inst = line_instance();
    Solution sol{.solver = "manual",
                 .seed = {},
                 .routes = {Route{0, {NodeIndex{1}, NodeIndex{2}}}},
                 .unserved = {NodeIndex{3}, NodeIndex{4}}};
    const auto eval = antphy::evaluate(inst, sol);
    // 0→A (10 km) → B (10 km) → 0 (20 km) = 40 km
    REQUIRE(eval.objectives.distance_m == Approx(40'000.0));
    REQUIRE(eval.objectives.makespan_s == Approx(40.0 * 60.0));
}

TEST_CASE("La penalización de urgencia depende del orden de visita", "[solution]") {
    const auto inst = line_instance();
    const Solution a_first{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{1}, NodeIndex{2}}}},
                           .unserved = {NodeIndex{3}, NodeIndex{4}}};
    const Solution b_first{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{2}, NodeIndex{1}}}},
                           .unserved = {NodeIndex{3}, NodeIndex{4}}};
    // A es más urgente: visitarlo primero debe penalizar menos.
    REQUIRE(antphy::evaluate(inst, a_first).objectives.urgency_penalty <
            antphy::evaluate(inst, b_first).objectives.urgency_penalty);
}

TEST_CASE("El validador detecta violaciones", "[solution]") {
    const auto inst = line_instance(/*capacity=*/10.0, /*vehicles=*/2);

    SECTION("exceso de capacidad") {
        const Solution sol{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{1}, NodeIndex{2}, NodeIndex{3}}}},
                           .unserved = {NodeIndex{4}}};
        REQUIRE_FALSE(antphy::validate(inst, sol).has_value());
    }
    SECTION("nodo visitado dos veces") {
        const Solution sol{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{1}, NodeIndex{2}}},
                                      Route{0, {NodeIndex{2}, NodeIndex{3}}}},
                           .unserved = {NodeIndex{4}}};
        REQUIRE_FALSE(antphy::validate(inst, sol).has_value());
    }
    SECTION("nodo olvidado") {
        const Solution sol{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{1}, NodeIndex{2}}}},
                           .unserved = {NodeIndex{3}}};
        REQUIRE_FALSE(antphy::validate(inst, sol).has_value());
    }
    SECTION("más vehículos que la flota") {
        const Solution sol{.solver = "m",
                           .seed = {},
                           .routes = {Route{0, {NodeIndex{1}}}, Route{0, {NodeIndex{2}}},
                                      Route{0, {NodeIndex{3}}}},
                           .unserved = {NodeIndex{4}}};
        REQUIRE_FALSE(antphy::validate(inst, sol).has_value());
    }
}
