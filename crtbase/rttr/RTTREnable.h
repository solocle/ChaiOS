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

#ifndef __RTTR_RTTRENABLE_H__
#define __RTTR_RTTRENABLE_H__

#include "base/TypeTraits.h"
#include "base/StaticAssert.h"

namespace RTTR
{
namespace impl
{
//! A simple typelist
struct nil {};
template<class T, class U = nil> struct typelist
{
    typedef T head;
    typedef U tail;
};

/////////////////////////////////////////////////////////////////////////////////////
/*!
 * This trait checks if a given type T has a typedef named \a baseClassList.
 * has_base_class_list_impl::value is true, when it has this type, otherwise false.
 */
template <typename T>
class has_base_class_list_impl
{
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename C>
    static YesType& test(typename C::baseClassList*);

    template <typename>
    static NoType& test(...);

public:
    static const bool value = (sizeof(YesType) == sizeof(test<T>(0)));
};

/*!
 * If T has a typedef called \a 'baseClassList' then inherits from true_type, otherwise inherits from false_type. 
 */
template<class T, class = void>
struct has_base_class_list : Traits::integral_constant<bool, false> 
{};

template<class T>
struct has_base_class_list<T, typename enable_if<has_base_class_list_impl<T>::value>::type > : Traits::integral_constant<bool, true>
{};

/*!
 * This class fills from a given typelist the corresponding TypeInfo objects into a std::vector.
 */
template<class> 
struct TypeInfoFromBaseClassList;

template<>
struct TypeInfoFromBaseClassList<typelist<nil> > 
{
    static RTTR_INLINE void fill(std::vector<TypeInfo>&) 
    { 
    }
};

template<class T, class U> 
struct TypeInfoFromBaseClassList<typelist<T, U> > 
{
    static RTTR_INLINE void fill(std::vector<TypeInfo>& v)
    {
        RTTR_STATIC_ASSERT(has_base_class_list<T>::value, PARENT_CLASS_HAS_NO_BASE_CLASS_LIST_DEFINIED__USE_RTTR_ENABLE);
        v.push_back(MetaTypeInfo<T>::getTypeInfo());
        // retrieve also the TypeInfo of all base class of the base classes
        TypeInfoFromBaseClassList<typename T::baseClassList>::fill(v);
        TypeInfoFromBaseClassList<U>::fill(v);
    }
};

/*!
 * This helper trait returns a vector with TypeInfo object of all base classes.
 * When there is no typelist defined or the class has no base class, an empty vector is returned.
 */
template<class T>
struct BaseClasses
{
    private:
        // extract the info
        static RTTR_INLINE void retrieve_impl(std::vector<TypeInfo>& v, Traits::true_type)
        {
            TypeInfoFromBaseClassList<typename T::baseClassList>::fill(v);
        }

        // no type list defined
        static RTTR_INLINE void retrieve_impl(std::vector<TypeInfo>&, Traits::false_type)
        {
        }
    public:
        static RTTR_INLINE std::vector<TypeInfo> retrieve()
        {
            std::vector<TypeInfo> result;
            retrieve_impl(result, typename has_base_class_list<T>::type());
            return result;
        }
};

} // end namespace impl
} // end namespace RTTR

#define TYPE_LIST()             RTTR::impl::typelist<RTTR::impl::nil>
#define TYPE_LIST_1(A)          RTTR::impl::typelist<A, TYPE_LIST() >
#define TYPE_LIST_2(A,B)        RTTR::impl::typelist<A, TYPE_LIST_1(B) >
#define TYPE_LIST_3(A,B,C)      RTTR::impl::typelist<A, TYPE_LIST_2(B,C) >
#define TYPE_LIST_4(A,B,C,D)    RTTR::impl::typelist<A, TYPE_LIST_3(B,C,D) >
#define TYPE_LIST_5(A,B,C,D,E)  RTTR::impl::typelist<A, TYPE_LIST_4(B,C,D,E) >

#define RTTR_ENABLE() \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST() baseClassList;\
private:

#define RTTR_ENABLE_DERIVED_FROM(A) \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST_1(A) baseClassList;\
private:

#define RTTR_ENABLE_DERIVED_FROM_2(A,B) \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST_2(A,B) baseClassList;\
private:

#define RTTR_ENABLE_DERIVED_FROM_3(A,B,C) \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST_3(A,B,C) baseClassList;\
private:

#define RTTR_ENABLE_DERIVED_FROM_4(A,B,C,D) \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST_4(A,B,C,D) baseClassList;\
private:

#define RTTR_ENABLE_DERIVED_FROM_5(A,B,C,D,E) \
public:\
    virtual RTTR_INLINE RTTR::TypeInfo getTypeInfo() const { return RTTR::impl::getTypeInfoFromInstance(this); }  \
    typedef TYPE_LIST_5(A,B,C,D,E) baseClassList;\
private:

#endif // __RTTR_RTTRENABLE_H__
