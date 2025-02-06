//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/subscription.hh
 * Define the class Subscription.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((subscription.o))

 

*******************************************************************************/


#ifndef JEOD_SUBSCRIPTION_HH
#define JEOD_SUBSCRIPTION_HH


// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * A class that inherits from the ActivateInterface class must provide
 * activate and deactivate methods.
 */
class ActivateInterface {

 // Data types

 // Member functions
 public:

      /**
    * Default constructor.
    */
   ActivateInterface() {}

      /**
    * Destructor.
    */
   virtual ~ActivateInterface() {}


      /**
    * Mark the object as active.
    */
   virtual void activate (void) = 0;

      /**
    * Mark the object as inactive.
    */
   virtual void deactivate (void) = 0;

};


/**
 * A class that inherits from the SubscribeInterface class must provide
 * subscribe and unsubscribe methods.
 */
class SubscribeInterface {

 // Data types

 // Member functions
 public:

      /**
    * Default constructor.
    */
   SubscribeInterface() {}

      /**
    * Destructor.
    */
   virtual ~SubscribeInterface() {}


      /**
    * Add a subscription to the object.
    */
   virtual void subscribe (void) = 0;

      /**
    * Remove a subscription from the object.
    */
   virtual void desubscribe (void) = 0;

};


/**
 * A Subscription object provides two approaches of marking something as
 * being active or inactive: The activate and deactivate methods versus the
 * subscribe and unsubscribe methods. The class also provides a mean for
 * selecting only one of these two approaches as valid.
 *
 * This class uses the non-virtual interface design pattern. Derived classes
 * should not override the non-virtual public interfaces. They should instead
 * override the private set_active_state method.
 */
class Subscription {

 JEOD_MAKE_SIM_INTERFACES(Subscription)

 // Data types
 public:

    /**
     * The Subscription::Mode enum specifies the mode in which a Subscription
     * object is operating.
     */
    enum Mode {
       Detect    = 0,  ///< First scheme used wins.
       Subscribe = 1,  ///< Activation is via subscribe/unsubscribe only.
       Activate  = 2,  ///< Activation is via activate/deactivate only.
       Freeform  = 3   ///< Users can use either scheme; conflicts may arise.
    };


 // Member functions
 public:

   // Constructors and destructor

   // The default constructor sets the mode to Detect.
   Subscription();

   // The non-default constructor sets the mode as indicated.
   explicit Subscription(Mode);

   // Destructor. Note that there are no resources to destruct.
   virtual ~Subscription();


   // Note well: The public interfaces are not virtual methods.
   // These should not be be overridden.


   // Getters

   // is_active: Is the object active?
   bool is_active (void) const;

   // subscriptions: Return the subscription count
   unsigned int subscriptions (void) const;

   // get_subscription_mode: Return the object's mode.
   Mode get_subscription_mode (void) const;


   // Setters

   // activate: Mark the object as active
   void activate (void);

   // deactivate: Mark the object as inactive
   void deactivate (void);


   // subscribe: Increment the subscription count and activate
   void subscribe (void);

   // unsubscribe: Decrement the subscription count and deactivate when zero
   void unsubscribe (void);


 protected:

   // set_subscription_mode: Set the subscription mode.
   virtual void set_subscription_mode (Mode value);

  // set_active_status: This is the low-level method that derived classes can
  // override. The default implementation simply sets the value of the
  // active flag. Compliant derived classes must forward a call to this
  // method to either the parent class or to this class.
  virtual void set_active_status (bool value);


 // Member data
 // Note well: These data are protected; they should but cannot be private.
 // Derived classes should use the getters to examine these data members.
 // Simulation users should not set these members in normal simulation input
 // files.
 // They are marked as visible to Trick for the purpose of checkpoint/restart.
 protected:

   /**
    * The mode in which the object is operating.
    */
   Mode mode; //!< trick_units(--)

   /**
    * Number of subscribers for this object.
    */
   unsigned int subscribers; //!< trick_units(--)

   /**
    * Flag indicating whether the object is active.
    */
   bool active; //!< trick_units(--)

};


/**
 * Subscription class default constructor.
 */
inline
Subscription::Subscription (
   void)
:
   mode(Detect),
   subscribers(0),
   active(false)
{
   // Empty
}


/**
 * Subscription class non-default constructor.
 * \param[in] init_mode Initial mode
 */
inline
Subscription::Subscription (
   Mode init_mode)
:
   mode(init_mode),
   subscribers(0),
   active(false)
{
   // Empty
}


/**
 * Subscription class destructor. There are no resources to destruct.
 */
inline
Subscription::~Subscription (
   void)
{
   // Empty
}


/**
 * Return the value of the active data member.
 * @return Is the object active?
 */
inline bool
Subscription::is_active (
   void)
const
{
   return active;
}



/**
 * Return the value of the subscribers data member.
 * @return Number of subscriptions.
 */
inline unsigned int
Subscription::subscriptions (
   void)
const
{
   return subscribers;
}


/**
 * Set the value of the mode data member.
 * \param[in] value Subscription mode
 */
inline void
Subscription::set_subscription_mode (
   Mode value)
{
   mode = value;
}


/**
 * Return the value of the mode data member.
 * @return Operating mode.
 */
inline Subscription::Mode
Subscription::get_subscription_mode (
   void)
const
{
   return mode;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
