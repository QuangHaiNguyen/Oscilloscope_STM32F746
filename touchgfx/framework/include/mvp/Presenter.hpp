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
#ifndef PRESENTER_HPP
#define PRESENTER_HPP

namespace touchgfx
{
/**
 * @class Presenter Presenter.hpp mvp/Presenter.hpp
 *
 * @brief The Presenter base class that all application-specific presenters should derive from.
 *
 *        The Presenter base class that all application-specific presenters should derive from.
 *        Only contains activate and deactivate virtual functions which are called
 *        automatically during screen transition.
 */
class Presenter
{
public:

    /**
     * @fn virtual void Presenter::activate()
     *
     * @brief Place initialization code for the Presenter here.
     *
     *        The activate function is called automatically when a screen transition causes
     *        this Presenter to become active. Place initialization code for the Presenter here.
     */
    virtual void activate()
    {
    }

    /**
     * @fn virtual void Presenter::deactivate()
     *
     * @brief Place cleanup code for the Presenter here.
     *
     *        The deactivate function is called automatically when a screen transition causes
     *        this Presenter to become inactive. Place cleanup code for the Presenter here.
     */
    virtual void deactivate()
    {
    }

    /**
     * @fn virtual Presenter::~Presenter()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Presenter()
    {
    }

protected:

    /**
     * @fn Presenter::Presenter()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Presenter()
    {
    }
};

} // namespace touchgfx
#endif // PRESENTER_HPP
