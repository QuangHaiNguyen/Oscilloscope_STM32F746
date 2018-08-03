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
#include <gui/common/Graph.hpp>
#include <math.h>
#include <stdio.h>

//extern int  adc_chn1_buffer_trigg[100];

int count;

Graph::Graph()
	: counter(0)
{
	count = 0;
	graphColor = Color::getColorFrom24BitRGB(0x07, 0x89, 0xBA);
	graphBlendColor = Color::getColorFrom24BitRGB(71, 71, 71);

	rightCurveSegment = 0;

	setWidth(graphWidth);
	setHeight(graphHeight);

	for (int i = 0; i < graphWidth; ++i)
	{
		curveSegments[i].y = 0;
		curveSegments[i].alphas[0] = 0;
	}
}

void Graph::draw(const Rect& invalidatedArea) const
{
	uint16_t* frameBuffer = (uint16_t*)HAL::getInstance()->lockFrameBuffer();

	drawCurve(frameBuffer, invalidatedArea);

	HAL::getInstance()->unlockFrameBuffer();
}

void Graph::calculate()
{
	if (rightCurveSegment < graphWidth - 1)
	{
		rightCurveSegment++;
	}
	else
	{
		counter++;
	}

	//generate new value each frame in the unused spot
	uint16_t widthBetweenPoints = getWidth() / graphPointsInsideGraphWindow;
	uint8_t unusedIndex = (counter / widthBetweenPoints) % graphPointsInGraph;
	values[unusedIndex] = (int)*(p_gragh_data + count);

	calculateRightCurveSegment();
}

static int maxAlphas = 0;
void Graph::calculateRightCurveSegment()
{
	// Make initial curve segment drawing smoother
	if (2 == rightCurveSegment)
	{
		curveSegments[0] = curveSegments[1];
	}

	uint8_t alphasAbove[graphAlphasPerColumn];
	uint8_t numberOfAlphasAbove = 0;
	uint8_t alphasBelow[graphAlphasPerColumn];
	uint8_t numberOfAlphasBelow = 0;

	uint16_t x = rightCurveSegment;

	uint16_t circularX = (x + counter) % graphWidth;
	CurveSegment& curveSegment = curveSegments[circularX];

	float valuePrev = valueAt(x - 1);
	float valueCur = valueAt(x);
	float valueNext = valueAt(x + 1);

	float slope = (valueNext - valuePrev) / 2.0f;
	float constant = valueCur - (slope * x);

	float a = -slope;
	float b = 1.0;
	float c = -constant;

	int16_t yValue = (int16_t)floor(valueCur);

	//above
	int16_t y = yValue;
	while (true)
	{
		if (numberOfAlphasAbove >= graphAlphasPerColumn)
		{
			break;
		}
		else
		{
			float distance = abs(a * x + b * y + c) / sqrtf(a * a + b * b);
			if (distance < graphLineWidth)
			{
				alphasAbove[numberOfAlphasAbove] = 255;
				numberOfAlphasAbove++;
			}
			else if (distance < graphLineWidth + graphLineBlur)
			{
				uint8_t alpha = (uint8_t)(255 - (255 / graphLineBlur * (distance - graphLineWidth)));
				if (alpha > 0)
				{
					alphasAbove[numberOfAlphasAbove] = alpha;
					numberOfAlphasAbove++;
				}
			}
			else
			{
				break;
			}
		}
		y--;
	}

	// below
	y = yValue + 1;
	while (true)
	{
		if (numberOfAlphasBelow >= graphAlphasPerColumn)
		{
			break;
		}
		else
		{
			float distance = abs(a * x + b * y + c) / sqrtf(a * a + b * b);
			if (distance < graphLineWidth)
			{
				alphasBelow[numberOfAlphasBelow] = 255;
				numberOfAlphasBelow++;
			}
			else if (distance < graphLineWidth + graphLineBlur)
			{
				uint8_t alpha = (uint8_t)(255 - (255 / graphLineBlur * (distance - graphLineWidth)));
				if (alpha > 0)
				{
					alphasBelow[numberOfAlphasBelow] = alpha;
					numberOfAlphasBelow++;
				}
			}
			else
			{
				break;
			}
		}
		y++;
	}

	for (int i = 0; i < numberOfAlphasAbove; i++)
	{
		curveSegment.alphas[i] = alphasAbove[numberOfAlphasAbove - i - 1];
	}

	for (int i = 0; i < numberOfAlphasBelow && numberOfAlphasAbove + i < graphAlphasPerColumn; i++)
	{
		curveSegment.alphas[numberOfAlphasAbove + i] = alphasBelow[i];
	}

	curveSegment.alphas[MIN(numberOfAlphasAbove + numberOfAlphasBelow, graphAlphasPerColumn - 1)] = 0;
	curveSegment.y = yValue - numberOfAlphasAbove + 1;

	if (maxAlphas < MIN(numberOfAlphasAbove + numberOfAlphasBelow, graphAlphasPerColumn - 1))
	{
		maxAlphas = MIN(numberOfAlphasAbove + numberOfAlphasBelow, graphAlphasPerColumn - 1);
	}
}

void Graph::drawCurve(uint16_t* frameBuffer, const Rect& invalidatedArea) const
{
	// invalidatedArea is relative to enclosing container
	Rect absoluteRect = getRect();
	translateRectToAbsolute(absoluteRect);

	for (int x = (invalidatedArea.x);
		(x < invalidatedArea.right()) && (x < rightCurveSegment);
		++x)
	{
		CurveSegment curveSegment = curveSegments[(x + counter) % graphWidth];

		int y = curveSegment.y;
		uint8_t* alphas = &curveSegment.alphas[0];

		// Graph is drawn with absolute coordinates
		uint32_t offset = (absoluteRect.x + x) + HAL::DISPLAY_WIDTH * (absoluteRect.y + y);

		while (*alphas)
		{
			if (y >= invalidatedArea.y && y < invalidatedArea.bottom())
			{
				if (*alphas == 255)
				{
					frameBuffer[offset] = graphColor;
				}
				else
				{
					frameBuffer[offset] = blend(graphColor, graphBlendColor, *alphas);
				}
			}
			alphas++;
			y++;
			offset += HAL::DISPLAY_WIDTH;
		}
	}
}

bool Graph::withinCurrentCurve(const int16_t xCoordinate) const
{
	return (xCoordinate >= 0) && (xCoordinate < rightCurveSegment);
}

int16_t Graph::getYCoordinate(const int16_t xCoordinate) const
{
	if (!withinCurrentCurve(xCoordinate))
	{
		return 0;
	}
	else
	{
		// Respect circular buffer by using counter and modulo graphWidth
		return curveSegments[(counter + xCoordinate) % graphWidth].y;
	}
}

int16_t Graph::getYValue(const int16_t xCoordinate) const
{
	if (!withinCurrentCurve(xCoordinate))
	{
		return 0;
	}
	else
	{
		return (graphHeight - getYCoordinate(xCoordinate)) + static_cast<int16_t>(graphTopMargin);
	}
}

bool Graph::isScrolling() const
{
	return rightCurveSegment >= (graphWidth - 1);
}

void Graph::SetGraphColor(colortype color)
{
	graphColor = color;
	
}

void Graph::SetGraphData(int *data)
{
	p_gragh_data = data;
}

Rect Graph::getSolidRect() const
{
	return Rect(0, 0, 0, 0);
}

void Graph::handleTickEvent()
{
   count = 0;
	// Draw two graph points per tick
	for (int i = 0; i < NUMBER_OF_POINT; i++)
	{
		calculate();
		count++;
	}
	invalidate();
}

float Graph::valueAt(uint16_t x) const
{
	uint16_t widthBetweenPoints = getWidth() / graphPointsInsideGraphWindow;
	uint8_t index0 = ((x + counter) / widthBetweenPoints + 1) % graphPointsInGraph;
	uint8_t index1 = (index0 + 1) % graphPointsInGraph;
	uint8_t index2 = (index0 + 2) % graphPointsInGraph;
	uint8_t index3 = (index0 + 3) % graphPointsInGraph;
	float xNormalized = ((x + counter) % widthBetweenPoints + 0.0f) / widthBetweenPoints;

	return cubicInterpolate(values[index0], values[index1], values[index2], values[index3], xNormalized);
}

void Graph::setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
	Widget::setPosition(x, y, width, height);
	for (int i = 0; i < graphPointsInGraph; i++)
	{
		values[i] = 0;
	}
}

