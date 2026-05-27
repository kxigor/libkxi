cmake_minimum_required(VERSION 3.25)

project(libkxi LANGUAGES CXX)

# =========================================================
# 0. STANDALONE vs SUBPROJECT
# =========================================================
# PROJECT_IS_TOP_LEVEL is set by project() since CMake 3.21.
# When libkxi is consumed via add_subdirectory / FetchContent, this is FALSE
# and the dev-time machinery (app, tests, qa checks, sanitizers, coverage)
# stays disabled by default.

if(PROJECT_IS_TOP_LEVEL)
  set(LIBKXI_DEV_DEFAULT ON)
else()
  set(LIBKXI_DEV_DEFAULT OFF)
endif()

# =========================================================
# 1. OPTIONS (all prefixed LIBKXI_ to avoid clashing with the parent project)
# =========================================================
option(LIBKXI_BUILD_APP           "Build the example app"             ${LIBKXI_DEV_DEFAULT})
option(LIBKXI_BUILD_TESTS         "Build the GTest test suite"        ${LIBKXI_DEV_DEFAULT})
option(LIBKXI_ENABLE_FORMAT_CHECK "Register clang-format as CTest"    ${LIBKXI_DEV_DEFAULT})
option(LIBKXI_ENABLE_TIDY_CHECK   "Register clang-tidy as CTest"      ${LIBKXI_DEV_DEFAULT})
option(LIBKXI_ENABLE_COVERAGE     "Enable gcov/lcov coverage targets" OFF)

# Format/tidy/coverage only make sense alongside tests.
if(NOT LIBKXI_BUILD_TESTS)
  set(LIBKXI_ENABLE_FORMAT_CHECK OFF CACHE BOOL "Disabled: requires LIBKXI_BUILD_TESTS=ON" FORCE)
  set(LIBKXI_ENABLE_TIDY_CHECK   OFF CACHE BOOL "Disabled: requires LIBKXI_BUILD_TESTS=ON" FORCE)
  set(LIBKXI_ENABLE_COVERAGE     OFF CACHE BOOL "Disabled: requires LIBKXI_BUILD_TESTS=ON" FORCE)
endif()

# =========================================================
# 2. TOP-LEVEL-ONLY GLOBAL SETTINGS
# =========================================================
# These touch *global* CMake state. When libkxi is a subproject, the parent
# decides build type, exports and the C++ standard.

if(PROJECT_IS_TOP_LEVEL)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
  endif()
  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# =========================================================
# 3. THE LIBRARY (always built — that's the whole point)
# =========================================================
add_subdirectory(include)

# =========================================================
# 4. DEV-ONLY MACHINERY (skipped for subproject consumers)
# =========================================================
if(LIBKXI_BUILD_APP OR LIBKXI_BUILD_TESTS OR LIBKXI_ENABLE_COVERAGE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(ProjectOptions)
  include(ProjectSanitizers)
  include(ProjectCoverage)
endif()

if(LIBKXI_BUILD_APP)
  add_subdirectory(app)
endif()

if(LIBKXI_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
