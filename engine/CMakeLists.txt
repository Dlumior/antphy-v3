cmake_minimum_required(VERSION 3.25)

project(antphy
    VERSION 0.1.0
    DESCRIPTION "Motor de ruteo humanitario con colonia de hormigas"
    LANGUAGES CXX)

# --- Opciones -----------------------------------------------------------------
option(ANTPHY_BUILD_TESTS "Compilar las pruebas" ${PROJECT_IS_TOP_LEVEL})
option(ANTPHY_BUILD_CLI "Compilar el ejecutable antphy-solve" ${PROJECT_IS_TOP_LEVEL})
option(ANTPHY_WARNINGS_AS_ERRORS "Tratar advertencias como errores" ON)
option(ANTPHY_ENABLE_SANITIZERS "Compilar con ASan + UBSan" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Dependencies)
include(CompilerWarnings)

# --- Librería principal -------------------------------------------------------
add_library(antphy_core
    src/instance.cpp
    src/solution.cpp
    src/nearest_neighbor.cpp
    src/json_io.cpp)
add_library(antphy::core ALIAS antphy_core)

target_include_directories(antphy_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(antphy_core PUBLIC nlohmann_json::nlohmann_json)
target_compile_features(antphy_core PUBLIC cxx_std_23)
antphy_set_warnings(antphy_core)

if(ANTPHY_ENABLE_SANITIZERS)
    target_compile_options(antphy_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(antphy_core PUBLIC -fsanitize=address,undefined)
endif()

# --- Ejecutable ---------------------------------------------------------------
if(ANTPHY_BUILD_CLI)
    add_executable(antphy-solve cli/main.cpp)
    target_link_libraries(antphy-solve PRIVATE antphy::core)
    antphy_set_warnings(antphy-solve)
    install(TARGETS antphy-solve RUNTIME DESTINATION bin)
endif()

# --- Pruebas ------------------------------------------------------------------
if(ANTPHY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
