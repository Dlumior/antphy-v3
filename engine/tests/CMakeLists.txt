add_executable(antphy_tests
    test_instance.cpp
    test_solution.cpp
    test_nearest_neighbor.cpp
    test_json_io.cpp)
target_link_libraries(antphy_tests PRIVATE antphy::core Catch2::Catch2WithMain)
target_compile_definitions(antphy_tests PRIVATE
    ANTPHY_EXAMPLES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../contracts/examples")
antphy_set_warnings(antphy_tests)

include(Catch)
catch_discover_tests(antphy_tests)
