/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/src/de4xx_file_update.cc
 * Define De4xxFile::update.
 */

/*******************************************************************************

Purpose:
 ()

Library dependency:
 ((de4xx_file_update.cc)
  (de4xx_file.cc)
  (environment/ephemerides/ephem_interface/src/ephem_messages.cc)
  (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstddef>
#include <cstdint>
#include <dlfcn.h>
#include <limits>
#include <sstream>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "utils/math/include/numerical.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/de4xx_file.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Calcuate the position and velocity states of selected planetary bodies at
 * some point in time. The EphemeridesState structure embedded in the Ephemerides
 * structure contains an update indicator and state for each body. The state for
 * a body is updated if the body's indicator indicates that a state update is
 * needed.
 *
 * Body selection --
 *   The 'active_bodies' array in the  EphemeridesState structure indicates which
 *   planets' states are to be updated. The function updates the position and
 *   velocity for the selected bodies.
 *
 * Time specification --
 *   Four input variables are available for specifying the time.
 *   - tt_offset is the offset between the remaining input times and Terrestrial
 *     Time (aka Terrestrial Dynamic Time, or TDT). Set this to zero if the
 *     other input times are already expressed in Terrestrial Time.
 *   - For highest precision, set epoch_time to the Julian date at midnight of
 *     the time point of interest and set either del_day or del_time to the
 *     difference between the time point of interest and the epoch_time.
 *   - An alternative that retains full precession is to pass the time at the
 *     start of the simulation in epoch_time and del_day and the time into the
 *     simulation in del_time. In this approach, epoch_time represents the
 *     Julian date at the midnight preceding the start of the simulation and
 *     del_day represents the time between the epoch time and simulation start.
 *   - For ease of use, set epoch_time to the Julian date representing the
 *     the time point of interest and set both del_day and del_time to 0.0.
 *     Note that this approach has a machine granularity of about 0.2 msec.
 *   - An intermediate alternative is to set the epoch_time to the start time
 *     of the simulation, del_day to zero and del_time to the simulation time
 *     in seconds. This approach will result in a small temporal bias due to
 *     the precision loss in the epoch time.
 *
 * Outputs --
 *   Positions and velocities are expressed in the ICRF coordinate system.
 *   The states of the Sun and planets, including Earth-Moon barycenter, are
 *   expressed with respect to the solar system barycenter. Lunar states are
 *   expressed with respect to the center of the Earth.
 *
 * NOTA BENE --
 *   The states of unselected bodies may or may not be changed.
 *
 * \par Assumptions and Limitations
 *  - Assumption 1.
 *    Ephemeris file is open.
 *  - Assumption 2.
 *    Offset times in del_day and del_time are small. See description above.
 *  - Assumption 3.
 *    The caller will not reference the states of bodies not requested in the
 *    active_bodies array. The unselected body states are fair game and may or
 *    may not be modified by this function.
 *  - Limitation 1.
 *    No light speed time-of-travel corrections.
 *    Such corrections must be made by the caller if needed.
 *  - Limitation 2.
 *    No relativistic time corrections for the difference between Terrestrial
 *    and Barycenter Dynamic Time.
 *  - Limitation 3.
 *    The states of the sun and the major planets are expressed in ICRF
 *    coordinates relative to the Solar System barycenter.
 *    The state of the Moon is expressed in ICRF coordinates relative
 *    to the center of the Earth.
 * \param[in] time Time since reference\n Units: s
 */
void De4xxFile::update(double time)
{
    int nactive;

    double fblk;
    uint32_t recno;

    /* Count the number of active bodies. */
    nactive = 0;
    for(uint32_t ii = 0; ii < io.metaData->number_file_items; ii++)
    {
        De4xxFileItem * item_ii = &(item[ii]);

        /* Check for availability and count active bodies. */
        if(item_ii->active)
        {
            if(!item_ii->avail)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     EphemeridesMessages::item_not_in_file,
                                     "Body %d ephemeris not available",
                                     ii);

                // Not reached
                return;
            }
            nactive++;
        }
    }

    /* Nothing to do if no bodies are active. */
    if(nactive == 0)
    {
        return;
    }

    /* Ensure the ephemerides database is open. */
    if(io.file == nullptr)
    {
        MessageHandler::fail(__FILE__, __LINE__, EphemeridesMessages::internal_error, "Ephemeris file is not open");

        // Not reached
        return;
    }

    /* Compute the integral and fractional record numbers. */
    fblk = ref_time.block_no + (time - ref_time.init_time) / (86400.0 * io.metaData->delta_epoch);
    recno = static_cast<uint32_t>(fblk);
    fblk -= static_cast<double>(recno);

    /* Record the update time. */
    update_time = time;

    /* Read and parse the record if needed. */

    if(recno != io.recno)
    {
        uint32_t starting_segment_record = io.segment_recno;
        uint32_t update_segment_idx = std::numeric_limits<int>::max();
        if(recno > io.recno)
        {
            if(recno >= (io.total_num_recs - io.segmentData[io.metaData->number_segments - 1].num_recs))
            {
                update_segment_idx = io.metaData->number_segments - 1;
                starting_segment_record = io.total_num_recs - io.segmentData[io.metaData->number_segments - 1].num_recs;
            }
            else
            {
                for(uint32_t ii = io.segment_index; ii < io.metaData->number_segments; ++ii)
                {
                    if(recno >= (starting_segment_record + (io.segmentData[ii].num_recs)))
                    {
                        starting_segment_record += (io.segmentData[ii].num_recs);
                    }
                    else
                    {
                        update_segment_idx = ii;
                        break;
                    }
                }
            }
        }
        else
        {
            if(recno >= (io.segmentData[0].num_recs))
            {
                for(uint32_t ii = io.segment_index; ii > 0; --ii)
                {
                    if(recno < starting_segment_record)
                    {
                        starting_segment_record -= (io.segmentData[ii - 1].num_recs);
                    }
                    else
                    {
                        update_segment_idx = ii - 1;
                        break;
                    }
                }
            }
            else
            {
                update_segment_idx = 0;
                starting_segment_record = 0;
            }
        }
        if(update_segment_idx != io.segment_index)
        {
            io.segment_index = update_segment_idx;
            io.segment_recno = starting_segment_record;
            std::stringstream segment_var_name("segment_coeffs_");
            segment_var_name << "segment_coeffs_" << io.segment_index;

            // Clear dlerror
            dlerror();

            io.coeffs_segment_starting_addr = (double *)dlsym(io.file, segment_var_name.str().c_str());
            if(io.coeffs_segment_starting_addr == nullptr)
            {
                char * dlError = dlerror();
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     EphemeridesMessages::file_error,
                                     "Error obtaining ephemeris file symbol '%s' from '%s' for input: %s",
                                     segment_var_name.str().c_str(),
                                     file_spec.pathname.c_str(),
                                     dlError);

                // Not reached
                return;
            }
        }
        io.current_record_starting_addr = &(
            io.coeffs_segment_starting_addr[(recno - io.segment_recno) * io.metaData->ncoeff]);
        io.recno = recno;
        coef.coef = &(io.coeffs_segment_starting_addr[(recno - io.segment_recno) * io.metaData->ncoeff]);
    }

    //   capture_mem_stats();

    /* Interpolate position and velocity. */
    interpolate(time, fblk);
}

/**
 * Calcuate the position and velocity states of selected planetary bodies at
 * some point in time.
 * \param[in] time Time since reference\n Units: s
 * \param[in] fblk Fractional block
 */
void De4xxFile::interpolate(double time, double fblk)
{
    double fsub;
    int subint;

    double chebyx, twox;
    std::size_t nitems;
    std::size_t nterms;
    double dnpoly;
    int item_offset;

    double pscale;
    double vscale;

    std::size_t ii, jj;
    int kk;

    /* Interpolate position and velocity. */
    for(ii = 0; ii < io.metaData->number_file_items; ii++)
    {
        De4xxFileItem * item_ii = &(item[ii]);
        if(item_ii->active)
        {
            EphemerisDataItemMeta & itemData = io.itemData[item_ii->item_idx];

            /* Get body-specific polynomial descriptors. */
            pscale = item_ii->pscale;
            nitems = item_ii->nitems;
            nterms = itemData.nterms;
            dnpoly = static_cast<double>(itemData.npoly);

            /* Compute the sub-interval of the loaded time span that contains
             * the input epoch time. */
            fsub = fblk * dnpoly;
            subint = static_cast<int>(fsub);
            fsub -= subint;

            /* Compute Chebychev coefficients T_k(x) and their derivatives
             * dT_k/dx(x) for x = 2*fsub-1:
             *   T[0] = 1
             *   T[1] = x
             *   T[k] = 2 x T[k-1] - T[k-2]
             *   dT[0]/dx = 0
             *   dT[1]/dx = 1
             *   dT[k]/dx = 2 T[k-1] + 2 x dT[k-1]/dx - dT[k-2]/dx */
            chebyx = fsub + fsub - 1.0;
            if((coef.chebyterms < nterms) || (!Numerical::compare_exact(coef.chebyx, chebyx)))
            {
                coef.chebyterms = nterms;
                coef.chebyx = chebyx;
                twox = chebyx + chebyx;
                coef.chebypoly[0] = 1.0;
                coef.chebypoly[1] = chebyx;
                coef.chebyderiv[0] = 0.0;
                coef.chebyderiv[1] = 1.0;
                for(jj = 2; jj < nterms; jj++)
                {
                    coef.chebypoly[jj] = twox * coef.chebypoly[jj - 1] - coef.chebypoly[jj - 2];
                    coef.chebyderiv[jj] = coef.chebypoly[jj - 1] + coef.chebypoly[jj - 1] +
                                          twox * coef.chebyderiv[jj - 1] - coef.chebyderiv[jj - 2];
                }
            }

            /* Compute the velocity scale factor.
             *   dr/dt = dr/dx dx/dfs dfs/dfb dfb/dd dd/dt
             *   dr/dx = value computed by applying coeffs vc
             *   x  = 2fs-1                    => dx/dfs  = 2
             *   fs = fs0 + fb*dnpoly          => dfs/dfb = dnpoly
             *   fb = fb0 + d / delta_epoch    => dfb/dd  = 1/delta_epoch
             *   d  = t/86400.0                => dd/dt  = 1/86400.0
             * Thus
             *   vscale = 2*dnpoly / (delta_epoch*86400)
             * Note: The JPL ephemeris file expresses distance in kilometers.
             * Scale velocity scale by the pscale. */
            vscale = 2.0 * dnpoly / (io.metaData->delta_epoch * 86400.0) * pscale;

            /* Compute the C-language offset to the coefficients for this
             * sub-interval. */
            item_offset = itemData.offset - 1 + nitems * nterms * subint;

            /* Interpolate to get position, velocity for each component. */
            for(jj = 0; jj < nitems; jj++)
            {
                std::size_t coefs_starting_idx = item_offset + jj * nterms;
                item_ii->state[0][jj] = 0.0;
                item_ii->state[1][jj] = 0.0;
                for(kk = nterms - 1; kk >= 0; kk--)
                {
                    item_ii->state[0][jj] += coef.chebypoly[kk] * coef.coef[coefs_starting_idx + kk];
                    item_ii->state[1][jj] += coef.chebyderiv[kk] * coef.coef[coefs_starting_idx + kk];
                }

                /* The JPL ephemeris file expresses distance in kilometers.
                 * Scale position, velocity to yield meters, meters/sec^2 */
                item_ii->state[0][jj] *= pscale;
                item_ii->state[1][jj] *= vscale;
            }

            /* Timestamp the data. */
            item_ii->update_time = time;
        }
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
