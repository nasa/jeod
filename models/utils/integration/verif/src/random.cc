/*******************************************************************************

Purpose:
  (Implement some random classes.)

Library dependencies:
  ((random.o))

 

*******************************************************************************/


// System includes
#include <cstdlib>

// JEOD includes
#include "../include/random.hh"


// Numbers pulled out of a hat.
#define RANDOM_SEED_0 26550
#define RANDOM_SEED_1 47345
#define RANDOM_SEED_2 29358
#define RANDOM_SEED_3 51636


//! Namespace jeod
namespace jeod {

/*
Purpose: (Constructor, with seed set from numbers pulled out of a hat.)
*/
RandomBase::RandomBase (
   unsigned int nskip,             // In:    -- Skip count
   bool initialize)                // In:    -- Complete initialization?
:
   seeded (false),
   skip_count (nskip),
   use_count (0)
{
   // Set the seed with numbers pulled out of a hat.
   xsubi_init[0] = RANDOM_SEED_0;
   xsubi_init[1] = RANDOM_SEED_1;
   xsubi_init[2] = RANDOM_SEED_2;

   // Set the current state to garbage (but not 0xdf!)
   xsubi[0] = xsubi[1] = xsubi[2] = 0xfdfdU;

   // Complete initialization if requested.
   if (initialize) {
      reset ();
   }
}


/*
Purpose: (Constructor, with seed set by a seed generator.)
*/
RandomBase::RandomBase (
   RandomSeedGenerator & seed_gen, // Inout: -- Random seed generator
   unsigned int nskip,             // In:    -- Skip count
   bool initialize)                // In:    -- Complete initialization?
:
   seeded (false),
   skip_count (nskip),
   use_count (0)
{
   // Use the seed generator to initialize the seed.
   seed_gen.get_seed (xsubi_init);

   // Set the current state to garbage (but not 0xdf!)
   xsubi[0] = xsubi[1] = xsubi[2] = 0xfdfdU;

   // Complete initialization if requested.
   if (initialize) {
      reset ();
   }
}


/*
Purpose: (Set/reset a RandomBase to its initial state.)
*/
void
RandomBase::reset (
   void)
{

   xsubi[0] = xsubi_init[0];
   xsubi[1] = xsubi_init[1];
   xsubi[2] = xsubi_init[2];

   for (unsigned int ii = 0; ii < skip_count; ++ii) {
      (void) jrand48 (xsubi); // flawfinder: ignore
   }

   use_count = skip_count;
   seeded = true;
}


/*
Purpose: (Return a random integer between -2**31 and 2**31-1, inclusive.)
*/
long
RandomBase::random_long (       // Return: -- Random integer
   void)
{
   if (! seeded) {
      reset ();
   }
   use_count++;
   return jrand48 (xsubi); // flawfinder: ignore
}


/*
Purpose: (Return a uniform random number.)
*/
double
RandomBase::random_u01_scalar ( // Return: -- Random number from U[0,1)
   void)
{
   if (! seeded) {
      reset ();
   }
   use_count++;
   return erand48 (xsubi); // flawfinder: ignore
}


/*
Purpose: (Constructor, with seed set from numbers pulled out of a hat.)
*/
RandomSeedGenerator::RandomSeedGenerator (
   unsigned int nskip,             // In:    -- Skip count
   bool initialize)                // In:    -- Complete initialization?
:
   RandomBase (nskip, initialize)
{}


/*
Purpose: (Populate a seed for initializing some random generator.)
*/
void
RandomSeedGenerator::get_seed (
   unsigned short seed[3])     // Out: -- Seed
{
   long lseed = random_long ();
   unsigned short * useed = reinterpret_cast <unsigned short *> (&lseed);
   unsigned int idx0, idx1;

   if ((sizeof(long) == 8) &&
       ((useed[0] == 0) || (useed[0] == 0xffffU)) &&
       (useed[1] == useed[0])) {
      idx0 = 3;
      idx1 = 2;
   }
   else {
      idx0 = 0;
      idx1 = 1;
   }

   seed[0] = useed[idx0];
   seed[1] = useed[idx1];
   seed[2] = RANDOM_SEED_3;
}


/*
Purpose: (Constructor.)
*/
RandomGenerator::RandomGenerator (
   RandomSeedGenerator & seed_gen, // Inout: -- Random seed generator
   unsigned int nskip,             // In:    -- Skip count
   bool initialize)                // In:    -- Complete initialization?
:
   RandomBase (seed_gen, nskip, initialize)
{}


/*
Purpose: (Constructor.)
*/
RandomVectorUniform01::RandomVectorUniform01 (
   RandomSeedGenerator & seed_gen,
   unsigned int vsize,
   unsigned int nskip,
   bool initialize)
:
   RandomGenerator (seed_gen, vsize*nskip, initialize),
   vector_size (vsize)
{}


#undef RANDOM_SEED_0
#undef RANDOM_SEED_1
#undef RANDOM_SEED_2
#undef RANDOM_SEED_3
} // End JEOD namespace
