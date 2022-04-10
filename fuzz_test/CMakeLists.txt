# A fuzz test runs until it finds an error. This particular one is going to rely on libFuzzer.
add_executable(fuzz_test
  fuzz_test.cpp
)
target_link_libraries(fuzz_test
  PRIVATE project_options
  PRIVATE project_warnings
  PRIVATE -coverage
  PRIVATE -fsanitize=fuzzer,undefined,address
)
target_compile_options(fuzz_test
  PRIVATE -fsanitize=fuzzer,undefined,address
)

# Allow short runs during automated testing to see if something new breaks
set(FUZZ_RUNTIME 10 CACHE STRING
  "Number of seconds to run fuzz tests during ctest run")

add_test(
  NAME fuzz_tester_run
  COMMAND fuzz_test -max_total_time=${FUZZ_RUNTIME}
)
