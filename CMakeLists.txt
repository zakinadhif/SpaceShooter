cmake_minimum_required(VERSION 3.18.4)

project(SpaceShooter)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS code/*.cpp)

add_executable("${PROJECT_NAME}" ${SOURCES})

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_compile_options(-Wall -Wextra -pedantic -Werror -Og)

# Set to use C++17
set_target_properties(
	"${PROJECT_NAME}"
	PROPERTIES
	CXX_STANDARD 17
	)

list(APPEND CMAKE_MODULE_PATH "/usr/local/cmake/Modules/")

find_package(box2d 2.4 REQUIRED)
find_package(Thor 2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(ImGui-SFML REQUIRED)

target_include_directories(
	"${PROJECT_NAME}" PRIVATE
	"${YAML_CPP_INCLUDE_DIR}"
	"${THOR_INCLUDE_DIR}"
	header
	)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
	"${YAML_CPP_LIBRARIES}"
	"${THOR_LIBRARY_RELEASE}"
	sfml-graphics
	sfml-window
	sfml-system
	spdlog::spdlog
	ImGui-SFML::ImGui-SFML
	box2d::box2d
	)
