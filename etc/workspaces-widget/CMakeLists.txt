cmake_minimum_required(VERSION 4.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

project(sway-workspace-widget)

set(KF6_MIN_VERSION "6.30.0")

find_package(ECM 6 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMQmlModule)

find_package(Qt6 REQUIRED Core)

find_package(KF6 REQUIRED Config)
find_package(Plasma 6 REQUIRED)

add_subdirectory(state)
add_subdirectory(workspaces)
