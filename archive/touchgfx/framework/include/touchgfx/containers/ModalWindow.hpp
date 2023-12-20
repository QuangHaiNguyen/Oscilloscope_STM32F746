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
#ifndef MODALWINDOW_HPP
#define MODALWINDOW_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{

/**
 * @class   ModalWindow ModalWindow.hpp include/gui/common/ModalWindow.hpp
 *
 * @brief   Container for displaying a modal window and hijacking touch event to underlaying view
 *          and widgets.
 *
 *          Container for displaying a modal window and hijacking touch event to underlaying view
 *          and widgets. The container has a background image and a surround box that acts as a
 *          shade on top of the rest of the screen. The background image must be set (using the
 *          setBackground method) and the shade can be adjusted (using the setShadeAlpha and
 *          setShadeColor methods).
 *
 *          The ModalWindow can either be used directly by adding widgets/containers to the
 *          ModalWindow from your view or by sub-classing it if you need a specific ModalWindow
 *          with predefined behavior across your application.
 *
 *          The ModalWindow should be instantiated in the view class and added as the last
 *          element (to always be on top). The ModalWindow will fill up the entire screen so it
 *          should always be placed at x=0, y=0.
 *
 *          To control the visibility of the ModalWindow use the show and hide methods.
 */
class ModalWindow : public Container
{
public:

    /**
     * @fn ModalWindow::ModalWindow();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ModalWindow();

    /**
     * @fn virtual ModalWindow::~ModalWindow();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ModalWindow();

    /**
     * @fn virtual void ModalWindow::setBackground(const BitmapId& bmpId);
     *
     * @brief Sets the background of the actual window.
     *
     *        Sets the background of the actual window. The remaining area of the screen will be covered by the shade.
     *        The background image is centered on the screen.
     *
     * @param   bmpId Identifier for the background bitmap.
     */
    virtual void setBackground(const BitmapId& bmpId);

    /**
     * @fn virtual void ModalWindow::setBackground(const BitmapId& bmpId, int16_t backgroundX, int16_t backgroundY);
     *
     * @brief Sets the background of the actual window.
     *
     *        Sets the background of the actual window. The remaining area of the screen will
     *        be covered by the shade. The background image will be placed at the backgroundX
     *        and backgroundY coordinate.
     *
     * @param bmpId       Identifier for the bitmap.
     * @param backgroundX The background x coordinate.
     * @param backgroundY The background y coordinate.
     */
    virtual void setBackground(const BitmapId& bmpId, int16_t backgroundX, int16_t backgroundY);

    /**
     * @fn virtual uint16_t ModalWindow::getBackgroundWidth();
     *
     * @brief Gets the width of the actual window (the background images).
     *
     *        Gets the width of the actual window (the background images). Whereas the
     *        getWidth()
     *        method will return the width including the shade.
     *
     * @return The width of the actual window.
     */
    virtual uint16_t getBackgroundWidth() const;

    /**
     * @fn virtual uint16_t ModalWindow::getBackgroundHeight();
     *
     * @brief Gets the height of the actual window (the background images).
     *
     *        Gets the height of the actual window (the background images). Whereas the
     *        getHeight()
     *        method will return the height including the shade.
     *
     * @return The height of the actual window.
     */
    virtual uint16_t getBackgroundHeight() const;

    /**
     * @fn virtual void ModalWindow::add(Drawable& d);
     *
     * @brief Adds a drawable to the ModalWindow.
     *
     *        Adds a drawable to the ModalWindow. The drawable will be placed relative to the
     *        background image.
     *
     * @param [in] d The drawable to add.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void ModalWindow::remove(Drawable& d);
     *
     * @brief Removes the drawable from the ModalWindow.
     *
     *        Removes the drawable from the ModalWindow.
     *
     * @param [in] d The drawable to remove.
     */
    virtual void remove(Drawable& d);

    /**
     * @fn virtual void ModalWindow::setShadeAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha value of the background shade.
     *
     *        Sets the alpha value of the background shade. Default = 96.
     *
     * @param alpha The new alpha.
     */
    virtual void setShadeAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t ModalWindow::getShadeAlpha() const;
     *
     * @brief Gets the alpha value of the background shade.
     *
     *        Gets the alpha value of the background shade.
     *
     * @return The background shades alpha.
     */
    virtual uint8_t getShadeAlpha() const;

    /**
     * @fn virtual void ModalWindow::setShadeColor(colortype color);
     *
     * @brief Sets the color of the background shade.
     *
     *        Sets the color of the background shade. Default = Black.
     *
     * @param color The new color.
     */
    virtual void setShadeColor(colortype color);

    /**
     * @fn virtual colortype ModalWindow::getShadeColor() const;
     *
     * @brief Gets the color of the background shade.
     *
     *        Gets the color of the background shade.
     *
     * @return The color of the background shade.
     */
    virtual colortype getShadeColor() const;

    /**
     * @fn virtual void ModalWindow::show();
     *
     * @brief Make the ModalWindow visible.
     *
     *        Make the ModalWindow visible.
     */
    virtual void show();

    /**
     * @fn virtual void ModalWindow::hide();
     *
     * @brief Make the ModalWindow invisible.
     *
     *        Make the ModalWindow invisible.
     */
    virtual void hide();

    /**
     * @fn virtual bool ModalWindow::isShowing();
     *
     * @brief Query if this ModalWindow is showing.
     *
     *        Query if this ModalWindow is showing.
     *
     * @return true if showing, false if not.
     */
    virtual bool isShowing() const;

protected:
    Box backgroundShade;        ///< The background shade
    Container windowContainer;  ///< The window container that defines the active container area where both the windowBackground and added drawables are placed.
    Image windowBackground;     ///< The window background
};

}

#endif // MODALWINDOW_HPP
