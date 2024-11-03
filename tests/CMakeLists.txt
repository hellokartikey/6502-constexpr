add_executable(test_6502)

target_link_libraries(
  test_6502
  GTest::gtest_main
  6502++
)

target_sources(
  test_6502
PRIVATE
  memory.cpp
  cpu.cpp
)

include(GoogleTest)
gtest_discover_tests(test_6502)
