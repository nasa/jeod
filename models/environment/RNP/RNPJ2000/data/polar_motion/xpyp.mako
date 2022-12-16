/*******************************************************************************
PURPOSE:
   (${outType[0].upper() + outType[1:]} Earth Orientation Parameters (EOP) for polar motion.
    This data file was generated from IERS data "EOP 14 C04 (IAU2000)"
    using the "parser.py" script.)

REFERENCE:
   ((http://www.iers.org)
    (https://datacenter.iers.org/data/224/eopc04_14_IAU2000.62-now.txt)
    (Seidelmann, "Explanatory Supplement to the Astronomical Almanac,"
     University Science Books, Mill Valley, California, 1992.))

ASSUMPTIONS AND LIMITATIONS:
    ((Interpolation between tabular values is handled by JEOD))
*******************************************************************************/


namespace jeod {
class PolarMotionJ2000Init;
}
#define JEOD_FRIEND_CLASS PolarMotionJ2000Init_xpyp_${outType}_default_data

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation_init.hh"
#include "environment/RNP/RNPJ2000/include/polar_motion_j2000_init.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Unsupported includes
#include "../include/xpyp_${outType}.hh"



//! Namespace jeod
namespace jeod {


void
PolarMotionJ2000Init_xpyp_${outType}_default_data::initialize (
   PolarMotionJ2000Init * PolarMotionJ2000Init_ptr)
{
   PolarMotionJ2000Init_ptr->override_table = false;
   PolarMotionJ2000Init_ptr->last_table_index = ${len(entries)-1};
   PolarMotionJ2000Init_ptr->polar_mjd =
      JEOD_ALLOC_PRIM_ARRAY (${len(entries)}, double);
   PolarMotionJ2000Init_ptr->xp_tbl =
      JEOD_ALLOC_PRIM_ARRAY (${len(entries)}, double);
   PolarMotionJ2000Init_ptr->yp_tbl =
      JEOD_ALLOC_PRIM_ARRAY (${len(entries)}, double);

static const double ARCSECONDS_TO_RADIANS = 4.84813681109536e-06;

% for entryIdx in range(len(entries)):
   PolarMotionJ2000Init_ptr->polar_mjd[${entryIdx}]   = ${entries[entryIdx].mjd}; /* ${entries[entryIdx].year} ${entries[entryIdx].month} ${entries[entryIdx].day} */
   PolarMotionJ2000Init_ptr->xp_tbl[${entryIdx}] = ARCSECONDS_TO_RADIANS * (${entries[entryIdx].xp});
   PolarMotionJ2000Init_ptr->yp_tbl[${entryIdx}] = ARCSECONDS_TO_RADIANS * (${entries[entryIdx].yp});
%endfor
}


} // End JEOD namespace

