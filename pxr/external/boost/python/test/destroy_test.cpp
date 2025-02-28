//
// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include "pxr/external/boost/python/detail/destroy.hpp"

#include <cassert>

int count;
int marks[] = {
    -1
    , -1, -1
    , -1, -1, -1, -1
    , -1
};
int* kills = marks;

struct foo
{
    foo() : n(count++) {}
    ~foo()
    {
        *kills++ = n;
    }
    int n;

    // This used to cause compiler errors with MSVC 9.0.
    foo& operator~();
    foo& T();
};

void assert_destructions(int n)
{
    for (int i = 0; i < n; ++i)
        assert(marks[i] == i);
    assert(marks[n] == -1);
}

int main()
{
    assert_destructions(0);
    
    foo* f1 = new foo;
    PXR_BOOST_NAMESPACE::python::detail::destroy_referent<foo const volatile&>(f1);
    assert_destructions(1);
    
    foo* f2 = new foo[2];
    typedef foo x[2];
    
    PXR_BOOST_NAMESPACE::python::detail::destroy_referent<x const&>(f2);
    assert_destructions(3);

    typedef foo y[2][2];
    x* f3 = new y;
    PXR_BOOST_NAMESPACE::python::detail::destroy_referent<y&>(f3);
    assert_destructions(7);

    return 0;
}
