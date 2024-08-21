add_library(6502++)

target_link_libraries(
  6502++
  fmt::fmt
)

target_sources(6502++
PUBLIC
  common.h
  core.h
  cpu.h
  bit.h
)

add_executable(6502)

target_sources(
  6502
PRIVATE
  main.cpp
)

target_link_libraries(
  6502
  fmt::fmt
  6502++
)
