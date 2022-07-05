cmake_minimum_required(VERSION 3.18.4)

project(Egienx)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(box2d 2.4 REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(EnTT REQUIRED)

# Egienx
file(GLOB_RECURSE EGIENX_SOURCES CONFIGURE_DEPENDS src/Egienx/*.cpp)

add_library(egienx ${EGIENX_SOURCES})

target_compile_options(
	egienx PRIVATE
	-Wall -Wextra -pedantic -Og
	)

target_compile_features(
	egienx PUBLIC
	cxx_std_20
	)

target_include_directories(
	egienx
	PRIVATE
	"${YAML_CPP_INCLUDE_DIR}"
	PUBLIC
	src/Egienx
	)

target_link_libraries(
	egienx
	PUBLIC
	sfml-graphics
	sfml-window
	sfml-system
	PRIVATE
	"${YAML_CPP_LIBRARIES}"
	spdlog::spdlog
	ImGui-SFML::ImGui-SFML
	box2d::box2d
	EnTT::EnTT
	)

# Egnitor
file(GLOB_RECURSE EGNITOR_SOURCES CONFIGURE_DEPENDS src/Egnitor/*.cpp)

add_executable(egnitor ${EGNITOR_SOURCES})

target_compile_options(
	egnitor PRIVATE
	-Wall -Wextra -pedantic -Og
	)

target_compile_features(
	egnitor PUBLIC
	cxx_std_20
	)

target_include_directories(
	egnitor
	PRIVATE
	src/Egnitor
	)

target_link_libraries(
	egnitor
	PRIVATE
	egienx
	)

