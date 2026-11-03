#include "antphy/io/json_io.hpp"
#include "antphy/solvers/nearest_neighbor.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>

namespace {
std::string read_file(const std::string& path) {
    std::ifstream f(path);
    return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
}
} // namespace

TEST_CASE("El ejemplo de contracts se lee y se resuelve", "[io]") {
    const auto text = read_file(std::string(ANTPHY_EXAMPLES_DIR) + "/tumbes-demo.json");
    REQUIRE_FALSE(text.empty());

    const auto inst = antphy::io::parse_instance(text);
    REQUIRE(inst.has_value());
    REQUIRE(inst->size() == 7);

    const auto sol = antphy::solve_nearest_neighbor(*inst);
    const auto out = nlohmann::json::parse(antphy::io::to_json(*inst, sol));
    REQUIRE(out["schema_version"] == "1");
    REQUIRE(out["feasible"] == true);
    REQUIRE(out["objectives"].contains("urgency_penalty"));
}

TEST_CASE("Entradas inválidas devuelven un error, no una excepción", "[io]") {
    REQUIRE_FALSE(antphy::io::parse_instance("no es json").has_value());
    REQUIRE_FALSE(antphy::io::parse_instance(R"({"schema_version":"99"})").has_value());
    REQUIRE_FALSE(antphy::io::parse_instance(R"({"schema_version":"1","name":"x"})").has_value());
}
