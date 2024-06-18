cmake_minimum_required (VERSION 3.1)
project (n88util)

# === Versioning

set(N88UTIL_MAJOR_VERSION 2)
set(N88UTIL_MINOR_VERSION 0)
set(N88UTIL_PATCH_VERSION 0)
set(N88UTIL_VERSION
  ${N88UTIL_MAJOR_VERSION}.${N88UTIL_MINOR_VERSION}.${N88UTIL_PATCH_VERSION})

configure_file (
    "${PROJECT_SOURCE_DIR}/n88util_version.h.in"
    "${PROJECT_BINARY_DIR}/n88util_version.h")

# Ensure that CMake behaves predictably
set (CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON)
set (CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON)
set (CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY ON)

# == Options and dependencies

option (ENABLE_TESTING "Build unit and other tests" OFF)
if (ENABLE_TESTING)
    enable_testing()
    find_package (GTest REQUIRED)
endif (ENABLE_TESTING)

option (ENABLE_RANGE_CHECKING "Enable array range checking" OFF)
if (ENABLE_RANGE_CHECKING)
    add_definitions (-DRANGE_CHECKING)
endif (ENABLE_RANGE_CHECKING)
mark_as_advanced (ENABLE_RANGE_CHECKING)

set (boost_components)

option (ENABLE_TimeStamp "Enable TimeStamp." OFF)
if (ENABLE_TimeStamp)
    set (boost_components ${boost_components} timer)
endif()

option (ENABLE_TrackingAllocator "Enable TrackingAllocator." OFF)
if (ENABLE_TrackingAllocator)
    set (boost_components ${boost_components} thread system)
    add_definitions (-DN88_TRACK_ALLOCATIONS)
endif()

find_package (Boost 1.56.0 COMPONENTS ${boost_components} REQUIRED)
if (MSVC)
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
endif (MSVC)
if (WIN32)
    # Disable auto-linking
    add_definitions (-DBOOST_ALL_NO_LIB)
endif()

include_directories (${Boost_INCLUDE_DIRS})
include_directories ("${CMAKE_SOURCE_DIR}/include")
# The following is for n88util_version.h
include_directories ("${PROJECT_BINARY_DIR}")

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)

# === Source code files

set (SRC
  source/binhex.cpp
  source/text.cpp)

if (ENABLE_TrackingAllocator)
    set (SRC ${SRC} source/TrackingAllocator.cpp)
endif()

# == Create library

include (GenerateExportHeader)

add_library (n88util ${SRC})
set_target_properties(n88util 
    PROPERTIES
        VERSION ${N88UTIL_VERSION}
        SOVERSION ${N88UTIL_MAJOR_VERSION}.${N88UTIL_MINOR_VERSION})

generate_export_header (n88util)

if (ENABLE_TimeStamp)
  target_link_libraries (n88util
		PRIVATE
			Boost::timer
	)
endif()

if (ENABLE_TrackingAllocator)
  target_link_libraries (n88util
		PRIVATE
			Boost::thread Boost::system
	)
endif()

# On Linux for glibc < 2.17 need also to link to rt
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include (cmake/ConfigureTests.cmake)
    get_glibc_version (GLIBC_VERSION)
    if (GLIBC_VERSION)
        if (GLIBC_VERSION VERSION_LESS "2.17")
            find_library (RT_LIBRARY rt)
            target_link_libraries (n88util PRIVATE rt)
        endif ()
    endif()
endif()

# === Install

install (TARGETS n88util
         EXPORT n88utilTargets
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)

install (DIRECTORY "${CMAKE_SOURCE_DIR}/include/n88util" DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/n88util_export.h DESTINATION include/n88util)

# === Testing

if (ENABLE_TESTING)
    add_subdirectory (tests)
endif (ENABLE_TESTING)

# === CMake config files for importing this project

# Set some locations for the install
set(INSTALL_LIB_DIR lib)
set(INSTALL_BIN_DIR bin)
set(INSTALL_INCLUDE_DIR include)
if(WIN32 AND NOT CYGWIN)
    set(INSTALL_CMAKE_DIR CMake)
else()
    set(INSTALL_CMAKE_DIR "lib/cmake/n88util-${N88UTIL_MAJOR_VERSION}.${N88UTIL_MINOR_VERSION}")
endif()

# Make relative paths absolute
foreach(p LIB BIN INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
      set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# Add all targets to the build-tree export set
export (TARGETS n88util
    NAMESPACE n88util::
    FILE "${PROJECT_BINARY_DIR}/n88utilTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE n88util)

# Create the n88utilConfig.cmake and n88utilConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}")
configure_file(n88utilConfig.cmake.in
    "${PROJECT_BINARY_DIR}/n88utilConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${N88UTIL_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(n88utilConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/n88utilConfig.cmake" @ONLY)
# ... for both
configure_file(n88utilConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/n88utilConfigVersion.cmake" @ONLY)

# Install the n88utilConfig.cmake and n88utilConfigVersion.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/n88utilConfig.cmake"
    "${PROJECT_BINARY_DIR}/n88utilConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT n88utilTargets
	NAMESPACE
		n88util::
	DESTINATION
  	"${INSTALL_CMAKE_DIR}"
	COMPONENT
		dev
)
