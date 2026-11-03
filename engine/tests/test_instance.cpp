#include "antphy/model/instance.hpp"
#include "fixtures.hpp"

#include <catch2/catch_test_macros.hpp>

using antphy::test::line_instance;

TEST_CASE("Una instancia bien formada es válida", "[instance]") {
    const auto inst = line_instance();
    REQUIRE(inst.validate().has_value());
    REQUIRE(inst.total_demand() == 16.0);
}

TEST_CASE("Ids duplicados se rechazan", "[instance]") {
    auto inst = line_instance();
    inst.nodes[2].id = "A";
    const auto result = inst.validate();
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().find("duplicado") != std::string::npos);
}

TEST_CASE("Urgencia fuera de [0,1] se rechaza", "[instance]") {
    auto inst = line_instance();
    inst.nodes[1].urgency = 1.5;
    REQUIRE_FALSE(inst.validate().has_value());
}
