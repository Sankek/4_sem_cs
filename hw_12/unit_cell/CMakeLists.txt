project(unit_cell)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(unit_cell functions.hpp link.cpp link.hpp main.cpp particle.cpp particle.hpp
        system.cpp system.hpp UnitCell.h UnitCell.cpp)
target_link_libraries(unit_cell sfml-graphics)
