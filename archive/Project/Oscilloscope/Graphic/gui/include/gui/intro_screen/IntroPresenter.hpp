/**************************************************************************************************
* NAME: MainPresenter.hpp                                                                         *
*                                                                                                 *
* PURPOSE: Header for MainPresenter.cpp                                                           *
*                                                                                                 *
* PULIC VARIABLES:                                                                                *
*                                                                                                 *
* Variable        Type            Description                                                     *
* --------        ----            -----------                                                     *
*                                                                                                 *
* PUBLIC FUNCTIONS:                                                                               *
*                                                                                                 *
* Name                   Return      Description                                                  *
* ----                   ------      -----------                                                  *
* IncreaseYOffset          void      Increase the y offset of the Graph struct stored inside Model*
* DecreaseYOffset          void      Decrease the y offset of the Graph struct stored inside Model*
* ReturnGraphParameter      int      Retunr the Parameter of the graph to View                    *
*                                                                                                 *
* PRIVATE VARIABLES:                                                                              *
*                                                                                                 *
* Variable                    Type                 Description                                    *
* --------                    ----                 -----------                                    *
*                                                                                                 *
* PRIVATE FUNCTIONS:                                                                              *
*                                                                                                 *
* Name                    Return               Description                                        *
* ----                    ------               -----------                                        *
*                                                                                                 *
* Date        Author       Change Id Release  Description Of Change                               *
* ----        ------       --------- -------  ---------------------                               *
* 30.06.2016  Hai Nguyen          01     2.0  Original                                            * 
*                                                                                                 *
**************************************************************************************************/
#ifndef INTRO_PRESENTER_HPP
#define INTRO_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/common/main_header.h>
#include <stdlib.h>

using namespace touchgfx;

class IntroView;

/*
 * The Presenter for a template screen. In the MVP paradigm, the presenter acts upon
 * events from the model and the view. By deriving from ModelListener, the presenter
 * implements the interface which the model uses to inform the current presenter of
 * events and model state changes. The presenter also contains a reference to the
 * associated view, and uses this to update the view accordingly.
 *
 */
class IntroPresenter : public Presenter, public ModelListener
{
public:
	IntroPresenter(IntroView& v);
    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /*
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~IntroPresenter() {};

private:
	IntroPresenter();

    IntroView& view;
};

#endif // INTRO_PRESENTER_HPP
