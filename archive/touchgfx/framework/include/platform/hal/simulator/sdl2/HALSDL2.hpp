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
#ifndef HALSDL2_HPP
#define HALSDL2_HPP

#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/touch/TouchController.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

namespace touchgfx
{
/**
 * @class HALSDL2 HALSDL2.hpp platform/hal/simulator/sdl/HALSDL2.hpp
 *
 * @brief HAL implementation for the TouchGFX simulator.
 *
 *        HAL implementation for the TouchGFX simulator.
 *
 * @see HAL
 */
class HALSDL2 : public HAL
{
public:

    /**
     * @fn HALSDL2::HALSDL2(DMA_Interface& dma, LCD& lcd, TouchController& touchCtrl, uint16_t width, uint16_t height)
     *
     * @brief Constructor. Initializes members.
     *
     *        Constructor. Initializes members.
     *
     * @param [in] dma       Reference to DMA interface.
     * @param [in] lcd       Reference to the LCD.
     * @param [in] touchCtrl Reference to Touch Controller driver.
     * @param width          Width of the display.
     * @param height         Height of the display.
     */
    HALSDL2(DMA_Interface& dma, LCD& lcd, TouchController& touchCtrl, uint16_t width, uint16_t height) :
        HAL(dma, lcd, touchCtrl, width, height),
        debugInfoEnabled(false),
        customTitle(0),
        portraitSkin(),
        landscapeSkin(),
        currentSkin(0),
        isSkinActive(true),
        borderless(false),
        flashInvalidatedRect(false),
        windowDrag(false)
    {
        setVsyncInterval(30.f); // Simulate 20Hz per default for backward compatibility
        updateCurrentSkin();
    }

    /**
     * @fn virtual void HALSDL2::taskEntry();
     *
     * @brief Main event loop.
     *
     *        Main event loop. Will wait for VSYNC signal, and then process next frame. Call
     *        this function from your GUI task.
     *
     * @note This function never returns!
     */
    virtual void taskEntry();

    /**
     * @fn virtual bool HALSDL2::sampleKey(uint8_t& key);
     *
     * @brief Sample key event from keyboard.
     *
     *        Sample key event from keyboard.
     *
     * @param [out] key Output parameter that will be set to the key value if a key press was
     *                  detected.
     *
     * @return True if a key press was detected and the "key" parameter is set to a value.
     */
    virtual bool sampleKey(uint8_t& key);

    /**
     * @fn virtual void HALSDL2::flushFrameBuffer();
     *
     * @brief This function is called whenever the framework has performed a complete draw.
     *
     *        On some platforms, a local frame buffer needs to be pushed to the display through
     *        a SPI channel or similar. Implement that functionality here. This function is
     *        called whenever the framework has performed a complete draw.
     */
    virtual void flushFrameBuffer();

    /**
     * @fn virtual void HALSDL2::flushFrameBuffer(const Rect& rect);
     *
     * @brief This function is called whenever the framework has performed a partial draw.
     *
     *        This function is called whenever the framework has performed a partial draw.
     *
     * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
     *
     * @see flushFrameBuffer(). This function is called whenever the framework has performed a
     *      partial draw.
     */
    virtual void flushFrameBuffer(const Rect& rect);

    /**
     * @fn virtual bool HALSDL2::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes);
     *
     * @brief This function performs a platform-specific memcpy.
     *
     *        This function performs a platform-specific memcpy, if supported by the hardware.
     *
     * @param [out] dest Pointer to destination memory.
     * @param src        Pointer to source memory.
     * @param numBytes   Number of bytes to copy.
     *
     * @return true if the copy succeeded, false if copy was not performed.
     */
    virtual bool blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes);

    /**
     * @fn virtual void HALSDL2::blitSetTransparencyKey(uint16_t key);
     *
     * @brief If Blit-operations are supported, transparency-keying support is implicitly assumed.
     *
     *        If Blit-operations are supported, transparency-keying support is implicitly
     *        assumed.
     *
     * @param key The "transparent" color value.
     */
    virtual void blitSetTransparencyKey(uint16_t key);

    /**
     * @fn void HALSDL2::setVsyncInterval(float ms);
     *
     * @brief Sets vsync interval.
     *
     *        Sets vsync interval for simulating same tick speed as the real hardware. Due to
     *        limitations in the granularity of SDL, the generated ticks in the simulator might
     *        not occur at the exact time, but accumulated over several ticks, the precision is
     *        very good.
     *
     * @param ms The milliseconds between ticks.
     *
     * @note That you can also use HAL::setFrameRateCompensation() in the simulator. The effect of
     *       this can easily be demonstrated by dragging the console output window of the
     *       simulator (when running from Visual Studio) as this will pause the SDL and generate a
     *       lot of ticks when the console window is released. Beware that since the missed vsyncs
     *       are accumulated in an 8 bit counter, only up to 255 ticks may be missed, so at
     *       VsyncInterval = 16.6667, dragging the windows for more than 255 * 16.6667ms = 4250ms
     *       = 4.25s will not generate all the ticks that were actually missed. This situation is,
     *       however, not very realistic, as normally just a couple of vsyncs are skipped.
     */
    void setVsyncInterval(float ms);

    /**
     * @fn bool HALSDL2::doSampleTouch(int32_t& x, int32_t& y) const;
     *
     * @brief Samples the position of the mouse cursor.
     *
     *        Samples the position of the mouse cursor.
     *
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     *
     * @return True if touch detected, false otherwise.
     */
    bool doSampleTouch(int32_t& x, int32_t& y) const;

    /**
     * @fn virtual bool HALSDL2::sdl_init(int argcount, char** args);
     *
     * @brief Initializes SDL.
     *
     *        Initializes SDL.
     *
     * @param argcount  Number of arguments.
     * @param [in] args Arguments.
     *
     * @return True if init went well, false otherwise.
     */
    virtual bool sdl_init(int argcount, char** args);

    /**
     * @fn void HALSDL2::setWindowTitle(const char* title)
     *
     * @brief Sets window title.
     *
     *        Sets window title of the TouchGFX simulator.
     *
     * @param title The title, if null the original "TouchGFX simulator" will be used.
     *
     * @see getWindowTitle
     */
    void setWindowTitle(const char* title);

    /**
     * @fn const char* HALSDL2::getWindowTitle() const
     *
     * @brief Gets window title.
     *
     *        Gets window title.
     *
     * @return null "TouchGFX simulator" unless set to something else using setWindowTitle().
     *
     * @see setWindowTitle
     */
    const char* getWindowTitle() const;

    /**
     * @fn void HALSDL2::loadSkin(touchgfx::DisplayOrientation orientation, int x, int y);
     *
     * @brief Loads a skin for a given display orientation.
     *
     *        Loads a skin for a given display orientation that will be rendered in the
     *        simulator window with the the TouchGFX framebuffer placed inside the bitmap at
     *        the given coordinates. Different bitmaps can be loaded in landscape and portrait
     *        mode. If the provided bitmap cannot be loaded, the TouchGFX framebuffer will be
     *        displayed as normal. If the png files contain areas with alpha &lt; 255, this
     *        will be used to create a shaped window.
     *
     * @param orientation The orientation.
     * @param x           The x coordinate.
     * @param y           The y coordinate.
     *
     * @note The skins must be named "portrait.png" and "landscape.png" and placed inside the
     *       "simulator/" folder. The build process of the simulator will automatically copy the
     *       skins to the folder where the executable simulator is generated.
     * @note When as skin is set, the entire framebuffer is rendered through SDL whenever there is
     *       a change. Without a skin, only the areas with changes is rendered through SDL.
     */
    void loadSkin(touchgfx::DisplayOrientation orientation, int x, int y);

    /**
     * @fn void HALSDL2::saveScreenshot();
     *
     * @brief Saves a screenshot.
     *
     *        Saves a screenshot to the default folder and default filename.
     */
    void saveScreenshot();

    /**
     * @fn virtual void HALSDL2::saveScreenshot(char* folder, char* filename);
     *
     * @brief Saves a screen shot.
     *
     *        Saves a screen shot.
     *
     * @param [in] folder   Folder name to place the screen shot in.
     * @param [in] filename Filename to save the screen shot to.
     */
    virtual void saveScreenshot(char* folder, char* filename);

protected:

    /**
     * @fn virtual uint16_t* HALSDL2::getTFTFrameBuffer() const;
     *
     * @brief Gets TFT frame buffer.
     *
     *        Gets TFT frame buffer.
     *
     * @return null if it fails, else the TFT frame buffer.
     */
    virtual uint16_t* getTFTFrameBuffer() const;

    /**
     * @fn void HALSDL2::setTFTFrameBuffer(uint16_t* addr);
     *
     * @brief Sets TFT frame buffer.
     *
     *        Sets TFT frame buffer.
     *
     * @param [in] addr The address of the TFT frame buffer.
     */
    void setTFTFrameBuffer(uint16_t* addr);

    /**
     * @fn virtual void HALSDL2::renderLCD_FrameBufferToMemory(const Rect& rectToUpdate, uint16_t* frameBuffer);
     *
     * @brief Update frame buffer using an SDL Surface.
     *
     *        Update frame buffer using an SDL Surface.
     *
     * @param rectToUpdate     Area to update.
     * @param [in] frameBuffer Target frame buffer.
     */
    virtual void renderLCD_FrameBufferToMemory(const Rect& _rectToUpdate, uint16_t* frameBuffer);

    /**
     * @fn virtual void HALSDL2::disableInterrupts()
     *
     * @brief Disables the DMA and LCD interrupts.
     *
     *        Disables the DMA and LCD interrupts.
     */
    virtual void disableInterrupts()
    {
    }

    /**
     * @fn virtual void HALSDL2::enableInterrupts()
     *
     * @brief Enables the DMA and LCD interrupts.
     *
     *        Enables the DMA and LCD interrupts.
     */
    virtual void enableInterrupts()
    {
    }

    /**
     * @fn virtual void HALSDL2::configureLCDInterrupt()
     *
     * @brief Configures LCD interrupt.
     *
     *        Configures LCD interrupt.
     */
    virtual void configureLCDInterrupt()
    {
    }

    /**
     * @fn virtual void HALSDL2::enableLCDControllerInterrupt()
     *
     * @brief Enables the LCD interrupt.
     *
     *        Enables the LCD interrupt.
     */
    virtual void enableLCDControllerInterrupt()
    {
    }

    /**
     * @fn virtual void HALSDL2::configureInterrupts()
     *
     * @brief Configures the interrupts relevant for TouchGFX.
     *
     *        Configures the interrupts relevant for TouchGFX. This primarily entails setting
     *        the interrupt priorities for the DMA and LCD interrupts.
     */
    virtual void configureInterrupts()
    {
    }

    /**
     * @fn void HALSDL2::performDisplayOrientationChange();
     *
     * @brief Perform the actual display orientation change.
     *
     *        Perform the actual display orientation change.
     */
    void performDisplayOrientationChange();

private:
    uint16_t* upscaleTo16bpp(uint16_t* src, uint16_t width, uint16_t height, uint16_t depth);
    uint16_t* doRotate(uint16_t* src, int16_t width, int16_t height);
    void recreateWindow(bool updateContent = true);
    void pushTouch(bool down) const;
    bool popTouch() const;
    void updateTitle(int32_t x, int32_t y) const;
    void alphaChannelCheck(SDL_Surface* surface, bool& isOpaque, bool& hasSemiTransparency);
    void updateCurrentSkin();
    int getCurrentSkinX() const;
    int getCurrentSkinY() const;


    bool debugInfoEnabled;

    float msBetweenTicks;
    float msPassed;

    static uint16_t icon[];
    const char* customTitle;

    class SkinInfo
    {
    public:
        SDL_Surface* surface;
        bool isOpaque;
        bool hasSemiTransparency;
        int offsetX;
        int offsetY;
        SkinInfo() : surface(0), isOpaque(true), hasSemiTransparency(false), offsetX(0), offsetY(0) {}
        virtual ~SkinInfo() {}
    };

    char programPath[300];
    SkinInfo portraitSkin;
    SkinInfo landscapeSkin;
    SkinInfo* currentSkin;
    bool isSkinActive;
    bool borderless;
    bool flashInvalidatedRect;

    bool windowDrag;
    int windowDragX;
    int windowDragY;

    static int32_t _xMouse;
    static int32_t _yMouse;
    static int32_t _x;
    static int32_t _y;
    static bool isWindowBeingDragged;
    static int initialWindowX;
    static int initialWindowY;
    static int initialMouseX;
    static int initialMouseY;
    static bool _lastTouch;
    static bool _touches[5];
    static int _numTouches;

    static uint8_t keyPressed;
};

} // namespace touchgfx

#endif // HALSDL2_HPP
