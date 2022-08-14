cmake_minimum_required(VERSION 3.18.4)

project(Egienx)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG        docking
)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY "https://github.com/eliasdaler/imgui-sfml.git"
  GIT_TAG        master
)

FetchContent_Declare(
  entt
  URL            "https://github.com/skypjack/entt/archive/refs/tags/v3.10.1.zip"
)

FetchContent_Declare(
  box2d
  URL            "https://github.com/erincatto/box2d/archive/refs/tags/v2.4.1.zip"
)
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  spdlog
  URL            "https://github.com/gabime/spdlog/archive/refs/tags/v1.10.0.zip"
)

FetchContent_Declare(
  yaml-cpp
  URL            "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.zip"
)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY "https://github.com/SFML/SFML.git"
  GIT_TAG 2.5.1
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(imgui-sfml)
FetchContent_MakeAvailable(entt)
FetchContent_MakeAvailable(box2d)
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(yaml-cpp)
FetchContent_MakeAvailable(SFML)

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
	ImGui-SFML::ImGui-SFML
	PRIVATE
	"${YAML_CPP_LIBRARIES}"
	spdlog::spdlog
	box2d
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
	EnTT::EnTT
	)

