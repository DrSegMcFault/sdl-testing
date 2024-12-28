cmake_minimum_required(VERSION 3.29)

project(sdl-testing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(CLI11 CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE SourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE IncludeFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hxx include/*.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

target_link_libraries(${PROJECT_NAME}
 PRIVATE
 $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
 $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
 CLI11::CLI11)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})
