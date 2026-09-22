cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

project(sway-extensions)

find_package(Qt6 REQUIRED COMPONENTS Core DBus)

qt_standard_project_setup()

set(
	SOURCES
	src/main.cpp
	src/screensaver.cpp
)

qt_add_dbus_adaptor(
	SOURCES
	dbus/org.freedesktop.ScreenSaver.xml
	src/screensaver.h
	ScreenSaver
)
qt_add_dbus_interface(SOURCES dbus/kf6_org.kde.Solid.PowerManagement.PolicyAgent.xml policyagent)

qt_add_executable(
	sway-extensions
	${SOURCES}
)

target_link_libraries(sway-extensions PRIVATE Qt6::Core Qt6::DBus)
