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
#ifndef VIEW_HPP
#define VIEW_HPP

#include <touchgfx/Screen.hpp>
#include <mvp/Presenter.hpp>

namespace touchgfx
{
/**
 * @class View View.hpp mvp/View.hpp
 *
 * @brief This is a generic touchgfx::Screen specialization for normal applications.
 *
 *        This is a generic touchgfx::Screen specialization for normal applications. It
 *        provides a link to the Presenter class.
 *
 * @note All views in the application must be a subclass of this type.
 *
 * @tparam T The type of Presenter associated with this view.
 *
 * @see Screen
 */
template<class T>
class View : public Screen
{
public:

    /**
     * @fn View::View()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    View() : presenter(0)
    {
    }

    /**
     * @fn void View::bind(T& presenter)
     *
     * @brief Binds an instance of a specific Presenter type (subclass) to the View instance.
     *
     *        Binds an instance of a specific Presenter type (subclass) to the View instance.
     *        This function is called automatically when a new presenter/view pair is activated.
     *
     * @param [in] presenter The specific Presenter to be associated with the View.
     */
    void bind(T& presenter)
    {
        this->presenter = &presenter;
    }

protected:
    T* presenter;   ///< Pointer to the Presenter associated with this view.
};

} // namespace touchgfx
#endif // VIEW_HPP
