cmake_minimum_required(VERSION 3.14)
project(MortgageProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Biblioteca com a logica de negocio
add_library(mortgage_lib src/mortgage.cpp)
target_include_directories(mortgage_lib PUBLIC include)
target_compile_options(mortgage_lib PRIVATE -Wall -Wextra -Wpedantic)

# Executavel de linha de comando
add_executable(mortgage_app src/main.cpp)
target_link_libraries(mortgage_app PRIVATE mortgage_lib)

# Testes (Google Test)
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(mortgage_tests tests/test_mortgage.cpp)
target_link_libraries(mortgage_tests PRIVATE mortgage_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mortgage_tests)
