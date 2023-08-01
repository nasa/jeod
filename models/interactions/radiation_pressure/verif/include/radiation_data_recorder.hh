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

/************************** TRICK HEADER***************************************
PURPOSE:
    (Data recording through facet pointers)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (That there are <= 10 facets in the surface.  If there are more, change
      the 10s in the data definition and recompile.)

Library dependencies:
    (../src/radiation_data_recorder.cc)


*******************************************************************************/

#ifndef JEOD_RADIATION_DATA_RECORDER_HH
#define JEOD_RADIATION_DATA_RECORDER_HH


#define JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS 10
// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "interactions/radiation_pressure/include/flat_plate_radiation_facet.hh"


//! Namespace jeod
namespace jeod {

class RadiationSurface;

class RadiationDataRecorder  {

   JEOD_MAKE_SIM_INTERFACES(RadiationDataRecorder)

public:
  double F_absorption[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double F_specular[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double F_diffuse[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double F_emission[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double force[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double torque[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS][3];
  double temperature[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS];
  double power_absorb[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS];
  double power_emit[JEOD_RADIATION_DATA_RECORDER_HH_NUM_FACETS];


  RadiationDataRecorder();
  virtual ~RadiationDataRecorder();

  void record_data (RadiationSurface * surf_ptr);


};

} // End JEOD namespace

#endif
