/*
PURPOSE:
    (Provides data values for the TAI to UTC time converter.
     This data file was generated by executing the "parser.py"
     script.)

     Time Span: mm dd yy to mm dd yy - ${leapSeconds[0].month} ${leapSeconds[0].day} ${leapSeconds[0].year} to ${leapSeconds[-1].month} ${leapSeconds[-1].day} ${leapSeconds[-1].year} )

REFERENCE:
    ((http://www.iers.org)
     (http://data.iers.org/products/214/14443/orig/eopc04_08_IAU2000.62-now)
     (Seidelmann, "Explanatory Supplement to the Astronomical Almanac,"
      University Science Books, Mill Valley, California, 1992.))

ASSUMPTIONS AND LIMITATIONS:
    ((DUT1 is interpolated linearly between tabular values wrt date))
*/


namespace jeod {
class TimeConverter_TAI_UTC;
}
#define JEOD_FRIEND_CLASS TimeConverter_TAI_UTC_tai_to_utc_default_data

// JEOD includes
//#include "environment/time/include/time_converter.hh"
#include "environment/time/include/time_converter_tai_utc.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Unsupported includes
#include "../include/tai_to_utc.hh"



//! Namespace jeod
namespace jeod {


void
TimeConverter_TAI_UTC_tai_to_utc_default_data::initialize (
   TimeConverter_TAI_UTC * TimeConverter_TAI_UTC_ptr)
{
   TimeConverter_TAI_UTC_ptr->override_data_table = false;
   TimeConverter_TAI_UTC_ptr->last_index = ${len(leapSeconds)-1};
   TimeConverter_TAI_UTC_ptr->when_vec =
      JEOD_ALLOC_PRIM_ARRAY (${len(leapSeconds)}, double);
   TimeConverter_TAI_UTC_ptr->val_vec =
      JEOD_ALLOC_PRIM_ARRAY (${len(leapSeconds)}, int);

% for entryIdx in range(len(leapSeconds)):
   TimeConverter_TAI_UTC_ptr->when_vec[${entryIdx}] = ${leapSeconds[entryIdx].mjd-40000.0};
   TimeConverter_TAI_UTC_ptr->val_vec[${entryIdx}] = ${leapSeconds[entryIdx].numSeconds};
% endfor
}


} // End JEOD namespace

