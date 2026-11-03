// antphy-solve: lee una instancia JSON y escribe la solución JSON.
//
//   antphy-solve instancia.json            # lee de archivo
//   antphy-solve - < instancia.json        # lee de stdin
//   antphy-solve --solver nearest_neighbor instancia.json
//
// Códigos de salida: 0 = ok, 1 = entrada inválida, 2 = uso incorrecto.

#include "antphy/io/json_io.hpp"
#include "antphy/solvers/nearest_neighbor.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kUsage =
    "Uso: antphy-solve [--solver nearest_neighbor] <instancia.json | ->\n";

std::string read_all(std::istream& in) {
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

} // namespace

int main(int argc, char** argv) {
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    std::string_view solver = "nearest_neighbor";
    std::string_view input;

    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-h" || args[i] == "--help") {
            std::cout << kUsage;
            return 0;
        }
        if (args[i] == "--solver" && i + 1 < args.size()) {
            solver = args[++i];
        } else {
            input = args[i];
        }
    }
    if (input.empty()) {
        std::cerr << kUsage;
        return 2;
    }
    if (solver != "nearest_neighbor") {
        std::cerr << "solver desconocido: " << solver << " (disponible: nearest_neighbor)\n";
        return 2;
    }

    std::string text;
    if (input == "-") {
        text = read_all(std::cin);
    } else {
        std::ifstream file{std::string(input)};
        if (!file) {
            std::cerr << "no se pudo abrir " << input << '\n';
            return 1;
        }
        text = read_all(file);
    }

    auto instance = antphy::io::parse_instance(text);
    if (!instance) {
        std::cerr << "instancia inválida: " << instance.error() << '\n';
        return 1;
    }

    const antphy::Solution solution = antphy::solve_nearest_neighbor(*instance);
    std::cout << antphy::io::to_json(*instance, solution) << '\n';
    return 0;
}
