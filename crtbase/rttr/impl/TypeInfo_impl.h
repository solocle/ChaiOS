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

#include "base/StaticAssert.h"

namespace RTTR
{

RTTR_INLINE TypeInfo::TypeInfo()
:   m_id(0)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE TypeInfo::TypeInfo(TypeInfo::TypeId id)
:   m_id(id)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE TypeInfo::TypeInfo(const TypeInfo& other)
:   m_id(other.m_id)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE TypeInfo& TypeInfo::operator=(const TypeInfo& other)
{
    m_id = other.m_id;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator<(const TypeInfo& other) const
{
    return (m_id < other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator>(const TypeInfo& other) const
{
    return (m_id > other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator>=(const TypeInfo& other) const
{
    return (m_id >= other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator<=(const TypeInfo& other) const
{
    return (m_id <= other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator==(const TypeInfo& other) const
{
    return (m_id == other.m_id); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::operator!=(const TypeInfo& other) const
{
    return (m_id != other.m_id); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE TypeInfo::TypeId TypeInfo::getId() const 
{ 
    return m_id;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool TypeInfo::isValid() const
{
    return (m_id != 0); 
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace impl
{

template <typename T>
struct MetaTypeInfo
{
    enum { Defined = 0 };
    static TypeInfo getTypeInfo()
    {
        // when you get this error, you have to declare first the type with this macro
        return T::TYPE_NOT_REGISTERED__USE_RTTR_DECLARE_META_TYPE();
    }
};

template <typename T>
struct AutoRegisterType;

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE TypeInfo getTypeInfoFromInstance(const T*)
{
    return impl::MetaTypeInfo<T>::getTypeInfo();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = Traits::is_same<T, typename raw_type<T>::type >::value>
struct RawTypeInfo
{
    static RTTR_INLINE TypeInfo get() { return TypeInfo(); } // we have to return an empty TypeInfo, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct RawTypeInfo<T, false>
{
    static RTTR_INLINE TypeInfo get() { return MetaTypeInfo<typename raw_type<T>::type>::getTypeInfo(); }
};

/////////////////////////////////////////////////////////////////////////////////

/*! 
 * Determine if the given type \a T has the method
 * 'TypeInfo getTypeInfo() const' declared.
 */
template <typename T>
class has_getTypeInfo_func_impl
{
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename U, RTTR::TypeInfo (U::*)() const> 
    class check { };

    template <typename C>
    static YesType& f(check<C, &C::getTypeInfo>*);

    template <typename C>
    static NoType& f(...);

public:
    static const bool value = (sizeof(f<typename raw_type<T>::type>(0)) == sizeof(YesType));
};

/*!
 * If T has a member function 'TypeInfo getTypeInfo() const;' then inherits from true_type, otherwise inherits from false_type. 
 */
template<class T, class = void>
struct has_getTypeInfo_func : Traits::integral_constant<bool, false> 
{};

template<class T>
struct has_getTypeInfo_func<T, typename enable_if<has_getTypeInfo_func_impl<T>::value>::type > : Traits::integral_constant<bool, true>
{};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool>
struct TypeInfoFromInstance;

//! Specialization for retrieving the TypeInfo from the instance directly
template<typename T>
struct TypeInfoFromInstance<T, false> // the typeInfo function is not available
{
    static RTTR_INLINE TypeInfo get(T&)
    {
        return impl::MetaTypeInfo<typename Traits::remove_cv<typename Traits::remove_reference<T>::type>::type>::getTypeInfo();
    }
};

//! Specialization for retrieving the TypeInfo from the instance directly
template<typename T>
struct TypeInfoFromInstance<T, true>
{
    static RTTR_INLINE TypeInfo get(T& object)
    {
        return object.getTypeInfo();
    }
};

} // end namespace impl

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE TypeInfo TypeInfo::get()
{
    return impl::MetaTypeInfo<typename Traits::remove_cv<typename Traits::remove_reference<T>::type>::type>::getTypeInfo();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE TypeInfo TypeInfo::get(T*)
{
    return impl::MetaTypeInfo<T*>::getTypeInfo();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE TypeInfo TypeInfo::get(T& object)
{
    return impl::TypeInfoFromInstance<T, impl::has_getTypeInfo_func<T>::value>::get(object);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool TypeInfo::isTypeDerivedFrom() const
{
    return isTypeDerivedFrom(impl::MetaTypeInfo<T>::getTypeInfo());
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace RTTR

#define RTTR_CAT_IMPL(a,b) a##b
#define RTTR_CAT(a,b) RTTR_CAT_IMPL(a,b)

#define RTTR_DECLARE_META_TYPE(T)                                                               \
namespace RTTR                                                                                  \
{                                                                                               \
    namespace impl                                                                              \
    {                                                                                           \
        template<>                                                                              \
        struct MetaTypeInfo< T >                                                                \
        {                                                                                       \
            enum { Defined = 1 };                                                               \
            static RTTR_INLINE RTTR::TypeInfo getTypeInfo()                                     \
            {                                                                                   \
                static const TypeInfo val = registerOrGetType(#T, RawTypeInfo<T>::get(),        \
                                                              BaseClasses<T>::retrieve());      \
                return val;                                                                     \
            }                                                                                   \
        };                                                                                      \
    }                                                                                           \
} // end namespace RTTR

#define RTTR_DEFINE_META_TYPE(T)                                                                \
namespace RTTR                                                                                  \
{                                                                                               \
    namespace impl                                                                              \
    {                                                                                           \
        template<>                                                                              \
        struct AutoRegisterType<T>                                                              \
        {                                                                                       \
            AutoRegisterType()                                                                  \
            {                                                                                   \
                MetaTypeInfo<T>::getTypeInfo();                                                 \
            }                                                                                   \
        };                                                                                      \
    }                                                                                           \
}                                                                                               \
static const RTTR::impl::AutoRegisterType<T> RTTR_CAT(autoRegisterType,__COUNTER__);

#define RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(T) RTTR_DECLARE_META_TYPE(T)                   \
                                                    RTTR_DECLARE_META_TYPE(T*)                  \
                                                    RTTR_DECLARE_META_TYPE(const T*)

#define RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS(T) RTTR_DEFINE_META_TYPE(T)                     \
                                                   RTTR_DEFINE_META_TYPE(T*)                    \
                                                   RTTR_DEFINE_META_TYPE(const T*)
