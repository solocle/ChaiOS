/************************************************************************************
*                                                                                   *
*   Copyright (c) 2013 Axel Menzel <info@axelmenzel.de>                             *
*                                                                                   *
*   This file is part of the Runtime Type Reflection System (RTTR).                 *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef __RTTR_TYPETRAITS_H__
#define __RTTR_TYPETRAITS_H__

#include "base/RawType.h"

#ifdef RTTR_USE_BOOST
    #include <boost/type_traits.hpp>
#else
    #include <type_traits>
#endif

namespace RTTR
{

#ifdef RTTR_USE_BOOST
    namespace Traits = boost;
#else
    #if RTTR_COMPILER == RTTR_COMPILER_MSVC
        #if RTTR_COMP_VER > 1500
            namespace Traits = std;
        #elif RTTR_COMP_VER == 1500
            namespace Traits = std::tr1;
        #else
            #error "No type_traits for this Visual Studio version available! Please upgrade Visual Studio or use Boost."
        #endif
    #else
        namespace Traits = std;
    #endif
#endif

// This is an own enable_if implementation, so we can reuse it with boost::integral_constant and std::integral_constant.
namespace impl
{
  template<bool B, class T = void>
  struct enable_if {};

  template<class T>
  struct enable_if<true, T> { typedef T type; };
} // end namespace impl

} // end namespace RTTR

#endif // __RTTR_TYPETRAITS_H__
