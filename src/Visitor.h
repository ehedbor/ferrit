#pragma once

#include <any>


/**
 * Generates an implementation for a visitor's accept method.
 *
 * @param visitorType name of the visitor class
 * @param methodCall name of the visit method, ommiting the initial "visit" portion
 */
#define MAKE_VISITABLE(visitorType, methodCall)                                \
    VisitResult accept(visitorType &visitor) const override {                  \
        return visitor.visit##methodCall(*this);                               \
    }

/**
 * Generates a base-class definition of an accept method.
 */
#define MAKE_BASE_VISITABLE(visitorType) \
    virtual VisitResult accept(visitorType &visitor) const = 0;

namespace ferrit {
    /**
     * The result of a visitXXX() function.
     *
     * @note \c std::any has to be used for visitor return types because C++
     * does not permit templated virtual functions, and there is no supertype
     * in common between different visitor implementations.
     */
    using VisitResult = std::any;
}
