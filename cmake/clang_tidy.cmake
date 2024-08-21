find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)

set(CMAKE_CXX_CLANG_TIDY
  ${CLANG_TIDY_EXE}
  --config-file=${PROJECT_SOURCE_DIR}/.clang-tidy
)
