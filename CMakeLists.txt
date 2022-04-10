cmake_minimum_required(VERSION 3.16)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 17)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup dependencies:
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(_project_options
  URL https://github.com/aminya/project_options/archive/refs/tags/v0.20.0.zip
  URL_HASH SHA1=97b2d19b4b713e3fd2e6a6e93dd3622452a3195a
  GIT_PROGRESS ON
)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG aebde943523bb75da79ba45eb5b2e88d39a620e1
  GIT_PROGRESS ON
)

FetchContent_Declare(box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG v2.4.1
  GIT_PROGRESS ON
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(box2d)
if(NOT box2d_POPULATED)
  set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
  set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "")
  set(BOX2D_BUILD_DOCS OFF CACHE INTERNAL "")
  set(BOX2D_USER_SETTINGS OFF CACHE INTERNAL "")
  FetchContent_Populate(box2d)
  add_subdirectory(${box2d_SOURCE_DIR} ${box2d_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE TRUE CACHE BOOL "Enable 'developer mode'")

# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

# Set the project name and language
project(
  termRider
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/ArthurSonzogni/termRider"
  LANGUAGES CXX C)

add_executable(termBreaker
  src/ball.cpp
  src/ball.hpp
  src/board.cpp
  src/board.hpp
  src/brick.cpp
  src/brick.hpp
  src/contact_listener.cpp
  src/contact_listener.hpp
  src/main.cpp
  src/intro.cpp
  src/intro.cpp
)
target_include_directories(termBreaker
  PRIVATE src
)
target_link_system_libraries(termBreaker
  PRIVATE box2d
  PRIVATE ftxui::component
  PRIVATE ftxui::dom
  PRIVATE ftxui::screen
)


set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(BUILDING_MULTI_CONFIG)
  if(NOT CMAKE_BUILD_TYPE)
    # Make sure that all supported configuration types have their
    # associated conan packages available. You can reduce this
    # list to only the configuration types you use, but only if one
    # is not forced-set on the command line for VS
    message(TRACE "Setting up multi-config build types")
    set(CMAKE_CONFIGURATION_TYPES
        Debug
        Release
        RelWithDebInfo
        MinSizeRel
        CACHE STRING "Enabled build types" FORCE)
  else()
    message(TRACE "User chose a specific build type, so we are using that")
    set(CMAKE_CONFIGURATION_TYPES
        ${CMAKE_BUILD_TYPE}
        CACHE STRING "Enabled build types" FORCE)
  endif()
endif()

include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)


# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment the options to enable them:
if (EMSCRIPTEN)
else() 
  dynamic_project_options(
    PCH_HEADERS <vector> <string>
  )
  target_link_system_libraries(termBreaker
    PRIVATE project_options
    PRIVATE project_warnings
  )
  target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
endif()

# Adding the tests:
option(ENABLE_TESTING "Enable the tests" ON)
if(ENABLE_TESTING)
  message("Building Tests. Be sure to check out test/constexpr_tests for constexpr
testing")
  add_subdirectory(test)
endif()

option(ENABLE_FUZZING "Enable the fuzz tests" OFF)
if(ENABLE_FUZZING)
  message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
  add_subdirectory(fuzz_test)
endif()

if (EMSCRIPTEN)
  set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
  set(ENABLE_DEVELOPER_MODE FALSE CACHE BOOL "Enable 'developer mode'")
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD")

  # 32MB should be enough to run all the examples, in debug mode.
  target_link_options(component PUBLIC "SHELL: -s TOTAL_MEMORY=33554432")
  target_link_options(component PUBLIC "SHELL: -s ASSERTIONS=1")
  foreach(file
      "index.html"
      "run_webassembly.py")
    configure_file(${file} ${file})
  endforeach(file)
endif()


# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
  get_all_targets(all_targets)
  set_target_properties(${all_targets}
    PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT termBreaker)

# Add other targets that you want installed here, be default we just package the one executable
# we know we want to ship
package_project(TARGETS termBreaker)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
