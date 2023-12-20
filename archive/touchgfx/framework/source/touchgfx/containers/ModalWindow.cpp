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
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{

ModalWindow::ModalWindow() :
    Container()
{
    Container::setWidth(HAL::DISPLAY_WIDTH);
    Container::setHeight(HAL::DISPLAY_HEIGHT);

    int defaultShadeAlpha = 96;
    colortype defaultShadeColor = Color::getColorFrom24BitRGB(0x0, 0x0, 0x0);

    backgroundShade.setPosition(0, 0, getWidth(), getHeight());
    backgroundShade.setColor(defaultShadeColor);
    backgroundShade.setTouchable(true);
    ModalWindow::setShadeAlpha(defaultShadeAlpha);
    Container::add(backgroundShade);

    Container::add(windowContainer);
    windowContainer.add(windowBackground);
}

ModalWindow::~ModalWindow()
{
}

void ModalWindow::setBackground(const BitmapId& bmpId)
{
    windowBackground.setBitmap(Bitmap(bmpId));
    windowBackground.setXY(0, 0);

    windowContainer.setPosition((getWidth() - windowBackground.getWidth()) / 2, (getHeight() - windowBackground.getHeight()) / 2, windowBackground.getWidth(), windowBackground.getHeight());
    invalidate();
}

void ModalWindow::setBackground(const BitmapId& bmpId, int16_t backgroundX, int16_t backgroundY)
{
    setBackground(bmpId);
    windowContainer.setXY(backgroundX, backgroundY);
}

uint16_t ModalWindow::getBackgroundWidth() const
{
    return windowBackground.getWidth();
}

uint16_t ModalWindow::getBackgroundHeight() const
{
    return windowBackground.getHeight();
}

void ModalWindow::add(Drawable& d)
{
    windowContainer.add(d);
}

void ModalWindow::remove(Drawable& d)
{
    windowContainer.remove(d);
}

void ModalWindow::setShadeAlpha(uint8_t alpha)
{
    backgroundShade.setAlpha(alpha);
    backgroundShade.invalidate();
}

uint8_t ModalWindow::getShadeAlpha() const
{
    return backgroundShade.getAlpha();
}

void ModalWindow::setShadeColor(colortype color)
{
    backgroundShade.setColor(color);
    backgroundShade.invalidate();
}

touchgfx::colortype ModalWindow::getShadeColor() const
{
    return backgroundShade.getColor();
}

void ModalWindow::show()
{
    setVisible(true);
    invalidate();
}

void ModalWindow::hide()
{
    setVisible(false);
    invalidate();
}

bool ModalWindow::isShowing() const
{
    return isVisible();
}

}
