/*
 *  Copyright 2014 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "bios_loader.h"


/** Initializes the display but does not power it up.
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t disp_init(void);

/** Turns the display on with the specified framebuffer showing
 *
 *  @param framebuffer  the data to show
 *  @param res          the resolution to use
 *  @param rate         the frame rate to use
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t disp_powerUp(void* framebuffer, Resolution_t res, FrameRate_t rate);
  
/** Turns the display off
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t disp_powerDown(void);

/** Sets the backlight level. 0% is off and 100% is fully on
 *
 *  @param percent   backlight in %
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t disp_backlight(int percent);

/** Returns the width (in pixels) of the display
 *
 *  @returns the display width
 */
uint16_t disp_width(void);

/** Returns the height (in pixels) of the display
 *
 *  @returns the display height
 */
uint16_t disp_height(void);

/** Returns the number of bytes used by each pixel
 *
 *  @returns bytes per pixel
 */
uint16_t disp_bytesPerPixel(void);

/** Returns the number of bytes used for each frame buffer
 *
 *  @returns width*height*bytesPerPixel
 */
uint32_t disp_fbSize(void);

/** Returns the display orientation
 *
 *  @returns the display orientation
 */
bool disp_landscape(void);

/** Returns true if the specified resolution can be used
 *
 *  @returns true if supported, false if not
 */
bool disp_isSupported(Resolution_t res);

/** Returns the current resolution
 *
 *  @returns the current resolution
 */
Resolution_t disp_currentResolution(void);

/** Replaces the current framebuffer.
 *
 * Note that this requires the caller or someone else to have a
 * reference to the existing framebuffer, otherwise that memory
 * is lost.
 *
 *  @param buff the new framebuffer
 */
void disp_setFramebuffer(void* buff);

/** Replaces the current framebuffer with the specified one.
 *
 * This function as opposed to the setFramebuffer() one does return
 * the old framebuffer. This way the caller can save the old one and
 * then swap it back when done.
 *
 *  @param buff the new framebuffer
 *  @returns the old framebuffer
 */
void* disp_swapFramebuffer(void* buff);

#endif
