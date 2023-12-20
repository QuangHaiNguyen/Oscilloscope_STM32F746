/***************************************************************************************************
* FILE NAME: ChannelControlPanel.cpp                                                               *
* PURPOSE:   Create the Control Panel for each chanel of the oscilloscope                          *
*            There we can do triggering, enable marker, change time and voltage scale              *
*                                                                                                  *
* FILE REFERENCES:                                                                                 *
*                                                                                                  *
* Name I/O Description                                                                             *
* ---- --- -----------                                                                             *
*                                                                                                  *
* EXTERNAL VARIABLES:                                                                              *
* Source: < >                                                                                      *
*                                                                                                  *
* Name Type I/O Description                                                                        *
* ---- ---- --- -----------                                                                        *
*                                                                                                  *
* EXTERNAL REFERENCES:                                                                             *
*                                                                                                  *
* Name Description                                                                                 *
* ---- -----------                                                                                 *
*                                                                                                  *
* ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES:                                     *
*                                                                                                  *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                                                          *
*                                                                                                  *
* NOTES:                                                                                           *
*                                                                                                  *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:                                               *
*                                                                                                  *
* DEVELOPMENT HISTORY:                                                                             *
*                                                                                                  *
* Date        Author      Change Id  Release  Description Of Change                                *
* ----        ------      ---------  -------  ---------------------                                *
* 28.06.2016  Hai Nguyen         01     1.0   Original File, adding File Prolog and comment        *
* 29.06.2016  Hai Nguyen		 02     2.0   Porting to touchgfx 4.6.0                            * 
*                                                                                                  *
***************************************************************************************************/


#include <gui\common\ChannelControlPanel.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx\Color.hpp>
#include <gui/model/Model.hpp>

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: ChannelControlPanel - constructor                                       *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/
ChannelControlPanel::ChannelControlPanel()
	:buttonClickedCallback(this, &ChannelControlPanel::buttonClicked)
{
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: ~ChannelControlPanel - deconstructor                                    *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/
ChannelControlPanel::~ChannelControlPanel()
{
}
/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: setup                                                                   *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT       TYPE     I/O   DESCRIPTION                                              *
* --------       ----     ---   -----------                                              *
* width           int       I   Width of the panel                                       *
* height          int       I   height of the panel                                      *
* buttonOn        uint16_1  I   Image ID of the background for the button ON state       *
* buttonOff       uint16_1  I   Image ID of the background for the button OFF state      *
* upArrowOn       uint16_1  I   Image ID of the background for the up button ON state    *
* upArrowOff      uint16_1  I   Image ID of the background for the up button OFF state   *
* downArroWOn     uint16_1  I   Image ID of the background for the down button ON state  *
* downArroWOff    uint16_1  I   Image ID of the background for the down button OFF state *
*                                                                                        *
* RETURNS:  void                                                                         *
*                                                                                        *
*****************************************************************************************/
void ChannelControlPanel::setup(int width, int height, uint16_t buttonOn, uint16_t buttonOff,
			uint16_t upArrowOn, uint16_t upArrowOff, uint16_t downArrowOn, uint16_t downArrowOff)
{
	
	y_offset = 0;
	x_offset = 0;

	/*
	* Flag to nofify which button is pressed to call the according event handling function
	*/

	triggerButtonClicked = false;
	fallingButtonClicked = false;
	markerButtonClicked = false;
	markerAButtonClicked = false;
	markerBButtonClicked = false;


	/*
	* Passing the Button ID into the local variable
	*/

	controlButtonOn = buttonOn;
	controlButtonOff = buttonOff;
	upOn =  upArrowOff;
	upOff = upArrowOn;
	downOn =  downArrowOff;
	downOff = downArrowOn;



	/*
	* Setup the background fo two viewports
	*/

	horizontalBkgndTxt.setBitmap(Bitmap(BITMAP_COUNTERBOX_ID));
	horizontalBkgndTxt.setXY(18, 168);

	verticalBkgndTxt.setBitmap(Bitmap(BITMAP_COUNTERBOX_ID));
	verticalBkgndTxt.setXY(101, 168);


	/*
	* Viewport: at all the text labels of the voltage/div or time/div into the viewport
	*/

	chnControlPanel.setPosition(0, 0, width, height - 15);

	horizontalTxtContainer.setPosition( horizontalBkgndTxt.getX(), 
		                                horizontalBkgndTxt.getY(),
		                                horizontalBkgndTxt.getWidth(), 
		                                horizontalBkgndTxt.getHeight());

	/*
	 * Setup Text lable for time/div
	 */
	timeTxt[0].setTypedText(TypedText(T_50US));
	timeTxt[0].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[1].setTypedText(TypedText(T_100US));
	timeTxt[1].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[2].setTypedText(TypedText(T_200US));
	timeTxt[2].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[3].setTypedText(TypedText(T_500US));
	timeTxt[3].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[4].setTypedText(TypedText(T_1MS));
	timeTxt[4].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[5].setTypedText(TypedText(T_2MS));
	timeTxt[5].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[6].setTypedText(TypedText(T_5MS));
	timeTxt[6].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[7].setTypedText(TypedText(T_10MS));
	timeTxt[7].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[8].setTypedText(TypedText(T_20MS));
	timeTxt[8].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[9].setTypedText(TypedText(T_50MS));
	timeTxt[9].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[10].setTypedText(TypedText(T_100MS));
	timeTxt[10].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[11].setTypedText(TypedText(T_200MS));
	timeTxt[11].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	timeTxt[12].setTypedText(TypedText(T_500MS));
	timeTxt[12].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

	for (int i = 0; i < NUMBER_OF_TIME_BASE; i++)
	{
		horizontalTxtContainer.add(timeTxt[i]);

		timeTxt[i].setXY(horizontalTxtContainer.getWidth() / 2 - timeTxt[i].getWidth() / 2,
			-timeTxt[i].getHeight());
	}

	selectedHorizontalTextIndex = 2;

	slideHorizotalText(DOWN);

	verticalTxtContainer.setPosition(verticalBkgndTxt.getX(), verticalBkgndTxt.getY(),
		verticalBkgndTxt.getWidth(), verticalBkgndTxt.getHeight());

	/*
	 * Setup Text lable for volt/div
	 */
	voltTxt[0].setTypedText(TypedText(T_10MV));
	voltTxt[0].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[1].setTypedText(TypedText(T_20MV));
	voltTxt[1].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[2].setTypedText(TypedText(T_50MV));
	voltTxt[2].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[3].setTypedText(TypedText(T_100MV));
	voltTxt[3].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[4].setTypedText(TypedText(T_200MV));
	voltTxt[4].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[5].setTypedText(TypedText(T_500MV));
	voltTxt[5].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[6].setTypedText(TypedText(T_1V));
	voltTxt[6].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	voltTxt[7].setTypedText(TypedText(T_2V));
	voltTxt[7].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

	for (int i = 0; i < NUMBER_OF_VOLT_BASE; i++)
	{
		verticalTxtContainer.add(voltTxt[i]);

		voltTxt[i].setXY(verticalTxtContainer.getWidth() / 2 - voltTxt[i].getWidth() / 2,
			-voltTxt[i].getHeight());
	}

	selectedVerticalTextIndex = 6;

	slideVerticalText(DOWN);

	markerLabel.setTypedText(TypedText(T_MARKERTXTLABEL));
	markerLabel.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	markerLabel.setXY((chnControlPanel.getWidth() - markerLabel.getWidth()) / 2, 345);

	/*
	* Text label: setup the text label for each control section or button
	*/

	horizontalTxt.setTypedText(TypedText(T_HORIZONTALTXT));
	horizontalTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	horizontalTxt.setXY(15, 100);

	verticalTxt.setTypedText(TypedText(T_VERTICALTXT));
	verticalTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	verticalTxt.setXY(105, 100);

	trgMenuTxt.setTypedText(TypedText(T_TRGTEXTLABEL));
	trgMenuTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	trgMenuTxt.setXY(width/2 - trgMenuTxt.getWidth()/2, 10);

	trgButtonTxt.setTypedText(TypedText(T_TRGTEXT));
	trgButtonTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	trgButtonTxt.setXY(15, 33);

	edgeButtonTxt.setTypedText(TypedText(T_FALLTEXT));
	edgeButtonTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	edgeButtonTxt.setXY(width / 3 + 15, 33);

	markerButtonTxt.setTypedText(TypedText(T_MARKERTEXT));
	markerButtonTxt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	markerButtonTxt.setXY(110, 33);

	markerAButton.setTypedText(TypedText(T_MARKERA));
	markerAButton.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	markerAButton.setXY(chnControlPanel.getWidth() / 3 - markerAButton.getWidth() / 2 - 10, 370);

	markerBButton.setTypedText(TypedText(T_MARKERB));
	markerBButton.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	markerBButton.setXY(2 * chnControlPanel.getWidth() / 3 - markerAButton.getWidth() / 2 + 10, 370);

	position_vertical_txt.setTypedText(TypedText(T_POSITION));
	position_vertical_txt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	position_vertical_txt.setXY(chnControlPanel.getWidth() / 3 - markerAButton.getWidth() / 2 - 5, 250);

	position_horizontal_txt.setTypedText(TypedText(T_POSITION));
	position_horizontal_txt.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	position_horizontal_txt.setXY(2 * chnControlPanel.getWidth() / 3 - markerAButton.getWidth() / 2 + 20, 250);

	/*
	* Button: setup the buttons in the control pannel: Trigger, Falling edge, Marker, 
	* arrow up, arrow down 
	*/

	TriggerButton.setBitmaps(Bitmap(controlButtonOff), Bitmap(controlButtonOn));
	TriggerButton.setXY(15, 55);
	TriggerButton.setAction(buttonClickedCallback);

	FallingButton.setBitmaps(Bitmap(controlButtonOff), Bitmap(controlButtonOn));
	FallingButton.setXY(width / 3 + 8, 55);
	FallingButton.setAction(buttonClickedCallback);

	MarkerButton.setBitmaps(Bitmap(controlButtonOff), Bitmap(controlButtonOn));
	MarkerButton.setXY(110, 55);
	MarkerButton.setAction(buttonClickedCallback);
		
	horizontalUp.setBitmaps(Bitmap(upOff), Bitmap(upOn));
	horizontalUp.setXY(27, 130);
	horizontalUp.setAction(buttonClickedCallback);

	horizontalDown.setBitmaps(Bitmap(downOff), Bitmap(downOn));
	horizontalDown.setXY(27, 210);
	horizontalDown.setAction(buttonClickedCallback);

	verticalUp.setBitmaps(Bitmap(upOff), Bitmap(upOn));
	verticalUp.setXY(110, 130);
	verticalUp.setAction(buttonClickedCallback);

	verticalDown.setBitmaps(Bitmap(downOff), Bitmap(downOn));
	verticalDown.setXY(110, 210);
	verticalDown.setAction(buttonClickedCallback);

	aButton.setBitmaps(Bitmap(controlButtonOff), Bitmap(controlButtonOn));
	aButton.setXY(chnControlPanel.getWidth() / 3 - aButton.getWidth() / 2 - 10, 390);
	aButton.setAction(buttonClickedCallback);

	bButton.setBitmaps(Bitmap(controlButtonOff), Bitmap(controlButtonOn));
	bButton.setXY(2 * chnControlPanel.getWidth() / 3 - bButton.getWidth() / 2 + 10, 390);
	bButton.setAction(buttonClickedCallback);


	/*
	* Line: setup lines to seperate between sections
	*/
	linePainter.setColor(Color::getColorFrom24BitRGB(246, 241, 237));
	line1.setLineWidth(3);
	line1.setPosition(0, 90, width, 5);
	line1.setStart(15, 0);
	line1.setEnd(width - 10, 0);
	line1.setPainter(linePainter);

	line2.setLineWidth(3);
	line2.setPosition(85, 5, 10, 320);
	line2.setStart(0, 100);
	line2.setEnd(0, 320);
	line2.setPainter(linePainter);

	line3.setLineWidth(3);
	line3.setPosition(0, 335, width, 5);
	line3.setStart(15, 0);
	line3.setEnd(width - 10, 0);
	line3.setPainter(linePainter);	

	move_up_button.setXY(15,280);
	move_up_button.setAction(buttonClickedCallback);
	move_up_button.setBitmaps(Bitmap(up_unpress) , Bitmap(up_press));
	move_up_button.setDelay(10);

	move_down_button.setXY(50, 280);
	move_down_button.setAction(buttonClickedCallback);
	move_down_button.setBitmaps(Bitmap(down_unpress), Bitmap(down_press));
	move_down_button.setDelay(10);

	move_left_button.setXY(95, 280);
	move_left_button.setAction(buttonClickedCallback);
	move_left_button.setBitmaps(Bitmap(left_unpress), Bitmap(left_press));
	move_left_button.setDelay(10);

	move_right_button.setXY(130, 280);
	move_right_button.setAction(buttonClickedCallback);
	move_right_button.setBitmaps(Bitmap(right_unpress), Bitmap(right_press));
	move_right_button.setDelay(10);

	/*
	* Add all the things to the control panel container 
	*/
	chnControlPanel.add(TriggerButton);
	chnControlPanel.add(FallingButton);
	chnControlPanel.add(MarkerButton);
	chnControlPanel.add(trgMenuTxt);
	chnControlPanel.add(trgButtonTxt);
	chnControlPanel.add(edgeButtonTxt);
	chnControlPanel.add(markerButtonTxt);
	chnControlPanel.add(line1);
	chnControlPanel.add(line2);
	chnControlPanel.add(horizontalTxt);
	chnControlPanel.add(verticalTxt);
	chnControlPanel.add(horizontalUp);
	chnControlPanel.add(horizontalDown);
	chnControlPanel.add(verticalUp);
	chnControlPanel.add(verticalDown);
	chnControlPanel.add(horizontalBkgndTxt);
	chnControlPanel.add(verticalBkgndTxt);
	chnControlPanel.add(horizontalTxtContainer);
	chnControlPanel.add(verticalTxtContainer);
	chnControlPanel.add(line3);
	chnControlPanel.add(markerLabel);
	chnControlPanel.add(markerAButton);
	chnControlPanel.add(markerBButton);
	chnControlPanel.add(position_vertical_txt);
	chnControlPanel.add(position_horizontal_txt);
	chnControlPanel.add(aButton);
	chnControlPanel.add(bButton);

	chnControlPanel.add(move_up_button);
	chnControlPanel.add(move_down_button);
	chnControlPanel.add(move_left_button);
	chnControlPanel.add(move_right_button);
	add(chnControlPanel);
}

void ChannelControlPanel::SetTriggerButtonOn(void)
{
	triggerButtonClicked = true;
	TriggerButton.forceState(true);
}

void ChannelControlPanel::SetMarkerButtonOn(void)
{
	markerButtonClicked = true;
	MarkerButton.forceState(true);
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  buttonClicked                                                          *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE             I/O   DESCRIPTION                                            *
* -------- ----             ---   -----------                                            *
* source   AbstractButton     I   the source of the pressed button                       *
*                                                                                        *
* RETURNS: void                                                                          *
*                                                                                        *
*****************************************************************************************/


void ChannelControlPanel ::buttonClicked(const AbstractButton &source)
{
	/*
	* Check which button is clicked and procced the approriate action:
	* Set button flag to true/false
	* Change the skin of the button to indicate on/off state
	* Or calling the event handling function
	*/
	if (&source == &TriggerButton)
	{
		if (triggerButtonClicked == FALSE)
		{
			triggerButtonClicked = TRUE;
		}
		else
		{
			triggerButtonClicked = FALSE;
		}
	}
	else if (&source == &FallingButton)
	{
		if (fallingButtonClicked == FALSE)
		{
			fallingButtonClicked = TRUE;
		}
		else
		{
			fallingButtonClicked = FALSE;
		}
	}
	else if (&source == &MarkerButton)
	{
		if (markerButtonClicked == FALSE)
		{
			markerButtonClicked = TRUE;
		}
		else
		{
			markerButtonClicked = FALSE;
			
		}
	}
	else if (&source == &horizontalUp)
	{
		slideHorizotalText(UP);
	}
	
	else if (&source == &horizontalDown)
	{
		slideHorizotalText(DOWN);
	}

	else if (&source == &verticalUp)
	{
		slideVerticalText(UP);
	}

	else if (&source == &verticalDown)
	{
		slideVerticalText(DOWN);
	}
	else if (&source == &aButton)
	{
		if (markerAButtonClicked == FALSE)
		{
			markerAButtonClicked = TRUE;
		}
		else
		{
			markerAButtonClicked = FALSE;
		}
	}
	else if (&source == &bButton)
	{
		if (markerBButtonClicked == FALSE)
		{
			markerBButtonClicked = TRUE;
		}
		else
		{
			markerBButtonClicked = FALSE;
		}
	}

	else if (&source == &move_down_button)
	{
		y_offset++;
		if (y_offset > 80)
		{
			y_offset = 80;
		}
	}
	
	else if (&source == &move_up_button)
	{
		y_offset--;
		if (y_offset < 0)
		{
			y_offset = 0;
		}
	}
	else if (&source == &move_right_button)
	{
		x_offset++;
		if (x_offset > 300)
		{
			x_offset = 300;
		}
	}
	else if (&source == &move_left_button)
	{
		x_offset--;
		if (x_offset < -99)
		{
			x_offset = -99;
		}
	}

}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  slideHorizontalText                                                    *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* direction SlideDirection     I   Direction of the slide animation should make          *
*                                                                                        *
* RETURNS: void                                                                          *
*                                                                                        *
*****************************************************************************************/
void ChannelControlPanel::slideHorizotalText(SlideDirection direction)
{
	/*
	* Check if there is any slide animation at the moment or not
	* Caculate slide duration, slide distance and coordinate of the new sliding object
	* Perform the slide
	*/

	for (int i = 0; i < NUMBER_OF_TIME_BASE; i++)
	{
		if (timeTxt[i].isRunning())
		{
			return;
		}
	}

	int animationDuration = 14;
	int animationDirection = (direction == DOWN) ? -1 : 1;
	int animationDistance = animationDirection * horizontalTxtContainer.getHeight();

	int nextSelectedTextIndex = (selectedHorizontalTextIndex + NUMBER_OF_TIME_BASE + animationDirection) % NUMBER_OF_TIME_BASE;

	timeTxt[selectedHorizontalTextIndex].startMoveAnimation(timeTxt[selectedHorizontalTextIndex].getX(),
					                                     	timeTxt[selectedHorizontalTextIndex].getY() + animationDistance ,
		                                                    animationDuration, 
															EasingEquations::cubicEaseOut);

	int yOffsetForTxt= (horizontalTxtContainer.getHeight() - timeTxt[nextSelectedTextIndex].getHeight()) / 2;

	timeTxt[nextSelectedTextIndex].moveTo(timeTxt[nextSelectedTextIndex].getX(), yOffsetForTxt - animationDistance);

	timeTxt[nextSelectedTextIndex].startMoveAnimation(timeTxt[nextSelectedTextIndex].getX(), 
		                                              timeTxt[nextSelectedTextIndex].getY() + animationDistance, 
		                                              animationDuration, 
													  EasingEquations::cubicEaseOut);

	selectedHorizontalTextIndex = nextSelectedTextIndex;
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  slideVerticalText                                                      *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* direction SlideDirection     I   Direction of the slide animation should make          *
*                                                                                        *
* RETURNS: void                                                                          *
*                                                                                        *
*****************************************************************************************/
void ChannelControlPanel::slideVerticalText(SlideDirection direction)
{
	/*
	* Check if there is any slide animation at the moment or not
	* Caculate slide duration, slide distance and coordinate of the new sliding object
	* Perform the slide
	*/

	for (int i = 0; i < NUMBER_OF_VOLT_BASE; i++)
	{
		if (voltTxt[i].isRunning())
		{
			return;
		}
	}

	int animationDuration = 14;
	int animationDirection = (direction == DOWN) ? -1 : 1;
	int animationDistance = animationDirection * verticalTxtContainer.getHeight();

	int nextSelectedTextIndex = (selectedVerticalTextIndex + NUMBER_OF_VOLT_BASE + animationDirection) % NUMBER_OF_VOLT_BASE;

	voltTxt[selectedVerticalTextIndex].startMoveAnimation(voltTxt[selectedVerticalTextIndex].getX(),
		                                                  voltTxt[selectedVerticalTextIndex].getY() + animationDistance, 
		                                                  animationDuration, 
		                                                  EasingEquations::cubicEaseOut);

	int yOffsetForTxt = (horizontalTxtContainer.getHeight() - voltTxt[nextSelectedTextIndex].getHeight()) / 2;

	voltTxt[nextSelectedTextIndex].moveTo(voltTxt[nextSelectedTextIndex].getX(), yOffsetForTxt - animationDistance);

	voltTxt[nextSelectedTextIndex].startMoveAnimation(voltTxt[nextSelectedTextIndex].getX(), 
		                                              voltTxt[nextSelectedTextIndex].getY() + animationDistance,
		                                              animationDuration, 
													  EasingEquations::cubicEaseOut);

	selectedVerticalTextIndex = nextSelectedTextIndex;
}


/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  isTriggerButtonClicked                                                  *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* void                                                                                   *
*                                                                                        *
* RETURNS: bool                                                                          *
*                                                                                        *
*****************************************************************************************/
bool ChannelControlPanel::isTriggerButtonClicked(void)
{
	return triggerButtonClicked;
}

bool ChannelControlPanel::isFallingButtonClicked(void)
{
	return fallingButtonClicked;
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  isMarkerButtonClicked                                                  *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* void                                                                                   *
*                                                                                        *
* RETURNS: bool                                                                          *
*                                                                                        *
*****************************************************************************************/
bool ChannelControlPanel::isMarkerButtonClicked(void)
{
	return markerButtonClicked;
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  isMarkerAButtonClicked                                                 *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* void                                                                                   *
*                                                                                        *
* RETURNS: bool                                                                          *
*                                                                                        *
*****************************************************************************************/
bool ChannelControlPanel::isMarkerAButtonClicked(void)
{
	return markerAButtonClicked;
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  isMarkerBButtonClicked                                                 *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* void                                                                                   *
*                                                                                        *
* RETURNS: bool                                                                          *
*                                                                                        *
*****************************************************************************************/
bool ChannelControlPanel::isMarkerBButtonClicked(void)
{
	return markerBButtonClicked;
}

int ChannelControlPanel::GetTimeBaseIndex(void)
{
	return selectedHorizontalTextIndex;
}

int ChannelControlPanel::GetVoltBaseIndex(void)
{
	return selectedVerticalTextIndex;
}


/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  SetUpButtonImage                                                       *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* up_on     uint16_t           I   Used to pass the Image ID to internal Image ID        *
* down_on   uint16_t           I   Used to pass the Image ID to internal Image ID        *       
* left_on   uint16_t           I   Used to pass the Image ID to internal Image ID        *       
* right_on  uint16_t           I   Used to pass the Image ID to internal Image ID        *   
* up_off    uint16_t           I   Used to pass the Image ID to internal Image ID        *
* down_off  uint16_t           I   Used to pass the Image ID to internal Image ID        *
* left_off  uint16_t           I   Used to pass the Image ID to internal Image ID        *
* right_off uint16_t           I   Used to pass the Image ID to internal Image ID        *
*                                                                                        *
* RETURNS: void                                                                          *
*                                                                                        *
*****************************************************************************************/
void ChannelControlPanel::SetUpButtonImage(uint16_t up_on,   uint16_t up_off,   uint16_t down_on,  uint16_t down_off,
							    		   uint16_t left_on, uint16_t left_off, uint16_t right_on, uint16_t right_off)
{
	up_press = up_on;
	down_press = down_on;
	left_press = left_on;
	right_press = right_on;
	up_unpress = up_off;
	down_unpress = down_off;
	left_unpress = left_off;
	right_unpress = right_off;
}

int ChannelControlPanel::GetYOffset(void)
{
	return y_offset;
}

int ChannelControlPanel::GetXOffset(void)
{
	return x_offset;
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME:  SetChannelPanelCallback                                                *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT  TYPE             I/O   DESCRIPTION                                           *
* --------  ----             ---   -----------                                           *
* callback  GenericCallback    I   Callback event from exernal                                                                                    *
*                                                                                        *
* RETURNS: bool                                                                          *
*                                                                                        *
*****************************************************************************************/
void ChannelControlPanel::SetChannelPanelCallback(GenericCallback <const uint8_t > &callback)
{
	this->ChannelPanelSelectedCallBack = &callback;
}