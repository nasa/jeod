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
 * @file models/utils/container/include/container.hh
 * Define the class JeodContainer, which adds checkpointability to
 * an STL sequence container replacement.
 */

/*******************************************************************************

Purpose:
  ()


*******************************************************************************/

#ifndef JEOD_MEMORY_CONTAINER_H
#define JEOD_MEMORY_CONTAINER_H

// Model includes
#include "checkpointable.hh"

// JEOD includes
#include "utils/memory/include/memory_manager.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstring>
#include <string>
#include <typeinfo>

//! Namespace jeod
namespace jeod
{

/**
 * A JeodContainer is a JEOD STL sequence container replacement
 * whose contents are checkpointable and restorable.
 */
template<typename ContainerType, typename ElemType>
class JeodContainer : public JeodCheckpointable,
                      public ContainerType
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodContainer)

public:
    // Types

    /**
     * This particular JeodContainer type.
     */
    using this_container_type = JeodContainer<ContainerType, ElemType>;

    /**
     * Import the ContainerType's container type.
     */
    using stl_container_type = typename ContainerType::stl_container_type;

    // Member functions

    /**
     * Default constructor.
     */
    JeodContainer()
        : JeodCheckpointable(),
          ContainerType(),
          checkpoint_iter()

    {
    }

    /**
     * Copy constructor.
     * @note
     * This copies the source's ContainerType contents only.
     * The Checkpointable contents and the added checkpoint members are
     * not copied.
     * @param source Container to be copied.
     */
    JeodContainer(const this_container_type & source)
        : JeodCheckpointable(),
          ContainerType(source),
          checkpoint_iter(),
          elem_type_descriptor(nullptr)
    {
    }

    /**
     * Copy constructor.
     * @note
     * This copies the source's ContainerType contents only.
     * The Checkpointable contents and the added checkpoint members are
     * not copied.
     * @param source Container to be copied.
     */
    JeodContainer(const stl_container_type & source)
        : JeodCheckpointable(),
          ContainerType(source),
          checkpoint_iter(),
          elem_type_descriptor(nullptr)
    {
    }

    /**
     * Assignment operator.
     * @note
     * This copies the source's ContainerType contents only.
     * The Checkpointable contents and the added checkpoint members are
     * not copied.
     * @param source Container to be copied.
     */
    JeodContainer & operator=(const this_container_type & source)
    {
        if(&source != this)
        {
            ContainerType::operator=(source);
        }
        return *this;
    }

    /**
     * Assignment operator.
     * @note
     * This copies the source's ContainerType contents only.
     * The Checkpointable contents and the added checkpoint members are
     * not copied.
     * @param source Container to be copied.
     */
    JeodContainer & operator=(const stl_container_type & source)
    {
        ContainerType::operator=(source);
        return *this;
    }

    /**
     * Destructor.
     */
    ~JeodContainer() override = default;

    /**
     * Swap STL sequence container contents --
     * but not the stuff related to checkpoint or restart.
     */
    void swap_contents(this_container_type & other)
    {
        ContainerType::swap(other);
    }

    /**
     * Swap STL sequence container contents --
     * but not the stuff related to checkpoint or restart.
     */
    void swap_contents(stl_container_type & other)
    {
        ContainerType::swap(other);
    }

    /**
     * Push a value onto the end of the contents.
     * This method is pure virtual because the value provided to the method
     * is a string. Translating the input string to the appropriate element type
     * is the responsibility of template instantiations.
     * @param value Value, in string form, to be added to the contents.
     * @note
     * This member should be protected or (even better) private.
     * It is marked as public to avoid problems with Trick and SWIG.
     */
    virtual void perform_insert_action(const std::string & value) = 0;

    /**
     * Cleanup detritus created during the restore process.
     * The default action is to do nothing.
     * @param value String name of cleanup target.
     * This member should be protected or (even better) private.
     * It is marked as public to avoid problems with Trick and SWIG.
     */
    virtual void perform_cleanup_action(const std::string & value JEOD_UNUSED) {}

    /**
     * Initialize a checkpointable object, called by the checkpoint manager.
     *
     * In the case of a JeodContainer, this method gets the descriptor for
     * the type of data stored in the container.
     */
    void initialize_checkpointable(const void * container JEOD_UNUSED,                 // In: -- Not used.
                                   const std::type_info & container_type JEOD_UNUSED,  // In: -- Not used.
                                   const std::string & elem_name JEOD_UNUSED) override // In: -- Not used.
    {
        if(elem_type_descriptor == nullptr)
        {
            elem_type_descriptor = JeodMemoryManager::get_type_descriptor(typeid(ElemType));
        }
    }

    /**
     * Prepare to checkpoint the object.
     *
     * In the case of a JeodContainer, this method initializes a checkpoint
     * iterator, data member checkpoint_iter, to the start of the contents.
     */
    void start_checkpoint() override
    {
        checkpoint_iter = this->begin();
    }

    /**
     * Advance to the next item to be checkpointed.
     *
     * In the case of a JeodContainer, this method simply advances the
     * checkpoint iterator to point to the next item in the contents.
     */
    void advance_checkpoint() override
    {
        ++checkpoint_iter;
    }

    /**
     * Indicate whether the checkpoint dump of this object is finished.
     *
     * In the case of a JeodContainer, the dump is finished when the internal
     * checkpoint iterator points beyond the last item in the contents. */
    bool is_checkpoint_finished() override
    {
        return (checkpoint_iter == this->end());
    }

    /**
     * Names the action to be performed prior to performing any of the
     * restore actions.
     *
     * In the case of a JeodContainer, the init name is always "clear".
     */
    const std::string get_init_name() override
    {
        return "clear";
    }

    /**
     * Return the name of the action to be printed along with the current value.
     *
     * In the case of a JeodContainer, the action name is always "insert".
     */
    const std::string get_item_name() override
    {
        return "insert";
    }

    /**
     * Names the action to be performed after to performing any of the
     * restore actions.
     *
     * In the case of a JeodContainer, the init name is always "cleanup".
     */
    const std::string get_final_name() override
    {
        return "cleanup";
    }

    /**
     * Perform a checkpoint-restart action that will, in part, restore the object
     * to its state at the time of the checkpoint.
     *
     * In the case of a JeodContainer, the actions are "clear", "insert", and
     * "cleanup". The checkpoint writer automatically creates an initial "clear"
     * entry as the first entry in the checkpoint file for a JeodCheckpointable
     * object and a "cleanup" entry as the final entry. An "insert" entry is
     * created for each element in the container's contents.
     */
    int perform_restore_action(const std::string & action_name, const std::string & action_value) override
    {
        int return_code;

        // "clear" action response is to clear the contents of the object.
        if(action_name.compare("clear") == 0)
        {
            this->clear();
            return_code = 0;
        }

        // "insert" action response is to insert the value in the contents.
        else if(action_name.compare("insert") == 0)
        {
            perform_insert_action(action_value);
            return_code = 0;
        }

        // "cleanup" action response is to remove detritus created during restart.
        else if(action_name.compare("cleanup") == 0)
        {
            perform_cleanup_action(action_value);
            return_code = 0;
        }

        // All other actions are illegal and should never happen.
        else
        {
            return_code = -1;
        }

        return return_code;
    }

protected:
    /**
     * Iterator for walking through the container during checkpoint.
     */
    typename ContainerType::iterator checkpoint_iter; //!< trick_io(**)

    /**
     * Memory model descriptor of the type of data stored in the container.
     */
    const JeodMemoryTypeDescriptor * elem_type_descriptor{}; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
