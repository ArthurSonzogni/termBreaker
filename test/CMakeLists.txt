enable_testing()

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1
  GIT_PROGRESS   ON
)

FetchContent_GetProperties(Catch2)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

include(CTest)
include(Catch)

add_library(catch_main OBJECT catch_main.cpp)
target_link_system_libraries(catch_main
  PUBLIC Catch2::Catch2
)

add_executable(termBreakerTests
  intro_unittests.cpp
  win_unittests.cpp
  main_menu_unittests.cpp
  game_unittests.cpp
)
target_link_system_libraries(termBreakerTests
  PRIVATE catch_main
  PRIVATE project_options
  PRIVATE project_warnings
  PRIVATE termBreakerLib
)

# automatically discover tests that are defined in catch based test files you
# can modify the unittests. Set TEST_PREFIX to whatever you want, or use
# different for different binaries
catch_discover_tests(termBreakerTests
  TEST_PREFIX "unittests."
  REPORTER xml
  OUTPUT_DIR .
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
)
