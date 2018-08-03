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
#ifndef LCD_HPP
#define LCD_HPP

#include <stdarg.h>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/TextProvider.hpp>


namespace touchgfx
{
struct Gradients;
struct Edge;
#undef LCD

/**
 * @class LCD LCD.hpp touchgfx/lcd/LCD.hpp
 *
 * @brief This class contains the various low-level drawing routines for drawing bitmaps, texts and
 *        rectangles.
 *
 *        This class contains the various low-level drawing routines for drawing bitmaps, texts
 *        and rectangles.
 *
 * @note All coordinates are expected to be in absolute coordinates!
 */
class LCD
{
public:

    /**
     * @fn virtual LCD::~LCD()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~LCD() { }

    /**
     * @fn virtual void LCD::init()
     *
     * @brief Performs initialization.
     *
     *        Performs initialization.
     */
    virtual void init() { }

    /**
     * @fn void LCD::drawGlyph(int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, int16_t maxX, int16_t maxY, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha = 255);
     *
     * @brief This method is deprecated and will be removed.
     *
     *        This method is deprecated and will be removed. Use drawString() instead. This
     *        function might not correctly render glyphs with negative value for "left" and
     *        could draw outside the allowed rectangle.
     *
     *        Draws a glyph with support for gray-map glyphs i.e. anti-aliasing using the gray
     *        color to determine the actual color to draw. Supports 1, 2, 4 and 8 bits per
     *        pixel.
     *
     * @deprecated.
     *
     * @param x            The x coordinate of the leftmost pixel.
     * @param y            The y coordinate of the topmost pixel.
     * @param offsetX      The offset x coordinate.
     * @param offsetY      The offset y coordinate.
     * @param maxX         The maximum x coordinate.
     * @param maxY         The maximum y coordinate.
     * @param glyph        The glyph.
     * @param glyphData    Information describing the glyph.
     * @param color        The color.
     * @param bitsPerPixel The number of bits per pixel.
     * @param alpha        Default value is 255 (solid)
     */
    void drawGlyph(int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, int16_t maxX, int16_t maxY, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha = 255);

    /**
     * @fn virtual void LCD::drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true) = 0;
     *
     * @brief Draws a portion of a bitmap.
     *
     *        Draws a portion of a bitmap.
     *
     * @param bitmap       The bitmap to draw.
     * @param x            The absolute x coordinate to place pixel (0,0) on the screen.
     * @param y            The absolute y coordinate to place pixel (0,0) on the screen.
     * @param rect         A rectangle describing what region of the bitmap is to be drawn.
     * @param alpha        Optional alpha value. Default is 255 (solid).
     * @param useOptimized if false, do not attempt to substitute (parts of) this bitmap with
     *                     faster fillrects.
     */
    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true) = 0;

    /**
     * @fn virtual void LCD::blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;
     *
     * @brief Blits a 2D source-array to the framebuffer.
     *
     *        Blits a 2D source-array to the framebuffer perfoming alpha-blending (and
     *        tranparency keying) as specified. Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data). The sourceData must be stored in a format suitable for
     *                             the selected display.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;

    /**
     * @fn virtual void LCD::blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;
     *
     * @brief Blits a 2D source-array to the frame buffer while converting the format.
     *
     *        Blits a 2D source-array to the frame buffer performing alpha-blending (and
     *        transparency keying) as specified. Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data). The sourceData must be stored in a format suitable for
     *                             the selected display.
     * @param sourceFormat         The bitmap format used in the source data.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels) = 0;

    /**
     * @fn virtual uint16_t* LCD::copyFrameBufferRegionToMemory(const Rect& region) = 0;
     *
     * @brief Copies a part of the frame buffer.
     *
     *        Copies a part of the frame buffer to an extra copy of the frame buffer also known
     *        as animation storage. Only the part specified with by parameter region is copied.
     *
     * @param region The part to copy.
     *
     * @return A pointer to the copy.
     *
     * @note There is only one instance of animation storage. The content of the animation storage
     *       outside the given region is undefined.
     *
     * @see blitCopy
     */
    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& region) = 0;

    /**
     * @fn virtual void LCD::fillRect(const Rect& rect, colortype color, uint8_t alpha = 255) = 0;
     *
     * @brief Draws a filled rectangle in the specified color.
     *
     *        Draws a filled rectangle in the specified color.
     *
     * @param rect  The rectangle to draw in absolute coordinates.
     * @param color The rectangle color.
     * @param alpha The rectangle opacity (255=solid)
     */
    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255) = 0;

    /**
     * @fn void LCD::drawHorizontalLine(int16_t x, int16_t y, uint16_t width, uint16_t lineWidth, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a horizontal line with the specified color.
     *
     *        Draws a horizontal line with the specified color.
     *
     * @param x         The x coordinate of the starting point, in absolute coordinates.
     * @param y         The y coordinate of the starting point, in absolute coordinates.
     * @param width     The length of the line.
     * @param lineWidth The width of the line.
     * @param color     The color to use.
     * @param alpha     The alpha value to use (default=solid)
     */
    void drawHorizontalLine(int16_t x, int16_t y, uint16_t width, uint16_t lineWidth, colortype color, uint8_t alpha = 255);

    /**
     * @fn void LCD::drawVerticalLine(int16_t x, int16_t y, uint16_t height, uint16_t lineWidth, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a vertical line with the specified color.
     *
     *        Draws a vertical line with the specified color.
     *
     * @param x         The x coordinate of the starting point, in absolute coordinates.
     * @param y         The y coordinate of the starting point, in absolute coordinates.
     * @param height    The length of the line.
     * @param lineWidth The width of the line.
     * @param color     The color to use.
     * @param alpha     The alpha value to use (default=solid)
     */
    void drawVerticalLine(int16_t x, int16_t y, uint16_t height, uint16_t lineWidth, colortype color, uint8_t alpha = 255);

    /**
     * @fn void LCD::drawRect(const Rect& rect, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a rectangle using the specified line color.
     *
     *        Draws a rectangle using the specified line color. Same as calling drawBorder with
     *        a line width of 1.
     *
     * @param rect  The rectangle x, y, width, height in absolute coordinates.
     * @param color The color to use.
     * @param alpha The alpha value to use (default=solid)
     */
    void drawRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    /**
     * @fn void LCD::drawBorder(const Rect& rect, uint16_t lineWidth, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a rectangle width the specified line width and color.
     *
     *        Draws a rectangle width the specified line width and color.
     *
     * @param rect      The rectangle x, y, width, height in absolute coordinates.
     * @param lineWidth The width of the line.
     * @param color     The color to use.
     * @param alpha     The alpha value to use (default=solid)
     */
    void drawBorder(const Rect& rect, uint16_t lineWidth, colortype color, uint8_t alpha = 255);

    /**
     * @struct StringVisuals LCD.hpp touchgfx/lcd/LCD.hpp
     *
     * @brief The visual elements when writing a string.
     *
     *        The visual elements when writing a string.
     */
    struct StringVisuals
    {
        const Font* font;              ///< The font to use.
        Alignment alignment;           ///< The alignment to use. Default is LEFT.
        TextDirection textDirection;   ///< The direction to use. Default is LTR
        TextRotation rotation;         ///< Orientation (rotation) of the text. Default is TEXT_ROTATE_0.
        colortype color;               ///< RGB color value. Default is 0 (black).
        uint16_t linespace;            ///< Line space in pixels for multiline strings. Default is 0.
        uint8_t alpha;                 ///< 8-bit alpha value. Default is 255 (solid).
        uint8_t indentation;           ///< Indentation of text inside rectangle. Text will start this far from the left/right edge
        WideTextAction wideTextAction; ///< What to do with wide text lines

        /**
         * @fn StringVisuals()
         *
         * @brief Construct an empty StringVisuals object.
         *
         *        Construct an empty StringVisuals object.
         */
        StringVisuals() :
            font(0), alignment(LEFT), textDirection(TEXT_DIRECTION_LTR), rotation(TEXT_ROTATE_0), color(0), linespace(0), alpha(255), indentation(0), wideTextAction(WIDE_TEXT_NONE)
        {
        }

        /**
         * @fn StringVisuals(const Font* font, colortype color, uint8_t alpha, Alignment alignment, uint16_t linespace, TextRotation rotation, TextDirection textDirection, uint8_t indentation, WideTextAction wideTextAction = WIDE_TEXT_NONE)
         *
         * @brief Construct a StringVisual object for rendering text.
         *
         * @param font           The Font with which to draw the text.
         * @param color          The color with which to draw the text.
         * @param alpha          Alpha blending. Default value is 255 (solid)
         * @param alignment      How to align the text.
         * @param linespace      Line space in pixels between each line, in case the text contains
         *                       newline characters.
         * @param rotation       How to rotate the text.
         * @param textDirection  The text direction.
         * @param indentation    The indentation of the text from the left and right of the text
         *                       area rectangle.
         * @param wideTextAction What to do with lines longer than the width of the TextArea.
         */
        StringVisuals(const Font* font, colortype color, uint8_t alpha, Alignment alignment, uint16_t linespace, TextRotation rotation, TextDirection textDirection, uint8_t indentation, WideTextAction wideTextAction = WIDE_TEXT_NONE)
        {
            this->font = font;
            this->color = color;
            this->alpha = alpha;
            this->alignment = alignment;
            this->textDirection = textDirection;
            this->rotation = rotation;
            this->linespace = linespace;
            this->indentation = indentation;
            this->wideTextAction = wideTextAction;
        }
    };

    /**
     * @fn void LCD::drawString(Rect widgetArea, const Rect& invalidatedArea, StringVisuals& stringVisuals, const Unicode::UnicodeChar* format, ...);
     *
     * @brief Draws the specified unicode string.
     *
     *        Draws the specified unicode string. Breaks line on newline.
     *
     * @param widgetArea         The area covered by the drawing widget in absolute coordinates.
     * @param invalidatedArea    The (sub)region of the widget area to draw, expressed relative to
     *                           the widget area. If the widgetArea is (x, y, width, height) = (10,
     *                           10, 20, 20) and invalidatedArea is (x, y ,width, height) = (5, 5,
     *                           6, 6) the widgetArea drawn on the LCD is (x, y, width, height) =
     *                           (15, 15, 6, 6).
     * @param [in] stringVisuals The string visuals (font, alignment, line space, color)
     *                           with which to draw this string.
     * @param format             A pointer to a zero terminated text string with optional
     *                           additional wildcard arguments.
     * @param ...                Variable arguments providing additional information.
     */
    void drawString(Rect widgetArea,
                    const Rect& invalidatedArea,
                    StringVisuals& stringVisuals,
                    const Unicode::UnicodeChar* format,
                    ...);

    /**
     * @fn virtual uint8_t LCD::bitDepth() const = 0;
     *
     * @brief Number of bits per pixel used by the display.
     *
     *        Number of bits per pixel used by the display.
     *
     * @return The number of bits per pixel.
     */
    virtual uint8_t bitDepth() const = 0;

    /**
     * @fn void LCD::drawTextureMapTriangle(const DrawingSurface& dest, const Point3D* vertices, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha = 255, uint16_t subDivisionSize = 12);
     *
     * @brief Texture map triangle. Draw a perspective correct texture mapped triangle. The
     *        vertices describes the surface, the x,y,z coordinates and the u,v coordinates of
     *        the texture. The texture contains the image data to be drawn The triangle line
     *        will be placed and clipped using the absolute and dirty rectangles The alpha will
     *        determine how the triangle should be alpha blended. The subDivisionSize will
     *        determine the size of the piecewise affine texture mapped portions of the
     *        triangle.
     *
     * @param dest              The description of where the texture is drawn - can be used to
     *                          issue a draw off screen.
     * @param vertices          The vertices of the triangle.
     * @param texture           The texture.
     * @param absoluteRect      The containing rectangle in absolute coordinates.
     * @param dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param renderVariant     The render variant - includes the algorithm and the pixel format.
     * @param alpha             the alpha. Default = 255.
     * @param subDivisionSize   the size of the subdivisions of the scan line. Default = 12.
     */
    void drawTextureMapTriangle(const DrawingSurface& dest,
                                const Point3D* vertices,
                                const TextureSurface& texture,
                                const Rect& absoluteRect,
                                const Rect& dirtyAreaAbsolute,
                                RenderingVariant renderVariant,
                                uint8_t alpha = 255,
                                uint16_t subDivisionSize = 12);

protected:
    static const uint16_t newLine = 10; ///< NewLine value.

    /**
     * @fn virtual void LCD::drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionLength) = 0;
     *
     * @brief Draw scan line. Draw one horizontal line of the texture map on screen. The scan line
     *        will be drawn using perspective correct texture mapping. The appearance of the
     *        line is determined by the left and right edge and the gradients structure. The
     *        edges contain the information about the x,y,z coordinates of the left and right
     *        side respectively and also information about the u,v coordinates of the texture
     *        map used. The gradients structure contains information about how to interpolate
     *        all the values across the scan line. The data drawn should be present in the
     *        texture argument.
     *
     *        The scan line will be drawn using the additional arguments. The scan line will be
     *        placed and clipped using the absolute and dirty rectangles The alpha will
     *        determine how the scan line should be alpha blended. The subDivisionSize will
     *        determine the size of the piecewise affine texture mapped lines.
     *
     * @param dest              The description of where the texture is drawn - can be used to
     *                          issue a draw off screen.
     * @param gradients         The gradients using in interpolation across the scan line.
     * @param leftEdge          The left edge of the scan line.
     * @param rightEdge         The right edge of the scan line.
     * @param texture           The texture.
     * @param absoluteRect      The containing rectangle in absolute coordinates.
     * @param dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param renderVariant     The render variant - includes the algorithm and the pixel format.
     * @param alpha             The alpha.
     * @param subDivisionLength The size of the subdivisions of the scan line. A value of 1 will
     *                          give a completely perspective correct texture mapped scan line. A
     *                          large value will give an affine texture mapped scan line.
     */
    virtual void drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionLength) = 0;

    /**
     * @fn virtual void LCD::drawGlyph(uint16_t* wbuf, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation = TEXT_ROTATE_0) = 0;
     *
     * @brief Private version of draw-glyph.
     *
     *        Private version of draw-glyph with explicit destination buffer pointer argument.
     *        For all parameters (except the buffer pointer) see the public version of
     *        drawGlyph()
     *
     * @param [out] wbuf      The destination (frame) buffer to draw to.
     * @param widgetArea      The canvas to draw the glyph inside.
     * @param x               Horizontal offset to start drawing the glyph.
     * @param y               Vertical offset to start drawing the glyph.
     * @param offsetX         Horizontal offset in the glyph to start drawing from.
     * @param offsetY         Vertical offset in the glyph to start drawing from.
     * @param invalidatedArea The area to draw inside.
     * @param glyph           Specifications of the glyph to draw.
     * @param glyphData       Data containing the actual glyph (dense format)
     * @param color           The color of the glyph.
     * @param bitsPerPixel    Bit depth of the glyph.
     * @param alpha           The transparency of the glyph.
     * @param rotation        Rotation to do before drawing the glyph.
     */
    virtual void drawGlyph(uint16_t* wbuf, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation = TEXT_ROTATE_0) = 0;

    /**
     * @fn static void LCD::rotateRect(Rect& rect, const Rect& canvas, const TextRotation rotation);
     *
     * @brief Rotate a rectangle inside another rectangle.
     *
     *        Rotate a rectangle inside another rectangle.
     *
     * @param [in,out] rect The rectangle to rotate.
     * @param canvas        The rectangle containing the rect to rotate.
     * @param rotation      Rotation to perform on rect.
     */
    static void rotateRect(Rect& rect, const Rect& canvas, const TextRotation rotation);

    /**
     * @fn static int LCD::realX(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);
     *
     * @brief Find the real, absolute x coordinate of a point inside a widget.
     *
     *        Find the real, absolute x coordinate of a point inside a widget with regards to
     *        rotation.
     *
     * @param [in] widgetArea The widget containing the point.
     * @param x               The x coordinate.
     * @param y               The y coordinate.
     * @param rotation        Rotation to perform.
     *
     * @return The absolute x coordinate after applying appropriate rotation.
     */
    static int realX(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);

    /**
     * @fn static int LCD::realY(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);
     *
     * @brief Find the real, absolute y coordinate of a point inside a widget.
     *
     *        Find the real, absolute y coordinate of a point inside a widget with regards to
     *        rotation.
     *
     * @param [in] widgetArea The widget containing the point.
     * @param x               The x coordinate.
     * @param y               The y coordinate.
     * @param rotation        Rotation to perform.
     *
     * @return The absolute y coordinate after applying appropriate rotation.
     */
    static int realY(const Rect& widgetArea, int16_t x, int16_t y, TextRotation rotation);

    /**
     * @fn void LCD::drawStringLTR(Rect widgetArea, const Rect& invalidatedArea, const StringVisuals& visuals, const Unicode::UnicodeChar* format, va_list pArg);
     *
     * @brief Draws the specified unicode string.
     *
     *        Draws the specified unicode string. Breaks line on newline. The string is assumed
     *        to contain only latin characters written left-to-right.
     *
     * @param widgetArea      The area covered by the drawing widget in absolute coordinates.
     * @param invalidatedArea The (sub)region of the widget area to draw, expressed relative to the
     *                        widget area. If the widgetArea is (x, y, width, height) = (10, 10,
     *                        20, 20) and invalidatedArea is (x, y ,width, height) = (5, 5, 6,
     *                        6) the widgetArea drawn on the LCD is (x, y, width, height) = (15,
     *                        15, 6, 6).
     * @param [in] visuals    The string visuals (font, alignment, line space, color)
     *                        with which to draw this string.
     * @param format          A pointer to a zero terminated text string with optional additional
     *                        wildcard arguments.
     * @param pArg            Variable arguments providing additional information.
     *
     * @see drawString
     */
    void drawStringLTR(const Rect& widgetArea,
                       const Rect& invalidatedArea,
                       const StringVisuals& visuals,
                       const Unicode::UnicodeChar* format,
                       va_list pArg);

    /**
     * @fn void LCD::drawStringRTL(Rect widgetArea, const Rect& invalidatedArea, const StringVisuals& visuals, const Unicode::UnicodeChar* format, va_list pArg);
     *
     * @brief Draws the specified unicode string.
     *
     *        Draws the specified unicode string. Breaks line on newline. The string can be
     *        either right-to-left or left-to-right and may contain sequences of Arabic /Hebrew
     *        and Latin characters.
     *
     * @param widgetArea      The area covered by the drawing widget in absolute coordinates.
     * @param invalidatedArea The (sub)region of the widget area to draw, expressed relative to the
     *                        widget area. If the widgetArea is (x, y, width, height) = (10, 10,
     *                        20, 20) and invalidatedArea is (x, y ,width, height) = (5, 5, 6,
     *                        6) the widgetArea drawn on the LCD is (x, y, width, height) = (15,
     *                        15, 6, 6).
     * @param [in] visuals    The string visuals (font, alignment, line space, color)
     *                        with which to draw this string.
     * @param format          A pointer to a zero terminated text string with optional additional
     *                        wildcard arguments.
     * @param pArg            Variable arguments providing additional information.
     *
     * @see drawString
     */
    void drawStringRTL(const Rect& widgetArea,
                       const Rect& invalidatedArea,
                       const StringVisuals& visuals,
                       const Unicode::UnicodeChar* format,
                       va_list pArg);

    /**
     * @fn static uint16_t LCD::stringWidth(TextProvider& textProvider, const Font& font, const int numChars, TextDirection textDirection);
     *
     * @brief Find string width.
     *
     *        Find string with of the given number of characters read from the given
     *        TextProvider.
     *
     * @param [in,out] textProvider The text provider.
     * @param font                  The font.
     * @param numChars              Number of characters.
     * @param textDirection         The text direction.
     *
     * @return An int16_t.
     */
    static uint16_t stringWidth(TextProvider& textProvider, const Font& font, const int numChars, TextDirection textDirection);

    /**
     * @fn static uint16_t LCD::getNumLines(TextProvider& textProvider, WideTextAction wideTextAction, TextDirection textDirection, const Font* font, int16_t width);
     *
     * @brief Gets number lines.
     *
     *        Gets number of lines for a given text taking word wrap into consideration. The
     *        font and width are required to find the number of lines in case word wrap is true.
     *
     * @param [in] textProvider The text provider.
     * @param wideTextAction    The wide text action in case lines are longer than the width of the text area.
     * @param textDirection     The text direction (LTR or RTL).
     * @param font              The font.
     * @param width             The width.
     *
     * @return The number lines.
     */
    static uint16_t getNumLines(TextProvider& textProvider, WideTextAction wideTextAction, TextDirection textDirection, const Font* font, int16_t width);

    friend class Font;
    friend class TextArea;
    friend class TextAreaWithWildcardBase;

private:
    typedef void (LCD::*DrawStringFunctionPointer)(const Rect& widgetArea, const Rect& invalidatedArea, const StringVisuals& stringVisuals, const Unicode::UnicodeChar* format, va_list _pArg);
    static DrawStringFunctionPointer drawStringFunction;    ///< The draw string function, either LTR or RTL

    class DrawStringInternalStruct
    {
    public:
        uint16_t*            frameBuffer;
        const Rect*          widgetArea;
        int16_t              widgetRectY;
        const Rect*          toDraw;
        const StringVisuals* stringVisuals;
        TextProvider*        drawTextProvider;

        DrawStringInternalStruct() :
            frameBuffer(0), widgetArea(0), widgetRectY(0), toDraw(0), stringVisuals(0), drawTextProvider(0)
        {
        }
    };
    typedef void (LCD::*DrawStringInternalFunctionPointer)(int16_t& offset, const Font* font, const TextDirection textDirection, TextProvider& widthTextProvider, const int numChars, DrawStringInternalStruct const* data);
    bool drawStringRTLLine(int16_t& offset, const Font* font, TextDirection textDirection, Unicode::UnicodeChar& currChar, TextProvider& textProvider, TextProvider& widthTextProvider, DrawStringInternalFunctionPointer internalFunction, const int numChars, DrawStringInternalStruct const* data);
    void drawStringWidthInternal(int16_t& offset, const Font* font, const TextDirection textDirection, TextProvider& textProvider, const int numChars, DrawStringInternalStruct const* data);
    void drawStringRTLInternal(int16_t& offset, const Font* font, const TextDirection textDirection, TextProvider& widthTextProvider, const int numChars, DrawStringInternalStruct const* data);
    bool drawStringInternal(uint16_t* frameBuffer, Rect const* widgetArea, int16_t widgetRectY, int16_t& offset, const Rect& invalidatedArea, StringVisuals const* stringVisuals, const TextDirection textDirection, TextProvider& textProvider, const int numChars);

    class WideTextInternalStruct
    {
    public:
        WideTextInternalStruct(TextProvider& _textProvider, uint16_t _maxWidth, TextDirection _textDirection, const Font* _font, WideTextAction action) : currChar(0), textProvider(_textProvider), textDirection(_textDirection), wideTextAction(action), font(_font), maxWidth(_maxWidth), charsRead(0), width(0), charsReadAhead(0), widthAhead(0), widthWithoutLastSpace(0)
        {
        }
        void addWord(uint16_t widthBeforeCurrChar, uint16_t widthBeforeLastSpace);
        void getStringLengthForLine();
        Unicode::UnicodeChar getCurrChar() const
        {
            return currChar;
        }
        uint16_t getLineWidth() const
        {
            return widthWithoutLastSpace;
        }
        uint16_t getCharsRead() const
        {
            return charsRead;
        }
    private:
        Unicode::UnicodeChar currChar;
        TextProvider&        textProvider;
        TextDirection        textDirection;
        WideTextAction       wideTextAction;
        const Font*          font;
        uint16_t             maxWidth;
        uint16_t             charsRead;
        uint16_t             width;
        uint16_t             charsReadAhead;
        uint16_t             widthAhead;
        uint16_t             widthWithoutLastSpace;
    };


};

} // namespace touchgfx

#endif // LCD_HPP
