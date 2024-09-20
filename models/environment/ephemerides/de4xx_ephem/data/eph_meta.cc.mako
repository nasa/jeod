/******************************************************************************

Purpose:
 (JPL Development Ephemeris converted to C++ source)

******************************************************************************/

// System includes

// JEOD includes
#include "environment/ephemerides/de4xx_ephem/include/de4xx_file.hh"

% if segIdx == 0:
jeod::EphemerisDataSetMeta metaData = {
    .number_file_items = ${meta.num_items},
    .start_epoch = ${meta.start_epoch},
    .stop_epoch = ${meta.stop_epoch},
    .delta_epoch = ${meta.delta_epoch},
    .number_segments = ${meta.number_segments},
    .ncoeff = ${meta.ncoeff},
    .de_constants = {${meta.consts['DENUM']}, ${meta.consts['LENUM']}, ${meta.consts['AU']},
                     ${meta.consts['EMRAT']}, ${meta.consts['CLIGHT']}, ${meta.consts['GM1']},
                     ${meta.consts['GM2']}, ${meta.consts['GMB']}, ${meta.consts['GM4']},
                     ${meta.consts['GM5']}, ${meta.consts['GM6']}, ${meta.consts['GM7']},
                     ${meta.consts['GM8']}, ${meta.consts['GM9']}, ${meta.consts['GMS']}}
};


jeod::EphemerisDataItemMeta itemData[${meta.num_items}] = {
% for ii in range(meta.num_items):
  % if ii == (meta.num_items-1):
    {.offset = ${meta.coef_start_index[ii]}, .nterms = ${meta.num_coef_per_component[ii]}, .npoly = ${meta.num_coef_sets[ii]}}
  % else:
    {.offset = ${meta.coef_start_index[ii]}, .nterms = ${meta.num_coef_per_component[ii]}, .npoly = ${meta.num_coef_sets[ii]}},
  % endif
% endfor
};

jeod::EphemerisDataSegmentMeta segmentData[${meta.number_segments}] = {
% for ii in range(meta.number_segments):
  % if ii == (meta.number_segments-1):
    {.num_recs = ${data[ii].num_records}, .start_epoch = ${data[ii].record_start_epoch[0]}, .stop_epoch = ${data[ii].record_stop_epoch[-1]}}
  % else:
    {.num_recs = ${data[ii].num_records}, .start_epoch = ${data[ii].record_start_epoch[0]}, .stop_epoch = ${data[ii].record_stop_epoch[-1]}},
  % endif
% endfor
};

% endif
// Coefficients table

// From ${meta.title}
// segment_coeffs_<segment_index>[num_recs][recsize]
double segment_coeffs_${segIdx}[${data[segIdx].num_records}][${meta.ncoeff}] = {
% for ii in range(data[segIdx].num_records):
    {
  % for jj in range(0, meta.ncoeff, 4):
     % if (meta.ncoeff - jj) == 1:
     ${data[segIdx].coefs[ii][jj]}
     % elif (meta.ncoeff - jj) == 2:
     ${data[segIdx].coefs[ii][jj]}, ${data[segIdx].coefs[ii][jj+1]}
     % elif (meta.ncoeff - jj) == 3:
     ${data[segIdx].coefs[ii][jj]}, ${data[segIdx].coefs[ii][jj+1]}, ${data[segIdx].coefs[ii][jj+2]}
     % elif (meta.ncoeff - jj) == 4:
     ${data[segIdx].coefs[ii][jj]}, ${data[segIdx].coefs[ii][jj+1]}, ${data[segIdx].coefs[ii][jj+2]}, ${data[segIdx].coefs[ii][jj+3]}
     % else:
     ${data[segIdx].coefs[ii][jj]}, ${data[segIdx].coefs[ii][jj+1]}, ${data[segIdx].coefs[ii][jj+2]}, ${data[segIdx].coefs[ii][jj+3]},
     % endif
  % endfor
  % if ii == (data[segIdx].num_records-1):
    }
  % else:
    },
  % endif
% endfor
};
