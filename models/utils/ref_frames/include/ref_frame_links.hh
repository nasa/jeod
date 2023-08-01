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
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_links.hh
 * Define the class RefFrameLinks, the class that encapsulates the links between
 * reference frames.
 *
 * MAINTENANCE NOTE --
 * This file is, by intent, very similar to dynamics/mass/mass_body_links.hh.
 * The version of Trick used at JEOD 2.0 beta release provided minimal support
 * for templates. These two files should eventually be merged through the use
 * of templates.
 */

/*******************************************************************************

Purpose:
 ()

 

*******************************************************************************/


#ifndef JEOD_REF_FRAME_LINKS_HH
#define JEOD_REF_FRAME_LINKS_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "ref_frame_messages.hh"
#include "tree_links.hh"


//! Namespace jeod
namespace jeod {

class RefFrameLinks;


/**
 * Encapsulates the links between reference frames.
 *
 * \par Assumptions and Limitations
 *  - Classes that use this class must keep the tree structure intact.
 */
class RefFrameLinks : public TreeLinks<RefFrameLinks,
                                       RefFrame,
                                       RefFrameMessages> {

   JEOD_MAKE_SIM_INTERFACES(RefFrameLinks)

 // Member functions
 public:


   /**
    * Non-default constructor.
    * @param container_in The RefFrame object that contains this object.
    */
   explicit RefFrameLinks (RefFrame & container_in)
   :
      TreeLinks<RefFrameLinks, RefFrame, RefFrameMessages> (
         container_in, default_path_size)
   { }

   /**
    * Destructor.
    */
   ~RefFrameLinks (void) override
   { }


 private:

   const static unsigned int default_path_size = 4; /* trick_units(--) @n
      The default length of a RefFrameLinks object's path_to_node_ array. */


   // The default, constructor, copy constructor and assignment operator for
   // this class are declared private and are not implemented.

   /**
    * Not implemented.
    */
   RefFrameLinks (void);

   /**
    * Not implemented.
    */
   RefFrameLinks (const RefFrameLinks&);

   /**
    * Not implemented.
    */
   void operator = (const RefFrameLinks&);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
