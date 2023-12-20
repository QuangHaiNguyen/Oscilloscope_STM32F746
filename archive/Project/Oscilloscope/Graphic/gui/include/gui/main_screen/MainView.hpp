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
#include <gui/main_screen/MainPresenter.hpp>
#include <gui\model\ModelListener.hpp>
#include <gui\common\main_header.h>

#include <touchgfx\widgets\ToggleButton.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx\widgets\TextAreaWithWildcard.hpp>

#include <touchgfx/containers/SlideMenu.hpp>
#include <touchgfx\containers\ListLayout.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>

#include <gui\common\Marker.hpp>
#include <gui\common\ChannelControlPanel.hpp>
#include <gui\common\TriggerLine.hpp>
#include <gui\common\Graph.hpp>
#include <gui\common\main_header.h>

#include <touchgfx\mixins\MoveAnimator.hpp>


using namespace touchgfx;

class MainView : public View<MainPresenter>
{
public:
	MainView()
		:buttonClickedCallback(this, &MainView::buttonClicked),
		 CtrlPanelBtnPressCallback(this, &MainView::CtrlPanelBtnPressed )
	{
	}
	virtual ~MainView() { }
	virtual void setupScreen();
	virtual void tearDownScreen();
	virtual void handleTickEvent();

	void buttonClicked(const AbstractButton & source);

	static const int NUMBER_OF_CHANNEL = 2;

	int selectedChnIndex;

	void Intro(void);

private:
	Box back_ground;

	Container chnTextViewPort;
	Container chnControlPanelViewPort;

	SlideMenu control_menu;
	
	int tickCounter;

	MoveAnimator <ChannelControlPanel> panelChn[NUMBER_OF_CHANNEL];

	MoveAnimator <TextArea> channelTxT[NUMBER_OF_CHANNEL];

	Image oziBackground;
	Image appBackground;
	Image controlPanelBackground;
	Image channelBackground;
	Image cursor_txt_background;
	Image trig1_txt_background;
	Image trig2_txt_background;
	Image arrow;
	Image heroes;

	Button rightButton;
	Button leftButton;

	ToggleButton chn1_enable;
	ToggleButton chn2_enable;

	Marker marker1;
	Marker marker2;

	TriggerLine triggLine1;
	TriggerLine triggLine2;

	Graph chan_1_graph;
	Graph chan_2_graph;

	Container graph_container;

	TextArea cursor_text;
	TextArea trigger_1_lvl;
	TextArea trigger_2_lvl;
	TextArea txt_ch1_ctrl_menu;
	TextArea txt_ch2_ctrl_menu;

	TextAreaWithOneWildcard cursor_value_wildcard;
	Unicode::UnicodeChar cursor_buff[10];

	TextAreaWithOneWildcard trig1_value_wildcard;
	Unicode::UnicodeChar trig1_buff[5];

	TextAreaWithOneWildcard trig2_value_wildcard;
	Unicode::UnicodeChar trig2_buff[5];

	enum SlideDirection
	{
		LEFT,
		RIGHT
	};

	void slideTexts(SlideDirection direction);
	void CtrlPanelBtnPressed(const uint8_t button_ID);

	Callback <MainView, const AbstractButton & > buttonClickedCallback;
	Callback <MainView, const uint8_t> CtrlPanelBtnPressCallback;

	int cursor_value;
	int alpha;
};

#endif // MAIN_VIEW_HPP

