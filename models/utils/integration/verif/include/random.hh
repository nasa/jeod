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
/*******************************************************************************

Purpose:
  (Define classes pertaining to random number generation using the
   POSIX rand48 family of pseudo-random number generators as a basis.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((../src/random.cc))


*******************************************************************************/

#ifndef JEOD_RANDOM_HH
#define JEOD_RANDOM_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class RandomSeedGenerator;

/*
Purpose:
  (Base class for generating random numbers.
   A Random by itself is not all that useful. It provides protected but not
   public methods for generating random numbers.
   Providing public methods is the job of derived classes.)
*/
class RandomBase
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RandomBase)

#ifdef JEOD_RANDOM_FRIEND
    friend class JEOD_RANDOM_FRIEND;
#endif

    // Member functions
public:
    // Default constructor.
    RandomBase(unsigned int nskip = 0, bool initialize = true);

    // Non-default constructor that uses a seed generator to set the seed.
    RandomBase(RandomSeedGenerator & seed_gen, unsigned int nskip = 0, bool initialize = true);

    // Destructor
    virtual ~RandomBase() = default;

    // Protected methods, for use by derived classes.
protected:
    // Reset the generator
    void reset();

    // Get a random long.
    long random_long();

    // Get a random number from [0,1)
    double random_u01_scalar();

    // Visible to the JEOD_RANDOM_FRIEND, for testing.
private:
    // Deferred initializer.
    void reseed(RandomSeedGenerator & seed_gen);

    // Setters; override defaults. Not recommended, but here they are.
    void set_xsubi_init(unsigned short new_xsubi[3]);
    void set_skip_count(unsigned int nskip);

    // Member data
    // Should be private but implemented as protected, #@$%it
protected:
    bool seeded{};
    unsigned int skip_count;
    unsigned int use_count{};
    unsigned short xsubi[3];
    unsigned short xsubi_init[3];
};

/*
Purpose:
  (Instances of this class reproducibly and reentrantly generate uniform random
   numbers between 0 and 1, inclusive of 0 but exclusive of 1.)
*/
class RandomSeedGenerator : public RandomBase
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RandomSeedGenerator)

#ifdef JEOD_RANDOM_FRIEND
    friend class JEOD_RANDOM_FRIEND;
#endif

    // Member functions
public:
    // Default constructor
    RandomSeedGenerator(unsigned int nskip = 0, bool initialize = true);

    // Destructor
    ~RandomSeedGenerator() override = default;

    // Get a seed for initializing some RandomBase derivative class. */
    void get_seed(unsigned short seed[3]);

    // Visible to the JEOD_RANDOM_FRIEND, for testing.
private:
    // Reset the generator
    void reset();
};

/*
Purpose:
  (Base class for generating random numbers.)
*/
class RandomGenerator : public RandomBase
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RandomGenerator)

#ifdef JEOD_RANDOM_FRIEND
    friend class JEOD_RANDOM_FRIEND;
#endif

    // Member functions
public:
    // Default constructor: There is none.

    // Non-default constructor. Note that the seed generator is required.
    RandomGenerator(RandomSeedGenerator & seed_gen, unsigned int nskip = 0, bool initialize = true);

    // Destructor
    ~RandomGenerator() override = default;
};

/*
Purpose:
  (Generates random n-vectors with each element indepdently drawn from U[0,1).)
*/
class RandomVectorUniform01 : public RandomGenerator
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RandomVectorUniform01)

#ifdef JEOD_RANDOM_FRIEND
    friend class JEOD_RANDOM_FRIEND;
#endif

    // Member functions
public:
    // Default constructor: There is none.

    // Non-default constructor; initializes immediately.
    RandomVectorUniform01(RandomSeedGenerator & seed_gen,
                          unsigned int vsize = 3,
                          unsigned int nskip = 0,
                          bool initialize = true);

    // Destructor
    ~RandomVectorUniform01() override = default;

    // Get a random vector, overridable.
    // This class returns a uniform vector with elements drawn from U[0,1).
    virtual void random_vector(double * vec);

    // Member data
protected:
    const unsigned int vector_size;
};

/*******************************************************************************
 * Other inlined methods; none of these have any library dependencies.
 ******************************************************************************/

/*
Purpose: (Override the default values for xsubi_init.)
*/
inline void RandomBase::set_xsubi_init(unsigned short new_xsubi[3]) // In: -- New seed
{
    xsubi_init[0] = new_xsubi[0];
    xsubi_init[1] = new_xsubi[1];
    xsubi_init[2] = new_xsubi[2];

    seeded = false;
}

/*
Purpose: (Reset the number of skips.)
*/
inline void RandomBase::set_skip_count(unsigned int nskip) // In: -- New skip count
{
    skip_count = nskip;

    seeded = false;
}

/*
Purpose: (Reseed the generator.)
*/
inline void RandomBase::reseed(RandomSeedGenerator & seed_gen)
{
    // Use the seed generator to initialize the seed.
    seed_gen.get_seed(xsubi_init);

    seeded = false;
}

/*
Purpose: (Get a random uniform n-vector, each element between 0 and 1.)
*/
inline void RandomVectorUniform01::random_vector(double * vec) // -- Out: Random vector from U[0,1)
{
    for(unsigned int ii = 0; ii < vector_size; ++ii)
    {
        vec[ii] = random_u01_scalar();
    }
}

} // namespace jeod

#endif
