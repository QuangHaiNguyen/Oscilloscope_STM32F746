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
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>
#include <touchgfx/UIEventListener.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/transitions/Transition.hpp>

namespace touchgfx
{

class Drawable;
class Screen;

/**
 * @class Application Application.hpp touchgfx/Application.hpp
 *
 * @brief The Application class is the main interface for manipulating screen contents.
 *
 *        The Application class is the main interface for manipulating screen contents. It
 *        holds a pointer to the currently displayed Screen, and delegates draw requests and
 *        events to that Screen. Additionally it contains some global application settings.
 *
 *        A user-defined application subclass can be defined to override standard functionality.
 *
 * @see UIEventListener
 */
class Application : public UIEventListener
{
public:

    /**
     * @fn static Application* Application::getInstance();
     *
     * @brief Gets the single instance application.
     *
     *        Gets the single instance application.
     *
     * @return The instance of this application.
     */
    static Application* getInstance();

    /**
     * @fn Screen* Application::getCurrentScreen()
     *
     * @brief Gets the current screen.
     *
     *        Gets the current screen.
     *
     * @return The current screen.
     */
    Screen* getCurrentScreen()
    {
        return currentScreen;
    }

    /**
     * @fn virtual void Application::switchScreen(Screen* newScreen);
     *
     * @brief Switch to another Screen.
     *
     *        Switch to another Screen. Will call tearDownScreen on current Screen before
     *        switching, and subsequently call setupScreen and draw automatically for the new
     *        Screen.
     *
     * @param [in] newScreen A pointer to the new screen.
     */
    virtual void switchScreen(Screen* newScreen);

    /**
     * @fn virtual void Application::appSwitchScreen(uint8_t screenId)
     *
     * @brief An application specific function for switching screen.
     *
     *        An application specific function for switching screen. Overloading this can
     *        provide a means to switch screen from places that does not have access to a
     *        pointer to the new screen. Base implementation is empty.
     *
     * @param screenId An id that maps to the desired screen.
     */
    virtual void appSwitchScreen(uint8_t screenId)
    {
    }

    /**
     * @fn virtual void Application::draw();
     *
     * @brief Initiate a draw operation of the entire screen.
     *
     *        Initiate a draw operation of the entire screen. Standard implementation is to
     *        delegate draw request to the current Screen.
     *
     * @deprecated Use rect version instead.
     */
    virtual void draw();

    /**
     * @fn virtual void Application::draw(Rect& rect);
     *
     * @brief Initiate a draw operation of the specified region of the screen.
     *
     *        Initiate a draw operation of the specified region of the screen. Standard
     *        implementation is to delegate draw request to the current Screen.
     *
     * @note Unlike Widget::draw this is safe to call from user code as it will properly traverse
     *       widgets in z-order.
     * @note The coordinates given must be absolute coordinates.
     *
     * @param [in] rect The area to draw.
     */
    virtual void draw(Rect& rect);

    /**
     * @fn virtual void Application::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Handle a click event.
     *
     *        Handle a click event. Standard implementation is to delegate the event to the
     *        current screen. Called by the framework when a click is detected by some platform
     *        specific means.
     *
     * @param evt The ClickEvent.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void Application::handleDragEvent(const DragEvent& evt);
     *
     * @brief Handle drag events.
     *
     *        Handle drag events. Called by the framework when a drag is detected by some
     *        platform specific means. Standard implementation is to delegate drag event to
     *        current screen.
     *
     * @param evt The drag event, expressed in absolute coordinates.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn virtual void Application::handleGestureEvent(const GestureEvent& evt);
     *
     * @brief Handle gestures.
     *
     *        Handle gestures. Called by the framework when a gesture is detected by some
     *        platform specific means. Standard implementation is to delegate drag event to
     *        current screen.
     *
     * @param evt The gesture event.
     */
    virtual void handleGestureEvent(const GestureEvent& evt);

    /**
     * @fn virtual void Application::handleTickEvent();
     *
     * @brief Handle tick.
     *
     *        Handle tick. Standard implementation is to delegate tick to the widgets that have
     *        registered to receive one. Called by some platform specific means.
     */
    virtual void handleTickEvent();

    /**
     * @fn virtual void Application::handleKeyEvent(uint8_t c);
     *
     * @brief Handle an incoming character received by the HAL layer.
     *
     *        Handle an incoming character received by the HAL layer. Standard implementation
     *        delegates to current screen (which, in turn, does nothing).
     *
     * @param c The incomming character to handle.
     */
    virtual void handleKeyEvent(uint8_t c);

    /**
     * @fn virtual void Application::handlePendingScreenTransition();
     *
     * @brief Evaluates the pending Callback instances.
     *
     *        Evaluates the pending Callback instances. If a callback is valid, it is executed
     *        and a Screen transition is executed. This base implementation is empty and does
     *        nothing.
     */
    virtual void handlePendingScreenTransition();

    /**
     * @fn virtual void Application::cacheDrawOperations(bool enableCache);
     *
     * @brief This functions allows for deferring draw operations to a later time.
     *
     *        This functions allows for deferring draw operations to a later time. If active,
     *        calls to draw will simply note that the specified area is dirty, but not perform
     *        any actual drawing. When disabling the draw cache, the dirty area will be flushed
     *        (drawn) immediately.
     *
     * @param enableCache if true, all future draw operations will be cached. If false draw caching
     *                    is disabled, and the current cache (if not empty) is drawn
     *                    immediately.
     */
    virtual void cacheDrawOperations(bool enableCache);

    /**
     * @fn void Application::registerTimerWidget(Drawable* w);
     *
     * @brief Adds a widget to the list of widgets receiving ticks.
     *
     *        Adds a widget to the list of widgets receiving ticks every frame (typically 16.67ms)
     *
     * @note The framework keeps track of the number of times a specific widget is registered.
     *
     * @param [in] w The widget to add.
     *
     * @see unregisterTimerWidget
     */
    void registerTimerWidget(Drawable* w);

    /**
     * @fn void Application::clearAllTimerWidgets();
     *
     * @brief Clears all currently registered timer widgets.
     *
     *        Clears all currently registered timer widgets.
     */
    void clearAllTimerWidgets();

    /**
     * @fn void Application::unregisterTimerWidget(const Drawable* w);
     *
     * @brief Removes a widget from the list of widgets receiving ticks.
     *
     *        Removes a widget from the list of widgets receiving ticks every frame (typically 16.67ms)
     *        milliseconds.
     *
     * @note If widget has been registered multiple times, an equal number of calls to unregister
     *       are required to stop widget from receiving tick events.
     *
     * @param [in] w The widget to remove.
     */
    void unregisterTimerWidget(const Drawable* w);

    static const uint8_t MAX_TIMER_WIDGETS     = 24; ///< Maximum number of widgets receiving ticks. @remarks Memory impact: x * (sizeof(Drawable*)+1)
    static const uint16_t TICK_INTERVAL_MS     = 10; ///< Deprecated, do not use this constant. Tick interval depends on VSYNC of your target platform.

protected:

    /**
     * @fn void Application::invalidateArea(Rect area);
     *
     * @brief Invalidates this area.
     *
     *        Invalidates this area.
     *
     * @param area The area to invalidate.
     */
    void invalidateArea(Rect area);

    /**
     * @fn Application::Application();
     *
     * @brief Procected constructor
     *
     *        Procected constructor.
     */
    Application();

    Vector<Drawable*, MAX_TIMER_WIDGETS> timerWidgets; ///< List of widgets that receive timer ticks.
    uint8_t timerWidgetCounter[MAX_TIMER_WIDGETS];     ///< A counter for each potentially registered timer widget. Increase when registering for timer events, decrease when unregistering.
    Vector<Rect, 8> cachedDirtyAreas;                  ///< When draw caching is enabled, these rects keeps track of the dirty screen area.
    Vector<Rect, 8> lastRects;                         ///< The dirty areas from last frame that needs to be redrawn because we have swapped frame buffers.
    bool drawCacheEnabled;                             ///< True when draw caching is active.
    bool transitionHandled;                            ///< True if the transition is done and Screen::afterTransition has been called.
    static Screen* currentScreen;                      ///< Pointer to currently displayed Screen.
    static Transition* currentTransition;              ///< Pointer to current transition.
    static Application* instance;                      ///< Pointer to the instance of the Application-derived subclass.
    ///< @note Must be set by subclass constructor!
};

} // namespace touchgfx
#endif // APPLICATION_HPP
