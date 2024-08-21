include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(fetch_googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.15.0
  GIT_PROGRESS TRUE
  SYSTEM
)

FetchContent_MakeAvailable(fetch_googletest)
