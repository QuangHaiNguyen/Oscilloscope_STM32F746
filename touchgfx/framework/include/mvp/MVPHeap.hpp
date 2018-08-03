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
#ifndef MVPHEAP_HPP
#define MVPHEAP_HPP

namespace touchgfx
{

class AbstractPartition;
class MVPApplication;

/**
 * @class MVPHeap MVPHeap.hpp mvp/MVPHeap.hpp
 *
 * @brief Generic heap class for MVP applications.
 *
 *        Generic heap class for MVP applications. Serves as a way of obtaining the memory
 *        storage areas for presenters, screens, transitions and the concrete application.
 *
 *        Subclassed by an application-specific heap which provides the actual storage areas.
 *        This generic interface is used only in makeTransition.
 */
class MVPHeap
{
public:

    /**
     * @fn MVPHeap::MVPHeap(AbstractPartition& pres, AbstractPartition& scr, AbstractPartition& tra, MVPApplication& app)
     *
     * @brief Constructor.
     *
     *        Constructs an MVPHeap.
     *
     * @param [in] pres A memory partition containing enough memory to hold the largest presenter.
     * @param [in] scr  A memory partition containing enough memory to hold the largest view.
     * @param [in] tra  A memory partition containing enough memory to hold the largest
     *                  transition.
     * @param [in] app  A reference to the MVPApplication instance.
     */
    MVPHeap(AbstractPartition& pres,
            AbstractPartition& scr,
            AbstractPartition& tra,
            MVPApplication& app) :  presenterStorage(pres),
        screenStorage(scr),
        transitionStorage(tra),
        frontendApplication(app)
    {
    }

    /**
     * @fn virtual MVPHeap::~MVPHeap()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~MVPHeap()
    {
    }

    AbstractPartition& presenterStorage;    ///< A memory partition containing enough memory to hold the largest presenter.
    AbstractPartition& screenStorage;       ///< A memory partition containing enough memory to hold the largest view.
    AbstractPartition& transitionStorage;   ///< A memory partition containing enough memory to hold the largest transition.
    MVPApplication&    frontendApplication; ///< A reference to the MVPApplication instance.
};

} // namespace touchgfx
#endif // MVPHEAP_HPP
