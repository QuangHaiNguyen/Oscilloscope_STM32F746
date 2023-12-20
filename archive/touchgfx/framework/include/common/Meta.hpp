/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#ifndef META_HPP
#define META_HPP

namespace touchgfx
{

/**
 * Template meta-programming tools are grouped in this namespace
 */
namespace meta
{
/**
 * @struct Nil Meta.hpp common/Meta.hpp
 *
 * @brief Nil-type, indicates the end of a TypeList.
 *
 *        Nil-type, indicates the end of a TypeList.
 */
struct Nil { };

/**
* @struct TypeList Meta.hpp common/Meta.hpp
 *
 * @brief TypeList, used for generating compile-time lists of types.
 *
 *        TypeList, used for generating compile-time lists of types.
 *
 * @tparam First Type of the first.
 * @tparam Next  Type of the next.
 */
template< typename First, typename Next >
struct TypeList
{
    typedef First first; ///< The first element in the TypeList
    typedef Next  next;  ///< Remainder of the TypeList
};

/**
 * @fn template < typename T1, typename T2, bool choose1 = (sizeof(T1) > sizeof(T2)) > struct type_max
 *
 * @brief Meta-function, selects the "maximum" type, i.e. the largest type.
 *
 *        Meta-function, selects the "maximum" type, i.e. the largest type.
 *
 * @tparam T1          Generic type parameter.
 * @tparam T2          Generic type parameter.
 * @tparam choose1     True if sizeof(T1) is larger than sizeof(T2).
 */
template < typename T1, typename T2, bool choose1 = (sizeof(T1) > sizeof(T2)) >
struct type_max
{
    typedef T1 type; ///< The resulting type (default case: sizeof(T1)>sizeof(T2))
};

/**
 * @struct type_max<T1,T2,false> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization for the case where sizeof(T2) >= sizeof(T1).
 *
 *        Specialization for the case where sizeof(T2) >= sizeof(T1).
 *
 * @tparam T1 Generic type parameter.
 * @tparam T2 Generic type parameter.
 */
template<typename T1, typename T2>
struct type_max<T1, T2, false>
{
    typedef T2 type; ///< The resulting type (default case: sizeof(T2)>=sizeof(T1))
};

/**
 * @struct select_type_maxsize Meta.hpp common/Meta.hpp
 *
 * @brief Meta-function signature, selects maximum type from TypeList.
 *
 *        Meta-function signature, selects maximum type from TypeList.
 *
 * @tparam T Generic type parameter.
 */
template<typename T> struct select_type_maxsize;

/**
 * @struct select_type_maxsize<TypeList<First,Next>> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization to dive into the list (inherits result from type_max).
 *
 * @tparam First Type of the first.
 * @tparam Next  Type of the next.
 */
template< typename First, typename Next >
struct select_type_maxsize< TypeList< First, Next > > : public type_max< First, typename select_type_maxsize< Next >::type >
{ };

/**
 * @struct select_type_maxsize<TypeList<First,Nil>> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization for loop termination (when type Nil encountered).
 *
 *        Specialization for loop termination (when type Nil encountered).
 *
 * @tparam First Type of the first.
 */
template< typename First >
struct select_type_maxsize< TypeList< First, Nil > >
{
    typedef First type;
};

/**
 * @struct list_join Meta.hpp common/Meta.hpp
 *
 * @brief Meta-function signature, joins typelist with type (or another typelist).
 *
 *        Meta-function signature, joins typelist with type (or another typelist).
 *
 * @tparam TList Type of the list.
 * @tparam T     Generic type parameter.
 */
template< typename TList, typename T >
struct list_join;

/**
 * @struct list_join<Nil,Nil> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization for termination.
 *
 *        Specialization for termination.
 */
template<>
struct list_join< Nil, Nil>
{
    typedef Nil result;
};

/**
 * @struct list_join<Nil,T> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization for "end-of-LHS", with RHS as type.
 *
 *        Specialization for "end-of-LHS", with RHS as type.
 *
 * @tparam T Generic type parameter.
 */
template< typename T >
struct list_join< Nil, T>
{
    typedef TypeList< T, Nil> result;
};

/**
 * @struct list_join<Nil,TypeList<First,Next>> Meta.hpp common/Meta.hpp
 *
 * @brief Specialization for "end-of-LHS", with RHS as a TypeList.
 *
 *        Specialization for "end-of-LHS", with RHS as a TypeList.
 *
 * @tparam First Type of the first.
 * @tparam Next  Type of the next.
 */
template< typename First, typename Next>
struct list_join< Nil, TypeList< First, Next > >
{
    typedef TypeList< First, Next > result;
};

/**
 * @struct list_join<TypeList<First,Next>,T> Meta.hpp common/Meta.hpp
 *
 * @brief Recursively joins a typelist (LHS) with a type or a type-list (RHS).
 *
 *        Recursively joins a typelist (LHS) with a type or a type-list (RHS).
 *
 * @tparam First Type of the first.
 * @tparam Next  Type of the next.
 * @tparam T     Generic type parameter.
 */
template< typename First, typename Next, typename T>
struct list_join< TypeList< First, Next >, T >
{
    typedef TypeList< First, typename list_join<Next, T>::result > result;
};
} // namespace meta

} // namespace touchgfx

#endif // META_HPP
