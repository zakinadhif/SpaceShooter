cmake_minimum_required(VERSION 3.18.4)

project(SpaceShooter)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable("${PROJECT_NAME}" ${SOURCES})

add_compile_options(-Wall -Wextra -pedantic -Werror -Og)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(box2d 2.4 REQUIRED)
find_package(Thor 2 REQUIRED)
find_package(TGUI 0.9 REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(EnTT REQUIRED)
find_package(CDT REQUIRED CONFIG)

target_compile_features(
	${PROJECT_NAME} PRIVATE
	cxx_std_20
	)

target_include_directories(
	"${PROJECT_NAME}" PRIVATE
	"${YAML_CPP_INCLUDE_DIR}"
	"${THOR_INCLUDE_DIR}"
	src
	)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
	"${YAML_CPP_LIBRARIES}"
	"${THOR_LIBRARY_RELEASE}"
	sfml-graphics
	sfml-window
	sfml-system
	tgui
	spdlog::spdlog
	ImGui-SFML::ImGui-SFML
	box2d::box2d
	EnTT::EnTT
	CDT
	)
