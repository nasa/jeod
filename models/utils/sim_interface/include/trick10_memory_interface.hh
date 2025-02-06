//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/trick10_memory_interface.hh
 * Define the interface for registering / deregistering memory with Trick.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/trick10_memory_interface.cc))



*******************************************************************************/

#ifndef JEOD_TRICK10_MEMORY_INTERFACE_HH
#define JEOD_TRICK10_MEMORY_INTERFACE_HH

#ifdef TRICK_VER
#if(TRICK_VER >= 10)

// System includes
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <string>

// JEOD includes

// Model includes
#include "jeod_class.hh"
#include "memory_attributes.hh"
#include "memory_interface.hh"
#include "simulation_interface.hh"
#include "trick_memory_interface.hh"

//! Namespace jeod
namespace jeod
{

// Forward declarations
class JeodCheckpointable;
class JeodMemoryManager;
class JeodMemoryTypeDescriptor;

} // namespace jeod

/**
 * Namespace Trick furnishes several standard functions for use in the Trick environment.
 */
namespace Trick
{
class ClassicCheckPointAgent;
} // namespace Trick

//! Namespace jeod
namespace jeod
{

/**
 * A TrickMemoryInterface implements the two required methods needed to register
 * and deregister memory with the simulation engine, Trick in this case.
 */
class JeodTrick10MemoryInterface : public JeodTrickMemoryInterface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodTrick10MemoryInterface)

public:
    // Member functions
    JeodTrick10MemoryInterface();
    ~JeodTrick10MemoryInterface() override = default;
    JeodTrick10MemoryInterface(const JeodTrick10MemoryInterface &) = delete;
    JeodTrick10MemoryInterface & operator=(const JeodTrick10MemoryInterface &) = delete;

    // Register a JeodCheckpointable object with the simulation engine.
    void register_container(const void * owner,
                            const JeodMemoryTypeDescriptor & owner_type,
                            const std::string & elem_name,
                            JeodCheckpointable & container) override;

    // Revoke registration of a JeodCheckpointable that is about to be deleted.
    void deregister_container(const void * owner,
                              const JeodMemoryTypeDescriptor & owner_type,
                              const std::string & elem_name,
                              JeodCheckpointable & container) override;

    // Get the simulation name (if any) of the address.
    const std::string get_name_at_address(const void * addr, const JeodMemoryTypeDescriptor * tdesc) const override;

    // Get the address (if any) corresponding to the given name.
    void * get_address_at_name(const std::string & name) const override;

    /**
     * The Trick10 memory interface supports checkpoint/restart.
     */
    bool is_checkpoint_restart_supported() const override
    {
        return true;
    }

    // Get the name of the current Trick checkpoint file.
    const std::string get_trick_checkpoint_file(bool checkpoint) override;

    // Dump the container checkpointable objects to the checkpoint file.
    void checkpoint_containers() override;

    // Restore the container checkpointables objects from the checkpoint file.
    void restore_containers() override;

    // Dump the allocation information to the checkpoint file.
    void checkpoint_allocations() override;

    // Restore the allocated data per the checkpoint file.
    void restore_allocations(JeodMemoryManager & memory_manager) override;

protected:
    // Member functions

    std::string get_container_id(const ContainerListEntry & entry) const;

    std::string translate_addr_to_name(const void * addr, const ATTRIBUTES * attr) const;

    void * translate_name_to_addr(const std::string & spec) const;

    // Member data
    /**
     * Trick checkpoint agent.
     */
    Trick::ClassicCheckPointAgent * trick_checkpoint_agent; //!< trick_io(**)
};

} // namespace jeod

#endif
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
