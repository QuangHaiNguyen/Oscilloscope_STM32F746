/**************************************************************************************
* NAME:     ChannelControlPanel.hpp                                                   *
*                                                                                     *
* PURPOSE:  Header of ChannelControlPanel.cpp                                         *
*                                                                                     *
* PULIC VARIABLES:                                                                    *
*                                                                                     *
* Variable        Type            Description                                         *
* --------        ----            -----------                                         *
*                                                                                     *
* PULIC FUNCTIONS:                                                                    *
*                                                                                     *
* Name                      Return      Description                                   *
* ----                      ------      -----------                                   *
* ChannelControlPanel       N/A         Constructor of Channel Control Panel          *
*                                        class                                        *
* ~ChannelControlPanel      virtual     Deconstructor of Channel Control Panel        *
*                                        class                                        *
* buttonClicked             void        Call back function for button clicked         *
*                                        enevt                                        *
* setup                     void        Setup the properties of the control           *
*                                        panel object                                 *
* isTriggerButtonClicked    bool        Return the Flag to indicate button is         *
*                                        clicked or not                               *
* isFallingButtonClicked    bool        Return the Flag to indicate button is         *
*                                        clicked or not                               *
* isMarkerButtonClicked     bool        Return the Flag to indicate button is         *
*                                        clicked or not                               *
* isMarkerAButtonClicked    bool        Return the Flag to indicate button is         *
*                                        clicked or not                               *
* isMarkerBButtonClicked    bool        Return the Flag to indicate button is         *
*                                        clicked or not                               *
* SetChannelPanelCallback               Assign callback from other class to an        *
*                                        internal call back                           *
*                                                                                     *
* PRIVATE VARIABLES:                                                                  *
*                                                                                     *
* Variable                    Type                 Description                        *
* --------                    ----                 -----------                        *
* triggerButtonClicked        bool                 Trigger Button Flag                * 
* fallingButtonClicked        bool                 Falling Button Flag                *
* markerButtonClicked         bool                 Marker  Button Flag                *
* markerAButtonClicked        bool                 Marker A Button Flag               *
* markerBButtonClicked        bool                 Marker B Button Flag               *
* selectedHorizontalTextIndex int                  Store the index value of the       *
*                                                    selected time base (horizontal)  *
* selectedVerticalTextIndex   int                  Store the index value of the       *
*                                                    selected voltage scale (vertical)*
* chnControlPanel             ScrollableContainer  Instance of ScrollableContainer    *
*                                                    Control Panel is created base on *
*                                                    this                             *
* horizontalTxtContainer      Container            Container object to store time     *
*                                                    base text values                 *
* verticalTxtContainer        Container            Container object to store voltage  *
*                                                    scale text values                *
* horizontalBkgndTxt          Image                Image for the background of the    *
*                                                    horizotal Container              *
* verticalBkgndTxt            Image                Image for the background of the    *
*                                                    vertical Container               *
* TriggerButton               Button               Trigger Button object              *
* FallingButton               Button               Falling Button object              *
* MarkerButton                Button               Trigger Line Enable Button object  *
* horizontalUp                Button               Increase time base Button object   *
* horizotalDown               Button               Decrease time base Button object   *
* verticalUp                  Button               Increase voltage Button object     *
* verticalDown                Button               Decrease voltage Button object     *
* aButton                     Button               Enable Marker A Button object      *
* bButton                     Button               Enable Marker B Button object      *
* move_up_button              RepeatButton         Move the Graph up                  *
* move_down_button            RepeatButton         Move the Graph down                *
* move_left_button            RepeatButton         Move the Graph left                *
* move_right_button           RepeatButton         Move the Graph right               *
* trgMenuTxt                  TextArea             Header Text of Triger menu         *
* trgButtonTxt                TextArea             Header Text of Trigger Button      *
* edgeButtonTxt               TextArea             Header Text of Falling Button      *
* markerButtonTxt             TextArea             Header Text of Marker Button       * 
* horizotalTxt                TextArea             Header text of horizontal section  *
* verticalTxt                 TextArea             Header text of vertical section    *
* markerLabel                 TextArea             Header Text of Marker section      *
* markerAButton               TextArea             Header Text of the Marker A button *
* markerBButton               TextArea             Header Text of the Marker B button *
* timeTxT                     <TextArea>           Text Array to contain all the value* 
*                                                       of the time base              *
* voltTxT                     <TextArea>           Text Array to contain all the value*
*                                                       of the voltage scale          *
* line1, line2, line3          Line                Line seperation between sections   *
* linePainter                  PainterRGB565       Contain the color to paint the     *
*                                                    lines                            *
* controlButtonOn              uint16_t            Store the ID of the button         *
* controlButtonOff             uint16_t            Store the ID of the button         *
* upOn                         uint16_t            Store the ID of the button         *
* upOff                        uint16_t            Store the ID of the button         *
* downOn                       uint16_t            Store the ID of the button         *
* downOff                      uint16_t            Store the ID of the button         *
* up_press                     uint16_t            Store the ID of the button         *
* down_press                   uint16_t            Store the ID of the button         *
* left_press                   uint16_t            Store the ID of the button         *
* right_press                  uint16_t            Store the ID of the button         *
* up_unpress                   uint16_t            Store the ID of the button         *
* down_unpress                 uint16_t            Store the ID of the button         *
* left_unpress                 uint16_t            Store the ID of the button         *
* right_unpress                uint16_t            Store the ID of the button         *
* SlideDirection               enum                UP, DOWN direction                 *
*                                                                                     *
* PRIVATE FUNCTIONS:                                                                  *
*                                                                                     *
* Name                    Return               Description                            *
* ----                    ------               -----------                            *
* slideHorizontalText     void                 Do the slide animation of the Time     *
*                                                 base text                           *
* slideVerticalText       void                 Do the slide animation of the Voltage  *
*                                                 scale text                          *
* buttonClickedCallback   Callback fucntion    Callback event for button clicked      *
* ChannelPanelSelectedCallBack                 Callback event for Channel selected    *
*                                                                                     *
* DEVELOPMENT HISTORY:                                                                *
*                                                                                     *
* Date        Author       Change Id Release  Description Of Change                   *
* ----        ------       --------- -------  ---------------------                   *
* 28.06.2016  Hai Nguyen          01     1.0  Original File, add Comment and Prolog   *
* 29.06.2016  Hai Nguyen          02     2.0  Porting to Touchgfx 4-6-0               *
*                                             Add four button to move the  graph      *
*                                                up/down and left/right               *
*                                             Add four variable to store image ID of  *
*                                                up/down and left/right buttons       *
* 30.06.2016  Hai Nguyen          03     2.0  Add some callback function to deal with *
*                                                external event                       *
*                                             Change the move graph up/down/left/right* 
*                                                buttons from Button to RepeatButton  *
*                                                                                     *
**************************************************************************************/


#ifndef CHANNELCONTROLPANEL_HPP
#define CHANNELCONTROLPANEL_HPP

#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <gui\common\main_header.h>

#include <touchgfx\widgets\ToggleButton.hpp>
#include<touchgfx\widgets\RepeatButton.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx\widgets\Image.hpp>
#include <touchgfx\widgets\Box.hpp>

#include <touchgfx/containers/Container.hpp>
#include <touchgfx\containers\ListLayout.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>

#include <touchgfx\mixins\MoveAnimator.hpp>
#include <gui/model/ModelListener.hpp>

#define TRUE  1
#define FALSE 0

using namespace touchgfx;

class ChannelControlPanel : public Container, public ModelListener
{
public:
	ChannelControlPanel();
	virtual ~ChannelControlPanel();
	void buttonClicked(const AbstractButton & source);
	
	void SetChannelPanelCallback(GenericCallback <const uint8_t > &callback);

	void setup(int width, int height, uint16_t buttonOn, uint16_t buttonOff,
	           uint16_t upArrowOn, uint16_t upArrowOff, uint16_t downArrowOn, uint16_t downArrowOff);

	void SetTriggerButtonOn(void);
	void SetMarkerButtonOn(void);

	bool isTriggerButtonClicked(void);
	bool isFallingButtonClicked(void);
	bool isMarkerButtonClicked(void);
	bool isMarkerAButtonClicked(void);
	bool isMarkerBButtonClicked(void);

	int GetTimeBaseIndex(void);
	int GetVoltBaseIndex(void);

	void SetUpButtonImage( uint16_t up_on, uint16_t up_off, uint16_t down_on, uint16_t down_off,
	                       uint16_t left_on, uint16_t left_off, uint16_t right_on, uint16_t right_off);
	int GetYOffset(void);
	int GetXOffset(void);
protected:

	bool triggerButtonClicked;
	bool fallingButtonClicked;
	bool markerButtonClicked;
	bool markerAButtonClicked;
	bool markerBButtonClicked;
	
	int selectedHorizontalTextIndex;
	int selectedVerticalTextIndex;

	static const int NUMBER_OF_TIME_BASE = 13;
	static const int NUMBER_OF_VOLT_BASE = 8;

	ScrollableContainer chnControlPanel;

	Container horizontalTxtContainer;
	Container verticalTxtContainer;

	Image horizontalBkgndTxt;
	Image verticalBkgndTxt;

	ToggleButton TriggerButton;
	ToggleButton FallingButton;
	ToggleButton MarkerButton;
	Button horizontalUp;
	Button horizontalDown;
	Button verticalUp;
	Button verticalDown;
	ToggleButton aButton;
	ToggleButton bButton;

	RepeatButton move_up_button;
	RepeatButton move_down_button;
	RepeatButton move_left_button;
	RepeatButton move_right_button;

	TextArea trgMenuTxt;
	TextArea trgButtonTxt;
	TextArea edgeButtonTxt;
	TextArea markerButtonTxt;
	TextArea horizontalTxt;
	TextArea verticalTxt;
	TextArea markerLabel;
	TextArea markerAButton;
	TextArea markerBButton;
	TextArea position_vertical_txt;
	TextArea position_horizontal_txt;
	MoveAnimator <TextArea> timeTxt[NUMBER_OF_TIME_BASE];
	MoveAnimator <TextArea> voltTxt[NUMBER_OF_VOLT_BASE];

	Line line1;
	Line line2;
	Line line3;

	PainterRGB565 linePainter;

	uint16_t controlButtonOn;
	uint16_t controlButtonOff;
	uint16_t upOn;
	uint16_t upOff;
	uint16_t downOn;
	uint16_t downOff;

	uint16_t up_press;
	uint16_t down_press;
	uint16_t left_press;
	uint16_t right_press;

	uint16_t up_unpress;
	uint16_t down_unpress;
	uint16_t left_unpress;
	uint16_t right_unpress;

	int y_offset;
	int x_offset;

	enum  SlideDirection
	{
		UP,
		DOWN
	};


	void slideHorizotalText(SlideDirection direction);
	void slideVerticalText(SlideDirection direction);

	Callback <ChannelControlPanel, const AbstractButton & > buttonClickedCallback;
	GenericCallback <const uint8_t >* ChannelPanelSelectedCallBack;

};
#endif