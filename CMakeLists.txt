
############ Setup project and cmake
# Minimum cmake requirement. We should require a quite recent
# cmake for the dependency find macros etc. to be up to date.
cmake_minimum_required (VERSION 3.16)

############ Paths

set (WELLNMEA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (WELLNMEA_INCLUDE ${WELLNMEA_ROOT}/wellnmea)
set (WELLNMEA_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})
set (WELLNMEA_BIN ${WELLNMEA_BUILD_ROOT}/bin)
set (WELLNMEA_LIB ${WELLNMEA_BUILD_ROOT}/lib)

# CMake install step prefix. I assume linux users want the prefix to
# be the default /usr or /usr/local so this is only adjusted on Windows.
# This must be set prior to any call to project or it will not be read correctly.
# - Windows: Build the INSTALL project in your solution file.
# - Linux/OSX: make install.
if (WIN32)
    set (CMAKE_INSTALL_PREFIX "${WELLNMEA_ROOT}/install" CACHE PATH "")
endif ()

############ Project name and version
set (WELLNMEA_MAJOR_VERSION 0)
set (WELLNMEA_MINOR_VERSION 8)
set (WELLNMEA_PATCH_VERSION 2)
set (WELLNMEA_VERSION ${WELLNMEA_MAJOR_VERSION}.${WELLNMEA_MINOR_VERSION}.${WELLNMEA_PATCH_VERSION})

if(POLICY CMP0048)
  cmake_policy(GET CMP0048 _version_policy)
endif()

if(_version_allowed STREQUAL NEW)
  project (wellnmea VERSION ${WELLNMEA_VERSION})
else()
  project (wellnmea)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if (WIN32 AND NOT CYGWIN)
  set (DEF_INSTALL_CMAKE_DIR cmake)
else ()
  set (DEF_INSTALL_CMAKE_DIR lib/cmake/wellnmea)
endif ()
set (INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach (p INCLUDE CMAKE)
  set (var INSTALL_${p}_DIR)
  if (NOT IS_ABSOLUTE "${${var}}")
    set (${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif ()
endforeach ()

# Set CMake library search policy
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    cmake_policy (SET CMP0005 NEW)
endif ()

# Disable unnecessary build types
set (CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo;Debug" CACHE STRING "Configurations" FORCE)

# Include our cmake macros
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (CMakeHelpers)


############  Build customization

# Override from command line "CMake -D<OPTION>=TRUE/FALSE/0/1/ON/OFF"
option (BUILD_EXAMPLES "Build wellnmea examples." FALSE)
option (BUILD_TESTS "Build wellnmea tests." FALSE)

if (BUILD_TESTS OR BUILD_EXAMPLES)
    enable_testing ()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    find_package(benchmark CONFIG REQUIRED)
endif()

############ Add projects

# Add main library
add_subdirectory (wellnmea)

# Add examples
if (BUILD_EXAMPLES)
    include_subdirs ("examples")
endif ()

# Add tests
if (BUILD_TESTS)
    include_subdirs ("test")
    include_subdirs ("benchmark")
endif ()

print_used_build_config()

export (PACKAGE wellnmea)

include(CMakePackageConfigHelpers)
configure_package_config_file(wellnmea-config.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wellnmea-config.cmake"
  PATH_VARS INSTALL_INCLUDE_DIR
  INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}"
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file("${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wellnmea-configVersion.cmake"
  VERSION ${WELLNMEA_VERSION}
  COMPATIBILITY ExactVersion)

# Install the wellnmea-config.cmake and wellnmea-configVersion.cmake
install (FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wellnmea-config.cmake"
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wellnmea-configVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
