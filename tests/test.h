#ifndef CONSTEXPR_6502_TESTS_TEST_H
#define CONSTEXPR_6502_TESTS_TEST_H

#include <gtest/gtest.h>

#include <type_traits>

template <bool Value>
struct test_true : std::false_type {};

template <>
struct test_true<true> : std::true_type {};

template <bool Value>
inline constexpr bool test_true_v = test_true<Value>::value;

#define HK_TEST(ARG) EXPECT_TRUE(test_true_v<ARG>)

#endif
