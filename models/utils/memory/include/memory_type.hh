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
 * @file models/utils/memory/include/memory_type.hh
 * Define the abstract class JeodMemoryTypeDescriptor and templates that create
 * instantiable classes that derive from JeodMemoryTypeDescriptor.
 */

/*******************************************************************************

Purpose:
  ()


*******************************************************************************/

#ifndef JEOD_MEMORY_TYPE_HH
#define JEOD_MEMORY_TYPE_HH

/**
 * @addtogroup classes
 * @{
 */

// Model includes
#include "jeod_alloc_construct_destruct.hh"
#include "jeod_alloc_get_allocated_pointer.hh"
#include "memory_attributes_templates.hh"
#include "memory_item.hh"
#include "memory_messages.hh"

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/sim_interface/include/memory_attributes.hh"

// System includes
#include <cstddef>
#include <cstring>
#include <new>
#include <string>
#include <type_traits>
#include <typeinfo>

//! Namespace jeod
namespace jeod
{

/**
 * Abstract class for managing data allocated as some specific type.
 *
 * A JeodMemoryTypeDescriptor is a clonable object that contains the name and
 * size of a specific data type. Instantiable subclasses of this class are
 * created by the class templates that derive from this base class.
 */
class JeodMemoryTypeDescriptor
{
public:
    // Static member functions

    /**
     * Enable/disable registration error messages
     * @param[in] val New value for check_for_registration_errors
     */
    static void set_check_for_registration_errors(bool val)
    {
        check_for_registration_errors = val;
    }

    // Constructors and destructor.

    // Note well: There is no default constructor for this class.
    // There is a non-default constructor, the typical mechanism for creating
    // a new instance, and a copy constructor, which is used for cloning.
    // There is no assignment operator by design.
    // The assignment operator doesn't make any more sense than a default cotr.
    // The assignment operator is declared private and is not defined.

    // Non-default constructor.
    JeodMemoryTypeDescriptor(const std::type_info & obj_typeid,
                             const JEOD_ATTRIBUTES_TYPE & type_attr,
                             std::size_t type_size,
                             bool is_exportable = true);

    // The default destructor would suffice were it not for the vtable and
    // Trick's library dependency capability.
    // Implementing this in a predictable place places the vtable in that same
    // predictable place, and that in turn keeps Trick happy.
    virtual ~JeodMemoryTypeDescriptor() = default;
    JeodMemoryTypeDescriptor(const JeodMemoryTypeDescriptor &) = default;
    JeodMemoryTypeDescriptor & operator=(const JeodMemoryTypeDescriptor &) = delete;

    // Accessors. Note that there are no setters; (most) member data are const.

    /**
     * Get the type info for the type.
     * @return Type info
     */
    const std::type_info & get_typeid() const
    {
        return obj_id;
    }

    /**
     * Get the name of the type.
     * @return Type name
     */
    const std::string & get_name() const
    {
        return name;
    }

    /**
     * Get the size of the type.
     * @return Type size
     */
    std::size_t get_size() const
    {
        return size;
    }

    /**
     * Get the simulation engine attributes for the type.
     * @return Type attributes
     */
    const JEOD_ATTRIBUTES_TYPE & get_attr() const
    {
        return attr;
    }

    /**
     * Get the simulation engine attributes for the type.
     * @return Type attributes
     */
    bool get_register_instances() const
    {
        return register_instances;
    }

    // Computational methods.

    /**
     * Determine the dimensionality of the type.
     * @return: Type dimensionality
     */
    std::size_t dimensionality() const
    {
        return pointer_dimension(name);
    }

    /**
     * Compute the size of a buffer.
     * @param[in] nelems Size of the array
     * @return: Buffer size
     */
    std::size_t buffer_size(unsigned int nelems) const
    {
        return nelems * size;
    }

    /**
     * Compute the size of a buffer.
     * @param[in] item Buffer descriptor
     * @return: Buffer size
     */
    std::size_t buffer_size(const JeodMemoryItem & item) const
    {
        return buffer_size(item.get_nelems());
    }

    /**
     * Compute the address of the byte just past the end a buffer.
     * @param[in] addr   Start of buffer
     * @param[in] nelems Size of the array
     */
    const void * buffer_end(const void * addr, unsigned int nelems) const
    {
        return reinterpret_cast<const void *>(reinterpret_cast<const char *>(addr) + buffer_size(nelems));
    }

    /**
     * Compute the address of the byte just past the end a buffer.
     * @param[in] addr Start of buffer
     * @param[in] item Buffer descriptor
     */
    const void * buffer_end(const void * addr, const JeodMemoryItem & item) const
    {
        return buffer_end(addr, item.get_nelems());
    }

    // Construct a type specification string.
    const std::string type_spec(const JeodMemoryItem & item) const;

    /**
     * Destroy memory.
     * @param[in] placement_new  Constructed with placement new?
     * @param[in] is_array       Allocated as an array?
     * @param[in] nelem          Number of elements
     * @param[in,out] addr       Address to destroy
     */
    void destroy_memory(bool placement_new, bool is_array, unsigned int nelem, void * addr) const
    {
        if(placement_new)
        {
            destruct_array(nelem, addr);
        }
        else if(is_array)
        {
            delete_array(addr);
        }
        else
        {
            delete_object(addr);
        }
    }

    // Public pure virtual interfaces.

    /**
     * Create a copy of the descriptor.
     * @return Copy.
     */
    virtual JeodMemoryTypeDescriptor * clone() const = 0;

    /**
     * Indicate whether the type associated with the descriptor is
     * a structured (non-primitive, non-pointer) type.
     */
    virtual bool is_structured() const = 0;

    /**
     * Construct an array of objects of the type.
     * The default implementation does nothing, which is the right thing
     * to do for primitive types, pointers, and abstract classes.
     */
    virtual void * construct_array(std::size_t nelem, void * addr) const = 0;

    /**
     * Find the most-derived object corresponding to the input pointer.
     */
    virtual const void * most_derived_pointer(const void * addr) const = 0;

    /**
     * Find the most-derived object corresponding to the input pointer.
     */
    virtual void * most_derived_pointer(void * addr) const = 0;

protected:
    // Static member functions.

    // Standardize the type name
    static std::string initialize_type_name(const std::string & type_name);

    // Get the dimensionality (number of asterisks) of a type
    static std::size_t pointer_dimension(const std::string & demangled_name);

    // Get the dimensionality (number of asterisks) of a type
    static const JeodMemoryTypeDescriptor * base_type(const std::string & demangled_name);

    // Internal pure virtual interfaces.

    /**
     * Delete an array of instances of the type associated with the descriptor.
     * In other words, delete[] addr.
     * @param[in,out] addr Address to be deleted
     */
    virtual void delete_array(void * addr) const = 0;

    /**
     * Delete a single instance of the type associated with the descriptor.
     * In other words, delete addr.
     * @param[in,out] addr Address to be deleted
     */
    virtual void delete_object(void * addr) const = 0;

    /**
     * Destruct (but do not delete) an array of @a nelem instances of
     * the type associated with the descriptor.
     * @param[in] nelem    Number of elements in addr
     * @param[in,out] addr Address to be destructed
     */
    virtual void destruct_array(std::size_t nelem, void * addr) const = 0;

    // Static data
    /**
     * When set, suspect memory interface results will be reported
     * as a warnings. No messages are issued when this flag is clear.
     */
    static bool check_for_registration_errors; //!< trick_units(--)

    // Member data

    /**
     * The RTTI descriptor of the type.
     */
    const std::type_info & obj_id; //!< trick_io(**)

    /**
     * The name of the type in code.
     */
    const std::string name; //!< trick_io(**)

    /**
     * The simulation engine attributes that describe the type.
     */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    const JEOD_ATTRIBUTES_TYPE attr{}; //!< trick_io(**)

    /**
     * The size of an instance of the type.
     */
    const std::size_t size{}; //!< trick_io(**)

    /**
     * Should instances be registered with the simulation engine?
     * If true (default value), instances of the type will be registered with the
     * simulation engine; the simulation engine is responsible for checkpointing
     * and restoring the contents of such instances.
     *
     * If false, instances will not be registered with the simulation engine;
     * the simulation engine is not responsible for checkpointing/restarting
     * such instances.
     */
    bool register_instances{}; //!< trick_io(**)
};

/**
 * Extends JeodMemoryTypeDescriptor to describe a specific type.
 * tparam Type The type to be described.
 */
template<typename Type> class JeodMemoryTypeDescriptorDerived : public JeodMemoryTypeDescriptor
{
public:
    // Typedefs

    /**
     * This class.
     */
    using TypeDescriptor = JeodMemoryTypeDescriptorDerived<Type>;

    /**
     * Attributes for the Type.
     */
    using Attributes = JeodSimEngineAttributes<Type, std::is_class<Type>::value>;

    // Constructors and destructor.

    /**
     * Default constructor.
     * Invoke the parent class non-default constructor with
     * type, attributes, and size information.
     */
    JeodMemoryTypeDescriptorDerived(bool is_exportable = true)
        : JeodMemoryTypeDescriptor(typeid(Type), Attributes::attributes(is_exportable), sizeof(Type), is_exportable)
    {
    }

    /**
     * Copy constructor; pass-through to the parent class equivalent.
     * @param[in] src  Item to be copied
     */
    JeodMemoryTypeDescriptorDerived(const JeodMemoryTypeDescriptorDerived & src)
        : JeodMemoryTypeDescriptor(src)
    {
    }

    ~JeodMemoryTypeDescriptorDerived() override = default;
    JeodMemoryTypeDescriptorDerived & operator=(const JeodMemoryTypeDescriptorDerived &) = delete;

    // Virtual interfaces, defined.

public:
    /**
     * Create a copy of the descriptor.
     * @return Copy.
     */
    JeodMemoryTypeDescriptor * clone() const override
    {
        auto * dup = new JeodMemoryTypeDescriptorDerived(*this);
        return static_cast<JeodMemoryTypeDescriptor *>(dup);
    }

    /**
     * Indicate whether the type associated with the descriptor is
     * a structured (non-primitive, non-pointer) type.
     */
    bool is_structured() const override
    {
        return std::is_class<Type>::value;
    }

    /**
     * Construct an array of objects of the type.
     */
    void * construct_array(std::size_t nelem, void * addr) const override
    {
        return jeod_alloc_construct_array<Type>(nelem, addr);
    }

    /**
     * Find the most-derived object corresponding to the input pointer.
     * @param[in] addr Pointer to be examined
     * @return Pointer to most-derived object.
     */
    const void * most_derived_pointer(const void * addr) const override
    {
        return most_derived_pointer(const_cast<void *>(addr));
    }

    /**
     * Find the most-derived object corresponding to the input pointer.
     * @param[in] addr Pointer to be examined
     * @return Pointer to most-derived object.
     */
    void * most_derived_pointer(void * addr) const override
    {
        auto * ptr = static_cast<Type *>(addr);
        return jeod_alloc_get_allocated_pointer(ptr);
    }

protected:
    /**
     * Delete an array of instances of type @a Type.
     * In other words, delete[] addr.
     * @param[in,out] addr Address to be deleted
     */
    void delete_array(void * addr) const override
    {
        auto * array = reinterpret_cast<Type *>(addr);
        delete[] array;
    }

    /**
     * Delete a single instance of type @a Type.
     * In other words, delete addr.
     * @param[in,out] addr Address to be deleted
     */
    void delete_object(void * addr) const override
    {
        auto * object = reinterpret_cast<Type *>(addr);
        delete object;
    }

    /**
     * Destroy an array of @a nelem instances of type @a Type.
     */
    void destruct_array(std::size_t nelem, void * addr) const override
    {
        jeod_alloc_destruct_array<Type>(nelem, addr);
    }
};

/**
 * Abstract class for describing a type without necessarily needing
 * to create a JeodMemoryTypeDescriptor of that type. The intent is to
 * avoid creating a type descriptor for a type if the type is already
 * represented in the type table.
 *
 * Usage of a JeodMemoryTypePreDescriptor is highly constrained.
 * There are two simple rules:
 *  - Never cache a pointer or reference to a JeodMemoryTypeDescriptor
 *    in long-term memory.
 *  - Never cache a pointer or reference to a JeodMemoryTypeDescriptor obtained
 *    by calling the JeodMemoryTypeDescriptor's get_descriptor method.
 */
class JeodMemoryTypePreDescriptor
{
public:
    virtual ~JeodMemoryTypePreDescriptor() = default;

    /**
     * Get the type info for the type.
     * @return Type info
     */
    virtual const std::type_info & get_typeid() const = 0;

    /**
     * Get a type descriptor for the type.
     * The returned value should not be cached in a permanent store.
     * The reference has a lifespan limited to that of the
     * JeodMemoryTypePreDescriptor object.
     * @return Type descriptor.
     */
    virtual const JeodMemoryTypeDescriptor & get_descriptor() = 0;
};

/**
 * A JeodMemoryTypePreDescriptorDerived describes a @a Type.
 */
template<typename Type> class JeodMemoryTypePreDescriptorDerived : public JeodMemoryTypePreDescriptor
{
public:
    // Typedefs

    /**
     * The type descriptor this class describes.
     */
    using TypeDescriptor = JeodMemoryTypeDescriptorDerived<Type>;

    /**
     * Default constructor.
     */
    explicit JeodMemoryTypePreDescriptorDerived(bool exportable = true)
        : is_exportable(exportable)
    {
    }

    /**
     * Copy constructor.
     */
    JeodMemoryTypePreDescriptorDerived(const JeodMemoryTypePreDescriptorDerived & src)
        : is_exportable(src.is_exportable)
    {
        if(src.descriptor)
        {
            descriptor = new TypeDescriptor(*src.descriptor);
        }
    }

    /**
     * Destructor.
     */
    ~JeodMemoryTypePreDescriptorDerived() override
    {
        delete descriptor;
    }

    /**
     * Get a reference to this object.
     *
     * This is an utter hack. Because the descriptor is created after the fact,
     * a function that receives a JeodMemoryTypePreDescriptor must either take
     * a copy or a non-const reference as input. A reference is preferred.
     * The problem: Non-const references cannot be bound to rvalues. They can
     * however be bound to other references, and hence this method.
     *
     * Note well: The returned reference has a lifespan limited to that of this
     * object. Use with great care. This is not intended for general consumption.
     *
     * @return Reference to this object.
     */
    JeodMemoryTypePreDescriptor & get_ref()
    {
        return *this;
    }

    /**
     * Get the type info for the type.
     * @return Type info
     */
    const std::type_info & get_typeid() const override
    {
        return typeid(Type);
    }

    /**
     * Get a type descriptor for the type.
     *
     * Note well: The referenced value has a lifespan limited to that of this
     * object. The returned value must not be cached in a permanent store.
     * Use new in conjunction with the copy constructor instead.
     *
     * @return Type descriptor.
     */
    const JeodMemoryTypeDescriptor & get_descriptor() override
    {
        if(!descriptor)
        {
            descriptor = new TypeDescriptor(is_exportable);
        }
        return *descriptor;
    }

private:
    TypeDescriptor * descriptor{}; // trick_io(**)
    bool is_exportable{true};      // trick_io(**)
};

} // namespace jeod

/**
 * @}
 */

#endif

/**
 * @}
 * @}
 * @}
 */
