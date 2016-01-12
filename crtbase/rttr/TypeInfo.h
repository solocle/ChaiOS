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

#ifndef __RTTR_TYPEINFO_H__
#define __RTTR_TYPEINFO_H__

#include "base/CorePrerequisites.h"
#include "base/TypeTraits.h"
#include <vector>
#include <string>

namespace RTTR
{
class TypeInfo;

namespace impl
{
    /*!
     * \brief Register the type info for the given name
     *
     * \remark When a type with the given name is already registered,
     *         then the TypeInfo for the already registered type will be returned.
     *
     * \return A valid TypeInfo object.
     */
    RTTR_API TypeInfo registerOrGetType(const char* name, const TypeInfo& rawTypeInfo,
                                        const std::vector<TypeInfo>& info);
    template<typename T, bool>
    struct RawTypeInfo;
} // end namespace impl

/*!
 * This class holds the type information for any arbitrary object.
 * 
 * Every class or atomic data type can have an unique TypeInfo object.
 * With the help of this object you can compare unknown types for equality at runtime.
 *
 * Preparation
 * -----------
 * Before you can retrieve data from TypeInfo, you have to register your struct or class.
 * Therefore use the macro #RTTR_DECLARE_META_TYPE(Type) to make the type known to the TypeInfo system.
 * To actual execute the registration process use the macro #RTTR_DEFINE_META_TYPE(Type) in global namespace.
 *
 * This example shows a typical usage:
\code{.cpp}
  // MyStruct.h
  struct MyStruct
  {
    int i;
  };
  
  RTTR_DECLARE_META_TYPE(MyStruct)
  
  // MyStruct.cpp; in global namespace
  RTTR_DEFINE_META_TYPE(MyStruct)
\endcode
 * 
 * Retrieve %TypeInfo
 * ------------------
 * There are three static template member functions for retrieving the TypeInfo:
 *
 *  TypeInfo::get<T>()
      \code{.cpp}
        TypeInfo::get<int>() == TypeInfo::get<int>()  // yields to true
        TypeInfo::get<int>() == TypeInfo::get<bool>() // yields to false
      \endcode
 *  TypeInfo::get<T>(T* ptr)
     \code{.cpp}

        struct Base {};
        struct Derived : Base {};
        struct Other : Base {};
        Derived d;
        Base* base = &d;
        TypeInfo::get<Derived>() == TypeInfo::get(base) // yields to true
        
        Other o;
        base = &o;
        TypeInfo::get<Derived>() == TypeInfo::get(base) // yields to false
     \endcode
 *  TypeInfo::get<T>(T& ref)
     \code{.cpp}
        Derived d;
        TypeInfo::get<Derived>() == TypeInfo::get(d) // yields to true
     \endcode
 *
 */
class RTTR_API TypeInfo
{
    public:
        typedef uint16 TypeId;

        /*!
         * \brief Assigns a TypeInfo to another one.
         *
         */
        TypeInfo(const TypeInfo& other);

        /*!
         * \brief Assigns a TypeInfo to another one.
         *
         * \return A TypeInfo object.
         */
        TypeInfo& operator=(const TypeInfo& other);

        /*!
         * \brief Comparison operator for sorting the TypeInfo data according to some internal criterion.
         *
         * \return True if this TypeInfo is less than the \a other.
         */
        bool operator<(const TypeInfo& other) const;

        /*!
         * \brief Comparison operator for sorting the TypeInfo data according to some internal criterion.
         *
         * \return True if this TypeInfo is greater than the \a other.
         */
        bool operator>(const TypeInfo& other) const;

        /*!
         * \brief Comparison operator for sorting the TypeInfo data according to some internal criterion.
         *
         * \return True if this TypeInfo is greater than or equal to \a other.
         */
        bool operator>=(const TypeInfo& other) const;

        /*!
         * \brief Comparison operator for sorting the TypeInfo data according to some internal criterion.
         *
         * \return True if this TypeInfo is less than or equal to \a other.
         */
        bool operator<=(const TypeInfo& other) const;

        /*!
         * \brief Compares this TypeInfo with the \a other TypeInfo and returns true 
         *        if both describe the same type, otherwise returns false.
         *
         * \return True if both TypeInfo are equal, otherwise false.
         */
        bool operator==(const TypeInfo& other) const;

        /*!
         * \brief Compares this TypeInfo with the \a other TypeInfo and returns true 
         *        if both describe different types, otherwise returns false.
         *
         * \return True if both TypeInfo are \b not equal, otherwise false.
         */
        bool operator!=(const TypeInfo& other) const;

        /*!
         * \brief Returns the id of this type.
         *
         * \note This id is unique at process runtime,
         *       but the id can be changed every time the process is executed.
         *
         * \return The TypeInfo id.
         */
        TypeId getId() const;

        /*!
         * \brief Returns the unique and human-readable name of the type.
         *
         * \return TypeInfo name.
         */
        std::string getName() const;
        
        /*!
         * \brief Returns true if this TypeInfo is valid, that means the TypeInfo holds valid data to a type.
         * 
         * \return True if this TypeInfo is valid, otherwise false.
         */
        bool isValid() const;

        /*!
         * \brief Returns true if this TypeInfo is derived from the given type \a T, otherwise false.
         *
         * \return Returns true if this TypeInfo is a derived type from \a T, otherwise false.
         */
        template<typename T>
        bool isTypeDerivedFrom() const;

        /*!
         * \brief Returns a TypeInfo object which represent the raw type.
         *
         * That means a the type without any qualifiers (const and volatile) nor any pointer.
         *
         * \remark When the current TypeInfo is already the raw type, it will return an copy from itself.
         *
         * \return The TypeInfo of the raw type.
         */
        TypeInfo getRawType() const;

        /*!
         * \brief Returns a TypeInfo object for the given template type \a T.
         *
         * \return TypeInfo for the template type \a T.
         */
        template<typename T> 
        static TypeInfo get();

        /*!
         * \brief Returns a TypeInfo object for the given instance \a object.
         *
         * \remark If the type of the expression is a cv-qualified type, the result of the TypeInfo::get expression refers to a
         *         TypeInfo object representing the cv-unqualified type. 
         *
         * \return TypeInfo for an \a object of type \a T.
         */
        template<typename T> 
        static TypeInfo get(T* object);

        /*!
         * \brief Returns a TypeInfo object for the given instance \a object.
         *
         * \remark When TypeInfo::get is applied to a glvalue expression whose type is a polymorphic class type,
         *         the result refers to a TypeInfo object representing the type of the most derived object.
         \code{.cpp}
          class D { ... };
          D d1;
          const D d2;
          TypeInfo::get(d1)  == TypeInfo::get(d2);         // yields true
          TypeInfo::get<D>() == TypeInfo::get<const D>();  // yields true
          TypeInfo::get<D>() == TypeInfo::get(d2);         // yields true
          TypeInfo::get<D>() == TypeInfo::get<const D&>(); // yields true
         \endcode
         *
         * \return TypeInfo for an \a object of type \a T.
         */
        template<typename T> 
        static TypeInfo get(T& object);

    private:

        /*!
         * Constructs an empty and invalid TypeInfo object.
         */
        TypeInfo();

        /*!
         * \brief Constructs a valid TypeInfo object.
         *
         * \param id The unique id of the data type.
         */
        TypeInfo(TypeId id);

        /*!
         * \brief Returns true if this TypeInfo is derived from the given TypeInfo \a other, otherwise false.
         *
         * \return Returns true if this TypeInfo is a derived type from \a other, otherwise false.
         */
        bool isTypeDerivedFrom(const TypeInfo& other) const;

        RTTR_API friend TypeInfo impl::registerOrGetType(const char* name, const TypeInfo& rawTypeInfo,
                                                         const std::vector<TypeInfo>& info);
        template<typename T, bool>
        friend struct impl::RawTypeInfo;
    private:
        TypeId  m_id;
};

#ifdef DOXYGEN
/*!
 * This macro makes the type \p Type known to the RTTR::TypeInfo system.
 *
 * The macro should be placed directly under declaration of the custom class or struct of \p Type.
 * So RTTR::TypeInfo can access it in every translation unit (*.cpp file).
 *
 * When using a \p Type without this registration, it will lead to a compile time error 
 * with following message 'Type::TYPE_NOT_REGISTERED__USE_RTTR_DECLARE_META_TYPE'
 *
 * The following example will demonstrate the usage:
 \code{.cpp}
 // MyStruct.h
 struct MyStruct
 {
   bool visible;
 };

 RTTR_DECLARE_META_TYPE(MyStruct)
 \endcode
 *
 * When MyStruct is in a namespace, make sure you putt the macro outside the namespace,
 * otherwise RTTR::TypeInfo cannot access the \p Type.
 \code{.cpp}
 namespace NSMyStruct
 {
 }

 RTTR_DECLARE_META_TYPE(NSMyStruct::MyStruct)
 \endcode
 */
#define RTTR_DECLARE_META_TYPE(Type)


/*!
 * This macro makes the type \p Type immediately known to the RTTR::TypeInfo system.
 * Place this macro inside the global namespace of one translation unit. Normally it is placed
 * inside the corresponding cpp file of type \p Type.
 *
 * The reason for this macro is, to make sure there is no race condition during the registration process
 * and on the other side to make sure that the registration process has finished before main was executed.
 \code{.cpp}
 // MyStruct.cpp
 RTTR_DEFINE_META_TYPE(MyStruct)
 \endcode
 */
#define RTTR_DEFINE_META_TYPE(Type)
#endif

} // end namespace RTTR

#include "impl/TypeInfo_impl.h"

#endif // __RTTR_TYPEINFO_H__
