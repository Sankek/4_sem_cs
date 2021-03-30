project(hreadsafe_comparison)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(Boost 1.75.0)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(threadsafe_comparison threadsafe_comparison.cpp)
endif()

