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
#ifndef MVPAPPLICATION_HPP
#define MVPAPPLICATION_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <common/Meta.hpp>
#include <common/Partition.hpp>
#include <mvp/View.hpp>
#include <mvp/MVPHeap.hpp>
#include <new>
#include <cassert>

namespace touchgfx
{

class Presenter;

/**
 * @class MVPApplication MVPApplication.hpp mvp\MVPApplication.hpp
 *
 * @brief A specialization of the TouchGFX Application class.
 *
 *        A specialization of the TouchGFX Application class that provides the necessary glue
 *        for transitioning between presenter/view pairs.
 *
 *        It maintains a callback for transitioning and evaluates this at each tick.
 *
 * @see Application
 */
class MVPApplication : public Application
{
public:

    /**
     * @fn MVPApplication::MVPApplication()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    MVPApplication() :
        currentPresenter(0),
        pendingScreenTransitionCallback(0)
    {
        instance = this;
    }

    /**
     * @fn virtual MVPApplication::~MVPApplication()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~MVPApplication() { }

    /**
     * @fn void MVPApplication::handlePendingScreenTransition()
     *
     * @brief Handles the pending screen transition.
     *
     *        Delegates the work to evaluatePendingScreenTransition()
     */
    virtual void handlePendingScreenTransition()
    {
        evaluatePendingScreenTransition();
    }

protected:
    Presenter* currentPresenter;    ///< Pointer to the currently active presenter.

    GenericCallback<>* pendingScreenTransitionCallback; ///< Callback for screen transitions. Will be set to something valid when a transition request is made.

    /**
     * @fn void MVPApplication::evaluatePendingScreenTransition()
     *
     * @brief Evaluates the pending Callback instances.
     *
     *        Evaluates the pending Callback instances. If a callback is valid, it is executed
     *        and a Screen transition is executed.
     */
    void evaluatePendingScreenTransition()
    {
        if (pendingScreenTransitionCallback && pendingScreenTransitionCallback->isValid())
        {
            pendingScreenTransitionCallback->execute();
            pendingScreenTransitionCallback = 0;
        }
    }
};

/**
 * @fn static inline void prepareTransition(Screen** currentScreen, Presenter** currentPresenter, Transition** currentTrans)
 *
 * @brief Prepare screen transition. Private helper function for makeTransition. Do not use.
 *
 * @param [in] currentScreen    If non-null, the current screen.
 * @param [in] currentPresenter If non-null, the current presenter.
 * @param [in] currentTrans     If non-null, the current transaction.
 */
static inline void prepareTransition(Screen** currentScreen, Presenter** currentPresenter, Transition** currentTrans)
{
    Application::getInstance()->clearAllTimerWidgets();

    if (*currentTrans)
    {
        (*currentTrans)->tearDown();
    }
    if (*currentTrans)
    {
        (*currentTrans)->~Transition();
    }
    if (*currentScreen)
    {
        (*currentScreen)->tearDownScreen();
    }
    if (*currentPresenter)
    {
        (*currentPresenter)->deactivate();
    }
    if (*currentScreen)
    {
        (*currentScreen)->~Screen();
    }
    if (*currentPresenter)
    {
        (*currentPresenter)->~Presenter();
    }
}

/**
 * @fn static inline void finalizeTransition(Screen* newScreen, Presenter* newPresenter, Transition* newTransition)
 *
 * @brief Finalize screen transition. Private helper function for makeTransition. Do not use.
 *
 * @param [in] newScreen     If non-null, the new screen.
 * @param [in] newPresenter  If non-null, the new presenter.
 * @param [in] newTransition If non-null, the new transition.
 */
static inline void finalizeTransition(Screen* newScreen, Presenter* newPresenter, Transition* newTransition)
{
    newScreen->setupScreen();
    newPresenter->activate();
    newScreen->bindTransition(*newTransition);
    newTransition->init();

    Application::getInstance()->draw();
}

/**
 * @fn template< class ScreenType, class PresenterType, class TransType, class ModelType > PresenterType* makeTransition(Screen** currentScreen, Presenter** currentPresenter, MVPHeap& heap, Transition** currentTrans, ModelType* model)
 *
 * @brief Function for effectuating a screen transition (i.e. makes the requested new presenter/view
 *        pair active). Once this function has returned, the new screen has been transitioned
 *        to. Due to the memory allocation strategy of using the same memory area for all
 *        screens, the old view/presenter will no longer exist when this function returns.
 *
 *        Will properly clean up old screen (tearDownScreen, Presenter::deactivate) and call
 *        setupScreen/activate on new view/presenter pair. Will also make sure the view,
 *        presenter and model are correctly bound to each other.
 *
 * @tparam ScreenType    Class type for the View.
 * @tparam PresenterType Class type for the Presenter.
 * @tparam TransType     Class type for the Transition.
 * @tparam ModelType     Class type for the Model.
 * @param [in] currentScreen    Pointer to pointer to the current view.
 * @param [in] currentPresenter Pointer to pointer to the current presenter.
 * @param [in] heap             Reference to the heap containing the memory storage in which
 *                              to allocate.
 * @param [in] currentTrans     Pointer to pointer to the current transition.
 * @param [in] model            Pointer to model.
 *
 * @return Pointer to the new Presenter of the requested type. Incidentally it will be the same
 *         value as the old presenter due to memory reuse.
 */
template< class ScreenType, class PresenterType, class TransType, class ModelType  >
PresenterType* makeTransition(Screen** currentScreen, Presenter** currentPresenter, MVPHeap& heap, Transition** currentTrans, ModelType* model)
{
    assert(sizeof(ScreenType) <= heap.screenStorage.element_size() && "View allocation error: Check that all views are added to FrontendHeap::ViewTypes");
    assert(sizeof(PresenterType) <= heap.presenterStorage.element_size() && "Presenter allocation error: Check that all presenters are added to FrontendHeap::PresenterTypes");
    assert(sizeof(TransType) <= heap.transitionStorage.element_size() && "Transition allocation error: Check that all transitions are added to FrontendHeap::TransitionTypes");

    prepareTransition(currentScreen, currentPresenter, currentTrans);

    TransType* newTransition = new(&heap.transitionStorage.at< TransType >(0)) TransType;
    ScreenType* newScreen = new(&heap.screenStorage.at< ScreenType >(0)) ScreenType;
    PresenterType* newPresenter = new(&heap.presenterStorage.at< PresenterType >(0)) PresenterType(*newScreen);
    *currentTrans = newTransition;
    *currentPresenter = newPresenter;
    *currentScreen = newScreen;
    model->bind(newPresenter);
    newPresenter->bind(model);
    newScreen->bind(*newPresenter);

    finalizeTransition((Screen*) newScreen, (Presenter*) newPresenter, (Transition*) newTransition);

    return newPresenter;
}

} // namespace touchgfx
#endif // MVPAPPLICATION_HPP
