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
#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <common/AbstractPartition.hpp>
#include <common/Meta.hpp>

namespace touchgfx
{
/**
 * @class Partition Partition.hpp common/Partition.hpp
 *
 * @brief This type provides a concrete Partition of memory-slots capable of holding any of the
 *        specified list of types.
 *
 *        The Partition is not aware of the types stored in the Partition memory, hence it
 *        provides no mechanism for deleting C++ objects when the Partition is clear()'ed.
 *
 *        This class implements AbstractPartition.
 *
 * @tparam ListOfTypes        Type of the list of types.
 * @tparam NUMBER_OF_ELEMENTS Type of the number of elements.
 *
 * @see AbstractPartition
 */
template< typename ListOfTypes, uint16_t  NUMBER_OF_ELEMENTS>
class Partition : public AbstractPartition
{
public:

    /**
     * @typedef ListOfTypes SupportedTypesList
     *
     * @brief Provides a generic public type containing the list of supported types.
     *
     *        Provides a generic public type containing the list of supported types.
     */
    typedef ListOfTypes SupportedTypesList;

    /**
     * Compile-time generated constants specifying the "element" or "slot" size used by this partition
     */
    enum
    {
        INTS_PR_ELEMENT = (sizeof(typename meta::select_type_maxsize< SupportedTypesList >::type) + sizeof(int) - 1) / sizeof(int),
        SIZE_OF_ELEMENT = INTS_PR_ELEMENT * sizeof(int)
    };

    /**
     * @fn Partition::Partition()
     *
     * @brief Default constructor.
     *
     *        Constructs an empty Partition.
     */
    Partition() : AbstractPartition()
    {
    }

    /**
     * @fn virtual Partition::~Partition()
     *
     * @brief Destructor.
     */
    virtual ~Partition()
    {
    }

    /**
     * @fn virtual uint16_t Partition::capacity() const
     *
     * @brief Specialization of AbstractPartition::capacity().
     *
     *        Specialization of AbstractPartition::capacity().
     *
     * @return An uint16_t.
     *
     * @see touchgfx::AbstractPartition::capacity()
     */
    virtual uint16_t capacity() const
    {
        return NUMBER_OF_ELEMENTS;
    }

    /**
     * @fn virtual uint32_t Partition::element_size()
     *
     * @brief Specialization of AbstractPartition::element_size().
     *
     *        Specialization of AbstractPartition::element_size().
     *
     * @return An uint32_t.
     *
     * @see touchgfx::AbstractPartition::element_size()
     */
    virtual uint32_t element_size()
    {
        return sizeof(stBlocks[0]);
    }

protected:

    /**
     * @fn virtual void* Partition::element(uint16_t index)
     *
     * @brief Specialization of AbstractPartition::element()
     *
     * @param index Zero-based index of the.
     *
     * @return null if it fails, else a void*.
     *
     * @see touchgfx::AbstractPartition::element()
     */
    virtual void* element(uint16_t index)
    {
        return &stBlocks[index];
    }

    /**
     * @fn virtual const void* Partition::element(uint16_t index) const
     *
     * @brief Specialization of AbstractPartition::element() const.
     *
     * @param index Zero-based index of the.
     *
     * @return null if it fails, else a void*.
     *
     * @see touchgfx::AbstractPartition::element()
     */
    virtual const void* element(uint16_t index) const
    {
        return &stBlocks[index];
    }

private:
    /**
     * Internal type used for storage, in order to ensure "natural" alignment of elements
     */
    struct Block
    {
        int filler[INTS_PR_ELEMENT];
    };

    Block stBlocks[NUMBER_OF_ELEMENTS]; ///< Actual memory storage
};

} // namespace touchgfx
#endif // PARTITION_HPP
