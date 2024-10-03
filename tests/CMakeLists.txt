add_executable(test_6502)

target_link_libraries(
  test_6502
  GTest::gtest_main
  6502++
)

target_include_directories(
  test_6502
PRIVATE
  ${PROJECT_SOURCE_DIR}/src
)

target_sources(
  test_6502
PRIVATE
  memory.cpp
  cpu.cpp
)

include(GoogleTest)
gtest_discover_tests(test_6502)
