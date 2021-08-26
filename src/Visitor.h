#pragma once

#include <any>


namespace es {
    /**
     * The result of a visitXXX() function.
     *
     * @note \c std::any has to be used for visitor return types because C++
     * does not permit templated virtual functions, and there is no supertype
     * in common between different visitor implementations.
     */
    using VisitResult = std::any;
}
