project(local_messenger)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install CACHE PATH "install here" FORCE)

find_package(Boost 1.75.0)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(messenger_server messenger_server.cpp)
add_executable(messenger_client messenger_client.cpp)

install(TARGETS messenger_client DESTINATION client_debug CONFIGURATIONS Debug)
install(TARGETS messenger_server DESTINATION server_debug CONFIGURATIONS Debug)

