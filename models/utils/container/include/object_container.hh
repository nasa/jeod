//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/object_container.hh
 * Define class template JeodObjectContainer.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_OBJECT_CONTAINER_H
#define JEOD_MEMORY_OBJECT_CONTAINER_H

// Model includes
#include "container.hh"

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/config.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// System includes
#include <cstddef>
#include <string>

//! Namespace jeod
namespace jeod
{

/**
 * A JeodObjectContainer is a JeodContainer that contains objects
 * of type ElemType.
 */
template<typename ContainerType, typename ElemType>
class JeodObjectContainer : public JeodContainer<ContainerType, ElemType>
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodObjectContainer)

public:
    /**
     * Construct a JeodObjectContainer.
     */
    JeodObjectContainer()
        : index(0),
          copy(NULL)
    {
    }

    /**
     * Copy-construct a JeodObjectContainer.
     * @note
     * This copies the Container contents, but not the Checkpointable contents.
     * @param source Object container to be copied.
     */
    JeodObjectContainer(const JeodObjectContainer & source)
        : JeodContainer<ContainerType, ElemType>(source),
          index(0),
          copy(NULL)
    {
    }

    /**
     * Copy-construct a JeodObjectContainer.
     * @note
     * This copies the Container contents, but not the Checkpointable contents.
     * @param source Object container to be copied.
     */
    explicit JeodObjectContainer(const typename ContainerType::stl_container_type & source)
        : JeodContainer<ContainerType, ElemType>(source),
          index(0),
          copy(NULL)
    {
    }

    /**
     * Copy from a JeodObjectContainer.
     * @note
     * This copies the Container contents, but not the Checkpointable contents.
     * @param source Object container to be copied.
     */
    JeodObjectContainer & operator=(const JeodObjectContainer & source)
    {
        JeodContainer<ContainerType, ElemType>::operator=(source);
        return *this;
    }

    /**
     * Copy from an STL container.
     * @note
     * This copies the Container contents, but not the Checkpointable contents.
     * @param source Object container to be copied.
     */
    JeodObjectContainer & operator=(const typename ContainerType::stl_container_type & source)
    {
        JeodContainer<ContainerType, ElemType>::operator=(source);
        return *this;
    }

    /**
     * Destruct a JeodObjectContainer.
     */
    virtual ~JeodObjectContainer()
    {
        post_checkpoint();
    }

    /**
     * Prepare to checkpoint a JeodObjectContainer.
     * The contents of an object container is checkpointed by allocating a
     * C-style array of the same size as the container and populating the
     * array with copies of the container contents. The existing checkpoint
     * capabilities will checkpoint this array, so all that remains to be done
     * is to associate the array elements with the container.
     */
    void pre_checkpoint() override
    {
        if(this->size() > 0)
        {
            unsigned int ii = 0;
            copy = JEOD_ALLOC_CLASS_ARRAY(this->size(), ElemType);
            for(typename ContainerType::iterator iter = this->begin(); iter != this->end(); ++iter)
            {
                copy[ii] = *iter;
                ++ii;
            }
        }
    }

    /**
     * Cleanup after performing a checkpoint.
     */
    void post_checkpoint() override // cppcheck-suppress virtualCallInConstructor
    {
        if(copy != NULL)
        {
            JEOD_DELETE_ARRAY(copy);
        }
        copy = NULL;
    }

    /**
     * Cleanup after performing a restart.
     */
    void post_restart() override
    {
        post_checkpoint();
    }

    /**
     * Prepare to checkpoint the object in question.
     * The local checkpoint index is initialized to zero to reflect that the
     * parent class' checkpoint iterator starts at the zeroth element.
     */
    void start_checkpoint() override
    {
        index = 0;
        JeodContainer<ContainerType, ElemType>::start_checkpoint();
    }

    /**
     * Advance to the next item to be checkpointed.
     * The local checkpoint index is advanced to keep in sync with the
     * parent class' checkpoint iterator.
     */
    void advance_checkpoint() override
    {
        ++index;
        JeodContainer<ContainerType, ElemType>::advance_checkpoint();
    }

    /**
     * Return the value of the item to be written to the checkpoint file.
     * For a JeodObjectContainer, the value is the name of the corresponding
     * object in the C-style copy of the object's contents.
     */
    const std::string get_item_value() override
    {
        return JeodSimulationInterface::get_name_at_address(reinterpret_cast<void *>(&copy[index]),
                                                            this->elem_type_descriptor);
    }

    /**
     * Interpret the provided value and add it to the list.
     * For a JeodObjectContainer, the value should name an element of the
     * C-style copy of the object's contents.
     */
    void perform_insert_action(const std::string & value) // In: -- Value to be added.
        override
    {
        const ElemType * temp_obj = reinterpret_cast<ElemType *>(JeodSimulationInterface::get_address_at_name(value));

        this->insert(this->end(), *temp_obj);
    }

    /**
     * Return the value of the item to be written to the checkpoint file.
     * For a JeodObjectContainer, the value is the name of the corresponding
     * object in the C-style copy of the object's contents.
     */
    const std::string get_final_value() override
    {
        return JeodSimulationInterface::get_name_at_address(reinterpret_cast<void *>(copy), this->elem_type_descriptor);
    }

    /**
     * Cleanup detritus created during the restore process.
     * Here we delete the temporary array created during checkpoint.
     * @param value String name of cleanup target.
     */
    void perform_cleanup_action(const std::string & value) override
    {
        auto * temp_obj = reinterpret_cast<ElemType *>(JeodSimulationInterface::get_address_at_name(value));
        if(temp_obj != NULL)
        {
            JEOD_DELETE_ARRAY(temp_obj);
        }
    }

protected:
    /**
     * Index number into the copy; used during checkpoint process.
     */
    JEOD_SIZE_T index; //!< trick_io(**)

    /**
     * C-style array copy of the object; used during checkpoint process.
     */
    ElemType * copy; //!< trick_io(**)
};

// DO NOT USE.
// This is deprecated and will disappear with the next release.
// Use one of the JeodObjectXxx classes instead.
#define JEOD_OBJECT_CONTAINER(container_type, elem_type) JeodObjectContainer<Jeod##container_type<elem_type>, elem_type>

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
