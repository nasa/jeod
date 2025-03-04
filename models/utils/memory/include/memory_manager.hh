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
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/include/memory_manager.hh
 * Define the JeodMemoryManager class, the central agent of the memory model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/memory_manager.cc)
   (../src/memory_manager_protected.cc)
   (../src/memory_manager_static.cc))


*******************************************************************************/

#ifndef JEOD_MEMORY_MANAGER_HH
#define JEOD_MEMORY_MANAGER_HH

// Swig gets utterly confused by too much of this.
// Besides, it has no reason to poke into the memory model.
// Just hide the whole shebang from swig.
#ifndef SWIG

/**
 * \addtogroup classes
 * @{
 */

// System includes
#include <cstddef>
#include <list>
#include <map>
#include <ostream>
#include <pthread.h>
#include <string>
#include <typeinfo>

// JEOD includes
#include "utils/container/include/checkpointable.hh"
#include "utils/sim_interface/include/config.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/sim_interface/include/memory_interface.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// Model includes
#include "memory_item.hh"
#include "memory_table.hh"
#include "memory_type.hh"

//! Namespace jeod
namespace jeod
{

/**
 * This class provides the interface between the macros in jeod_alloc.hh and
 * the rest of the JEOD memory model.
 * The public interface is via the publicly visible static methods.
 * All nonstatic member functions are private.
 * Each public static method relays the method call to the singleton memory
 * manager via a correspondingly named private member function.
 *
 * \par Singleton
 * The class is intended to be a singleton.
 * The private static member JeodMemoryManager::Master points to this
 * singular instance. The constructor sets that static member if it is null.
 * The constructor issues a non-fatal error when multiple instances of the
 * class are created.
 *
 * \par Thread Safety
 * This class contains objects that must be accessed and updated in a
 * thread-safe manner. The member data that must be used atomically are
 *  - JeodMemoryManager::alloc_table - Maps memory addresses to memory items
 *  - JeodMemoryManager::type_table - Maps RTTI names to type descriptors
 *  - JeodMemoryManager::string_table - Maps unique strings to themselves.
 *  - JeodMemoryManager::cur_data_size - Current size of allocated data.
 *  - JeodMemoryManager::max_data_size - Maximum of the above.
 *  - JeodMemoryManager::max_table_size - Maximum allocation table size.
 *  - JeodMemoryManager::allocation_number - Number of allocations made.
 * \par
 * To ensure the constraint is satisfied, access to the these elements is
 * protected by means of a mutex and is limited to a small number of methods.
 * A pair of methods, JeodMemoryManager::begin_atomic_block and
 * JeodMemoryManager::end_atomic_block systematize the use of the mutex.
 * The methods that operate on the protected data are
 *  - Constructor and destructor.@n
 *    The constructor operates on the protected data before it creates the
 *    mutex and marks the JeodMemoryManager object as usable. The destructor
 *    marks the object as unusable and destroys the mutex before operating
 *    on the protected data.
 *  - JeodMemoryManager::generate_shutdown_report, which is called by the
 *    destructor after it has destroyed the mutex.
 *  - Methods whose names end with _atomic. These methods use the
 *    begin_atomic_block / end_atomic_block paradigm to ensure that the
 *    operations are carried out atomically.
 *  - Methods whose names end with _nolock. These methods operate on protected
 *    data but do so without atomic protection. These methods are called
 *    only by _atomic methods from within their atomic protection block.
 *
 * \par Forbidden Word - Mutable
 * The data member JeodMemoryManager::mutex is mutable, a forbidden word
 * per the JEOD coding standards. The coding standards allow for waivers to
 * the standards if the exception is justified. This section provides the
 * explanation needed to enable the use of that word in this case.
 * \par
 * The @a mutable keyword tells the compiler to ignore modifications to
 * mutable elements in an otherwise @a const method. The @a mutex is mutable
 * because, athough its value does change with a successful lock, it is restored
 * to its prelock value with an unlock. A method that could otherwise qualify
 * as a const method can still be a const method by marking the mutex as
 * mutable. Mutexes are one of the well-accepted types of data that typically
 * marked as mutable.
 *
 * \par Assumptions and Constraints on the Simulation Developer
 * This class places restrictions on the simulation developer.
 *  - The simulation's MessageHandler object must be constructed prior
 *    to constructing the simulation's JeodMemoryManager object.
 *  - The simulation's MessageHandler object must not be destroyed prior
 *    to constructing the simulation's JeodMemoryManager object.
 *  - The simulation's JeodMemoryManager object must be constructed prior to
 *    invoking any of the JEOD_ALLOC_xxx macros in other models.
 *  - The simulation's JeodMemoryManager object must not be destroyed before
 *    other models release their allocated memory.
 * \par
 * The recommended solution is to create an instance of a compliant
 * SimulationInterface before creating any other models and to destroy
 * that SimulationInterface object after destroying all other models.
 * A simple way to achieve this in a Trick-07 simulation is to define a
 * Trick sim object that contains a TrickSimulationInterface element and
 * to place this sim object immediately after the sys sim object.
 *
 * \par Assumptions and Constraints on the Simulation Engine
 * This class makes certain assumptions of the behavior of the simulation
 * engine.
 *  - The simulation engine will not spawn threads that use the JEOD memory
 *    model to allocate memory until after the SimulationInterface object has
 *    been constructed.
 *  - The simulation engine will join all threads that use the JEOD memory
 *    model prior to destroying the SimulationInterface object.
 * \par
 * The Trick-07 and Trick-10 simulation engines satisfies these constraints.
 *
 * \par Assumptions and Constraints on the Simulation Developer
 * This class places certain limitations on the architecture of a JEOD-based
 * simulation.
 *  - The JeodMemoryManager destructor uses the simulation's message handler
 *    to report errors discovered during destruction and may eventually use
 *    the simulation's simulation engine memory interface to revoke the
 *    registration of memory allocated by JEOD that has not been freed.
 *    This in turn means that:
 *    -- The simulation's message handler and simulation engine memory
 *       interface must be destructed after destructing the memory manager.
 *    -- The destructors for those objects cannot use the memory manager.
 *  - The JEOD memory allocation and deallocation macros expand into calls
 *    to memory manager methods. The memory manager must be viable (post
 *    construction, pre destruction) for these calls to function properly.
 *    This in turn means that the memory manager must be constructed very
 *    early in the overall construction process and destructed very late in
 *    in the overall destruction process.
 *  - The supported solution to both of these issues is to use a compliant
 *    derived class of the JeodSimulationInterface class and to ensure that
 *    this composite object created early and destroyed late. In a Trick-07
 *    simulation, this can be accomplished simply by placing a declaration of
 *    an object of type JeodTrickSimInterface near the top of an S_define file.
 *    The recommended placement is just after the Trick system sim object.
 */
class JeodMemoryManager
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodMemoryManager)

public:
    JeodMemoryManager() = delete;
    explicit JeodMemoryManager(const JeodMemoryManager &) = delete;
    JeodMemoryManager & operator=(const JeodMemoryManager &) = delete;

    // Types

    /**
     * The memory manager as a whole and individual operations have a debug
     * level. The debug levels and the message handler must be set to a
     * sufficiently high level to enable and see the debugging output.
     */
    enum DebugLevel
    {
        Debug_off = 0,     ///< Debugging is off.
        Summary_only = 1,  ///< Summary information; Allocation data are not stored.
        Error_details = 2, ///< Allocation data stored and used with error messages.
        Full_details = 3   ///< Blow-by-blow accounting of all transactions.
    };

    /**
     * The type lookup by type name needs to know whether the provided name
     * is a typeid name or a demangled name.
     */
    enum NameType
    {
        Typeid_type_name = 0,   ///< Name is from a std::type_info.name()
        Demangled_type_name = 1 ///< Name is what people might use
    };

    /**
     * The type table is indexed by an integer and contains type descriptors.
     * This class bundles the two together.
     */
    struct TypeEntry
    {
        /**
         * Type table index number
         */
        uint32_t index; //!< trick_io(**)

        /**
         * Type descriptor
         */
        const JeodMemoryTypeDescriptor * tdesc; //!< trick_io(**)

        /**
         * Pair constructor.
         */
        TypeEntry(uint32_t num, const JeodMemoryTypeDescriptor * desc)
            : index(num),
              tdesc(desc)
        {
        }
    };

    // Static functions
    // The bulk of the external interface to the memory manager is through these
    // static member functions.

    // Register a class with the manager
    static const TypeEntry register_class(JeodMemoryTypePreDescriptor & tdesc);

    // Get the type descriptor by typeid
    static const JeodMemoryTypeDescriptor * get_type_descriptor(const std::type_info & typeid_info);

    // Get the type descriptor by name
    static const JeodMemoryTypeDescriptor * get_type_descriptor(NameType name_type, const std::string & name);

    // Allocate and register memory for use with placement new
    static void * create_memory(
        bool is_array, unsigned int nelems, int fill, const TypeEntry & tentry, const char * file, unsigned int line);

    // Query whether memory was allocated by JEOD.
    static bool is_allocated(const void * addr, const char * file, unsigned int line);

    // Common standard new / placement new deletion method.
    static void destroy_memory(void * addr, bool delete_array, const char * file, unsigned int line);

    // Add a checkpointable object to the list of such.
    static void register_container(const void * container,
                                   const std::type_info & container_type,
                                   const std::string & elem_name,
                                   JeodCheckpointable & checkpointable);

    // Remove a checkpointable object from the list of such.
    static void deregister_container(const void * container,
                                     const std::type_info & container_type,
                                     const std::string & elem_name,
                                     JeodCheckpointable & checkpointable);

    // Set the mode. This should only be called from the sim interface.
    static void set_mode(JeodSimulationInterface::Mode new_mode);

    // Set the debug level.
    static void set_debug_level(unsigned int level);
    static void set_debug_level(DebugLevel level);

    // Enable/disable guard words
    static void set_guard_enabled(bool value);

    // Testing interfaces

    // Query whether all allocated memory has been freed.
    static bool is_table_empty();

    // Member functions

    // Note: The default constructor, copy constructor, and assignment operator
    // are deleted. This class is a singleton constructed solely by a
    // non-default constructor.

    // Constructor (non-default) and destructor.
    explicit JeodMemoryManager(JeodMemoryInterface &);
    virtual ~JeodMemoryManager();

    // The next two members are public for the sake of the simulation interface.
    // These are mondo dangerous. The sim interface knows what it is doing.
    // Everyone else: You do not know what you are doing. Do not call these.
    // Never! Never ever ever ever ever ever ever call these functions! Never!

    // Wipe out allocated memory. Every single bit of it.
    void restart_clear_memory();

    // Restore an allocation as recorded in a checkpoint file.
    void restart_reallocate(const std::string & mangled_type_name,
                            uint32_t unique_id,
                            uint32_t nelements,
                            bool is_array);

private:
    // Types

    /**
     * An AllocTable maps memory addresses to memory descriptions.
     */
    using AllocTable = std::map<const void *, JeodMemoryItem>;

    /**
     * The type type itself is a memory table with copy implemented by clone().
     */
    using TypeTable = JeodMemoryTableClonable<JeodMemoryTypeDescriptor>;

    // Static functions

    // Check for a non-null Master memory manager.
    static bool check_master(bool error_is_fatal, int line);

    // Static data

    /**
     * The singleton instance of the JeodMemoryManager class.
     * The constructor sets this pointer.
     */
    static JeodMemoryManager * Master; //!< trick_io(*o) trick_units(--)

    // Member functions

    // Methods called by the public interfaces

    // Generate a shutdown report.
    void generate_shutdown_report();

    // Create and register memory.
    void * create_memory_internal(
        bool is_array, unsigned int nelems, int fill, const TypeEntry & tentry, const char * file, unsigned int line);

    // Register allocated memory.
    void register_memory_internal(const void * addr,
                                  uint32_t unique_id,
                                  bool placement_new,
                                  bool is_array,
                                  unsigned int nelems,
                                  const TypeEntry & tentry,
                                  const char * file,
                                  unsigned int line);

    // Query whether memory was allocated by JEOD.
    bool is_allocated_internal(const void * addr, const char * file, unsigned int line);

    // Common standard new / placement new deletion method.
    void destroy_memory_internal(void * addr, bool delete_array, const char * file, unsigned int line);

    // Set the mode.
    void set_mode_internal(JeodSimulationInterface::Mode new_mode);

    // Even more private methods

    // Atomic operations

    // Prepare for a set of atomic operations. See usage.
    void begin_atomic_block() const;

    // End a set of atomic operations.
    void end_atomic_block(bool ignore_errors) const;

    // type_table accessors

    // Get the entry for a type given a type descriptor.
    const TypeEntry get_type_entry_atomic(JeodMemoryTypePreDescriptor & tdesc);

    // Get the entry for a type given a mangled or demangled name.
    const TypeEntry get_type_entry_atomic(NameType name_type, const std::string & type_name) const;

    // Retrieve the index for the specified type from the type table.
    bool get_type_index_nolock(const JeodMemoryTypeDescriptor & tdesc, uint32_t * idx);

    // Get the descriptor for a type given its typeid.
    const JeodMemoryTypeDescriptor * get_type_descriptor_atomic(const std::type_info & typeid_info) const;

    // Get the descriptor for a type given its index.
    const JeodMemoryTypeDescriptor & get_type_descriptor_atomic(unsigned int idx) const;

    // Get the descriptor for a type.
    const JeodMemoryTypeDescriptor & get_type_descriptor_nolock(const JeodMemoryItem & item) const;

    // string_table accessors

    // Retrieve a string from the string table.
    const std::string & get_string_atomic(unsigned int idx) const;

    // Add a string to the string table.
    unsigned int add_string_atomic(const std::string & str);

    // alloc_table accessors

    // Create a unique identifier for an allocation
    uint32_t get_alloc_id_atomic(const char * file, unsigned int line);

    // Reset the allocation id for restart
    void reset_alloc_id_atomic(uint32_t unique_id);

    // Find and maybe delete an entry from the table
    void find_alloc_entry_atomic(const void * addr,
                                 bool delete_entry,
                                 const char * file,
                                 unsigned int line,
                                 void *& found_addr,
                                 JeodMemoryItem & found_item,
                                 const JeodMemoryTypeDescriptor *& found_type);

    // Record allocation of memory.
    void add_allocation_atomic(const void * addr,
                               const JeodMemoryItem & item,
                               const JeodMemoryTypeDescriptor & tdesc,
                               const char * file,
                               unsigned int line);

    // Delete the oldest entry in the table.
    void delete_oldest_alloc_entry_atomic(void *& addr, JeodMemoryItem & item, const JeodMemoryTypeDescriptor *& type);

    // Memory allocation/deallocation

    // Low-level allocation method.
    void * allocate_memory(std::size_t nelems, std::size_t elem_size, bool guard, int fill) const;

    // Low level deallocation method.
    void free_memory(void * addr,
                     std::size_t length,
                     bool guard,
                     unsigned int alloc_idx,
                     const char * file,
                     unsigned int line) const;

    // Member data

    /**
     * The interface to the simulation engine's memory manager.
     */
    JeodMemoryInterface & sim_interface; //!< trick_io(*o) trick_units(--)

    /**
     * Debugging level.
     * - 0 = Minimal output, errors only.
     * - 1 = Summary report, generated just before exit(0).
     * - 2 = Report unfreed memory as well.
     * - 3 = Blow-by-blow report of each allocation and deallocation.
     */
    DebugLevel debug_level{Error_details}; //!< trick_units(--)

    /**
     * Number of allocated user bytes (excludes management overhead).
     */
    JEOD_SIZE_T cur_data_size{}; //!< trick_io(*o) trick_units(--)

    /**
     * Maximum value attained by cur_data_size.
     */
    JEOD_SIZE_T max_data_size{}; //!< trick_io(*o) trick_units(--)

    /**
     * Maximum value attained by alloc_table.size().
     */
    unsigned int max_table_size{}; //!< trick_io(*o) trick_units(--)

    /**
     * Number of allocations.
     * This always increments and can be adjusted upward on restarts.
     */
    unsigned int allocation_number{}; //!< trick_io(*o) trick_units(--)

    // Several of the remaining are hidden from Trick.
    // The memory model is not Trick-checkpointable.
    // It bootstraps itself instead.

    /**
     * Maps memory addresses to the descriptions of those addresses.
     */
    AllocTable alloc_table; //!< trick_io(**)

    /**
     * Maps typeid names to type descriptors.
     */
    TypeTable type_table; //!< trick_io(**)

    /**
     * Maps unique strings to themselves.
     */
    JeodMemoryReflectiveTable string_table; //!< trick_io(**)

    /**
     * Mutex that synchronizes access to the tables.
     */
    mutable pthread_mutex_t mutex{}; //!< trick_io(**)

    /**
     * Simulation interface mode.
     */
    JeodSimulationInterface::Mode mode{JeodSimulationInterface::Construction}; //!< trick_units(--)

    /**
     * Data can be guarded if this is set.
     * If not set, guards will never be established.
     */
    bool guard_enabled{true}; //!< trick_units(--)
};

/**
 * Retrieve the descriptor for the specified type from the type table.
 *
 * \par Assumptions and Limitations
 *  - The type is in the table. A core dump will result if it is not.
 *  - Operations on the type table must be atomic.
 *     This method *does not* satisfy that requirement.
 * @return Type descriptor
 * \param[in] item Memory descriptor
 */
inline const JeodMemoryTypeDescriptor & JeodMemoryManager::get_type_descriptor_nolock(const JeodMemoryItem & item) const
{
    return *(type_table.get(item.get_descriptor_index()));
}

} // namespace jeod

/**
 * @}
 */

#endif // End of #ifndef SWIG: Needed to keep SWIG processing sane.

#endif

/**
 * @}
 * @}
 * @}
 */
