cmake_minimum_required(VERSION 3.15)
project(4_sem_cs)

set(CMAKE_CXX_STANDARD 20)


if(MSVC)
    add_subdirectory(hw_1)
    add_subdirectory(hw_2)
    add_subdirectory(hw_3)
    add_subdirectory(hw_4)
    add_subdirectory(hw_5)
    add_subdirectory(hw_6)
    add_subdirectory(hw_7)
    add_subdirectory(hw_8)
    add_subdirectory(hw_9)
    add_subdirectory(hw_10)
endif()


if(MINGW)
    add_subdirectory(hw_11)
endif()
