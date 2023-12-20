/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
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
#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <cassert>
#include <touchgfx/hal/Types.hpp>

#ifdef __ARMCC_VERSION
// Keil compiler issues irrelevant warning relating to missing ctor initialization for BitmapData.
#pragma diag_suppress 368
#endif

namespace touchgfx
{
/**
 * @typedef uint16_t BitmapId
 *
 * @brief This type shall be used by the application to define unique IDs for all bitmaps in the
 *        system. The application shall define bitmap IDs in the range [0, number of bitmaps -
 *        1].
 */
typedef uint16_t BitmapId;

const BitmapId BITMAP_ANIMATION_STORAGE = 0xFFFEU; ///< A virtual id representing animation storage.
const BitmapId BITMAP_INVALID = 0xFFFFU;           ///< Define the bitmapId if an invalid bitmap

class BitmapData;

/**
 * @class Bitmap Bitmap.hpp touchgfx/Bitmap.hpp
 *
 * @brief This class provides a proxy object for a bitmap image.
 *
 *        This class provides a proxy object for a bitmap image stored in the application
 *        specific bitmap database. The proxy provides access to the raw bitmap data as well as
 *        metadata.
 */
class Bitmap
{
public:

    /**
     * @enum BitmapFormat
     *
     * @brief Data of a bitmap can be stored in the following formats.
     *
     *        Data of a bitmap can be stored in the following formats.
     */
    enum BitmapFormat
    {
        RGB565,   ///< 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
        RGB888,   ///< 24-bit, 8 bits for each of red, green and blue, no alpha channel
        ARGB8888, ///< 32-bit, 8 bits for each of red, green, blue and alpha channel
        BW,       ///< 1-bit, black / white, no alpha channel
        BW_RLE,   ///< 1-bit, black / white, no alpha channel compressed with horizontal RLE
        GRAY2,    ///< 2-bit grayscale
        GRAY4     ///< 4.bit grayscale
    };

    /**
     * @struct BitmapData Bitmap.hpp touchgfx/Bitmap.hpp
     *
     * @brief Data of a bitmap.
     *
     *        Data of a bitmap.
     */
    struct BitmapData
    {
        const uint8_t* const data;                  ///< The data of this bitmap
        const uint8_t* const alphaData;             ///< The data of the alpha channel (contains 0 if no alpha channel exist)
        const uint16_t       width;                 ///< The width of the bitmap
        const uint16_t       height;                ///< The height of the bitmap
        const uint16_t       solidRect_x;           ///< The x coordinate of the maximum solid rectangle of the bitmap
        const uint16_t       solidRect_y;           ///< The y coordinate of the maximum solid rectangle of the bitmap
        const uint16_t       solidRect_width;       ///< The width of the maximum solid rectangle of the bitmap
        const uint16_t       solidRect_height : 13; ///< The height of the maximum solid rectangle of the bitmap
        const uint8_t        format : 3;            ///< Determine the format of the data
    };

    /**
     * @struct DynamicBitmapData Bitmap.hpp touchgfx/Bitmap.hpp
     *
     * @brief Data of a dynamic bitmap.
     *
     *        Data of a dynamic bitmap.
     */
    struct DynamicBitmapData
    {
        Rect     solid;  ///< The solidRect of this bitmap
        uint16_t width;  ///< The width of the bitmap
        uint16_t height; ///< The height of the bitmap
        uint16_t format; ///< Determine the format of the data
        uint8_t  inuse;  ///< zero if not in use
        uint8_t  alpha;  ///< true if separate alpha for 565 format (always false)
    };

    /**
     * @struct CacheTableEntry Bitmap.hpp touchgfx/Bitmap.hpp
     *
     * @brief Cache bookkeeping.
     *
     *        Cache bookkeeping.
     */
    struct CacheTableEntry
    {
        uint8_t* data; ///< Pointer to location of image data for this bitmap in the cache. 0 if bitmap not cached.
    };

    /**
     * @fn explicit Bitmap::Bitmap(const BitmapId id = BITMAP_INVALID)
     *
     * @brief Creates and binds a Bitmap instance to the corresponding entry in the BitmapData
     *        array.
     *
     *        Creates and binds a Bitmap instance to the corresponding entry in the BitmapData
     *        array.
     *
     * @param id The unique bitmap identifier.
     */
    explicit Bitmap(const BitmapId id = BITMAP_INVALID) : bitmapId(id)
    {
    }

    /**
     * @fn BitmapId Bitmap::getId() const
     *
     * @brief Gets the id of this Bitmap.
     *
     *        Gets the id of this Bitmap.
     *
     * @return The id of this Bitmap.
     */
    BitmapId getId() const
    {
        assert(bitmaps != 0 && "Bitmap database has not been initialized.");
        return bitmapId;
    }

    /**
     * @fn const uint8_t* Bitmap::getData() const;
     *
     * @brief Gets a pointer to the Bitmap data.
     *
     *        Gets a pointer to the Bitmap data.
     *
     * @note If this bitmap is cached, it will return the cached version of bitmap data.
     *
     * @return A pointer to the raw bitmap data.
     */
    const uint8_t* getData() const;

    /**
     * @fn const uint8_t* Bitmap::getAlphaData() const;
     *
     * @brief Gets a pointer to the alpha data, if present in the bitmap.
     *
     *        Gets a pointer to the alpha data, if present in the bitmap.
     *
     * @note If this bitmap is cached, it will return the cached version of alpha data for this
     *       bitmap.
     *
     * @return A pointer to the raw alpha channel data. If no alpha channel exist 0 is returned.
     */
    const uint8_t* getAlphaData() const;

    /**
     * @fn BitmapFormat Bitmap::getFormat() const;
     *
     * @brief Gets the format of how the bitmap is stored.
     *
     *        Gets the format of how the bitmap is stored.
     *
     * @return The format of how the bitmap data is stored.
     */
    BitmapFormat getFormat() const;

    /**
     * @fn uint16_t Bitmap::getWidth() const
     *
     * @brief Gets the width of the Bitmap in pixels.
     *
     *        Gets the width of the Bitmap in pixels.
     *
     * @return The bitmap width in pixels.
     */
    uint16_t getWidth() const;

    /**
     * @fn uint16_t Bitmap::getHeight() const
     *
     * @brief Gets the height of the Bitmap in pixels.
     *
     *        Gets the height of the Bitmap in pixels.
     *
     * @return The bitmap height in pixels.
     */
    uint16_t getHeight() const;

    /**
     * @fn Rect Bitmap::getRect() const
     *
     * @brief Gets the rectangle describing the dimensions of the Bitmap.
     *
     *        Gets the rectangle describing the dimensions of the Bitmap.
     *
     * @return a Rect describing the dimensions of this bitmap.
     */
    Rect getRect() const
    {
        return Rect(0, 0, getWidth(), getHeight());
    }

    /**
     * @fn bool Bitmap::isAlphaPerPixel() const
     *
     * @brief Query if this object has an alpha channel.
     *
     *        Query if this object has an alpha channel.
     *
     * @return True if the bitmap contains an alpha channel (an alpha value for each pixel)
     */
    bool isAlphaPerPixel() const
    {
        assert(bitmaps != 0 && "Bitmap database has not been initialized.");
        return ((bitmaps != 0) && (bitmapId < numberOfBitmaps)) ? (bitmaps[bitmapId].alphaData != 0) : false;
    }

    /**
     * @fn Rect Bitmap::getSolidRect() const
     *
     * @brief Gets the largest solid rectangle in the bitmap.
     *
     *        Gets the largest solid, i.e. not transparent, rectangle in the bitmap.
     *
     * @return The maximum solid rectangle of the bitmap.
     */
    Rect getSolidRect() const;

    /**
     * @fn bool Bitmap::hasTransparentPixels() const
     *
     * @brief Query if this object has transparent pixels.
     *
     * @return True if this bitmap has transparent pixels.
     */
    bool hasTransparentPixels() const;

    /**
     * @fn static void Bitmap::registerBitmapDatabase(const BitmapData* data, const uint16_t n, uint16_t* cachep = 0, uint32_t csize = 0);
     *
     * @brief Registers an array of bitmaps.
     *
     *        Registers an array of bitmaps. All Bitmap instances are bound to this database.
     *
     * @param data            A reference to the BitmapData storage array.
     * @param n               The number of bitmaps in the array.
     * @param [in,out] cachep (Optional) Pointer to memory region in which bitmap data can be
     *                        cached.
     * @param csize           Size of cache memory region in bytes (0 if unused)
     * @param numberOfDynamicBitmaps Number of dynamic bitmaps to be allowed in the cache.
     */
    static void registerBitmapDatabase(const BitmapData* data, const uint16_t n, uint16_t* cachep = 0, uint32_t csize = 0,
                                       uint32_t numberOfDynamicBitmaps = 0);

    /**
     * @fn static bool Bitmap::cache(BitmapId id);
     *
     * @brief Cache this bitmap into RAM.
     *
     *        Cache this bitmap into unused RAM in the bitmap cache.
     *
     * @note A memory region large enough to hold this bitmap must be configured and a large
     *       enough part of it must be available. Caching of a bitmap may involve a
     *       defragmentation of the bitmap cache. See TouchGFX documentation for details on
     *       caching.
     *
     * @param id The id of the bitmap to cache.
     *
     * @return true if caching went well, false otherwise.
     *
     * @see registerBitmapDatabase
     */
    static bool cache(BitmapId id);

    /**
     * @fn static bool Bitmap::cacheReplaceBitmap(BitmapId out, BitmapId in);
     *
     * @brief Replace a bitmap in RAM with another Bitmap.
     *
     *        Replace a bitmap in RAM with another Bitmap. The Bitmaps must have same size.
     *
     * @param out The id of the bitmap to remove from the cache.
     * @param in  The id of the bitmap to cache.
     *
     * @return true if the replacement went well, false otherwise.
     */
    static bool cacheReplaceBitmap(BitmapId out, BitmapId in);

    /**
     * @fn static bool Bitmap::cacheRemoveBitmap(BitmapId id);
     *
     * @brief Remove this bitmap from the RAM cache.
     *
     *        Remove this bitmap from the RAM cache.
     *
     * @note The bitmap will be removed from the RAM cache. Unless the bitmap is otherwise stored
     *       in (slow) RAM it can not be drawn anymore and must be cached again before use. The
     *       RAM freed can be used for caching of another bitmap. See TouchGFX documentation for
     *       details on caching.
     *
     * @param id The id of the bitmap to cache.
     *
     * @return true if bitmap was found and removed, false otherwise.
     *
     * @see registerBitmapDatabase
     */
    static bool cacheRemoveBitmap(BitmapId id);

    /**
     * @fn static uint8_t* Bitmap::cacheGetAddress(BitmapId id);
     *
     * @brief Get address of cache buffer for this bitmap
     *
     *        Get address of cache buffer for this bitmap. Note: The address is only valid
     *        until next Bitmap::cache() call.
     *
     * @param id The id of the bitmap in cache.
     *
     * @return Address if bitmap was found, zero otherwise.
     */
    static uint8_t* cacheGetAddress(BitmapId id);

    /**
     * @fn static bool Bitmap::cacheIsCached(BitmapId id);
     *
     * @brief Check if the Bitmap is cached.
     *
     *        Check if the Bitmap is cached.
     *
     * @param id The id of the bitmap.
     *
     * @return true if bitmap is cached.
     */
    static bool cacheIsCached(BitmapId id);

    /**
     * @fn static bool Bitmap::cacheAll();
     *
     * @brief Cache all bitmaps from the Bitmap Database into RAM.
     *
     *        Cache all bitmaps from the Bitmap Database into RAM.
     *
     * @note A memory region large enough to hold all bitmaps must be configured. See TouchGFX
     *       documentation for details on caching.
     *
     * @return True if all bitmaps where cached.
     *
     * @see cache.
     */
    static bool cacheAll();

    /**
     * @fn static void Bitmap::clearCache();
     *
     * @brief Clears the cached bitmaps from RAM.
     *
     *        Clears the cached bitmaps from RAM.
     */
    static void clearCache();

    /**
     * @fn static BitmapId Bitmap::dynamicBitmapCreate(const uint16_t width, const uint16_t height, BitmapFormat format);
     *
     * @brief Create a dynamic bitmap.
     *
     *        Create a dynamic bitmap.
     *
     * @param width  Width of the bitmap.
     * @param height Height of the bitmap.
     * @param format Bitmap format of the bitmap.
     *
     * @return BitmapId of the new bitmap or BITMAP_INVALID if memory full.
     *
     * @note Creation of a new dynamic bitmap may cause existing dynamic bitmaps to be moved in
     *       memory. Do not rely on bitmap memory addresses of dynamic bitmaps obtained from
     *       dynamicBitmapGetAddress() to be valid across calls to dynamicBitmapCreate().
     *
     * @see dynamicBitmapAddress
     */
    static BitmapId dynamicBitmapCreate(const uint16_t width, const uint16_t height, BitmapFormat format);

    /**
     * @fn static bool Bitmap::dynamicBitmapDelete(BitmapId id);
     *
     * @brief Delete a dynamic bitmap.
     *
     *        Delete a dynamic bitmap.
     *
     * @param id The BitmapId of the dynamic bitmap.
     *
     * @return true if it succeeds, false if it fails.
     */
    static bool dynamicBitmapDelete(BitmapId id);

    /**
     * @fn static uint8_t* Bitmap::dynamicBitmapGetAddress(BitmapId id);
     *
     * @brief Get the address of the dynamic bitmap data.
     *
     *        Get the address of the dynamic bitmap data. It is important that the address of a
     *        dynamic bitmap is not stored elsewhere as a dynamic bitmap may be moved in memory
     *        when other bitmaps are added and removed. Only store the BitmapId and ask for the
     *        address of the bitmap data when needed.
     *
     * @param id The BitmapId of the dynamic bitmap.
     *
     * @return null if it fails, else an uint8_t*.
     *
     * @note The address of a dynamic bitmap may change when other dynamic bitmaps are added and
     *       removed. Never store the address of dynamic images, only store the BitmapId as that
     *       will not change.
     */
    static uint8_t* dynamicBitmapGetAddress(BitmapId id);

    /**
     * @fn static bool Bitmap::dynamicBitmapSetSolidRect(BitmapId id, const Rect& solidRect);
     *
     * @brief Set the solid rectangle of a dynamic bitmap
     *
     *        Set the solid rectangle of a dynamic bitmap. Only relevant for RGBA8888 bitmaps.
     *        The solid part of the bitmap is drawn faster than the transparent parts.
     *
     * @param id        The identifier.
     * @param solidRect The solid rectangle.
     *
     * @return true if it succeeds, false if it fails.
     */
    static bool dynamicBitmapSetSolidRect(BitmapId id, const Rect& solidRect);

    /**
     * @fn bool Bitmap::operator==(const Bitmap& other) const
     *
     * @brief Equality operator.
     *
     *        Equality operator.
     *
     * @param other The bitmap to compare with.
     *
     * @return True if this bitmap has the same id as the other bitmap.
     */
    bool operator==(const Bitmap& other) const
    {
        return (bitmapId == other.bitmapId);
    }

    /**
     * @fn bool Bitmap::operator!=(const Bitmap& other) const
     *
     * @brief Inequality operator.
     *
     *        Inequality operator.
     *
     * @param other The bitmap to compare with.
     *
     * @return True if this bitmap has a different id than the other bitmap.
     */
    bool operator!=(const Bitmap& other) const
    {
        return !(*this == other);
    }

    /**
     * @fn static void Bitmap::setCache(uint16_t* cachep, uint32_t csize, uint32_t numberOfDynamicBitmaps = 0);
     *
     * @brief Register a memory region in which bitmap data can be cached.
     *
     *        Register a memory region in which bitmap data can be cached.
     *
     * @param [in,out] cachep        Pointer to memory region in which bitmap data can be cached.
     * @param csize                  Size of cache memory region in bytes.
     * @param numberOfDynamicBitmaps Number of dynamic bitmaps to be allowed in the cache.
     */
    static void setCache(uint16_t* cachep, uint32_t csize, uint32_t numberOfDynamicBitmaps = 0);

private:
    /**
     * @fn static void Bitmap::compactCache();
     *
     * @brief Compact the bitmap cache to get continuous free memory on top.
     *
     *        Compact the bitmap cache to get continuous free memory on top.
     *
     * @note This method is called by Bitmap::cache when required.
     */
    static void compactCache();

    static uint32_t getSizeOfBitmap(BitmapId id);

    static bool copyBitmapToCache(BitmapId id, uint8_t* const dst);

    BitmapId                  bitmapId;
    static const BitmapData*  bitmaps;
    static DynamicBitmapData* dynBitmaps;
    static CacheTableEntry*   cacheTable;      //address of allocation point cache
    static BitmapId*          allocationTable; //order of allocations in cache
    static uint8_t*           nextFreeData;
    static uint16_t           nextAllocationIndex;
    static uint32_t           memoryRemaining;
    static uint32_t           totalMemory;
    static uint16_t           numberOfBitmaps;
    static uint16_t           numberOfDynamicBitmaps;
    static uint16_t           uncachedCount;  //uncached images, sort of ...
};

} // namespace touchgfx

#endif // BITMAP_HPP
