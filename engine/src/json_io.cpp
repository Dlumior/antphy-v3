#include "antphy/io/json_io.hpp"

#include <format>
#include <nlohmann/json.hpp>

namespace antphy::io {
namespace {

using nlohmann::json;

std::expected<SquareMatrix<double>, std::string> parse_matrix(const json& j, std::size_t n,
                                                              std::string_view name) {
    if (!j.is_array() || j.size() != n) {
        return std::unexpected(std::format("matrices.{} debe tener {} filas", name, n));
    }
    SquareMatrix<double> m(n);
    for (std::size_t i = 0; i < n; ++i) {
        if (!j[i].is_array() || j[i].size() != n) {
            return std::unexpected(
                std::format("matrices.{}[{}] debe tener {} columnas", name, i, n));
        }
        for (std::size_t k = 0; k < n; ++k) {
            m(i, k) = j[i][k].get<double>();
        }
    }
    return m;
}

} // namespace

std::expected<Instance, std::string> parse_instance(std::string_view json_text) {
    json doc = json::parse(json_text, nullptr, /*allow_exceptions=*/false);
    if (doc.is_discarded()) {
        return std::unexpected("JSON inválido");
    }
    try {
        if (doc.value("schema_version", "") != "1") {
            return std::unexpected("schema_version no soportado (se espera \"1\")");
        }

        Instance inst;
        inst.name = doc.at("name").get<std::string>();
        inst.depot = NodeIndex{doc.at("depot").get<std::size_t>()};

        for (const json& jn : doc.at("nodes")) {
            inst.nodes.push_back(Node{
                .id = jn.at("id").get<std::string>(),
                .name = jn.value("name", ""),
                .demand = jn.at("demand").get<double>(),
                .urgency = jn.value("urgency", 0.0),
            });
        }
        for (const json& jv : doc.at("vehicles")) {
            VehicleType v{
                .id = jv.at("id").get<std::string>(),
                .capacity = jv.at("capacity").get<double>(),
                .count = jv.value("count", std::size_t{1}),
                .max_duration_s = std::nullopt,
            };
            if (jv.contains("max_duration_s")) {
                v.max_duration_s = jv.at("max_duration_s").get<double>();
            }
            inst.vehicles.push_back(std::move(v));
        }

        const std::size_t n = inst.nodes.size();
        const json& jm = doc.at("matrices");
        auto dist = parse_matrix(jm.at("distance_m"), n, "distance_m");
        if (!dist) {
            return std::unexpected(dist.error());
        }
        auto dur = parse_matrix(jm.at("duration_s"), n, "duration_s");
        if (!dur) {
            return std::unexpected(dur.error());
        }
        inst.distance_m = std::move(*dist);
        inst.duration_s = std::move(*dur);

        if (auto ok = inst.validate(); !ok) {
            return std::unexpected(ok.error());
        }
        return inst;
    } catch (const json::exception& e) {
        return std::unexpected(std::format("instancia mal formada: {}", e.what()));
    }
}

std::string to_json(const Instance& instance, const Solution& solution, int indent) {
    const Evaluation eval = evaluate(instance, solution);

    json routes = json::array();
    for (std::size_t r = 0; r < solution.routes.size(); ++r) {
        json stops = json::array();
        for (const NodeIndex s : solution.routes[r].stops) {
            stops.push_back(instance.nodes[s.value].id);
        }
        routes.push_back({
            {"vehicle", instance.vehicles[solution.routes[r].vehicle_type].id},
            {"stops", std::move(stops)},
            {"load", eval.routes[r].load},
            {"distance_m", eval.routes[r].distance_m},
            {"duration_s", eval.routes[r].duration_s},
        });
    }

    json unserved = json::array();
    for (const NodeIndex u : solution.unserved) {
        unserved.push_back(instance.nodes[u.value].id);
    }

    json out = {
        {"schema_version", "1"},
        {"instance", instance.name},
        {"solver", solution.solver},
        {"feasible", validate(instance, solution).has_value()},
        {"routes", std::move(routes)},
        {"unserved", std::move(unserved)},
        {"objectives",
         {
             {"distance_m", eval.objectives.distance_m},
             {"makespan_s", eval.objectives.makespan_s},
             {"urgency_penalty", eval.objectives.urgency_penalty},
         }},
    };
    if (solution.seed) {
        out["seed"] = *solution.seed;
    }
    return out.dump(indent);
}

} // namespace antphy::io
