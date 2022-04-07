find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(docopt CONFIG)

# Generic test that uses conan libs
add_executable(intro
  main.cpp
  ball.hpp
  ball.cpp
  brick.hpp
  brick.cpp
  contact_listener.hpp
  contact_listener.cpp
)
target_link_system_libraries(intro
  PRIVATE box2d
  PRIVATE ftxui::component
  PRIVATE ftxui::dom
  PRIVATE ftxui::screen
  PRIVATE project_options
  PRIVATE project_warnings
)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
