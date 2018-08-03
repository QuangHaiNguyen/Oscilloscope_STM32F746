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

#ifndef TOUCHPANEL_H
#define TOUCHPANEL_H

#include "bios_loader.h"

typedef void (*touchEventFunc)(void);

/**
 * Initialize the touch controller. This method must be called before
 * calibrating or reading data from the controller
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_init(void);


/**
 * Tests if a touch controller is available or not.
 *
 * Note that this function only returns a valid value
 * after the display has been intitialized.
 *
 * @return true if there is a touch controller
 */
bool touch_isTouchSupported(void);

/**
 * Read up to num coordinates from the touch panel.
 *
 * @param coords a list of at least num coordinates
 * @param num the number of coordinates to read
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_read(touch_coordinate_t* coord, int num);

/**
 * Returns information about the touch panel
 *
 * @param resistive true for Resistive, false for Capacitive
 * @param maxPoints the maximum number of simultaneous touches
 * @param calibration true if the controller can be calibrated
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_info(bool* resistive, int* maxPoints, bool* calibrated);

/**
 * Start to calibrate the display
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_calibrateStart(void);

/**
 * Get the next calibration point. Draw an indicator on the screen
 * at the coordinates and ask the user to press/click on the indicator.
 * Please note that waitForCalibratePoint() must be called after this
 * method.
 *
 * @param x    the x coordinate is written to this argument
 * @param y    the y coordinate is written to this argument
 * @param last true if this is the last coordinate in the series
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_getNextCalibratePoint(uint16_t* x, uint16_t* y, bool* last);

/**
 * Wait for a calibration point to have been pressed and recored.
 * This method must be called just after getNextCalibratePoint().
 *
 * @param morePoints true is written to this argument if there
 * are more calibrations points available; otherwise it will be false
 * @param timeout maximum number of milliseconds to wait for
 * a calibration point. Set this argument to 0 to wait indefinite.
 *
 *  @returns
 *       Ok on success
 *       An error code on failure
 */
BiosLoaderError_t touch_waitForCalibratePoint(bool* morePoints, uint32_t timeout);

touchEventFunc touch_setListener(touchEventFunc handler);

#endif
