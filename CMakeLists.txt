cmake_minimum_required(VERSION 3.29.0)
project(Nova VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_executable(Nova
               ${CMAKE_CURRENT_SOURCE_DIR}/Nova/src/main.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/Nova/src/layers/nova_mainlayer.cpp
              )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/NeoInfused)

add_precompiled_header("nova_pch.hpp" "nova_pch.cpp" ${CMAKE_CURRENT_SOURCE_DIR}/Nova/src Nova ${CMAKE_CURRENT_SOURCE_DIR}/Nova/src)
target_include_directories(Nova PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Nova/src)
target_link_libraries(Nova NeoInfused)
