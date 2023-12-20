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
#ifndef CALLBACK_HPP
#define CALLBACK_HPP

namespace touchgfx
{
/**
 * @class GenericCallback Callback.hpp touchgfx/Callback.hpp
 *
 * @brief GenericCallback is the base class for callbacks.
 *
 *        GenericCallback is the base class for callbacks. @see Callback for an explanation of
 *        callbacks.
 *
 *        The reason this base class exists, is that a normal Callback requires the class type
 *        where the callback function resides to be known. This is problematic for ie.
 *        framework widgets like AbstractButton, on which it should be possible to register a
 *        callback on object types that are user-specific and thus unknown to AbstractButton.
 *        This is solved by having AbstractButton contain a pointer to a GenericCallback
 *        instead. This pointer must then be initialized to point on an instance of Callback,
 *        created by the user, which is initialized with the appropriate object type.
 *
 * @note As with Callback, this class exists in three versions to support callback functions
 *       taking zero, one or two arguments.
 *
 * @tparam T1 The type of the first argument in the member function, or void if none.
 * @tparam T2 The type of the second argument in the member function, or void if none.
 */
template <class T1 = void, class T2 = void>
class GenericCallback
{
public:

    /**
     * @fn virtual ~GenericCallback()
     *
     * @brief Destructor.
     *
     *        Empty virtual destructor.
     */
    virtual ~GenericCallback()
    {
    }

    /**
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     *
     * @param val1 This value will be passed as the first argument in the function call.
     * @param val2 This value will be passed as the second argument in the function call.
     */
    virtual void execute(T1 val1, T2 val2) = 0;

    /**
     * @fn virtual bool isValid() const = 0;
     *
     * @brief Function to check whether the Callback has been initialized with values.
     *
     *        Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const = 0;
};

/**
 * @class GenericCallback<T1,void> Callback.hpp touchgfx/Callback.hpp
 *
 * @brief GenericCallback is the base class for callbacks.
 *
 *        GenericCallback is the base class for callbacks. @see Callback for an explanation of
 *        callbacks.
 *
 *        The reason this base class exists, is that a normal Callback requires the class type
 *        where the callback function resides to be known. This is problematic for ie.
 *        framework widgets like AbstractButton, on which it should be possible to register a
 *        callback on object types that are user-specific and thus unknown to AbstractButton.
 *        This is solved by having AbstractButton contain a pointer to a GenericCallback
 *        instead. This pointer must then be initialized to point on an instance of Callback,
 *        created by the user, which is initialized with the appropriate object type.
 *
 * @note As with Callback, this class exists in three versions to support callback functions
 *       taking zero, one or two arguments.
 *
 * @tparam T1 The type of the first argument in the member function, or void if none.
 */
template <class T1>
class GenericCallback<T1, void>
{
public:

    /**
     * @fn virtual ~GenericCallback()
     *
     * @brief Destructor.
     *
     *        Empty virtual destructor.
     */
    virtual ~GenericCallback()
    {
    }

    /**
     * @fn virtual void execute(T1 val1) = 0;
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     *
     * @param val1 This value will be passed as the first argument in the function call.
     */
    virtual void execute(T1 val1) = 0;

    /**
     * @fn virtual bool isValid() const = 0;
     *
     * @brief Function to check whether the Callback has been initialized with values.
     *
     *        Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const = 0;
};

/**
 * @class GenericCallback<void> Callback.hpp touchgfx/Callback.hpp
 *
 * @brief GenericCallback is the base class for callbacks.
 *
 *        GenericCallback is the base class for callbacks. @see Callback for an explanation of
 *        callbacks.
 *
 *        The reason this base class exists, is that a normal Callback requires the class type
 *        where the callback function resides to be known. This is problematic for ie.
 *        framework widgets like AbstractButton, on which it should be possible to register a
 *        callback on object types that are user-specific and thus unknown to AbstractButton.
 *        This is solved by having AbstractButton contain a pointer to a GenericCallback
 *        instead. This pointer must then be initialized to point on an instance of Callback,
 *        created by the user, which is initialized with the appropriate object type.
 *
 * @note As with Callback, this class exists in three versions to support callback functions
 *       taking zero, one or two arguments.
 */
template <>
class GenericCallback<void>
{
public:

    /**
     * @fn virtual ~GenericCallback()
     *
     * @brief Destructor.
     *
     *        Empty virtual destructor.
     */
    virtual ~GenericCallback()
    {
    }

    /**
     * @fn virtual void execute() = 0;
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     */
    virtual void execute() = 0;

    /**
     * @fn virtual bool isValid() const = 0;
     *
     * @brief Function to check whether the Callback has been initialized with values.
     *
     *        Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const = 0;
};

/**
 * @struct Callback Callback.hpp touchgfx/Callback.hpp
 *
 * @brief A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        It is used for registering callbacks between widgets. For instance, a Button can be
 *        configured to call a member function when it is clicked.
 *
 *        The class is templated in order to provide the class type of the object in which the
 *        member function resides, and the argument types of the function to call.
 *
 *        The Callback class exists in three versions, for supporting member functions with 0,
 *        1 or 2 arguments. The compiler will infer which type to use automatically.
 *
 *        Example button_example shows how to use callbacks.
 *
 * @note The member function to call must return void. The function can have zero, one or two
 *       arguments of any type.
 *
 * @tparam dest_type The type of the class in which the member function resides.
 * @tparam T1        The type of the first argument in the member function, or void if none.
 * @tparam T2        The type of the second argument in the member function, or void if none.
 */
template <class dest_type, typename T1 = void, typename T2 = void>
struct Callback : public GenericCallback<T1, T2>
{
    /**
     * @fn Callback()
     *
     * @brief Default constructor.
     *
     *        Initializes an empty callback.
     */
    Callback() : pobject(0), pmemfun_2(0)
    {
    }

    /**
     * @fn Callback(dest_type* pobject, void (dest_type::*pmemfun_2)(T1, T2))
     *
     * @brief Initializes a Callback with an object and a pointer to the member function in that
     *        object to call.
     *
     *        Initializes a Callback with an object and a pointer to the member function in
     *        that object to call.
     *
     * @param [in] pobject   Pointer to the object on which the function should be called.
     * @param [in] pmemfun_2 Address of member function. This is the version where function
     *                       takes two arguments.
     */
    Callback(dest_type* pobject, void (dest_type::*pmemfun_2)(T1, T2))
    {
        this->pobject = pobject;
        this->pmemfun_2 = pmemfun_2;
    }

    /**
     * @fn virtual void execute(T1 t1, T2 t2)
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     *
     * @param t1 This value will be passed as the first argument in the function call.
     * @param t2 This value will be passed as the second argument in the function call.
     */
    virtual void execute(T1 t1, T2 t2)
    {
        (pobject->*pmemfun_2)(t1, t2);
    }

    /**
     * @fn virtual bool isValid() const
     *
     * @brief Function to check whether the Callback has been initialized with values.
     *
     *        Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const
    {
        return (pobject != 0) && (pmemfun_2 != 0);
    }

private:
    dest_type* pobject;
    void (dest_type::* pmemfun_2)(T1, T2);
};

/**
 * @struct Callback<dest_type,T1,void> Callback.hpp touchgfx/Callback.hpp
 *
 * @brief A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        It is used for registering callbacks between widgets. For instance, a Button can be
 *        configured to call a member function when it is clicked.
 *
 *        The class is templated in order to provide the class type of the object in which the
 *        member function resides, and the argument types of the function to call.
 *
 *        The Callback class exists in three versions, for supporting member functions with 0,
 *        1 or 2 arguments. The compiler will infer which type to use automatically.
 *
 *        Example button_example shows how to use callbacks.
 *
 * @note The member function to call must return void. The function can have zero, one or two
 *       arguments of any type.
 *
 * @tparam dest_type The type of the class in which the member function resides.
 * @tparam T1        The type of the first argument in the member function, or void if none.
 */
template <class dest_type, typename T1>
struct Callback<dest_type, T1, void> : public GenericCallback<T1>
{
    /**
     * @fn Callback()
     *
     * @brief Default constructor.
     *
     *        Initializes an empty callback.
     */
    Callback() : pobject(0), pmemfun_1(0) { }

    /**
     * @fn Callback(dest_type* pobject, void (dest_type::*pmemfun_1)(T1))
     *
     * @brief Initializes a Callback with an object and a pointer to the member function in that
     *        object to call.
     *
     *        Initializes a Callback with an object and a pointer to the member function in
     *        that object to call.
     *
     * @param [in] pobject   Pointer to the object on which the function should be called.
     * @param [in] pmemfun_1 Address of member function. This is the version where function
     *                       takes one argument.
     */
    Callback(dest_type* pobject, void (dest_type::*pmemfun_1)(T1))
    {
        this->pobject = pobject;
        this->pmemfun_1 = pmemfun_1;
    }

    /**
     * @fn virtual void execute(T1 t1)
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     *
     * @param t1 This value will be passed as the first argument in the function call.
     *
     * @see isValid()
     */
    virtual void execute(T1 t1)
    {
        (pobject->*pmemfun_1)(t1);
    }

    /**
     * @fn virtual bool isValid() const
     *
     * @brief Query if this object is valid.
     *
     *        Query if this object is valid.
     *
     * @return true if valid, false if not.
     */
    virtual bool isValid() const
    {
        return (pobject != 0) && (pmemfun_1 != 0);
    }

private:
    dest_type* pobject;
    void (dest_type::* pmemfun_1)(T1);

};

/**
 * @struct Callback<dest_type,void,void> Callback.hpp touchgfx/Callback.hpp
 *
 * @brief A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *        It is used for registering callbacks between widgets. For instance, a Button can be
 *        configured to call a member function when it is clicked.
 *
 *        The class is templated in order to provide the class type of the object in which the
 *        member function resides, and the argument types of the function to call.
 *
 *        The Callback class exists in three versions, for supporting member functions with 0,
 *        1 or 2 arguments. The compiler will infer which type to use automatically.
 *
 *        Example button_example shows how to use callbacks.
 *
 * @note The member function to call must return void. The function can have zero, one or two
 *       arguments of any type.
 *
 * @tparam dest_type The type of the class in which the member function resides.
 */
template <class dest_type>
struct Callback<dest_type, void, void> : public GenericCallback<>
{
    /**
     * @fn Callback()
     *
     * @brief Default constructor.
     *
     *        Initializes an empty callback.
     */
    Callback() : pobject(0), pmemfun_0(0) { }

    /**
     * @fn Callback(dest_type* pobject, void (dest_type::*pmemfun_0)())
     *
     * @brief Initializes a Callback with an object and a pointer to the member function in that
     *        object to call.
     *
     *        Initializes a Callback with an object and a pointer to the member function in
     *        that object to call.
     *
     * @param [in] pobject   Pointer to the object on which the function should be called.
     * @param [in] pmemfun_0 Address of member function. This is the version where function
     *                       takes zero arguments.
     */
    Callback(dest_type* pobject, void (dest_type::*pmemfun_0)())
    {
        this->pobject = pobject;
        this->pmemfun_0 = pmemfun_0;
    }

    /**
     * @fn virtual void execute()
     *
     * @brief Calls the member function.
     *
     *        Calls the member function. Do not call execute unless isValid() returns true (ie.
     *        a pointer to the object and the function has been set).
     */
    virtual void execute()
    {
        (pobject->*pmemfun_0)();
    }

    /**
     * @fn virtual bool isValid() const
     *
     * @brief Function to check whether the Callback has been initialized with values.
     *
     *        Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const
    {
        return (pobject != 0) && (pmemfun_0 != 0);
    }

private:
    dest_type* pobject;
    void (dest_type::* pmemfun_0)();

};

} // namespace touchgfx

#endif // CALLBACK_HPP
