/*******************************************************************************************
* FILE NAME: TriggerLine.cpp                                                               *
* PURPOSE:   Create Trigger Line Widget to indicate the level of the trigger               *
*            This widget is touchable                                                      *
*                                                                                          *
* FILE REFERENCES:                                                                         *
*                                                                                          *
* Name I/O Description                                                                     *
* ---- --- -----------                                                                     *
*                                                                                          *
* EXTERNAL VARIABLES:                                                                      *
* Source: < >                                                                              *
*                                                                                          *
* Name Type I/O Description                                                                *
* ---- ---- --- -----------                                                                *
*                                                                                          *
* EXTERNAL REFERENCES:                                                                     *
*                                                                                          *
* Name Description                                                                         *
* ---- -----------                                                                         *
*                                                                                          *
* ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES:                             *
*                                                                                          *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                                                  *
*                                                                                          *
* NOTES:                                                                                   *
*                                                                                          *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:                                       *
*                                                                                          *
* DEVELOPMENT HISTORY:                                                                     *
*                                                                                          *
* Date        Author      Change Id  Release  Description Of Change                        *
* ----        ------      ---------  -------  ---------------------                        *
* 28.06.2016  Hai Nguyen         01     1.0   Original File, adding File Prolog and comment*
* 29.06.2016  Hai Nguyen         02     2.0   Porting to touchgfx 4.6.0                    *
*                                                                                          *
*******************************************************************************************/

#include <gui/common/TriggerLine.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>


/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: TriggerLine - constructor                                               *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *  
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/

TriggerLine::TriggerLine():markerDraggedCallback(this, &TriggerLine::handleMarkerDragEvent)
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
* x_position      int       I   Position of the marker on x Coordinate                   *
* marker_length   int       I   The length of the marker                                 *
* graph_height    int       I   The limit of the y Coordinate, to keep the graph in the  *
*                                  screen                                                *
* marker_color    uint16_t  I   Define the color of the line                             *
*                                                                                        *
* RETURNS:  void                                                                         *
*                                                                                        *
*****************************************************************************************/

void TriggerLine::setup(int x_position, int marker_length, int graph_height, uint16_t marker_color)
{
	/* LOCAL VARIABLES:
	*
	* Variable      Type      Description
	* ------------- -------   -------------------------------------
	* height_limit  int       Copy the value of graph_height
	* y_marker      int       Copy the value of y_position
	*/
	height_limit = graph_height;
	y_marker = x_position;
	length = marker_length;
	trigger_position = y_marker;
	/*
	* Configure the width, height, color and position of the trigger line
	* Enable touchable
	* Add to the screen
	*/
	marker_painter.setColor(marker_color);
	marker.setLineWidth(2);
	marker.setPosition(-300, y_marker, marker_length + 500, 50 );
	marker.setStart(0, 5);
	marker.setEnd(marker_length + 500, 5);
	marker.setPainter(marker_painter);
	marker.setDragAction(markerDraggedCallback);
	marker.setTouchable(true);
	add(marker);
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: ~TriggerLine - deconstructor                                            *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/
TriggerLine::~TriggerLine()
{
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: handleMarkerDragEvent                                                   *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE       I/O DESCRIPTION                                                    *
* -------- ----       --- -----------                                                    *
* evt      DragEvent    I Pointer to the DragEvent class                                 *
*                                                                                        *
* RETURNS:  void                                                                         *
*                                                                                        *
*****************************************************************************************/
void TriggerLine::handleMarkerDragEvent(const DragEvent& evt)
{
	/*
	 *  Check if the Displacement of DragEvent is exceed the boundary or not
	 *  If yes, keep it at the boundary
	 *  If no, move to the new position
	 */

	if (evt.getDeltaY() > (height_limit - 10))
		y_marker = height_limit - 10;
	else if (y_marker + evt.getDeltaY() < 0)
		y_marker = 0;
	else if (y_marker + evt.getDeltaY() > (volt_offset ))
		y_marker = (volt_offset);
	else
	{
		y_marker = y_marker + evt.getDeltaY();

	}
	marker.setSnapPosition(-300, y_marker);
	trigger_position = y_marker;
	marker.invalidate();
}

/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: EnableLine                                                              *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE       I/O DESCRIPTION                                                    *
* -------- ----       --- -----------                                                    *
* enable   bool         I Flag to check if we want to enable the trigger line or not     *
*                                                                                        *
* RETURNS:  void                                                                         *
*                                                                                        *
*****************************************************************************************/
void TriggerLine::EnableLine(bool enable)
{
	marker.setVisible(enable);
	marker.invalidate();
}

int TriggerLine::TriggerPosition(void)
{
	return trigger_position;
}

void TriggerLine::SetYOffset(int y)
{
}

void TriggerLine::SetVoltOffset(int value)
{
	volt_offset = value;
}
