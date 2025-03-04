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
/*******************************************************************************

Purpose:
  (Define the class AngularVariance, which is used to compute the error between
   the true and integrated rotational state and to model the behavior of the
   rotational simple harmonic oscillator.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((../src/angular_variance.cc))



*******************************************************************************/

#ifndef JEOD_RANDOM_ORIENTATION_HH
#define JEOD_RANDOM_ORIENTATION_HH

// System includes
#include <cmath>
#include <cstdio>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "new_orientation.hh"
#include "random.hh"

//! Namespace jeod
namespace jeod
{

/*
Purpose:
  (Specify an orientation in terms of three numbers between 0 and 1.
   This specification makes it quite easy to generate orientations randomly.)
*/
class VectorOrientation : public NewOrientation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, VectorOrientation)

public:
    // Constructor
    VectorOrientation(bool init_from_vector = true);

    // Initializer
    void initialize() override;

    // Setters; no getters (yet)
    virtual void set_initialize_from_vector(bool value);
    virtual void set_vector(const double value[3]);
    virtual void set_vector(double value0, double value1, double value2);

protected:
    bool initialize_from_vector; /* trick_units(--) @n
       If set, the orientation will be generated from the vector orientation
       specification. If clear, the orientation will be set based on the
       base orientation specifications. */

    double vector[3]{}; /* trick_units(--) @n
       Orientation specification. All must be elements of [0,1). Elements are
        - 0: Specifies the rotation angle about the eigenaxis.
             The input value is converted to angle by multiplying by 2*pi.
        - 1: Specifies the rotation eigenaxis inclination.
             The input value is converted to angle via
               cosine(inclination) = 2*vector[1] - 1
        - 2: Specifies the rotation eigenaxis declination.
             The input value is converted to angle by multiplying by 2*pi.
       Note that (0,0,0) specifies the null rotation -- as does any setting with
       vector[0] equal to zero. */
};

inline void VectorOrientation::set_initialize_from_vector(bool value)
{
    initialize_from_vector = value;
}

inline void VectorOrientation::set_vector(const double value[3])
{
    vector[0] = value[0];
    vector[1] = value[1];
    vector[2] = value[2];
}

inline void VectorOrientation::set_vector(double value0, double value1, double value2)
{
    vector[0] = value0;
    vector[1] = value1;
    vector[2] = value2;
}

/*
Purpose:
  (Randomly generate the vector used to specify an orientation.)
*/
class RandomOrientation : public VectorOrientation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RandomOrientation)

public:
    // Constructor
    RandomOrientation(RandomSeedGenerator & seed_gen, bool random_orientation = false);

    // Initializer
    void initialize() override;

    // Set skip count (only used prior to initialize)
    void set_skip_count(unsigned int value);

    // Set skip count (only used prior to initialize)
    void set_generate_random_vector(bool value);

protected:
    bool generate_random_vector;
    unsigned int skip_count{};
    RandomVectorUniform01 rvec;
};

inline void RandomOrientation::set_skip_count(unsigned int value)
{
    skip_count = value;
}

inline void RandomOrientation::set_generate_random_vector(bool value)
{
    generate_random_vector = value;
}

} // namespace jeod

#endif
