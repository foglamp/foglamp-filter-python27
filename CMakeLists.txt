cmake_minimum_required(VERSION 2.4.0)

# Set the plugin name to build
project(python27)

# Supported options:
# -DFOGLAMP_INCLUDE
# -DFOGLAMP_LIB
# -DFOGLAMP_SRC
# -DFOGLAMP_INSTALL
#
# If no -D options are given and FOGLAMP_ROOT environment variable is set
# then FogLAMP libraries and header files are pulled from FOGLAMP_ROOT path.

set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

# Set plugin type (south, north, filter)
set(PLUGIN_TYPE "filter")

# Add here all needed FogLAMP libraries as list
set(NEEDED_FOGLAMP_LIBS common-lib plugins-common-lib filters-common-lib)

# Find source files
file(GLOB SOURCES *.cpp)

# Find Python.h header file
find_path(Python2.7_INCLUDE NAMES Python.h PATHS /usr/include/python2.7 /usr/include/python27)
if (NOT Python2.7_INCLUDE)
	message(WARNING
	" Python 2.7 header file 'Python.h' not found in /usr/include. \n"
	" Target '${PROJECT_NAME}' filter is not built.")
	return()
else()
	message(STATUS "Found Python 2.7 header file 'Python.h' in " ${Python2.7_INCLUDE})
endif()

# Find Python 2.7 library
find_library(FoundPython_2.7 NAMES python2.7 python27)
if (NOT FoundPython_2.7)
	message(ERROR "Python 2.7 library not found.")
	return()
else()
	message(STATUS "Found Python 2.7 library in " ${FoundPython_2.7})
endif()

# Find FogLAMP includes and libs, by including FindFogLAMP.cmak file
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(FogLAMP)
# If errors: make clean and remove Makefile
if (NOT FOGLAMP_FOUND)
	if (EXISTS "${CMAKE_BINARY_DIR}/Makefile")
		execute_process(COMMAND make clean WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
		file(REMOVE "${CMAKE_BINARY_DIR}/Makefile")
	endif()
	# Stop the build process
	message(FATAL_ERROR "FogLAMP plugin '${PROJECT_NAME}' build error.")
endif()
# On success, FOGLAMP_INCLUDE_DIRS and FOGLAMP_LIB_DIRS variables are set 

# Add ./include
include_directories(include)

# Add FogLAMP include dir(s)
include_directories(${FOGLAMP_INCLUDE_DIRS})

# Add other include paths this plugin needs
if (FOGLAMP_SRC)
	message(STATUS "Using third-party includes " ${FOGLAMP_SRC}/C/thirdparty/Simple-Web-Server)
	include_directories(${FOGLAMP_SRC}/C/thirdparty/Simple-Web-Server)
else()
	include_directories(${FOGLAMP_INCLUDE_DIRS}/Simple-Web-Server)
endif()

# Add Python 2.7 header files
include_directories(${Python2.7_INCLUDE})

# Add FogLAMP lib path
link_directories(${FOGLAMP_LIB_DIRS})

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Add FogLAMP library names
target_link_libraries(${PROJECT_NAME} ${NEEDED_FOGLAMP_LIBS})

# Add additional libraries
# Add Python 2.7 library
target_link_libraries(${PROJECT_NAME} -lpython2.7)

# Set the build version 
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

set(FOGLAMP_INSTALL "" CACHE INTERNAL "")
# Install library
if (FOGLAMP_INSTALL)
	message(STATUS "Installing ${PROJECT_NAME} in ${FOGLAMP_INSTALL}/plugins/${PLUGIN_TYPE}/${PROJECT_NAME}")
	install(TARGETS ${PROJECT_NAME} DESTINATION ${FOGLAMP_INSTALL}/plugins/${PLUGIN_TYPE}/${PROJECT_NAME})
endif()
