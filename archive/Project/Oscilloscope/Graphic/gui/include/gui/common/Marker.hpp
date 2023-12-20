/***************************************************************************
* NAME:     Marker.hpp                                                     *
*                                                                          *
* PURPOSE:  Header of Marker.cpp                                           *
*                                                                          *
* PULIC VARIABLES:                                                         *
*                                                                          *
* Variable        Type            Description                              *
* --------        ----            -----------                              *
*                                                                          *
* PULIC FUNCTIONS:                                                         *
*                                                                          *
* Name                  Return      Description                            *
* ----                  ------      -----------                            *
* Maker                 N/A         Constructor for Marker object          *
* ~Marker               virtual     Deconstructor of Marker object         *
* setup                 void        Setup the properties of Marker         *
*                                     object                               *
* handleMarkerDragEvent void        Event Handle for Drag event of the     *
*                                     object                               *
* EnableLine            void        Enable the object on the screen        *
*                                                                          *
* PRIVATE VARIABLES:                                                       *
*                                                                          *
* Variable        Type            Description                              *
* --------        ----            -----------                              *
* marker          Snapper<Line>   Instance of a Line object                *
*                                  with snapper properties                 *
* marker_painter  PainterRGB565   Instance of Painter object with          *
*                                  color format RGB565                     *
* x_marker        int             X position of the trigger line           *
* width_limit     int             Maximum width that the trigger line      *
*                                   can be moved                           *
*                                                                          *
* PRIVATE FUNCTIONS:                                                       *
*                                                                          *
* Name                    Return               Description                 *
* ----                    ------               -----------                 *
* markerDraggedCallback   Callback function    Callback of Drag event      *
*                                                                          *
* DEVELOPMENT HISTORY:                                                     *
*                                                                          *
* Date        Author     Change Id Release  Description Of Change          *
* ----        ------     --------- -------  ---------------------          *
* 28.06.2016  Hai Nguyen        01     1.0  Original File, Adding prolog   *
* 29.06.2016  Hai Nguyen		02	   2.0  Porting to touchgfx 4.6.0      *
*                                                                          *
***************************************************************************/

#ifndef MOVABLELINE_HPP
#define MOVABLELINE_HPP

#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/mixins/Snapper.hpp>

using namespace touchgfx;

class  Marker : public Container
{
public:
	Marker();

	virtual ~Marker();

	void setup(int x_position, int marker_height, int graph_width, uint16_t marker_color);
	void handleMarkerDragEvent(const DragEvent&);
	void EnableLine(bool enable);
	int MarkerPosition(void);
private:

	Snapper<Line> marker;
	PainterRGB565 marker_painter;
	Callback< Marker, const DragEvent& > markerDraggedCallback;
	int x_marker;
	int width_limit;
	int marker_position;
};
#endif