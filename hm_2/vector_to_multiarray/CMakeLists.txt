project(vector_to_multiarray)

find_package(Boost 1.75.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(vector_to_multiarray vector_to_multiarray.cpp)
endif()
