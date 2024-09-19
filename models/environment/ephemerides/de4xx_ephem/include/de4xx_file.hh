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
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/include/de4xx_file.hh
 * Define the class responsible for reading the DE4xx ephemeris file.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((JPL ephemeris model reference needed)))

Assumptions and limitations:
  ((Assumption 1.
    Host machine is big-endian or little-endian.)
   (Assumption 2.
    32-bit integers occupy 4 chars and doubles, 8 chars)
   (Assumption 3.
    IEEE-standard doubles))

Library dependencies:
  ((../src/de4xx_file.cc))



*******************************************************************************/

#ifndef JEOD_DE4xx_FILE_HH
#define JEOD_DE4xx_FILE_HH

// System includes
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>

// JEOD includes
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/sim_interface/include/config.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "de4xx_base.hh"

//! Namespace jeod
namespace jeod
{

class De4xxFile;

/**
 * Container for the metadata from the DE model header
 */
struct EphemerisDataSetMeta
{
    /**
     * Return the number of ephemeris items provided in DE data.
     * (e.g., 13 for DE405/421, 15 for DE440)
     */
    uint32_t number_file_items;

    /**
     * Julian date of start of dataset
     */
    double start_epoch; //!< trick_units(day)

    /**
     * Julian date of end of dataset
     */
    double stop_epoch; //!< trick_units(day)

    /**
     * Julian period length of each segment
     */
    double delta_epoch; //!< trick_units(day)

    /**
     * Number of ascp files (segments) provided by DE model
     */
    uint32_t number_segments; //!< trick_units(--)

    /**
     * Size of each data record throughout the dataset.
     */
    uint32_t ncoeff; //!< trick_units(--)

    /**
     * Array of supplied constants required by JEOD
     */
    double de_constants[De4xxBase::De4xx_Const_MaxConsts];
};

/**
 *  Structure containing the header metadata for sizing/locating the data
 *  entries with the data segments
 */
struct EphemerisDataItemMeta
{
    /**
     * Offsets into coeffs array
     */
    uint32_t offset; //!< trick_units(--)

    /**
     * Chebychev polynomial terms
     */
    uint32_t nterms; //!< trick_units(--)

    /**
     * Number polynomials per data block
     */
    uint32_t npoly; //!< trick_units(--)
};

/**
 * Metadata implied from each data segment
 */
struct EphemerisDataSegmentMeta
{
    /**
     * The number of records in the file.
     */
    uint32_t num_recs; //!< trick_units(--)

    /**
     * Julian date of start of file
     */
    double start_epoch; //!< trick_units(day)

    /**
     * Julian date of end of file
     */
    double stop_epoch; //!< trick_units(day)
};

/**
 * Specifies which file to use (user input initialization-time data).
 */
class De4xxFileSpec
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileSpec)

    friend class De4xxFile;

public:
    // Member functions
    De4xxFileSpec();
    De4xxFileSpec(const De4xxFileSpec &) = delete;
    De4xxFileSpec & operator=(const De4xxFileSpec &) = delete;

    /**
     * Set ephemeris model number.
     * This number is used to specify the de file to use
     * the pathname is of the form \<ephem_file_dir\>/libde\<denumIn\>.so
     * Defaults to PWD/build/de4xx_lib/libde\<denumIn\>.so
     */
    void set_model_number(int denum_in);

    /**
     * Get Ephemeris model number.
     * This number is used to specify the de file to use
     * the pathname is of the form PWD/build/de4xx_lib/libde\<denumIn\>.so
     */
    uint32_t get_model_number()
    {
        return denum;
    }

    /**
     * Set ephemeris data model directory.
     * This number is used to specify the de file to use
     * the pathname is of the form \<ephem_file_dir\>/libde\<denumIn\>.so
     * Defaults to PWD/build/de4xx_lib/libde\<denumIn\>.so
     */
    void set_model_directory(const std::string & dirIn);

    /**
     * Get Ephemeris data model directory.
     * This number is used to specify the de file to use
     * the pathname is of the form \<ephem_file_dir\>/libde\<denumIn\>.so
     * Defaults to PWD/build/de4xx_lib/libde\<denumIn\>.so
     */
    std::string get_model_directory()
    {
        return ephem_file_dir;
    }

protected:
    // Member data

    // User inputs
    /**
     * Ephemeris model number.
     * This must match the DE number in the data file; a sanity check
     */
    uint32_t denum{405}; //!< trick_units(--)

    /**
     * Ephemeris file directory
     */
    std::string ephem_file_dir{"build/de4xx_lib"}; //!< trick_units(--)

    /**
     * Ephemeris file name
     */
    std::string ephem_file_name; //!< trick_units(--)

    // Internally-computed items (visible for logging and checkpoint)
    /**
     * Ephemeris file path name
     */
    std::string pathname; //!< trick_io(*o) trick_units(--)
};

/**
 * Contains data used directly for reading the ephemeris file.
 */
class De4xxFileIO
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileIO)

    friend class De4xxFile;

    // Member data
public:
    /**
     * Metadata (e.g., sizing) regarding the selected DE ephemeris data set
     */
    EphemerisDataSetMeta * metaData{}; //!< trick_units(--) trick_io(**)

    /**
     * Metadata (e.g., number of terms) regarding each ephemeris item
     * (e.g., Mercury) contained in the JPL data
     */
    EphemerisDataItemMeta * itemData{}; //!< trick_units(--) trick_io(**)

    /**
     * Metadata (e.g., number of records) regarding each polynomial segment
     * of the JPL data
     */
    EphemerisDataSegmentMeta * segmentData{}; //!< trick_units(--) trick_io(**)

    /**
     * Pointer to first value in the segment
     */
    double * coeffs_segment_starting_addr{}; //!< trick_units(--) trick_io(**)

    /**
     * Pointer to first value in the record
     */
    double * current_record_starting_addr{}; //!< trick_units(--) trick_io(**)

    /**
     * The current record number.
     */
    uint32_t recno{std::numeric_limits<int>::max()}; //!< trick_units(--)

    /**
     * The current segment number.
     */
    uint32_t segment_index{}; //!< trick_units(--)

    /**
     * The current segment record number.
     */
    uint32_t segment_recno{}; //!< trick_units(--)

    /**
     * The number of records in the dataset.
     */
    uint32_t total_num_recs{}; //!< trick_units(--)

    /**
     * The maximum number of Chebychev terms in the file.
     */
    uint32_t max_terms{}; //!< trick_units(--)

protected:
    /**
     * The dl handle for the ephemeris shared object.
     */
    void * file{}; //!< trick_units(--) trick_io(**)

    // Member functions
public:
    De4xxFileIO() = default;
    De4xxFileIO(const De4xxFileIO &) = delete;
    De4xxFileIO & operator=(const De4xxFileIO &) = delete;
};

/**
 * Contains data extracted from the ephemeris file header.
 */
class De4xxFileHeader
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileHeader)

    friend class De4xxFile;

    // Member data
public:
    /**
     * Astronomical unit in meters
     */
    double au{}; //!< trick_units(m)

    /**
     * Speed of light
     */
    double vlight{}; //!< trick_units(m/s)

    /**
     * Earth:Moon mass ratio
     */
    double em_mass_ratio{}; //!< trick_units(--)

    /**
     * Ratio of Earth-to-barycenter and Earth-to-Moon distances
     * Note: Also equal to the ratio of Moon and Earth+Moon masses.
     */
    double be_em_dist_ratio{}; //!< trick_units(--)

    /**
     * Ratio of Barycenter-to-Moon and Earth-to-Moon distances
     * Note: Also equal to the ratio of Earth and Earth+Moon masses.
     */
    double bm_em_dist_ratio{}; //!< trick_units(--)

    /**
     * Ratio of Earth to Earth-moon L1 point and Earth-to-Moon distances
     */
    double e1_em_dist_ratio{}; //!< trick_units(--)

    /**
     * Ratio of Earth-Moon barycenter to L1 point and Earth-to-Moon distances
     */
    double b1_em_dist_ratio{}; //!< trick_units(--)

    /**
     * Body gravitational constants
     */
    double * gmbody; //!< trick_units(m3/s2)

    // Member functions
public:
    De4xxFileHeader();
    ~De4xxFileHeader();
    De4xxFileHeader(const De4xxFileHeader &) = delete;
    De4xxFileHeader & operator=(const De4xxFileHeader &) = delete;
};

/**
 * Contains data regarding one of the items in a DE ephemeris file.
 */
class De4xxFileItem
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileItem)

    friend class De4xxFile;

    // Member data
public:
    /**
     * Is this item's state to be computed? (external input)
     */
    bool active{}; //!< trick_units(--)

    /**
     * Is this item represented in the ephemeris file?
     */
    bool avail{}; //!< trick_units(--)

    /* Index referenced in io.itemData array */
    uint32_t item_idx{}; //!< trick_units(--)

    /**
     * Vector size
     */
    int32_t nitems{3}; //!< trick_units(--)

    /**
     * Zeroth derivative scale factor
     */
    double pscale{1000.0}; //!< trick_units(--)

    /**
     * Update time (simulation time)
     */
    double update_time{-99e99}; //!< trick_units(s)

    /**
     * State data (zeroth, first derivative)
     */
    double state[2][3]{}; //!< trick_units(--)

    // Member functions
public:
    De4xxFileItem();
    De4xxFileItem(const De4xxFileItem &) = delete;
    De4xxFileItem & operator=(const De4xxFileItem &) = delete;
};

/**
 * Contains timing reference data.
 */
class De4xxFileRefTime
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileRefTime)

    friend class De4xxFile;

    // Member data
public:
    /**
     * Julian date of midnight preceding reference time point.
     */
    double epoch_date{-99e99}; //!< trick_units(day)

    /**
     * Fractional days past epoch date of reference time point.
     */
    double fdate{-99e99}; //!< trick_units(day)
    /**
     * Time offset, Typically, Terrestrial Time offset.
     */
    double time_offset{-99e99}; //!< trick_units(s)
    /**
     * Initialization time (seconds from reference, typically zero).
     */
    double init_time{-99e99}; //!< trick_units(s)

    /**
     * File block number corresponding to reference time.
     */
    double block_no{-99e99}; //!< trick_units(--)

    // Member functions
public:
    De4xxFileRefTime() = default;
    De4xxFileRefTime(const De4xxFileRefTime &) = delete;
    De4xxFileRefTime & operator=(const De4xxFileRefTime &) = delete;
};

/**
 * Contains Chebychev polynomial coefficients and terms.
 */
class De4xxFileCoef
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFileCoef)

    friend class De4xxFile;

    // Member data
public:
protected:
    /**
     * No. Chebychev polynomials terms
     */
    JEOD_SIZE_T chebyterms{}; //!< trick_units(--)

    /**
     * Chebychev x value
     */
    double chebyx{-99e99}; //!< trick_units(--)

    /**
     * Chebychev polynomial coeffs
     */
    double * chebypoly{}; //!< trick_units(--)

    /**
     * Derivative of chebypoly
     */
    double * chebyderiv{}; //!< trick_units(--)

    /**
     * Current block contents
     */
    double * coef{}; //!< trick_units(--) trick_io(**)

    // Member functions
public:
    De4xxFileCoef() = default;
    De4xxFileCoef(const De4xxFileCoef &) = delete;
    De4xxFileCoef & operator=(const De4xxFileCoef &) = delete;
};

/**
 * The FILE pointer in a De4xxFileIO cannot be restored by Trick.
 * This class provides that essential restart mechanism.
 */
class De4xxFileRestart : public SimpleCheckpointable
{
public:
    explicit De4xxFileRestart(De4xxFile & in);
    ~De4xxFileRestart() override = default;
    De4xxFileRestart(const De4xxFileRestart &) = delete;
    De4xxFileRestart & operator=(const De4xxFileRestart &) = delete;

    void simple_restore() override;

protected:
    /**
     * The De4xxFile object to be restored.
     */
    De4xxFile & de4xx_file; //!< trick_io(**)
};

/**
 * Provides the ability to read and interpret a DE4xx ephemeris file.
 */
class De4xxFile
{
    JEOD_MAKE_SIM_INTERFACES(jeod, De4xxFile)

    friend class De4xxFileRestart;

public:
    // Member functions
    De4xxFile();
    ~De4xxFile();
    De4xxFile(const De4xxFile &) = delete;
    De4xxFile & operator=(const De4xxFile &) = delete;

    // perform actions common to initialization and restart
    void pre_initialize();

    // initialize the object
    void initialize(double epoch_time, double del_day, double time_offset, double init_time);

    // Check whether the time is represented in the file
    bool time_is_in_range(double time) const;

    // Update the object
    void update(double time);

    // Shut down
    void shutdown();

    // Member data

    /**
     * File specification
     */
    De4xxFileSpec file_spec; //!< trick_units(--)

    /**
     * File header
     */
    De4xxFileHeader header; //!< trick_units(--)

    /**
     * Item data. Sized to fit number of entries in most recent DE4xx release
     */
    De4xxFileItem * item; //!< trick_units(--)

    /**
     * File descriptor
     */
    De4xxFileIO io; //!< trick_units(--)

    /**
     * Reference time
     */
    De4xxFileRefTime ref_time; //!< trick_units(--)

    /**
     * Chebychev coefs
     */
    De4xxFileCoef coef; //!< trick_units(--)

    /**
     * Restart handler
     */
    De4xxFileRestart restart; //!< trick_io(**)

    /**
     * Time of last update
     */
    double update_time{-99e99}; //!< trick_units(s)

    /*
     * Virtual memory usage
     */
    double vm_usage{}; //!< trick_units(--)

    /*
     * Resident set size
     */
    double resident_set{}; //!< trick_units(--)

    /*
     * Flag to enable/disable memory logging and output
     */
    bool logMemoryStats{true}; //!< trick_units(--)

private:
    // Member functions

    void open();

    void reopen();

    void close();

    void interpolate(double time, double fblk);

    void capture_mem_stats();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
