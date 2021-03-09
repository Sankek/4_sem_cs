project(transliteration)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs

find_package(Boost REQUIRED COMPONENTS locale)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

add_executable(transliteration transliteration.cpp)
target_include_directories(transliteration PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(transliteration ${Boost_LIBRARIES})


