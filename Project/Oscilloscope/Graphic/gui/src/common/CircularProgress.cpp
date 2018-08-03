
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

CircularProgress::CircularProgress()
{
    setTouchable(true);

    setWidth(100);
    setHeight(100);

	knobMapper.setBitmap(Bitmap(BITMAP_KNOBBUTTON_ID));

	int knobWidth = knobMapper.getBitmap().getWidth();
	int knobHeight = knobMapper.getBitmap().getHeight();

	knobMapper.setXY(0,0);
	knobMapper.setWidth(knobWidth + 80);
	knobMapper.setHeight(knobHeight + 80);
	knobMapper.setBitmapPosition(40, 40);
	knobMapper.setCameraDistance(300.0f);
	knobMapper.setOrigo(knobMapper.getBitmapPositionX() + (knobWidth / 2), knobMapper.getBitmapPositionY() + (knobHeight / 2), knobMapper.getCameraDistance());
	knobMapper.setCamera(knobMapper.getBitmapPositionX() + (knobWidth / 2), knobMapper.getBitmapPositionY() + (knobHeight / 2));
	knobMapper.setRenderingAlgorithm(TextureMapper::NEAREST_NEIGHBOR);

	add(knobMapper);
}

CircularProgress::~CircularProgress()
{
}

void CircularProgress::setProgress(int percentage)
{
    currentPercentage = percentage;
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;
    int angle = START_DEGREE + (END_DEGREE - START_DEGREE) * percentage / 100;
	knobMapper.updateAngles(0,0,(float)angle);
}	


int CircularProgress::getProgress()
{
    return currentPercentage;
}

void CircularProgress::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        updateBar(evt.getX(), evt.getY());
    }
}

void CircularProgress::handleDragEvent(const DragEvent& evt)
{
    updateBar(evt.getNewX(), evt.getNewY());
}

void CircularProgress::updateBar(int touchX, int touchY)
{
    int centerX = 0;
    int centerY = 0;

	centerX = 50;
	centerY = 50;

    int16_t vx = touchX - centerX;
    int16_t vy = touchY - centerY;

    int length;
    int angleInDegrees = CWRUtil::angle<int>(vx, vy, length);
    // Touch too close to center is ignored
    if (length < 50) return;

    // Convert high degress to negative degrees
    if (angleInDegrees > 180) angleInDegrees -= 360;
    // Allow touch within 30 degress of limits to easier touch end points
    if (angleInDegrees < START_DEGREE - 30) return;
    if (angleInDegrees > END_DEGREE + 30) return;
    // Touch out of range is set in range
    if (angleInDegrees < START_DEGREE) angleInDegrees = START_DEGREE;
    if (angleInDegrees > END_DEGREE) angleInDegrees = END_DEGREE;

    // Update bar
    setProgress((angleInDegrees - START_DEGREE) * 100 / (END_DEGREE - START_DEGREE));
}
