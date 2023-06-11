include(CTest)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2
  GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)

add_executable(termBreakerTests
  intro_unittests.cpp
  win_unittests.cpp
  main_menu_unittests.cpp
  game_unittests.cpp
)
target_link_libraries(termBreakerTests
  PRIVATE Catch2::Catch2WithMain
  PRIVATE termBreakerLib
)

catch_discover_tests(termBreakerTests)
