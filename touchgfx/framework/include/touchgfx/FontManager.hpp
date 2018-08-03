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
#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <touchgfx/Font.hpp>

namespace touchgfx
{
/**
 * @class FontProvider FontManager.hpp touchgfx/FontManager.hpp
 *
 * @brief A generic pure virtual definition of a FontProvider.
 *
 *        A generic pure virtual definition of a FontProvider, which is a class capable of
 *        returning a font based on a font id. An application-specific derivation of this class
 *        must be implemented.
 */
class FontProvider
{
public:

    /**
     * @fn virtual Font* FontProvider::getFont(FontId fontId) = 0;
     *
     * @brief Gets a font.
     *
     *        Gets a font.
     *
     * @param fontId The font id of the font to get.
     *
     * @return The font with a font id of fontId.
     */
    virtual Font* getFont(FontId fontId) = 0;

    /**
     * @fn virtual FontProvider::~FontProvider()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~FontProvider()
    {
    }

private:

};

/**
 * @class FontManager FontManager.hpp touchgfx/FontManager.hpp
 *
 * @brief This class is the entry point for looking up a font based on a font id.
 *
 *        This class is the entry point for looking up a font based on a font id. Must be
 *        initialized with the appropriate FontProvider by the application.
 */
class FontManager
{
public:

    /**
     * @fn static void FontManager::setFontProvider(FontProvider* fontProvider);
     *
     * @brief Sets the font provider.
     *
     *        Sets the font provider. Must be initialized with the appropriate FontProvider by
     *        the application.
     *
     * @param [in] fontProvider Sets the font provider. Must be initialized with the appropriate
     *                          FontProvider by the application.
     */
    static void setFontProvider(FontProvider* fontProvider);

    /**
     * @fn static Font* FontManager::getFont(FontId fontId);
     *
     * @brief Gets a font.
     *
     *        Gets a font.
     *
     * @param fontId The font id of the font to get.
     *
     * @return The font with a font id of fontId.
     */
    static Font* getFont(FontId fontId);

private:
    static FontProvider* provider;

};

} // namespace touchgfx

#endif // FONTMANAGER_HPP
