/******************************************************************************
* Copyright (c) 2018(-2023) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.22.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

/**
 * @file touchgfx/widgets/TextureMapper.hpp
 *
 * Declares the touchgfx::TextureMapper class.
 */
#ifndef TOUCHGFX_TEXTUREMAPPER_HPP
#define TOUCHGFX_TEXTUREMAPPER_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * The TextureMapper widget displays a transformed image. It can be used to generate effects
 * where an image should be rotated in two or three dimensions.
 *
 * The image can be freely scaled and rotated in three dimensions. The scaling and
 * rotation is done around the adjustable origin. A virtual camera is applied to the
 * rendered image yielding a perspective impression. The amount of perspective
 * impression can be adjusted. The transformed image is clipped according to the
 * dimensions of the TextureMapper widget. In order to make the image fully visible the
 * TextureMapper should be large enough to accommodate the transformed image, which may
 * be larger than the raw image.
 *
 * @see Widget
 *
 * @note The drawing of this widget is not trivial and typically has a significant performance
 *       penalty. The number of pixels drawn, the presence of global alpha or per pixel alpha
 *       inflicts the computation and should be considered.
 * @note This widget does not support 1 bit per pixel color depth.
 */
class TextureMapper : public Image
{
public:
    /**
     * Rendering algorithm to use when scaling the bitmap. Nearest neighbor simply finds the
     * closest pixel in the source bitmap. Bilinear interpolation averages 4 pixels to find
     * a much better pixel representation, and is therefore slower than the Nearest neighbor
     * algorithm.
     */
    enum RenderingAlgorithm
    {
        NEAREST_NEIGHBOR,      ///< Fast but not a very good image quality. Good for fast animations.
        BILINEAR_INTERPOLATION ///< Slower but better image quality. Good for static representation of a scaled image.
    };

    /**
     * Constructs a new TextureMapper with a default alpha value of 255 (solid) and a default Bitmap
     * (undefined) if none is specified. If a Bitmap is passed to the constructor, the width and
     * height of this widget is set to those of the bitmap.
     *
     * @param   bmp (Optional) The bitmap to display.
     *
     * @see setBitmap
     */
    TextureMapper(const Bitmap& bmp = Bitmap());

    /**
     * Sets the bitmap for this TextureMapper and updates the width and height of this widget to
     * match those of the Bitmap.
     *
     * @param   bmp The bitmap instance.
     *
     * @note    The user code must call invalidate() in order to update the image on the display.
     */
    virtual void setBitmap(const Bitmap& bmp);

    virtual void draw(const Rect& invalidatedArea) const;

    virtual Rect getSolidRect() const;

    /**
     * Sets the render algorithm to be used. Default setting is NEAREST_NEIGHBOR.
     *
     * @param  algorithm The algorithm to use when rendering.
     */
    virtual void setRenderingAlgorithm(RenderingAlgorithm algorithm)
    {
        currentRenderingAlgorithm = algorithm;
    }

    /**
     * Gets the algorithm used when rendering.
     *
     * @return The algorithm used when rendering.
     */
    virtual RenderingAlgorithm getRenderingAlgorithm() const
    {
        return currentRenderingAlgorithm;
    }

    /**
     * Sets the angles in radians of the image.
     *
     * @param   newXAngle   The new x Angle.
     * @param   newYAngle   The new y Angle.
     * @param   newZAngle   The new x Angle.
     *
     * @see updateAngles, getXAngle, getYAngle, getZAngle
     *
     * @note    The area covered by the image before/after changing the angles is NOT invalidated.
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void setAngles(float newXAngle, float newYAngle, float newZAngle);

    /**
     * Sets the x angle in radians.
     *
     * @param   newXAngle   The new x angle.
     *
     * @see setAngles, updateXAngle, getXAngle
     *
     * @note    The area covered by the image before/after changing the angle is NOT invalidated.
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void setXAngle(float newXAngle)
    {
        setAngles(newXAngle, yAngle, zAngle);
    }

    /**
     * Sets the y angle in radians.
     *
     * @param   newYAngle   The new y angle.
     *
     * @see setAngles, updateYAngle, getYAngle
     *
     * @note    The area covered by the image before/after changing the angle is NOT invalidated.
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void setYAngle(float newYAngle)
    {
        setAngles(xAngle, newYAngle, zAngle);
    }

    /**
     * Sets the z angle in radians.
     *
     * @param   newZAngle   The new z angle.
     *
     * @see setAngles, updateZAngle, getZAngle
     *
     * @note    The area covered by the image before/after changing the angle is NOT invalidated.
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void setZAngle(float newZAngle)
    {
        setAngles(xAngle, yAngle, newZAngle);
    }

    /**
     * Updates the angles in radians of the image. The area covered by the image before and after
     * changing the angles is invalidated, which is the smallest required rectangle.
     *
     * @param   newXAngle   The new x Angle.
     * @param   newYAngle   The new y Angle.
     * @param   newZAngle   The new x Angle.
     *
     * @see setAngles, updateXAngle, updateYAngle, updateZAngle, getXAngle, getYAngle, getZAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void updateAngles(float newXAngle, float newYAngle, float newZAngle);

    /**
     * Updates the x angle in radians.
     *
     * @param   newXAngle   The new x angle.
     *
     * @see updateAngles, getXAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void updateXAngle(float newXAngle)
    {
        updateAngles(newXAngle, yAngle, zAngle);
    }

    /**
     * Updates the y angle in radians.
     *
     * @param   newYAngle   The new y angle.
     *
     * @see updateAngles, getYAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void updateYAngle(float newYAngle)
    {
        updateAngles(xAngle, newYAngle, zAngle);
    }

    /**
     * Updates the z angle in radians.
     *
     * @param   newZAngle   The new z angle.
     *
     * @see updateAngles, getZAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual void updateZAngle(float newZAngle)
    {
        updateAngles(xAngle, yAngle, newZAngle);
    }

    /**
     * Get the x angle in radians.
     *
     * @return  The x angle.
     *
     * @see updateXAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual float getXAngle() const
    {
        return xAngle;
    }

    /**
     * Get the y angle in radians.
     *
     * @return  The y angle.
     *
     * @see updateYAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual float getYAngle() const
    {
        return yAngle;
    }

    /**
     * Get the z angle in radians.
     *
     * @return  The z angle.
     *
     * @see updateZAngle
     *
     * @note    Angles are given in radians, so a full circle is 2*PI.
     */
    virtual float getZAngle() const
    {
        return zAngle;
    }

    /**
     * Sets the scale of the image.
     *
     * @param  newScale The new scale value.
     *
     * @see updateScale, getScale
     */
    virtual void setScale(float newScale);

    /**
     * Updates the scale of the image. This implies invalidating the area covered by the texture
     * mapper.
     *
     * @param  newScale The new scale value.
     *
     * @see setScale, getScale
     */
    virtual void updateScale(float newScale);

    /**
     * Gets the scale of the image.
     *
     * @return The scale.
     *
     * @see setScale
     */
    virtual float getScale() const
    {
        return scale;
    }

    /**
     * Sets the transformation origo (center).
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     * @param  z The z coordinate.
     *
     * @see getOrigoX, getOrigoY, getOrigoZ
     */
    virtual void setOrigo(float x, float y, float z)
    {
        xOrigo = x;
        yOrigo = y;
        zOrigo = z;
        applyTransformation();
    }

    /**
     * Sets the transformation origo (center) in two dimensions. Leaves the z coordinate
     * untouched.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     *
     * @see getOrigoX, getOrigoY
     */
    virtual void setOrigo(float x, float y)
    {
        xOrigo = x;
        yOrigo = y;
        applyTransformation();
    }

    /**
     * Gets transformation origo x coordinate.
     *
     * @return The transformation origo x coordinate.
     *
     * @see setOrigo
     */
    virtual float getOrigoX() const
    {
        return xOrigo;
    }

    /**
     * Gets transformation origo y coordinate.
     *
     * @return The transformation origo y coordinate.
     *
     * @see setOrigo
     */
    virtual float getOrigoY() const
    {
        return yOrigo;
    }

    /**
     * Gets transformation origo z coordinate.
     *
     * @return The transformation origo z coordinate.
     *
     * @see setOrigo
     */
    virtual float getOrigoZ() const
    {
        return zOrigo;
    }

    /**
     * Sets the camera coordinate.
     *
     * @param  x The x coordinate for the camera.
     * @param  y The y coordinate for the camera.
     *
     * @see getCameraX, getCameraY
     */
    virtual void setCamera(float x, float y)
    {
        xCamera = x;
        yCamera = y;
        applyTransformation();
    }

    /**
     * Gets camera x coordinate.
     *
     * @return The camera x coordinate.
     *
     * @see setCamera
     */
    virtual float getCameraX() const
    {
        return xCamera;
    }

    /**
     * Gets camera y coordinate.
     *
     * @return The camera y coordinate.
     *
     * @see setCamera
     */
    virtual float getCameraY() const
    {
        return yCamera;
    }

    /**
     * Sets camera distance. If the given value is below
     * TextureMapper::MINIMAL_CAMERA_DISTANCE, it will be set to
     * TextureMapper::MINIMAL_CAMERA_DISTANCE.
     *
     * @param  d The new camera distance.
     *
     * @see getCameraDistance
     */
    virtual void setCameraDistance(float d)
    {
        cameraDistance = MAX(d, MINIMAL_CAMERA_DISTANCE);
        applyTransformation();
    }

    /**
     * Gets camera distance.
     *
     * @return The camera distance.
     *
     * @see setCameraDistance
     */
    virtual float getCameraDistance() const
    {
        return cameraDistance;
    }

    /**
     * Sets the position of the bitmap within the TextureMapper. The bitmap is clipped with
     * respect to the dimensions of the TextureMapper widget.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     *
     * @see getBitmapPositionX, getBitmapPositionY
     */
    virtual void setBitmapPosition(float x, float y)
    {
        xBitmapPosition = x;
        yBitmapPosition = y;
        applyTransformation();
    }

    /**
     * Sets the position of the bitmap within the TextureMapper. The bitmap is clipped with
     * respect to the dimensions of the TextureMapper widget.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     *
     * @see getBitmapPositionX, getBitmapPositionY
     */
    virtual void setBitmapPosition(int x, int y)
    {
        setBitmapPosition((float)x, (float)y);
    }

    /**
     * Gets bitmap position x coordinate.
     *
     * @return The bitmap position x coordinate.
     *
     * @see setBitmapPosition
     */
    virtual float getBitmapPositionX() const
    {
        return xBitmapPosition;
    }

    /**
     * Gets bitmap position y coordinate.
     *
     * @return The bitmap position y coordinate.
     *
     * @see setBitmapPosition
     */
    virtual float getBitmapPositionY() const
    {
        return yBitmapPosition;
    }

    /**
     * Get the x coordinate of the top left corner of the transformed bitmap.
     *
     * @return The X0 coordinate.
     */
    virtual float getX0() const
    {
        return imageX0;
    }

    /**
     * Get the x coordinate of the top right corner of the transformed bitmap.
     *
     * @return The X1 coordinate.
     */
    virtual float getX1() const
    {
        return imageX1;
    }

    /**
     * Get the x coordinate of the bottom right of the transformed bitmap.
     *
     * @return The X2 coordinate.
     */
    virtual float getX2() const
    {
        return imageX2;
    }

    /**
     * Get the x coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The X3 coordinate.
     */
    virtual float getX3() const
    {
        return imageX3;
    }

    /**
     * Get the y coordinate of the top left corner of the transformed bitmap.
     *
     * @return The Y0 coordinate.
     */
    virtual float getY0() const
    {
        return imageY0;
    }

    /**
     * Get the y coordinate of the top right corner of the transformed bitmap.
     *
     * @return The Y1 coordinate.
     */
    virtual float getY1() const
    {
        return imageY1;
    }

    /**
     * Get the y coordinate of the bottom right corner of the transformed bitmap.
     *
     * @return The Y2 coordinate.
     */
    virtual float getY2() const
    {
        return imageY2;
    }

    /**
     * Get the y coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The Y3 coordinate.
     */
    virtual float getY3() const
    {
        return imageY3;
    }

    /**
     * Get the z coordinate of the top left corner of the transformed bitmap.
     *
     * @return The Z0 coordinate.
     */
    virtual float getZ0() const
    {
        return imageZ0;
    }

    /**
     * Get the z coordinate of the top right corner of the transformed bitmap.
     *
     * @return The Z1 coordinate.
     */
    virtual float getZ1() const
    {
        return imageZ1;
    }

    /**
     * Get the z coordinate of the bottom right corner of the transformed bitmap.
     *
     * @return The Z2 coordinate.
     */
    virtual float getZ2() const
    {
        return imageZ2;
    }

    /**
     * Get the z coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The Z3 coordinate.
     */
    virtual float getZ3() const
    {
        return imageZ3;
    }

    /**
     * Invalidate the bounding rectangle of the transformed bitmap.
     *
     * @see getBoundingRect
     *
     * @deprecated Please use invalidateContent() instead.
     */
    TOUCHGFX_DEPRECATED("Please use invalidateContent() instead.", void invalidateBoundingRect() const);

    virtual void invalidateContent() const
    {
        if (alpha > 0)
        {
            Rect r = getBoundingRect();
            invalidateRect(r);
        }
    }

protected:
    /**
     * Transform the bitmap using the supplied origo, scale, rotation and camera. This
     * method is called by all the methods that manipulate origo, scale, rotation and camera.
     */
    void applyTransformation();

    /**
     * Gets bounding rectangle of the transformed bitmap. This is the smallest possible
     * rectangle which covers the image of the bitmap after applying scale and rotation.
     *
     * @return The bounding rectangle.
     */
    Rect getBoundingRect() const;

    /**
     * The TextureMapper will draw the transformed bitmap by drawing one transformed quad.
     * The quad is drawn from the points 0,1,2,3 using the x,y,z values from each point along
     * with the u,v coordinates in the bitmap associated with each point.
     *
     * @param      invalidatedArea The invalidated area.
     * @param [in] fb              The framebuffer.
     * @param      triangleXs      The triangle xs.
     * @param      triangleYs      The triangle ys.
     * @param      triangleZs      The triangle zs.
     * @param      triangleUs      The triangle us.
     * @param      triangleVs      The triangle vs.
     */
    void drawQuad(const Rect& invalidatedArea, uint16_t* fb, const float* triangleXs, const float* triangleYs, const float* triangleZs, const float* triangleUs, const float* triangleVs) const;

    /**
     * Returns the rendering variant based on the bitmap format, alpha value and rendering
     * algorithm.
     *
     * @return The RenderingVariant.
     */
    RenderingVariant lookupRenderVariant() const;

    RenderingAlgorithm currentRenderingAlgorithm; ///< The current rendering algorithm.

    static const int MINIMAL_CAMERA_DISTANCE = 1; ///< The minimal camera distance

    float xBitmapPosition; ///< The bitmap position x
    float yBitmapPosition; ///< The bitmap position y

    float xAngle; ///< The angle x in radians
    float yAngle; ///< The angle y in radians
    float zAngle; ///< The angle z in radians
    float scale;  ///< The scale

    float xOrigo; ///< The origo x coordinate
    float yOrigo; ///< The origo y coordinate
    float zOrigo; ///< The origo z coordinate

    float xCamera;        ///< The camera x coordinate
    float yCamera;        ///< The camera y coordinate
    float cameraDistance; ///< The camera distance

    float imageX0; ///< The coordinate for the image points
    float imageY0; ///< The coordinate for the image points
    float imageZ0; ///< The coordinate for the image points
    float imageX1; ///< The coordinate for the image points
    float imageY1; ///< The coordinate for the image points
    float imageZ1; ///< The coordinate for the image points
    float imageX2; ///< The coordinate for the image points
    float imageY2; ///< The coordinate for the image points
    float imageZ2; ///< The coordinate for the image points
    float imageX3; ///< The coordinate for the image points
    float imageY3; ///< The coordinate for the image points
    float imageZ3; ///< The coordinate for the image points

    uint16_t subDivisionSize; ///< The size of the affine sub divisions
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTUREMAPPER_HPP
