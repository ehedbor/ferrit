#pragma once

#include <result.hpp>

#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

/**
 * Evaluates the given \c expression (which should return a <tt>cpp::result</tt>).
 * If the value is present, it is bound to a new variable named <tt>variable</tt>.
 * Otherwise, the current function will return a new <tt>cpp::result</tt>
 * containing the error.
 */
#define TRY(variable, expression)                                              \
    auto CONCAT(_TRY_, __LINE__) = (expression);                               \
    if (!CONCAT(_TRY_, __LINE__)) {                                            \
        return cpp::fail(CONCAT(_TRY_, __LINE__).error());                     \
    }                                                                          \
    auto (variable) = std::move(*CONCAT(_TRY_, __LINE__))

/**
 * Evaluates the given \c expression (which should return a <tt>cpp::result</tt>).
 * If the value is not present, the current function will return a new
 * <tt>cpp::result</tt> containing the error.
 */
#define EXPECT(expression)                                                     \
    do {                                                                       \
        auto CONCAT(_EXPECT_, __LINE__) = (expression);                        \
        if (!CONCAT(_EXPECT_, __LINE__)) {                                     \
            return cpp::fail(CONCAT(_EXPECT_, __LINE__).error());              \
        }                                                                      \
    } while (false)

