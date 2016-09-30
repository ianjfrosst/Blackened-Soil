cmake_minimum_required(VERSION 2.8.4)
set(PROJECT_NAME "Blackened-Soil")
project(${PROJECT_NAME})

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_FIX   0)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR} *.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
set(SFML_ROOT "C:/Program Files/SFML-2.4.0/")
find_package(SFML COMPONENTS system window graphics audio REQUIRED)

set(INCLUDE_DIR
        ./
        ${SFML_INCLUDE_DIR}
        )

include_directories(${INCLUDE_DIR})


add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
