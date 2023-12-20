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
#ifndef MAIN_PRESENTER_HPP
#define MAIN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/common/main_header.h>
#include <stdlib.h>

using namespace touchgfx;

class MainView;

/*
 * The Presenter for a template screen. In the MVP paradigm, the presenter acts upon
 * events from the model and the view. By deriving from ModelListener, the presenter
 * implements the interface which the model uses to inform the current presenter of
 * events and model state changes. The presenter also contains a reference to the
 * associated view, and uses this to update the view accordingly.
 *
 */
class MainPresenter : public Presenter, public ModelListener
{
public:
    MainPresenter(MainView& v);
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

    virtual ~MainPresenter() {};

	void p_SetRawData(int channel);

	int * p_GetTriggerData(int channel);
	void p_SetTriggerData(int channel, int value);

	void p_SetYOffset(int channel, int value);
	int p_GetYOffset(int channel);

	int p_GetTriggerValue(int channel);
	void p_SetTriggerValue(int channel, int value);

	bool p_GetTrigger(int channel);
	void p_SetTrigger(int channel, bool value);

	bool p_GetTriggerType(int channel);
	void p_SetTriggerType(int channel, bool value);

	int p_GetTimeScale(int channel);
	void p_SetTimeScale(int channel, int value);

	int p_GetVoltageScale(int channel);
	void p_SetVoltageScale(int channel, int value);

	float p_GetTimeOffset(int channel);
	int p_GetVoltOffset(int channel);

	float p_VoltagePerPixel(int channel);

	void p_SetXOffset(int channel, int value);
	int p_GetXOffset(int channel);


private:
    MainPresenter();
    MainView& view;
};

#endif // MAIN_PRESENTER_HPP
