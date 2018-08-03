/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.6.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
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
#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <mvp/View.hpp>
#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class FrontendHeap;

/**
 * The FrontendApplication is the concrete implementation of an MVPApplication
 * for a specific application. Its primary job is to provide functions for
 * switching between screens (presenter/view pairs). By convention these functions
 * are usually called gotoXXScreen().
 *
 * Note that the base class Application is a singleton. Switching screen can be done
 * from anywhere (usually presenters) by e.g. the following:
 * static_cast<FrontendApplication*>(Application::getInstance())->gotoXXScreen();
 */
class FrontendApplication : public MVPApplication
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    /**
     * Request a transition to the "Template" screen. The actual switching will
     * be done at next tick (by gotoTemplateScreenImpl).
     *
     * @note The reason for not switching immediately is that the new presenter and view
     * reuse the same memory as the current ones, so the switch cannot occur instantaneously
     * since that would cause memory corruption at the point of calling this function.
     */
    void gotoMainScreen();

    /**
     * Called automatically every frame. Will call tick on the model and then delegate
     * the tick event to the framework for further processing.
     */
    virtual void handleTickEvent()
    {
        model.tick();
        touchgfx::MVPApplication::handleTickEvent();
    }
private:
    Callback<FrontendApplication> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;
    void gotoMainScreenImpl();
};

#endif /* FRONTENDAPPLICATION_HPP */
