#pragma once

#include <result.hpp>

#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define TRY(variable, expression)                                              \
    auto CONCAT(_TRY_, __LINE__) = (expression);                               \
    if (!CONCAT(_TRY_, __LINE__)) {                                            \
        return cpp::fail(CONCAT(_TRY_, __LINE__).error());                     \
    }                                                                          \
    auto (variable) = std::move(*CONCAT(_TRY_, __LINE__))


#define EXPECT(expression)                                                     \
    do {                                                                       \
        auto CONCAT(_EXPECT_, __LINE__) = (expression);                        \
        if (!CONCAT(_EXPECT_, __LINE__)) {                                     \
            return cpp::fail(CONCAT(_EXPECT_, __LINE__).error());              \
        }                                                                      \
    } while (false)

