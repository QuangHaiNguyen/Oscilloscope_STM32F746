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
#ifndef ABSTRACTDIRECTIONPROGRESS_HPP
#define ABSTRACTDIRECTIONPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>

namespace touchgfx
{

/**
 * @class AbstractDirectionProgress AbstractDirectionProgress.hpp touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp
 *
 * @brief An abstract direction progress.
 *
 *        An abstract direction progress for progress indicators that need a direction to be specified.
 */
class AbstractDirectionProgress : public AbstractProgressIndicator
{
public:

    /**
     * @typedef enum DirectionType
     *
     * @brief Values that represent directions.
     *
     *        Values that represent directions.
     */
    typedef enum
    {
        RIGHT,
        LEFT,
        DOWN,
        UP
    } DirectionType;

    /**
     * @fn AbstractDirectionProgress::AbstractDirectionProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AbstractDirectionProgress();

    /**
     * @fn virtual AbstractDirectionProgress::~AbstractDirectionProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~AbstractDirectionProgress();

    /**
     * @fn virtual void AbstractDirectionProgress::setDirection(DirectionType direction);
     *
     * @brief Sets a direction.
     *
     *        Sets a direction.
     *
     * @param direction The direction.
     */
    virtual void setDirection(DirectionType direction);

    /**
     * @fn virtual DirectionType AbstractDirectionProgress::getDirection() const;
     *
     * @brief Gets the direction.
     *
     *        Gets the direction.
     *
     * @return The direction.
     */
    virtual DirectionType getDirection() const;

protected:
    DirectionType progressDirection;    ///< The progress direction
};

}

#endif // ABSTRACTDIRECTIONPROGRESS_HPP
