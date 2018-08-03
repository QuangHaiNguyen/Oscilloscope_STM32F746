/**************************************************************************************************
* FILE NAME:  TemplateView.cpp                                                                    *
* PURPOSE:    Setup the osciliscope screen, setup other widgets/ objects attached to the screen   *                                                                                    *
*                                                                                                 *
* FILE REFERENCES:                                                                                *
*                                                                                                 *
* Name I/O Description                                                                            *
* ---- --- -----------                                                                            *
*                                                                                                 *
* EXTERNAL VARIABLES:                                                                             *
* Source: < >                                                                                     *
*                                                                                                 *
* Name Type I/O Description                                                                       *
* ---- ---- --- -----------                                                                       *
*                                                                                                 *
* EXTERNAL REFERENCES:                                                                            *
*                                                                                                 *
* Name Description                                                                                *
* ---- -----------                                                                                *
*                                                                                                 *
* ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES:                                    *
*                                                                                                 *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                                                         *
*                                                                                                 *
* NOTES:                                                                                          *
*                                                                                                 *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:                                              *
*                                                                                                 *
* DEVELOPMENT HISTORY:                                                                            *
*                                                                                                 *
* Date        Author      Change Id  Release   Description Of Change                              *
* ----        ------      ---------  -------   ---------------------                              *
* 28.06.2016  Hai Nguyen         01      1.0   Original Files, add comments and prolog            *
* 29.06.2016  Hai Nguyen         02      2.0   Porting to touchgfx 4.6.0                          *
*                                              Use SetUpButtonImage() from ChannelControlPannel   *
*                                                  class                                          *
* 30.06.2016  Hai Nguyen         03      2.0   Add CallBack and Callback function to deal with    *
*                                                  button insed the control panel                 *
* 01.07.2016  Hai Nguyen         04      2.0  Re-design interface                                 *
*                                                                                                 *
***************************************************************************************************/

#include <gui/main_screen/MainView.hpp>
#include <touchgfx\Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gui\model\Model.hpp>
#include <gui\common\main_header.h>



/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  setupScreen                                                                      *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::setupScreen()
{
	tickCounter = 0;
	/*
	* Background configuration: add application,
	*   grid, control panel and channel background
	*/
	presenter->activate();
	appBackground.setBitmap(Bitmap(BITMAP_APPBACKGROUND_ID));
	appBackground.setXY(0, 0);
	add(appBackground);

	oziBackground.setBitmap(Bitmap(BITMAP_OZIBACKGROUND_ID));
	oziBackground.setXY(3, 3);
	add(oziBackground);

	controlPanelBackground.setBitmap(Bitmap(BITMAP_CONTROLPANEL_ID));
	controlPanelBackground.setXY(314, 3);
	add(controlPanelBackground);

	channelBackground.setBitmap(Bitmap(BITMAP_CHANNELINFO_ID));
	channelBackground.setXY(349, 240);
	add(channelBackground);

	/*
	* Button section:  Configure buttons for changing between two channels
	*/

	rightButton.setBitmaps(Bitmap(BITMAP_RIGHTBUTTONUNPRESS_ID), Bitmap(BITMAP_RIGHTBUTTONPRESS_ID));
	rightButton.setXY(450, 240);
	rightButton.setAction(buttonClickedCallback);
	add(rightButton);

	leftButton.setBitmaps(Bitmap(BITMAP_LEFTBUTTONUNPRESS_ID), Bitmap(BITMAP_LEFTBUTTONPRESS_ID));
	leftButton.setXY(315, 240);
	leftButton.setAction(buttonClickedCallback);
	add(leftButton);

	/*
	* Text section:  Configure Text label "Channel 1, channel 2"
	*/
	channelTxT[0].setTypedText(TypedText(T_CHN2));
	channelTxT[0].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	channelTxT[0].setWidth(channelBackground.getWidth());
	channelTxT[0].setHeight(channelBackground.getHeight());

	channelTxT[1].setTypedText(TypedText(T_CHN1));
	channelTxT[1].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	channelTxT[1].setWidth(channelBackground.getWidth());
	channelTxT[1].setHeight(channelBackground.getHeight());

	/*
	* Control pannel section: setup control pannel for 2 channels, setup viewport for slide transition
	*                         between two channels
	*/
	panelChn[0].SetUpButtonImage(BITMAP_UP_PRESS_1_ID,
								 BITMAP_UP_UNPRESS_1_ID,
								 BITMAP_DOWN_PRESS_1_ID,
								 BITMAP_DOWN_UNPRESS_1_ID,
								 BITMAP_LEFT_PRESS_1_ID,
								 BITMAP_LEFT_UNPRESS_1_ID,
								 BITMAP_RIGHT_PRESS_1_ID,
								 BITMAP_RIGHT_UNPRESS_1_ID);

	panelChn[0].setup(controlPanelBackground.getWidth(),
					  controlPanelBackground.getHeight(),
					  BITMAP_CHN1CONTROLBUTTONON_ID,
					  BITMAP_CHN1CONTROLBUTTONOFF_ID,
					  BITMAP_CH1UPONN_ID,
					  BITMAP_CH1UPOFF_ID,
					  BITMAP_CH1DOWNONN_ID,
					  BITMAP_CH1DOWNOFF_ID);

	panelChn[0].SetChannelPanelCallback(CtrlPanelBtnPressCallback);


	panelChn[1].SetUpButtonImage(BITMAP_UP_PRESS_2_ID,
								 BITMAP_UP_UNPRESS_2_ID,
								 BITMAP_DOWN_PRESS_2_ID,
								 BITMAP_DOWN_UNPRESS_2_ID,
								 BITMAP_LEFT_PRESS_2_ID,
								 BITMAP_LEFT_UNPRESS_2_ID,
								 BITMAP_RIGHT_PRESS_2_ID,
								 BITMAP_RIGHT_UNPRESS_2_ID);

	panelChn[1].setup( controlPanelBackground.getWidth(),
					   controlPanelBackground.getHeight(),
					   BITMAP_CHN2CONTROLBUTTONON_ID,
					   BITMAP_CHN2CONTROLBUTTONOFF_ID,
					   BITMAP_CH2UPONN_ID,
					   BITMAP_CH2UPOFF_ID,
					   BITMAP_CH2DOWNONN_ID,
					   BITMAP_CH2DOWNOFF_ID);

	panelChn[1].SetChannelPanelCallback(CtrlPanelBtnPressCallback);

	panelChn[1].SetMarkerButtonOn();
	panelChn[1].SetTriggerButtonOn();

	chnTextViewPort.setPosition( channelBackground.getX(),
								 channelBackground.getY(),
								 channelBackground.getWidth(),
								 channelBackground.getHeight());

	chnControlPanelViewPort.setPosition( controlPanelBackground.getX(),
		                                 controlPanelBackground.getY() + 5,
		                                 controlPanelBackground.getWidth(),
		                                 controlPanelBackground.getHeight());

	for (int i = 0; i < NUMBER_OF_CHANNEL; i++)
	{
		/*
		* Add control pannels and channel text lable into the viewport for sliding animation
		*/

		chnTextViewPort.add(channelTxT[i]);
		chnControlPanelViewPort.add(panelChn[i]);

		channelTxT[i].setXY( -channelTxT[selectedChnIndex].getWidth(),
			                 (chnTextViewPort.getHeight() - channelTxT[i].getHeight()) / 2 + 4);

		panelChn[i].setPosition( -panelChn[selectedChnIndex].getWidth(),
			                     0,
			                     controlPanelBackground.getWidth(),
			                     controlPanelBackground.getHeight());
	}

	add(chnTextViewPort);
	add(chnControlPanelViewPort);
	selectedChnIndex = 0;
	slideTexts(LEFT);

	/*
	* Graph section: setup the graph
	*/

	chan_1_graph.setPosition(0, 
							 presenter->p_GetYOffset(CHANNEL_1), 
							 oziBackground.getWidth(), 
							 oziBackground.getHeight());

	chan_1_graph.SetGraphColor(Color::getColorFrom24BitRGB(255, 128, 0));
	chan_1_graph.SetGraphData(presenter->p_GetTriggerData(CHANNEL_1));

	chan_2_graph.setPosition(0, 
							 presenter->p_GetYOffset(CHANNEL_2), 
							 oziBackground.getWidth(), 
							 oziBackground.getHeight());

	chan_2_graph.SetGraphColor(Color::getColorFrom24BitRGB(102, 178, 255)); 
	chan_2_graph.SetGraphData(presenter->p_GetTriggerData(CHANNEL_2));

	graph_container.setPosition(oziBackground.getX(),
								oziBackground.getY(),
								oziBackground.getWidth(),
								oziBackground.getHeight());
	
	add(graph_container);

	Application::getInstance()->registerTimerWidget(&chan_1_graph);
	Application::getInstance()->registerTimerWidget(&chan_2_graph);


	/*
	* Marker section: Setup the Marker
	*/

	marker1.setPosition(oziBackground.getX(), oziBackground.getY(), oziBackground.getWidth(), oziBackground.getHeight());
	marker1.setup(marker1.getX(), marker1.getHeight(), marker1.getWidth(), Color::getColorFrom24BitRGB(112, 242, 68));
	add(marker1);

	marker2.setPosition(oziBackground.getX(), oziBackground.getY(), oziBackground.getWidth(), oziBackground.getHeight());

	marker2.setup( oziBackground.getWidth() - 50, marker2.getHeight(), marker2.getWidth(), Color::getColorFrom24BitRGB(255, 23, 133));

	add(marker2);

	/*
	* Trigger Line: Setup the Trigger Line
	*/
	triggLine1.setPosition(oziBackground.getX(),
		                  oziBackground.getY(),
		                  oziBackground.getWidth(),
		                  oziBackground.getHeight());

	triggLine1.setup(chan_2_graph.getY() + 100,
	              	marker2.getHeight(),
		            marker2.getWidth(),
		            Color::getColorFrom24BitRGB(255, 247, 0));

	add(triggLine1);

	presenter->p_SetTriggerValue(CHANNEL_2, presenter->p_GetVoltOffset(CHANNEL_2) - triggLine1.TriggerPosition());

	triggLine2.setPosition( oziBackground.getX(),
							chan_1_graph.getY(),
							oziBackground.getWidth(),
							oziBackground.getHeight());

	triggLine2.setup(	chan_1_graph.getY(),
						marker2.getHeight(),
						marker2.getWidth(),
						Color::getColorFrom24BitRGB(255, 32, 0));

	
	add(triggLine2);

	/*
	 *  Display Value section
	 */
	cursor_text.setTypedText(TypedText(T_CURSOR));
	cursor_text.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	cursor_text.setXY(25, 245);
	add(cursor_text);

	cursor_txt_background.setXY(70, 243);
	cursor_txt_background.setBitmap(Bitmap(BITMAP_TEXT_BACKGROUND_ID));
	add(cursor_txt_background);

	trigger_1_lvl.setTypedText(TypedText(T_TRIG1_LEVEL));
	trigger_1_lvl.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	trigger_1_lvl.setXY(125, 245);
	add(trigger_1_lvl);

	trig1_txt_background.setXY(165, 243);
	trig1_txt_background.setBitmap(Bitmap(BITMAP_TEXT_BACKGROUND_ID));
	add(trig1_txt_background);

	trigger_2_lvl.setTypedText(TypedText(T_TRIG2_LEVEL));
	trigger_2_lvl.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	trigger_2_lvl.setXY(220, 245);
	add(trigger_2_lvl);

	trig2_txt_background.setXY(260, 243);
	trig2_txt_background.setBitmap(Bitmap(BITMAP_TEXT_BACKGROUND_ID));
	add(trig2_txt_background);

	Unicode::snprintf(cursor_buff, 10, "%d", 0);
	cursor_value_wildcard.setTypedText(TypedText(T_CURSOR_VALUE));
	cursor_value_wildcard.setWildcard(cursor_buff);
	cursor_value_wildcard.setPosition(70, 244,50,20);
	cursor_value_wildcard.setColor(Color::getColorFrom24BitRGB(255, 255, 255));
	add(cursor_value_wildcard);


	Unicode::snprintf(trig1_buff, 5, "%d", 0);
	trig1_value_wildcard.setTypedText(TypedText(T_TRIGG1_VALUE));
	trig1_value_wildcard.setWildcard(trig1_buff);
	trig1_value_wildcard.setPosition(160, 244, 50, 20);
	trig1_value_wildcard.setColor(Color::getColorFrom24BitRGB(255, 255, 255));
	add(trig1_value_wildcard);
	trig1_value_wildcard.invalidate();

	Unicode::snprintf(trig2_buff, 5, "%d", 0);
	trig2_value_wildcard.setTypedText(TypedText(T_TRIGG2_VALUE));
	trig2_value_wildcard.setWildcard(trig2_buff);
	trig2_value_wildcard.setPosition(255, 244, 50, 20);
	trig2_value_wildcard.setColor(Color::getColorFrom24BitRGB(255, 255, 255));
	add(trig2_value_wildcard);
	trig2_value_wildcard.invalidate();

	/*
     * Control Menu section	
   	 */
	control_menu.setup(SlideMenu::EAST, Bitmap(BITMAP_SLIDEMENU_ID), Bitmap(BITMAP_SIDESLIDEBUTTON_ID), Bitmap(BITMAP_SIDESLIDEBUTTON_ID), 0, 0, 279, 0);
	control_menu.setXY(3, 235);
	control_menu.setVisiblePixelsWhenCollapsed(15);
	add(control_menu);

	txt_ch1_ctrl_menu.setTypedText(TypedText(T_CHN1_CTRL_MENU));
	txt_ch1_ctrl_menu.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	txt_ch1_ctrl_menu.setXY(5,8);
	control_menu.add(txt_ch1_ctrl_menu);

	chn1_enable.setBitmaps(Bitmap(BITMAP_CHN1CONTROLBUTTONOFF_ID), Bitmap(BITMAP_CHN1CONTROLBUTTONON_ID));
	chn1_enable.setXY(120, 7);
	chn1_enable.setAction(buttonClickedCallback);
	control_menu.add(chn1_enable);
	
	txt_ch2_ctrl_menu.setTypedText(TypedText(T_CHN2_CTRL_MENU));
	txt_ch2_ctrl_menu.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	txt_ch2_ctrl_menu.setXY(85, 8);
	control_menu.add(txt_ch2_ctrl_menu);

	chn2_enable.setBitmaps(Bitmap(BITMAP_CHN2CONTROLBUTTONOFF_ID), Bitmap(BITMAP_CHN2CONTROLBUTTONON_ID));
	chn2_enable.setXY(40, 7);
	chn2_enable.setAction(buttonClickedCallback);
	chn2_enable.forceState(true);
	graph_container.add(chan_2_graph);
	control_menu.add(chn2_enable);

	Intro();

	

}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  tearDownScreen                                                                   *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::tearDownScreen()
{
}


/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  buttonClicked                                                                    *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE             I/O    DESCRIPTION                                                     *
* -------- ----             ---    -----------                                                     *
* source   AbstractButton     I    The source of the clicked button                                *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::buttonClicked(const AbstractButton &source)
{
	/*Chenking which button is pressed then call the approriate event handler function*/

	if (&source == &leftButton)
	{
		slideTexts(LEFT);
	}

	else if (&source == &rightButton)
	{
		
		slideTexts(RIGHT);
	}
	else if (&source == &chn1_enable)
	{
		control_menu.resetExpandedStateTimer();
		if (chn1_enable.getState() == true)
		{
			graph_container.add(chan_1_graph);
		}
		else
		{
			graph_container.remove(chan_1_graph);

		}
	}
	else if (&source == &chn2_enable)
	{
		control_menu.resetExpandedStateTimer();
		if (chn2_enable.getState() == true)
		{
			graph_container.add(chan_2_graph);
		}
		else
		{
			graph_container.remove(chan_2_graph);
		}
	}
}

void MainView::Intro(void)
{
	back_ground.setPosition(0,0,480,272);
	back_ground.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	add(back_ground);

	heroes.setBitmap(Bitmap(BITMAP_HEROES_ID));
	heroes.setAlpha(alpha);
	heroes.setXY(25, 0);
	add(heroes);

	arrow.setBitmap(Bitmap(BITMAP_ARROW_ID));
	arrow.setXY(0, 0);
	add(arrow);
}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  slideTexts                                                                       *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT   TYPE       I/O   DESCRIPTION                                                          *
* --------   ----       ---   -----------                                                          *
* direction  Direction    I   direction of the slide animation                                     *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::slideTexts(SlideDirection direction)
{
	/*
	* Slide animation for control pannel and text labels
	* Calculate the position needed for the slide animation, procced the animation
	* Prepare for the next slide animation
	* Enable, disable graph according to the channel
	*/

	for (int i = 0; i < NUMBER_OF_CHANNEL; i++)
	{
		/* Checking if is there any slide animation or not*/
		if (channelTxT[i].isRunning())
			return;
	}

	int animationDuration = 14;
	int animationDirection = (direction == LEFT) ? -1 : 1;
	int animationDistance = animationDirection * chnTextViewPort.getWidth();
	int animationDistance1 = animationDuration * chnControlPanelViewPort.getWidth();

	int nextSelectedText = (selectedChnIndex + NUMBER_OF_CHANNEL + animationDirection) % NUMBER_OF_CHANNEL;

	/*
	* Slide selected object
	*/
	channelTxT[selectedChnIndex].startMoveAnimation(channelTxT[selectedChnIndex].getX() + animationDistance,
	                                               	4,
		                                            animationDuration,
		                                            EasingEquations::cubicEaseInOut);

	panelChn[selectedChnIndex].startMoveAnimation(panelChn[selectedChnIndex].getX() + animationDistance1,
		                                          panelChn[selectedChnIndex].getY(),
		                                          animationDuration,
		                                          EasingEquations::cubicEaseInOut);

	int xOffsetForText = (chnTextViewPort.getWidth() - channelTxT[nextSelectedText].getWidth()) / 2;

	int xOffsetForContainer = (chnControlPanelViewPort.getWidth() - panelChn[nextSelectedText].getWidth()) / 2;

	/*Prepare for the next slide*/
	channelTxT[nextSelectedText].moveTo(xOffsetForText - animationDistance, 4);
	panelChn[nextSelectedText].moveTo(xOffsetForContainer - animationDistance1,
		                              panelChn[nextSelectedText].getY());

	channelTxT[nextSelectedText].startMoveAnimation( channelTxT[nextSelectedText].getX() + animationDistance,
		                                             4,
													 animationDuration,
													 EasingEquations::cubicEaseOut);

	panelChn[nextSelectedText].startMoveAnimation( panelChn[nextSelectedText].getX() + animationDistance1,
												   panelChn[nextSelectedText].getY(),
												   animationDuration,
												   EasingEquations::cubicEaseOut);

	selectedChnIndex = nextSelectedText;
	
}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  handleTickEvent                                                                  *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT   TYPE       I/O   DESCRIPTION                                                          *
* --------   ----       ---   -----------                                                          *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::handleTickEvent()
{
	/*
	* For every tick, check which marker, trigger line is enabled/disabled
	* For every ten ticks, the graph is invalidated
	*/
	presenter->p_SetRawData(CHANNEL_1);
	presenter->p_SetRawData(CHANNEL_2);

	/*Used to store some temporary value for calculation*/
	float temp_value;

	if (selectedChnIndex == 0)
	{
		triggLine2.EnableLine(panelChn[0].isMarkerButtonClicked());
		triggLine1.EnableLine(false);
		marker1.EnableLine(panelChn[0].isMarkerAButtonClicked());
		marker2.EnableLine(panelChn[0].isMarkerBButtonClicked());

		presenter->p_SetTrigger(CHANNEL_1, panelChn[0].isTriggerButtonClicked());

		presenter->p_SetTriggerType(CHANNEL_1, panelChn[0].isFallingButtonClicked());

		cursor_value = abs(marker1.MarkerPosition() - marker2.MarkerPosition());

		temp_value = cursor_value * presenter->p_GetTimeOffset(CHANNEL_1);

		if (presenter->p_GetTimeScale(CHANNEL_1) > 3)
			temp_value = (temp_value / 1000);

		Unicode::snprintfFloat(cursor_buff, 10, "%.2f", temp_value);
		cursor_value_wildcard.invalidate();

		temp_value = presenter->p_GetTriggerValue(CHANNEL_1) * presenter->p_VoltagePerPixel(CHANNEL_1);
		if (presenter->p_GetVoltageScale(CHANNEL_1) > 5)
			temp_value = temp_value / 1000;

		presenter->p_SetTriggerValue(CHANNEL_1, presenter->p_GetVoltOffset(CHANNEL_1) - triggLine2.TriggerPosition());
		Unicode::snprintfFloat(trig2_buff, 5, "%.2f", temp_value);
		trig2_value_wildcard.invalidate();

		chan_1_graph.setY(panelChn[0].GetYOffset());
		presenter->p_SetYOffset(CHANNEL_1, panelChn[0].GetYOffset());
		presenter->p_SetXOffset(CHANNEL_1, panelChn[0].GetXOffset());
		chan_1_graph.invalidate();

		triggLine2.setY(2 * chan_1_graph.getY());
		triggLine2.SetVoltOffset(presenter->p_GetVoltOffset(CHANNEL_1));
		triggLine2.invalidate();

		presenter->p_SetTimeScale(CHANNEL_1, panelChn[0].GetTimeBaseIndex());
		presenter->p_SetVoltageScale(CHANNEL_1, panelChn[0].GetVoltBaseIndex());
	}
	else
	{
		
		triggLine1.EnableLine(panelChn[1].isMarkerButtonClicked());
		triggLine2.EnableLine(false);

		marker1.EnableLine(panelChn[1].isMarkerAButtonClicked());
		marker2.EnableLine(panelChn[1].isMarkerBButtonClicked());
		
		presenter->p_SetTrigger(CHANNEL_2, panelChn[1].isTriggerButtonClicked());
		presenter->p_SetTriggerType(CHANNEL_2, panelChn[1].isFallingButtonClicked());

		cursor_value = abs(marker1.MarkerPosition() - marker2.MarkerPosition());

		temp_value = cursor_value * presenter->p_GetTimeOffset(CHANNEL_2);
		if (presenter->p_GetTimeScale(CHANNEL_2) > 3)
			temp_value = (temp_value / 1000);

		Unicode::snprintfFloat(cursor_buff, 10, "%.2f", temp_value);
		cursor_value_wildcard.invalidate();

		temp_value = presenter->p_GetTriggerValue(CHANNEL_2) * presenter->p_VoltagePerPixel(CHANNEL_2);
		if (presenter->p_GetVoltageScale(CHANNEL_2) > 5)
			temp_value = temp_value / 1000;

		presenter->p_SetTriggerValue(CHANNEL_2, presenter->p_GetVoltOffset(CHANNEL_2) - triggLine1.TriggerPosition());
		Unicode::snprintfFloat(trig1_buff, 5, "%.2f", temp_value);
		trig1_value_wildcard.invalidate();

		chan_2_graph.setY(panelChn[1].GetYOffset());
		presenter->p_SetYOffset(CHANNEL_2, panelChn[1].GetYOffset());
		presenter->p_SetXOffset(CHANNEL_2, panelChn[1].GetXOffset());
		chan_2_graph.invalidate();
		

		triggLine1.setY( 2 * panelChn[1].GetYOffset());
		triggLine1.SetVoltOffset(presenter->p_GetVoltOffset(CHANNEL_2));
		triggLine1.invalidate();

		presenter->p_SetTimeScale(CHANNEL_2, panelChn[1].GetTimeBaseIndex());
		presenter->p_SetVoltageScale(CHANNEL_2, panelChn[1].GetVoltBaseIndex());
	}

	tickCounter++;
	if (tickCounter % 1 == 0)
	{
		alpha++;
		heroes.setAlpha(alpha);
		heroes.invalidate();
		arrow.setAlpha(255 - alpha);
		arrow.invalidate();
	}
	if (alpha == 255)
	{
		tickCounter = 0;
		remove(arrow);
		remove(heroes);
		remove(back_ground);
	}

}
/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  CtrlPanelBtnPressed                                                              *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT   TYPE       I/O   DESCRIPTION                                                          *
* --------   ----       ---   -----------                                                          *
* button_ID  uint8_t      I   The ID of the pressed button inside control panel                    *
*                                                                                                  *
* RETURNS:   void                                                                                  *
*                                                                                                  *
***************************************************************************************************/
void MainView::CtrlPanelBtnPressed(const uint8_t button_ID)
{
	uint8_t temp = 0;
}