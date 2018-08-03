/******************************************************************************
*
* @brief     This file is part of the TouchGFX 4.6.0 evaluation distribution.
*
* @author    Draupner Graphics A/S <http://www.touchgfx.com>
*
******************************************************************************
*
* @section Copyright
*
* This file is free software and is provided for example purposes. You may
* use, copy, and modify within the terms and conditions of the license
* agreement.
*
* This is licensed software for evaluation use, any use must strictly comply
* with the evaluation license agreement provided with delivery of the
* TouchGFX software.
*
* The evaluation license agreement can be seen on www.touchgfx.com
*
* @section Disclaimer
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
* no obligation to support this software. Draupner Graphics A/S is providing
* the software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Draupner Graphics A/S can not be held liable for any consequential,
* incidental, or special damages, or any other relief, or for any claim by
* any third party, arising from your use of this software.
*
*****************************************************************************/
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Color.hpp>
#include <stdlib.h>
#include <gui\common\main_header.h>



using namespace touchgfx;

// GraphScreen constants
static const uint16_t graphWidth = 306;
static const uint16_t graphHeight = 230;
static const uint16_t graphPointsInsideGraphWindow = NUMBER_OF_POINT;
static const uint16_t graphAlphasPerColumn = 15;
static const uint16_t graphPointsInGraph = (2 + graphPointsInsideGraphWindow + 2 + 1);

static const float graphLineWidth = 1.0f;
static const float graphLineBlur = 0.2f;

static const float graphBottomMargin = 85.0f;
static const float graphTopMargin = 70.0f;

class Graph : public Widget
{
public:
	Graph();

	virtual void draw(const Rect& invalidatedArea) const;
	virtual void handleTickEvent();
	virtual void setPosition(int16_t x, int16_t y, int16_t width, int16_t height);

	virtual Rect getSolidRect() const;

	bool isScrolling() const;
	bool withinCurrentCurve(const int16_t xCoordinate) const;

	int16_t getYCoordinate(const int16_t xCoordinate) const;
	int16_t getYValue(const int16_t xCoordinate) const;
	
	void SetGraphColor(colortype color);
	void SetGraphData(int * data);

private:

	 colortype graphColor;

	 colortype graphBlendColor;

	void calculate();

	void calculateRightCurveSegment();

	void drawCurve(uint16_t* frameBuffer, const Rect& invalidatedArea) const;

	float valueAt(uint16_t x) const;

	float cubicInterpolate(float p0, float p1, float p2, float p3, float x) const
	{
		return p1 + 0.5f * x * (p2 - p0 + x * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3 + x * (3.0f * (p1 - p2) + p3 - p0)));
	}

	float randomNumberBetween(float lowest, float highest) const
	{
		return lowest + (highest - lowest) * (rand() / (float)RAND_MAX);
	}

	uint16_t blend(uint16_t colTo, uint16_t colFrom, uint8_t alpha) const
	{
		uint16_t RMASK = 0xF800;
		uint16_t GMASK = 0x07E0;
		uint16_t BMASK = 0x001F;

		uint8_t ialpha = 256 - alpha;
		uint16_t bufpix;
		uint16_t newpix;
		uint16_t res;

		

		bufpix = colFrom;
		newpix = colTo;
		res =
			(RMASK & (((newpix & RMASK) * alpha +
			(bufpix & RMASK) * ialpha) >> 8)) |
				(GMASK & (((newpix & GMASK) * alpha +
			(bufpix & GMASK) * ialpha) >> 8)) |
					(BMASK & (((newpix & BMASK) * alpha +
			(bufpix & BMASK) * ialpha) >> 8));
		return res;
	}

	// Representation of the curve
	// for each x value we remember the y and the alpha values to blend
	struct CurveSegment
	{
		int16_t y;
		uint8_t alphas[graphAlphasPerColumn];
	};
	CurveSegment curveSegments[graphWidth];

	uint16_t rightCurveSegment;
	uint16_t counter;

	int16_t values[graphPointsInGraph];

	int * p_gragh_data;
};

#endif
