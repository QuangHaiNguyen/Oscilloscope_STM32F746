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
 * @file touchgfx/hal/Types.hpp
 *
 * Declares the touchgfx::colortype, touchgfx::Rect, touchgfx::Vector, touchgfx::Point,
 * touchgfx::Pair classes as well as some less used classes and structs.
 */
#ifndef TOUCHGFX_TYPES_HPP
#define TOUCHGFX_TYPES_HPP

#include <assert.h>
#include <stdint.h>
#include <touchgfx/hal/Config.hpp>

/**
 * A macro that returns the smallest of two items.
 *
 * @param  a The first item.
 * @param  b The second item.
 */
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/**
 * A macro that returns the largest of two items.
 *
 * @param a The first item.
 * @param b The second item.
 */
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/**
 * A macro that rounds a number up to the next multiple. Works for negative numbers, too.
 *
 * @param num      The number to round up.
 * @param multiple The multiple.
 */
#ifndef ROUNDUP
#define ROUNDUP(num, multiple) ((multiple) == 0 ? (num) : ((num) + (abs(multiple) - ((num) % abs(multiple))) % abs(multiple)))
#endif

/**
 * A macro that rounds a number down to the next multiple. Works for negative numbers, too.
 *
 * @param num      The number to round down.
 * @param multiple The multiple.
 */
#ifndef ROUNDDOWN
#define ROUNDDOWN(num, multiple) (-(ROUNDUP(-(num), multiple)))
#endif

namespace touchgfx
{
static const float PI = 3.14159265358979323846f; ///< PI

/**
 * This type can contain a color value. Note that in order to maintain backwards
 * compatibility, casting this type to an integral value will yield a 16-bit value. To
 * extract a 24/32-bit color from this type, use the getColor32 function.
 */
struct colortype
{
    /** Default constructor. Creates a black (0) color. */
    colortype()
        : color(0)
    {
    }

    /**
     * Constructor which creates a colortype with the given color. Use
     * Color::getColorFromRGB() to create a color that will work on your selected LCD type.
     *
     * @param  col The color.
     *
     * @see Color::getColorFromRGB
     */
    colortype(uint32_t col)
        : color(col)
    {
    }

    /**
     * Gets color as a 32bit value suitable for passing to Color::getRed(),
     * Color::getGreen() and Color::getBlue() which will handle all bitdeptchs.
     *
     * @return The color 32.
     *
     * @see Color::getRed, Color::getGreen, Color::getBlue
     */
    FORCE_INLINE_FUNCTION uint32_t getColor32() const
    {
        return color;
    }

    /**
     * Cast that converts the given colortype to an uint32_t.
     *
     * @return The result of the operation.
     */
    operator uint32_t() const
    {
        return color;
    }

    uint32_t color; ///< The color
};

/** Class representing a Rectangle with a few convenient methods. */
class Rect
{
public:
    /** Default constructor. Resulting in an empty Rect with coordinates 0,0. */
    Rect()
        : x(0), y(0), width(0), height(0)
    {
    }

    /**
     * Initializes a new instance of the Rect class.
     *
     * @param  rectX      The x coordinate.
     * @param  rectY      The y coordinate.
     * @param  rectWidth  The width.
     * @param  rectHeight The height.
     */
    Rect(int16_t rectX, int16_t rectY, int16_t rectWidth, int16_t rectHeight)
        : x(rectX), y(rectY), width(rectWidth), height(rectHeight)
    {
    }

    int16_t x;      ///< The x coordinate
    int16_t y;      ///< The y coordinate
    int16_t width;  ///< The width
    int16_t height; ///< The height

    /**
     * Gets the x coordinate of the right edge of the Rect, i.e. the number
     * of the first column just to the right of the Rect.
     *
     * @return x coordinate of the right edge (calculated as "x + width").
     */
    FORCE_INLINE_FUNCTION int16_t right() const
    {
        return x + width;
    }

    /**
     * Gets the y coordinate of the bottom edge of the Rect, i.e. the number
     * of the first row just below the Rect.
     *
     * @return y coordinate of the bottom edge (calculated as "y + height").
     */
    FORCE_INLINE_FUNCTION int16_t bottom() const
    {
        return y + height;
    }

    /**
     * Determines whether specified point lies inside this rectangle.
     *
     * @param  otherX The x coordinate of the point.
     * @param  otherY The y coordinate of the point.
     *
     * @return true if point lies inside rectangle.
     */
    bool intersect(int16_t otherX, int16_t otherY) const
    {
        return otherX >= x && otherX < right() && otherY >= y && otherY < bottom();
    }

    /**
     * Determines whether specified rectangle intersects with this rectangle.
     *
     * @param  other The other rectangle.
     *
     * @return true if the two rectangles intersect.
     */
    bool intersect(const Rect& other) const
    {
        return !(x >= other.right() || right() <= other.x || y >= other.bottom() || bottom() <= other.y);
    }

    /**
     * Determines whether the specified rectangle is completely included in this rectangle.
     *
     * @param  other The other rectangle.
     *
     * @return true if the specified rectangle is completely included.
     */
    bool includes(const Rect& other) const
    {
        return other.isEmpty() || (other.x >= x && other.y >= y && other.right() <= right() && other.bottom() <= bottom());
    }

    /**
     * Gets a rectangle describing the intersecting area between this rectangle and the
     * supplied rectangle.
     *
     * @param  other The other rectangle.
     *
     * @return Intersecting rectangle or empty Rect in case of no intersection.
     */
    Rect operator&(const Rect& other) const
    {
        Rect r = *this;
        r &= other;
        return r;
    }

    /**
     * Assigns this Rect to the intersection of the current Rect and the assigned Rect. The
     * assignment will result in a empty Rect if they do not intersect.
     *
     * @param  other The rect to intersect with.
     */
    void operator&=(const Rect& other)
    {
        if (intersect(other))
        {
            int16_t newX = MAX(x, other.x);
            int16_t newY = MAX(y, other.y);

            width = MIN(right(), other.right()) - newX;
            height = MIN(bottom(), other.bottom()) - newY;
            x = newX;
            y = newY;
        }
        else
        {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
        }
    }

    /**
     * Increases the area covered by this rectangle to encompass the area covered by
     * supplied rectangle.
     *
     * @param  other The other rectangle.
     */
    void expandToFit(const Rect& other)
    {
        if (!other.isEmpty())
        {
            if (isEmpty())
            {
                x = other.x;
                y = other.y;
                width = other.width;
                height = other.height;
            }
            else
            {
                int16_t newX = MIN(x, other.x);
                int16_t newY = MIN(y, other.y);

                int16_t endPointX = MAX(right(), other.right());
                int16_t endPointY = MAX(bottom(), other.bottom());

                x = newX;
                y = newY;
                width = endPointX - newX;
                height = endPointY - newY;
            }
        }
    }

    /**
     * Restrict the area to not exceed the given max width and max height. As a result, width or
     * height can be negative if the rect is completely outside Rect(0, 0, max_width, max_height),
     * but this is nicely handled by the isEmpty() function.
     *
     * @param   max_width   The maximum width.
     * @param   max_height  The maximum height.
     *
     * @see intersect, isEmpty
     */
    void restrictTo(int16_t max_width, int16_t max_height)
    {
        // Limit area to the screen (0,0,HAL::WIDTH,HAL::HEIGT)
        if (x < 0)
        {
            width += x;
            x = 0; // Negative width is ok (isEmpty => true)
        }
        if (width > max_width - x) // right() > max_width
        {
            width = max_width - x;
        }
        if (y < 0)
        {
            height += y;
            y = 0; // Negative height is ok (isEmpty => true)
        }
        if (height > max_height - y) // bottom() > max_height
        {
            height = max_height - y;
        }
    }

    /**
     * Compares equality of two Rect by the dimensions and position of these.
     *
     * @param  other The Rect to compare with.
     *
     * @return true if the compared Rect have the same dimensions and coordinates.
     */
    bool operator==(const Rect& other) const
    {
        return isEqual(other);
    }

    /**
     * Opposite of the == operator.
     *
     * @param  other The Rect to compare with.
     *
     * @return true if the compared Rect differ in dimensions or coordinates.
     */
    bool operator!=(const Rect& other) const
    {
        return !isEqual(other);
    }

    /**
     * Query if this object is empty.
     *
     * @return true if any of the dimensions are 0.
     */
    bool isEmpty() const
    {
        return width <= 0 || height <= 0;
    }

    /**
     * Calculate the area of the rectangle.
     *
     * @return area of the rectangle.
     */
    int32_t area() const
    {
        return isEmpty() ? 0 : width * height;
    }

private:
    bool isEqual(const Rect& other) const
    {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }
};

/**
 * A very simple container class using pre-allocated memory.
 *
 * @tparam T        The type of objects this container works on.
 * @tparam capacity The maximum number of objects this container can store.
 */
template <class T, uint16_t capacity>
class Vector
{
public:
    /** Default constructor. Constructs an empty vector. */
    Vector()
        : _size(0)
    {
    }

    /**
     * Index operator.
     *
     * @param  idx The index of the element to obtain.
     *
     * @return A reference to the element placed at index idx.
     */
    T& operator[](uint16_t idx)
    {
        return _elem[idx];
    }

    /**
     * Const version of the index operator.
     *
     * @param  idx The index of the element to obtain.
     *
     * @return A const reference to the element placed at index idx.
     */
    const T& operator[](uint16_t idx) const
    {
        return _elem[idx];
    }

    /**
     * Adds an element to the Vector if the Vector is not full.
     *
     *
     * Adds an element to the Vector if the Vector is not full. Does nothing if the Vector
     * is full.
     *
     * @param  e The element to add to the Vector.
     */
    void add(T e)
    {
        assert(_size < capacity && "Vector capacity exceeded");
        if (_size < capacity)
        {
            _elem[_size++] = e;
        }
    }

    /**
     * Removes an element from the Vector if found in the Vector. Does nothing if the
     * element is not found in the Vector. The == operator of the element is used when
     * comparing it with the elements in the Vector.
     *
     * @param  e The element to remove from the Vector.
     */
    void remove(T e)
    {
        for (int i = 0; i < _size; i++)
        {
            if (_elem[i] == e)
            {
                for (int j = i; j < _size && j < capacity - 1; j++)
                {
                    _elem[j] = _elem[j + 1];
                }
                _size--;
            }
        }
    }

    /**
     * Removes an element at the specified index of the Vector. Will "bubble-down" any
     * remaining elements after the specified index.
     *
     * @param  index The index to remove.
     *
     * @return The value of the removed element.
     */
    T removeAt(uint16_t index)
    {
        assert(index < _size);

        _size--;
        T tmp = _elem[index];
        for (int i = index; i < _size; i++)
        {
            _elem[i] = _elem[i + 1];
        }
        return tmp;
    }

    /**
     * Removes an element at the specified index of the Vector. The last element in the list
     * is moved to the position where the element is removed.
     *
     * @param  index The index to remove.
     */
    void quickRemoveAt(uint16_t index)
    {
        assert(index < _size);

        _size--;
        // No need to copy element when removing the last element in the vector
        if (index < _size)
        {
            _elem[index] = _elem[_size];
        }
    }

    /** Reverses the ordering of the elements in the Vector. */
    void reverse()
    {
        uint16_t a = 0;
        uint16_t b = _size;
        for (; a < --b; a++)
        {
            T tmp = _elem[a];
            _elem[a] = _elem[b];
            _elem[b] = tmp;
        }
    }

    /**
     * Checks if the Vector contains an element. The == operator of the element is used when
     * comparing it with the elements in the Vector.
     *
     * @param  elem The element.
     *
     * @return true if the Vector contains the element, false otherwise.
     */
    bool contains(T elem)
    {
        for (uint16_t i = 0; i < _size; i++)
        {
            if (elem == _elem[i])
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Gets the current size of the Vector which is the number of elements contained in the
     * Vector.
     *
     * Gets the current size of the Vector which is the number of elements contained in the
     * Vector.
     *
     * @return The size of the Vector.
     */
    uint16_t size() const
    {
        return _size;
    }

    /**
     * Query if this object is empty.
     *
     * @return true if the Vector contains no elements.
     */
    bool isEmpty() const
    {
        return _size == 0;
    }

    /**
     * Query the maximum capacity of the vector.
     *
     * @return The capacity the Vector was initialized with.
     */
    uint16_t maxCapacity() const
    {
        return capacity;
    }

    /**
     * Clears the contents of the container. It does not destruct any of the elements in the
     * Vector.
     */
    void clear()
    {
        _size = 0;
    }

private:
    T _elem[capacity];
    uint16_t _size;
};

/** A simple struct containing coordinates. */
struct Point
{
    int32_t x; ///< The x coordinate
    int32_t y; ///< The y coordinate

    /**
     * The squared distance from this Point to another Point.
     *
     * @param [in] o The point to get the squared distance to.
     *
     * @return The squared distance.
     */
    int32_t dist_sqr(struct Point& o)
    {
        return (x - o.x) * (x - o.x) + (y - o.y) * (y - o.y);
    }
};

/** Values that represent directions. */
enum Direction
{
    NORTH, ///< An enum constant representing the north option
    SOUTH, ///< An enum constant representing the south option
    EAST,  ///< An enum constant representing the east option
    WEST   ///< An enum constant representing the west option
};

/** Defines an alignment type. */
typedef uint8_t Alignment;
static const Alignment LEFT = 0;   ///< Text is left aligned
static const Alignment CENTER = 1; ///< Text is centered horizontally
static const Alignment RIGHT = 2;  ///< Text is right aligned

/** Defines a the direction to write text. */
typedef uint8_t TextDirection;
static const TextDirection TEXT_DIRECTION_LTR = 0; ///< Text is written Left-To-Right, e.g. English
static const TextDirection TEXT_DIRECTION_RTL = 1; ///< Text is written Right-To-Left, e.g. Hebrew

/** Values that represent frame buffers. */
enum FrameBuffer
{
    FB_PRIMARY,   ///< First framebuffer
    FB_SECONDARY, ///< Second framebuffer
    FB_TERTIARY   ///< Third framebuffer
};

/** Values that represent gradients. */
enum Gradient
{
    GRADIENT_HORIZONTAL, ///< Horizontal gradient.
    GRADIENT_VERTICAL    ///< Vertical gradient
};

/** Values that represent display rotations. */
enum DisplayRotation
{
    rotate0, ///< The display is oriented like the framebuffer
    rotate90 ///< The display is rotated 90 degrees compared to the framebuffer layout
};

/** Values that represent display orientations. */
enum DisplayOrientation
{
    ORIENTATION_LANDSCAPE, ///< The display has more pixels from left to right than from top to bottom
    ORIENTATION_PORTRAIT   ///< The display has more pixels from top to bottom than from right to left
};

/** Values that represent text rotations. */
enum TextRotation
{
    TEXT_ROTATE_0,   ///< Text is written from left to right
    TEXT_ROTATE_90,  ///< Text is written from top to bottom
    TEXT_ROTATE_180, ///< Text is written from right to left (upside down)
    TEXT_ROTATE_270  ///< Text is written bottom to top
};

/** Values that represent wide text actions. */
enum WideTextAction
{
    WIDE_TEXT_NONE,                          ///< Do nothing, simply cut the text in the middle of any character that extends beyond the width of the TextArea
    WIDE_TEXT_WORDWRAP,                      ///< Wrap between words, no ellipsis, keep wrapping lines
    WIDE_TEXT_WORDWRAP_ELLIPSIS,             ///< Wrap between words, ellipsis anywhere "Very long t..."
    WIDE_TEXT_WORDWRAP_ELLIPSIS_AFTER_SPACE, ///< Wrap between words, ellipsis only after space "Very long ..."
    WIDE_TEXT_CHARWRAP,                      ///< Wrap between any two characters, no ellipsis, keep wrapping lines
    WIDE_TEXT_CHARWRAP_ELLIPSIS,             ///< Wrap between any two characters, ellipsis anywhere, as used in Chinese
    WIDE_TEXT_CHARWRAP_DOUBLE_ELLIPSIS       ///< Wrap between any two characters, double ellipsis anywhere, as used in Chinese
};

/**
 * A simple struct for holding pairs of data.
 *
 * @tparam T1 The type of the first element.
 * @tparam T2 The type of the second element.
 */
template <class T1, class T2>
struct Pair
{
public:
    T1 first;  ///< The first element
    T2 second; ///< The second element

    /**
     * Constructor initializing the elements it holds, using their default constructors.
     */
    Pair()
        : first(T1()), second(T2())
    {
    }

    /**
     * Constructor initializing the elements it holds, using their copy constructor.
     *
     * @param  x Reference to the first element.
     * @param  y Reference to the second element.
     */
    Pair(const T1& x, const T2& y)
        : first(x), second(y)
    {
    }

    /**
     * Copy constructor.
     *
     * @tparam U Generic type parameter.
     * @tparam V Generic type parameter.
     * @param  p The pair to copy from.
     */
    template <class U, class V>
    Pair(const Pair<U, V>& p)
        : first(p.first), second(p.second)
    {
    }
};

/**
 * Describes a combination of rendering algorithm, image format, and alpha information. The
 * lowest bit is 0 for "Nearest neighbor", 1 for "Bilinear". The next bit is "0" for "no
 * alpha", "2" for "alpha". The rest is the Bitmap::Format shifted up by 2.
 */
typedef uint16_t RenderingVariant;
static const uint16_t RenderingVariant_NearestNeighbor = 0; ///< The rendering variant nearest neighbor bit value
static const uint16_t RenderingVariant_Bilinear = 1;        ///< The rendering variant bilinear bit value
static const uint16_t RenderingVariant_NoAlpha = 0;         ///< The rendering variant no alpha bit value
static const uint16_t RenderingVariant_Alpha = 2;           ///< The rendering variant alpha bit value
static const uint16_t RenderingVariant_FormatShift = 2;     ///< The rendering variant format shift

/** A fixed point value using 4 bits for the decimal part and 28 bits for the integral part. */
typedef int32_t fixed28_4;

/** A fixed point value using 16 bits for the decimal part and 16 bits for the integral part. */
typedef int32_t fixed16_16;

/** A 3D point. */
struct Point3D
{
    fixed28_4 X; ///< The X coordinate
    fixed28_4 Y; ///< The Y coordinate
    float Z;     ///< The Z coordinate
    float U;     ///< The U coordinate
    float V;     ///< The V coordinate
};

/**
 * A texture source. Contains a pointer to the data and the width and height of the texture. The
 * alpha channel is used in 565 rendering with alpha. The stride is the width used when
 * moving to the next line of the texture.
 */
struct TextureSurface
{
    const uint16_t* data;     ///< The pixel bits or indexes for color in CLUT entries
    const uint8_t* extraData; ///< The alpha channel or clut data
    int32_t width;            ///< The width
    int32_t height;           ///< The height
    int32_t stride;           ///< The stride
};

/**
 * The destination of a draw operation. Contains a pointer to where to draw and the stride of
 * the drawing surface.
 */
struct DrawingSurface
{
    uint16_t* address; ///< The bits
    int32_t stride;    ///< The stride
};

/** Text IDs as generated by the text converter are simple uint16_t typedefs. */
typedef uint16_t TypedTextId;

/** Values that represent dma types. */
enum DMAType
{
    DMA_TYPE_GENERIC, ///< Generic DMA Implementation
    DMA_TYPE_CHROMART ///< ChromART hardware DMA Implementation
};

/**
 * A list of the vector graphics primitives.
 *
 * @see VectorRenderer::drawPath
 */
enum VectorPrimitives
{
    VECTOR_PRIM_CLOSE = 0,       ///< Close the path
    VECTOR_PRIM_MOVE = 1,        ///< Move to a point
    VECTOR_PRIM_LINE = 2,        ///< Line to a point from current position
    VECTOR_PRIM_HLINE = 3,       ///< Horizontal line to a point from current position
    VECTOR_PRIM_VLINE = 4,       ///< Vertical line to a point from current position
    VECTOR_PRIM_BEZIER_QUAD = 5, ///< Quadratic Bezier (1 control point) curve to a point from the current position
    VECTOR_PRIM_BEZIER_CUBIC = 6 ///< Cubic Bezier (2 control points) curve to a point from the current position
};

/**
 * Dictionary entry used in LZW decompression.
 */
struct LZW9DictionaryEntry
{
    uint8_t character;    ///< Current character of the entry
    uint8_t length;       ///< Remaining length of the entry
    uint16_t prefixIndex; ///< Index to previous character
};

} // namespace touchgfx

#endif // TOUCHGFX_TYPES_HPP
