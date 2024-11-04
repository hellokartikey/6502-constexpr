add_compile_options(-fconstexpr-ops-limit=4294967296 -fconstexpr-loop-limit=2147483647)

add_executable(fib fib.cpp)
target_link_libraries(fib PRIVATE 6502++)
