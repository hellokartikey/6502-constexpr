include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(fetch_fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 11.0.2
  GIT_PROGRESS TRUE
  SYSTEM
)

FetchContent_MakeAvailable(fetch_fmt)
