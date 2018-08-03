/*******************************************************************************************
* FILE NAME: Marker.cpp                                                                    *
* PURPOSE:   Create Marker Widget to indicate the time difference, i.e. to measure time    *
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

#include <gui/common/Marker.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>


/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: Marker- constructor                                                     *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/
Marker::Marker():markerDraggedCallback(this, &Marker::handleMarkerDragEvent)
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
* marker_height   int       I   The height of the marker                                 *
* graph_width     int       I   The limit of the x Coordinate, to keep the graph inside  *
*                                  the screen                                            *
* marker_color    uint16_t  I   Define the color of the marker                           *
*                                                                                        *
* RETURNS:  void                                                                         *
*                                                                                        *
*****************************************************************************************/
void Marker::setup(int x_position, int marker_height, int graph_width, uint16_t marker_color)
{
	/* LOCAL VARIABLES:
	*
	* Variable      Type      Description
	* ------------- -------   -------------------------------------
	* width_limit   int       Copy the value of graph_width
	* x_marker      int       Copy the value of x_position
	*/

	width_limit = graph_width;
	x_marker = x_position;

	/*
	* Configure the width, height, color and position of the trigger line
	* Enable touchable
	* Add to the screen
	*/
	marker_painter.setColor(marker_color);
	marker.setLineWidth(2);
	marker.setPosition(x_marker, -300, 50, marker_height + 500);
	marker.setStart(5, 0);
	marker.setEnd(5, marker_height + 500);
	marker.setPainter(marker_painter);
	marker.setDragAction(markerDraggedCallback);
	marker.setTouchable(true);
	add(marker);
}


/*****************************************************************************************
*                                                                                        *
* FUNCTION NAME: ~Marker - deconstructor                                                 *
*                                                                                        *
* ARGUMENTS:                                                                             *
*                                                                                        *
* ARGUMENT TYPE I/O DESCRIPTION                                                          *
* -------- ---- --- -----------                                                          *
*                                                                                        *
* RETURNS:                                                                               *
*                                                                                        *
*****************************************************************************************/
Marker::~Marker()
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

void Marker::handleMarkerDragEvent(const DragEvent& evt)
{
	/*
	*  Check if the Displacement of DragEvent is exceed the boundary or not
	*  If yes, keep it at the boundary
	*  If no, move to the new position
	*/

	if (evt.getDeltaX() > (width_limit - 10))
		x_marker = width_limit - 10;
	else if ((x_marker + evt.getDeltaX()) < 10)
		x_marker = 10;
	else if ((x_marker + evt.getDeltaX()) > 290)
		x_marker = 290;
	else
	{
		x_marker = x_marker + evt.getDeltaX();
	}
	marker.setSnapPosition(x_marker, -300);
	marker_position = x_marker;
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
void Marker::EnableLine(bool enable)
{
	marker.setVisible(enable);
	marker.invalidate();
}

int Marker::MarkerPosition(void)
{
	return marker_position;
}
