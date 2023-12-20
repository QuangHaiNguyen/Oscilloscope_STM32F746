/**************************************************************************************************
* FILE NAME: TemplateView.hpp                                                                     *
*                                                                                                 *
* PURPOSE:   Header file of TemplateView.cpp                                                      *                                 *
*                                                                                                 *
* PULIC VARIABLES:                                                                                *
*                                                                                                 *
* Variable        Type            Description                                                     *
* --------        ----            -----------                                                     *
* selectecChannelIndex        int                  Store the index of the selected channel        *
*                                                                                                 *
* PUBLIC FUNCTIONS:                                                                               *
*                                                                                                 *
* Name                   Return      Description                                                  *
* ----                   ------      -----------                                                  *
* TemplateView           N/A         Constructor of TemplateView Class                            *
* ~TemplateView          N/A         Decosntructor of TemplateView Class                          *
* setupScreen            void        First called to setup the screen and widgets that it         *
*                                          contained                                              *
* tearDownScreen         void        Called when we delete screen                                 *
* handleTichEvent        void        Callback funtion for tick event                              *
* buttonClicked          void        Callback function for button clicked event                   *
*                                                                                                 *
* PRIVATE VARIABLES:                                                                              *
*                                                                                                 *
* Variable                    Type                 Description                                    *
* --------                    ----                 -----------                                    *
* chnTextViewPort             Container            Container of the text Channel 1, Channel 2     *
* chnControlPanelViewPort     Container            Container of the control panel object          *
*                                                     (1 or 2)                                    *
* tickCounter                 int                  Store the number of tick                       *
* panelChn[]                  ChannelControlPanel  Array to store multiple Control Pannel         *
* ChannelTxT[]                TextArea             Array to store multiple texts - name of Channel*
* oziBackground               Image                Grid Image of oscilloscope                     *
* appBackground               Image                Background of the application                  *
* controlPanelBackground      Image                Background of the control Panel                *
* channelBackground           Image                Background of the channel box                  *
* rightButton                 Button               Button object to move channel and control      *
*                                                     panel to the right                          *
* leftButton                  Button               Button object to move channel and control      *
*                                                     panel to the left                           *
* chn1Graph                   Graph                Graph object to plot the data from channel 1   *
* chn2Graph                   Graph                Graph object to plot the data from channel 2   *
* marker1                     Marker               Marker A object                                *
* marker2                     Marker               Marker B object                                *
* trggLine                    TriggerLine          Trigger Line Object                            *
* SlideDirection              enum                 LEFT, RIGHT                                    *
*                                                                                                 *
* PRIVATE FUNCTIONS:                                                                              *
*                                                                                                 *
* Name                    Return               Description                                        *
* ----                    ------               -----------                                        *
* slideText               void                 Take care of the sliding animation                 *
* CtrlPanelBtnPressed     void                 Take care of which button in the control panel is  * 
*                                                pressed                                          *
* CtrlPanelBtnPressCallback                    Callback event when button in control Panel is     *
*                                                pressed 
* buttonClickedCallback   Callback function    Callback of the button clicked event               *
*                                                                                                 *
* Date        Author       Change Id Release  Description Of Change                               *
* ----        ------       --------- -------  ---------------------                               *
* 28.06.2012  Hai Nguyen          01     1.0  Original File, Add Comment and Prolog               *
* 29.06.2016  Hai Nguyen          02     2.0  Porting to touchgfx 4.6.0                           *
* 30.06.2016  Hai Nguyen          03     2.0  Add event handler for button inside control panel   *
* 01.07.2016  Hai Nguyen          04     2.0  Re-design interface                                 *
*                                                                                                 *
**************************************************************************************************/
#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/intro_screen/IntroPresenter.hpp>
#include <gui\model\ModelListener.hpp>
#include <gui\common\main_header.h>

#include <touchgfx\mixins\MoveAnimator.hpp>


using namespace touchgfx;

class IntroView : public View<IntroPresenter>
{
public:
	IntroView()
	{
	}

	virtual ~IntroView() { }
	virtual void setupScreen();
	virtual void tearDownScreen();
	virtual void handleTickEvent();

	
private:

};

#endif // MAIN_VIEW_HPP

