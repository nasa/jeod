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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define the class Wrench.
 */

/*
Purpose: ()
*/


#ifndef JEOD_WRENCH_SCREW_THEORY_HH
#define JEOD_WRENCH_SCREW_THEORY_HH


#include "dynamics/mass/include/mass_point_state.hh"
#include "utils/math/include/vector3.hh"
#include "utils/sim_interface/include/jeod_class.hh"

#include <vector>


//! Namespace jeod 
namespace jeod {

/**
 * A wrench comprises a torque and a force applied at a point on a DynBody.
 * The torque should not include the torque due to the application of the
 * force.
 *
 * A Trick simulation issues vcollect statements such as
 *
 *      vcollect vehicle.dyn_body.collect_wrench.collection
 *      {
 *          wrench_model1.wrench,
 *          wrench_model2.wrench
 *      };
 */
class Wrench
{
    JEOD_MAKE_SIM_INTERFACES(Wrench)

public:

    // Member functions

    /**
     * Default constructor. The wrench is marked as active, and
     * the torque, force, and point vectors are all initialized to zero.
     * This constructor can also be used as a non-default constructor that
     * marks the wrench as inactive by calling it with one argument
     * (a boolean) whose value is false.
     * @param active_in  True (default) indicates the wrench is active.
     */
    explicit Wrench (
        bool active_in = true)
    :
        active(active_in)
    {
        Vector3::initialize (torque);
        Vector3::initialize (force);
        Vector3::initialize (point);
    }

    /**
     * Non-default constructor that sets all elements of the wrench.
     * @param torque_in    The intrinsic torque for this wrench.
     * @param force_in     The force applied at the point.
     * @param point_in     The point at which forces are applied.
     * @param active_in    True (default) indicates the wrench is active.
     */
    explicit Wrench (
        const double torque_in[3],
        const double force_in[3],
        const double point_in[3],
        bool active_in = true)
    :
        active(active_in)
    {
        Vector3::copy (torque_in, torque);
        Vector3::copy (force_in, force);
        Vector3::copy (point_in, point);
    }

    /**
     * Non-default constructor that sets the point and active flag.
     * The torque and force and initialized to zero.
     * @param point_in     The point at which forces are applied.
     * @param active_in    True (default) indicates the wrench is active.
     */
    explicit Wrench (
        const double point_in[3],
        bool active_in = true)
    :
        active(active_in)
    {
        Vector3::initialize (torque);
        Vector3::initialize (force);
        Vector3::copy (point_in, point);
    }


    /**
     * Destructor.
     */
    virtual ~Wrench () = default;

    /**
     * Copy constructor.
     */
    Wrench (const Wrench&) = default;

    /**
     * Copy assignment operator.
     */
    Wrench& operator= (const Wrench&) = default;

#ifndef SWIG
    /**
     * Move constructor.
     */
    Wrench (Wrench&&) = default;

    /**
     * Move assignment operator.
     */
    Wrench& operator= (Wrench&&) = default;
#endif


    /**
     * Increment this wrench by the other, but only if both are active.
     * The other wrench is effectively reseated to this wrench's point
     * prior to incrementing.
     * @param other  Wrench with which this wrench is to be incremented.
     * @return *this.
     */
    Wrench& operator+= (const Wrench& other)
    {
        if ((active) && (other.active))
        {
            double delta_pos[3];
            Vector3::diff (other.point, point, delta_pos);
            Vector3::incr (other.force, force);
            Vector3::incr (other.torque, torque);
            Vector3::cross_incr (delta_pos, other.force, torque);
        }

        return *this;
    }


    /**
     * Mark this wrench as active.
     */
    void activate ()
    {
        active = true;
    }


    /**
     * Mark this wrench as inactive.
     */
    void deactivate ()
    {
        active = false;
    }


    /**
     * Is this wrench active?
     */
    bool is_active () const
    {
        return active;
    }


    /**
     * Set the force and torque to zero. The point remains unaltered.
     */
    void reset_force_and_torque()
    {
        Vector3::initialize (torque);
        Vector3::initialize (force);
    }


    /**
     * Set the torque to zero. The force and point remain unaltered.
     */
    void reset_torque()
    {
        Vector3::initialize (torque);
    }


    /**
     * Set the force to zero. The torque and point remain unaltered.
     */
    void reset_force()
    {
        Vector3::initialize (force);
    }


    /**
     * Set the point to zero. The torque and force remain unaltered.
     */
    void reset_point()
    {
        Vector3::initialize (point);
    }


    /**
     * Set all vector elements of the wrench.
     * @param torque_in    The intrinsic torque for this wrench.
     * @param force_in     The force applied at the point.
     * @param point_in     The point at which forces are applied.
     */
    void set (
        const double torque_in[3],
        const double force_in[3],
        const double point_in[3])
    {
        Vector3::copy (torque_in, torque);
        Vector3::copy (force_in, force);
        Vector3::copy (point_in, point);
    }


    /**
     * Set the torque to the specified value.
     * The force and point of application remain unaltered.
     */
    void set_torque (const double torque_in[3])
    {
        Vector3::copy (torque_in, torque);
    }


    /**
     * Set the force to the specified value.
     * The torque and point of application remain unchanged.
     */
    void set_force (const double force_in[3])
    {
        Vector3::copy (force_in, force);
    }


    /**
     * Set the force and the point of application to the specified values.
     * The torque remain unchanged.
     */
    void set_force (
        const double force_in[3],
        const double point_in[3])
    {
        Vector3::copy (force_in, force);
        Vector3::copy (point_in, point);
    }


    /**
     * Set the point of application to the specified value.
     * The force and torque remain unchanged.
     */
    void set_point (
        const double point_in[3])
    {
        Vector3::copy (point_in, point);
    }


    /**
     * Scale the torque by the specified value.
     * The force and point of application remain unaltered.
     */
    void scale_torque (double scale)
    {
        Vector3::scale (scale, torque);
    }


    /**
     * Scale the force by the specified value.
     * The torque and point of application remain unchanged.
     */
    void scale_force (double scale)
    {
        Vector3::scale (scale, force);
    }


    /**
     * Const getter of the torque vector.
     */
    const double* get_torque() const
    {
        return torque;
    }


    /**
     * Const getter of the force vector.
     */
    const double* get_force() const
    {
        return force;
    }


    /**
     * Const getter of the point vector.
     */
    const double* get_point() const
    {
        return point;
    }


    /**
     * Accumulate the wrenches in the collection to form a combined wrench
     * about the current wrench point, which remains unchanged.
     * @param collection  The wrenches to be accumulated.
     */
    Wrench& accumulate (
        const std::vector<Wrench*>& collection)
    {
        reset_force_and_torque();
        for (const Wrench* wrench : collection)
        {
            *this += *wrench;
        }
        return *this;
    }


    /**
     * Accumulate the wrenches in the collection to form a combined wrench
     * about the specified wrench point.
     * @param collection  The wrenches to be accumulated.
     * @param new_point    The point about which the wrenches to be accumulated.
     */
    Wrench& accumulate (
        const std::vector<Wrench*>& collection,
        const double new_point[3])
    {
        set_point(new_point);
        return accumulate (collection);
    }


    /**
     * Construct an equivalent Wrench about the specified point.
     * @param new_point  The point about which this is to be represented.
     * @return Equivalent wrench about the specified point.
     */
    Wrench transform_to_point (
        const double new_point[3]) const
    {
        Wrench result{torque, force, new_point, active};
        double delta_pos[3];
        Vector3::diff (point, new_point, delta_pos);
        Vector3::cross_incr (delta_pos, force, result.torque);
        return result;
    }

    /**
     * Construct an equivalent Wrench about the current point, but
     * in a different reference frame.
     * @param point_state  Contains the position and orientation of the current
     *                    frame in the parent frame.
     * @return Equivalent wrench in the parent frame.
     */
    Wrench transform_to_parent (
        const MassPointState& point_state) const
    {
        Wrench result;
        Vector3::transform_transpose (point_state.T_parent_this, torque,
            result.torque);
        Vector3::transform_transpose (point_state.T_parent_this, force,
            result.force);
        Vector3::transform_transpose (point_state.T_parent_this, point,
            result.point);
        Vector3::incr (point_state.position, result.point);
        return result;
    }


private:

    // Member data

    /**
     * The torque exerted on the DynBody by the force/torque agent, expressed
     * in structural coordinates.
     *
     * This torque should not include the torque that results from the force
     * not passing through the center of mass. A typical thruster, for example,
     * should have the torque set to zero. On the other hand, a Hall effect
     * thruster will have a non-zero torque due to the swirling of the exhaust.
     */
    double torque[3]; //!< trick_units(N*m)

    /**
     * The force exerted on the DynBody by the force/torque agent, expressed
     * in structural coordinates.
     */
    double force[3]; //!< trick_units(N)

    /**
     * The structural coordinates of the point at which the force is applied.
     */
    double point[3]; //!< trick_units(m)

    /**
     * Indicated whether the wrench is active (true) or inactive (false).
     * inactive wrenches are not collected.
     */
    bool active; //!< trick_units(--)
};

} // End JEOD namespace


#endif


/**
 * @}
 * @}
 * @}
 */
