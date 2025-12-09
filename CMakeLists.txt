cmake_minimum_required(VERSION 4.0)
project(CppImplementation)

set(CMAKE_CXX_STANDARD 20)

#find_package(absl CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -Wunused")


add_executable(CppImplementation
        Problems/521.cpp)
#target_link_libraries(CppImplementation PRIVATE absl::flat_hash_map)
