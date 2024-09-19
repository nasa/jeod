
// System includes
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>

// Test harness includes
#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/color_text.hh"
#include "test_harness/include/test_sim_interface.hh"

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/time/include/time_converter_dyn_tai.hh"
#include "environment/time/include/time_converter_tai_tt.hh"
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_manager_init.hh"
#include "environment/time/include/time_tai.hh"
#include "environment/time/include/time_tt.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"

// Model includes
#include "environment/ephemerides/de4xx_ephem/include/de4xx_ephem.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"

// Local includes
#include "de_constants.hh"

using namespace jeod;

// Globals

static const DE4xxConstants * constants;
static ColorText color;

static const double jdate_2000 = 2451515.0;

static int de_model;
static int verbose;

// Internal functions
static bool vtransit_test(TimeManager & time_manager, EphemeridesManager & ephem_manager);

static bool dataset_test(const De4xxEphemeris & de4xx_ephem,
                         TimeManager & time_manager,
                         EphemeridesManager & ephem_manager);

static void pleph(double pv[2][3], EphemeridesManager & ephem_manager, int itgt, int ictr);

int main(int argc, char ** argv)
{
    // Test harness stuff
    // Note well: The sim interface needs to be declared first.
    TestSimInterface sim_interface;
    CmdlineParser cmdline_parser;

    // Time stuff
    TimeManager time_manager;
    TimeManagerInit time_manager_init;
    TimeTAI time_tai;
    TimeTT time_tt;
    TimeConverter_Dyn_TAI time_converter_dyn_tai;
    TimeConverter_TAI_TT time_converter_tai_tt;

    // Planets
    BasePlanet sun;
    BasePlanet mercury;
    BasePlanet venus;
    BasePlanet earth;
    BasePlanet moon;
    BasePlanet mars;
    BasePlanet jupiter;
    BasePlanet saturn;
    BasePlanet uranus;
    BasePlanet neptune;
    BasePlanet pluto;

    // Test article
    EphemeridesManager ephem_manager;
    De4xxEphemeris de4xx_ephem;

    // Test file
    char de_filename[128];

    // Local variables.
    bool passed_all = true;

    // Load and parse the command line.
    cmdline_parser.add_int("model", 405, &de_model);
    cmdline_parser.add_counter("verbose", &verbose);

    cmdline_parser.parse(argc, argv);

    // Construct the file to be tested.
    sprintf(de_filename, "libde%d.so", de_model);

    // Write a test signature.
    std::printf("========================================"
                "========================================\n\n"
                "JEOD epehemeris model test\n"
                "  Ephemeris file : %s\n"
                "  DE model       : %d\n",
                de_filename,
                de_model);

    // Initialize time.
    time_manager_init.initializer = "TT";
    time_manager_init.sim_start_format = TimeEnum::Julian;
    time_tt.initializing_value = jdate_2000;

    time_tai.initialize_from_name = "TT";

    time_tai.update_from_name = "Dyn";
    time_tt.update_from_name = "TAI";

    time_manager.register_time(time_tai);
    time_manager.register_converter(time_converter_dyn_tai);

    time_manager.register_time(time_tt);
    time_manager.register_converter(time_converter_tai_tt);

    time_manager.initialize(&time_manager_init);

    // Initialize the ephemeris model.
    de4xx_ephem.active = true;
    de4xx_ephem.set_model_number(de_model);
    de4xx_ephem.initialize_model(time_manager, ephem_manager);

    // Extract the constants from the file.
    constants = new DE4xxConstants(de4xx_ephem.get_header_data());

    // Initialize planets.
    sun.set_name("Sun");
    mercury.set_name("Mercury");
    venus.set_name("Venus");
    earth.set_name("Earth");
    moon.set_name("Moon");
    mars.set_name("Mars");
    jupiter.set_name("Jupiter");
    saturn.set_name("Saturn");
    uranus.set_name("Uranus");
    neptune.set_name("Neptune");
    pluto.set_name("Pluto");

    sun.register_planet(ephem_manager);
    mercury.register_planet(ephem_manager);
    venus.register_planet(ephem_manager);
    earth.register_planet(ephem_manager);
    moon.register_planet(ephem_manager);
    mars.register_planet(ephem_manager);
    jupiter.register_planet(ephem_manager);
    saturn.register_planet(ephem_manager);
    uranus.register_planet(ephem_manager);
    neptune.register_planet(ephem_manager);
    pluto.register_planet(ephem_manager);

    // Subscribe to the frames needed for the Venus transit test.
    ephem_manager.subscribe_to_frame("Sun.inertial");
    ephem_manager.subscribe_to_frame("Venus.inertial");
    ephem_manager.subscribe_to_frame("Earth.inertial");
    ephem_manager.subscribe_to_frame("EMBary.inertial");

    // Initialize the ephemerides.
    ephem_manager.initialize_ephemerides();
    ephem_manager.activate_ephemerides();
    ephem_manager.update_ephemerides();

    // Test the Venus transit @ 08:19:44 UTC June 8, 2004
    passed_all = vtransit_test(time_manager, ephem_manager) && passed_all;

    // Subscribe to all remaining frames provided by the model.
    ephem_manager.subscribe_to_frame("SSBary.inertial");
    ephem_manager.subscribe_to_frame("Mercury.inertial");
    ephem_manager.subscribe_to_frame("Moon.inertial");
    ephem_manager.subscribe_to_frame("Mars.inertial");
    ephem_manager.subscribe_to_frame("Jupiter.inertial");
    ephem_manager.subscribe_to_frame("Saturn.inertial");
    ephem_manager.subscribe_to_frame("Uranus.inertial");
    ephem_manager.subscribe_to_frame("Neptune.inertial");
    ephem_manager.subscribe_to_frame("Pluto.inertial");
    ephem_manager.subscribe_to_frame("Moon.pfix");
    passed_all = dataset_test(de4xx_ephem, time_manager, ephem_manager) && passed_all;

    /* Summarize the test results. */
    std::printf("\n%s test summary: %s\n\n"
                "========================================"
                "========================================\n\n\n",
                de_filename,
                color.pass_fail(passed_all).c_str());

    if(passed_all)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static bool vtransit_test(TimeManager & time_manager, EphemeridesManager & ephem_manager)
{
    double jun0804;   // day  Julian date for 8 June 2004 midnight
    double ttransit;  // s    08:19:44 in seconds
    double tt_offset; // s    Diff. between Terrestrial, Univ. time
    double tref;      // s    Reference time
    double ttravel;   // s    Lightpeed travel time from sun, venus

    RefFrame & sun_inertial = *(ephem_manager.find_ref_frame("Sun.inertial"));
    RefFrame & venus_inertial = *(ephem_manager.find_ref_frame("Venus.inertial"));
    RefFrame & earth_inertial = *(ephem_manager.find_ref_frame("Earth.inertial"));
    RefFrame & embary_inertial = *(ephem_manager.find_ref_frame("EMBary.inertial"));

    RefFrameState embary_state;
    RefFrameState earth_state;
    RefFrameState sun_wrt_earth;
    RefFrameState venus_wrt_earth;

    double venus_earth_hat[3];
    double sun_earth_hat[3];

    double rest, robs, rerr;
    double vest, vobs, verr;
    double threshold;
    double thetaexp, thetaobs, thetaerr;
    bool rpass, vpass, thetapass;

    bool passed_all = true;

    /* Write a test signature. */
    std::printf("\n"
                "-----------------"
                "Venus transit test - 08:19:44 UTC June 8, 2004"
                "-----------------\n");

    // Calculate the time of the Venus transit: 08:19:44 UTC June 8, 2004
    //  - Julian date at midnight 8 June 2004 = 2453164.5
    //  - Time of day                         = 08:19:44 UTC
    //  - Difference between Atomic and Universal Time:   32.0   secs (05/13/04)
    //  - Difference between Terrestrial and Atomic Time: 32.184 secs
    jun0804 = 2453164.5 - jdate_2000;
    // ttransit  = ((44.0 / 60.0 + 19.0) / 60.0 + 8.0) / 24.0;
    ttransit = (8.0 * 60.0 + 19.0) * 60.0 + 44.0;
    tt_offset = 32.184 + 32.;
    tref = jun0804 * 86400.0 + (ttransit + tt_offset);

    // Update the ephemerides to reflect the transit time.
    time_manager.update(tref);
    ephem_manager.update_ephemerides();

    // Calculate the states of the Sun and Venus wrt the Earth.
    venus_inertial.compute_relative_state(earth_inertial, venus_wrt_earth);
    sun_inertial.compute_relative_state(earth_inertial, sun_wrt_earth);

    // Comparison check: Compare the magnitudes of the displacement and
    // velocity vectors with values obtained from JPL Horizons.
    // The reference time is TT JD 2453164.8477799073.
    double rest2010, vest2010, rest2021, vest2021;
    // Data from JPL Horizons via DE440 (written 2010-07-19, accessed date?):
    //  Teph (Julian days)  Position mag (km)      Velocity mag (km/s)
    //  2453164.847222222   1.519903134179310e8    29.33992080990719
    //  2453164.847916667   1.519903326785795e8    29.33991846404838
    // Interpolating between these times yields

    rest2010 = 1.519903134179310e11 * 0.19693370052752313 + 1.519903326785795e11 * 0.8030662994724768;
    vest2010 = 29.33992080990719e3 * 0.19693370052752313 + 29.33991846404838e3 * 0.8030662994724768;
    // Data from JPL Horizons via DE431 (accessed 2021-06-24):
    //  Teph (Julian days)  Position (km)            Velocity (km/s)
    //  2453164.847222222   X -3.144257910384336E+07 X  2.863711615688864E+01
    //                      Y -1.487025172848707E+08 Y -6.383300996646371E+00
    //                      Z -1.077397618608177E+04 Z -1.305441059544776E-04
    //                      =================================================
    //                      R  1.519903764836356E+08 V  29.33992081471865
    //
    //  2453164.847916667   X -3.144086087363915E+07 X  2.863718899129001E+01
    //                      Y -1.487029002730382E+08 Y -6.382963451199504E+00
    //                      Z -1.077398401882499E+04 Z -1.305474809920604E-04
    //                      =================================================
    //                      R  1.519903957431765e+08 V  29.33991846885831
    // Interpolating between these times yields
    rest2021 = 1.519903764836356E+11 * 0.19693370052752313 + 1.519903957431765e+11 * 0.8030662994724768;
    vest2021 = 29.33992081471865E+03 * 0.19693370052752313 + 29.33991846885831E+03 * 0.8030662994724768;

    switch(de_model)
    {
        case 405:
            threshold = 1e-12;
            rest = rest2010;
            vest = vest2010;
            break;
        // Didn't grab HORIZONS est. at time of release, so bigger threshold
        case 421:
            threshold = 1e-08;
            rest = rest2010;
            vest = vest2010;
            break;
        case 440:
            threshold = 1e-12;
            rest = rest2021;
            vest = vest2021;
            break;
        default:
            std::printf("\n"
                        "-----------------"
                        "Not an acceptable model"
                        "-----------------\n");
            return false;
    }

    robs = Vector3::vmag(embary_inertial.state.trans.position);
    vobs = Vector3::vmag(embary_inertial.state.trans.velocity);

    rerr = 1.0 - robs / rest;
    verr = 1.0 - vobs / vest;

    rpass = std::abs(rerr) < threshold;
    vpass = std::abs(verr) < threshold;
    passed_all = passed_all && rpass && vpass;

    std::printf("\nEarth-Moon barycenter position and velocity check:\n"
                "  Position vector magnitude at 08:19:44 UTC June 8, 2004\n"
                "    value from JPL Horizons             = %8.3e m\n"
                "    value from JEOD ephemeris model     = %8.3e m\n"
                "    relative error                      = %s\n\n"
                "  Velocity vector magnitude at 08:19:44 UTC June 8, 2004\n"
                "    value from JPL Horizons             = %8.3e m/s\n"
                "    value from JEOD ephemeris model     = %8.3e m/s\n"
                "    relative error                      = %s\n\n",
                rest,
                robs,
                color.pass_fail(rpass, "%.3g", rerr).c_str(),
                vest,
                vobs,
                color.pass_fail(vpass, "%.3g", verr).c_str());

    // Calculate the states of the Sun and Venus at the time at which
    // light from the two bodies would reach the center of the Earth.
    ttravel = Vector3::vmag(venus_wrt_earth.trans.position) / constants->vlight;

    // Update the ephemerides to reflect light speed.
    // We need to do this without munging the Earth state.
    embary_state = embary_inertial.state;
    earth_state = earth_inertial.state;

    time_manager.update(tref - ttravel);
    ephem_manager.update_ephemerides();
    embary_inertial.state = embary_state;
    earth_inertial.state = earth_state;

    // Recompute the Venus-Earth relative states.
    venus_inertial.compute_relative_state(earth_inertial, venus_wrt_earth);

    // Do it again, this time for the Sun.
    ttravel = Vector3::vmag(sun_wrt_earth.trans.position) / constants->vlight;
    time_manager.update(tref - ttravel);
    ephem_manager.update_ephemerides();
    embary_inertial.state = embary_state;
    earth_inertial.state = earth_state;

    sun_inertial.compute_relative_state(earth_inertial, sun_wrt_earth);

    // Calculate unit vectors from the Earth to Venus and the Sun.
    Vector3::normalize(venus_wrt_earth.trans.position, venus_earth_hat);
    Vector3::normalize(sun_wrt_earth.trans.position, sun_earth_hat);

    /*  ref: http://sunearth.gsfc.nasa.gov */
    thetaexp = 626.9;
    thetaobs = acos(Vector3::dot(venus_earth_hat, sun_earth_hat)) * 180.0 / M_PI * 3600.0;
    thetaerr = thetaexp - thetaobs;
    thetapass = std::abs(thetaerr) < 0.05;

    std::printf("\nVenus-Earth-Sun angular separation check:\n"
                "  Time = 08:19:44 UTC June 8, 2004\n"
                "   Expected value                      = %.1f arc sec\n"
                "   Computed value                      = %.1f arc sec\n"
                "   error                               = %s arc sec\n",
                thetaexp,
                thetaobs,
                color.pass_fail(thetapass, "%.3f", thetaerr).c_str());

    passed_all = passed_all && thetapass;

    std::printf("\nVenus transit test status: %s\n\n"
                "-----------------"
                "----------------------------------------------"
                "-----------------\n",
                color.pass_fail(passed_all).c_str());

    return passed_all;
}

static bool dataset_test(const De4xxEphemeris & de4xx_ephem,
                         TimeManager & time_manager,
                         EphemeridesManager & ephem_manager)
{
    bool passed_all = true;
    FILE * testdata;

    char testpo[32];
    char line_hdr[6];
    char buf[256];
    double pv[2][3];
    double jdate, jtime, obs, val, del;
    int in_data, iline;
    bool velocity_test;
    bool angle_test;
    bool rate_test;
    const char * ang_name[2] = {"Nutation ", "Libration"};
    int nrel_checked[13][13][2];
    int nang_checked[2][2];
    int nchecked, nbad;
    int itgt, ictr, icoord;
    bool item_passed;
    bool verbose_hdr_printed = false;

    // Write a test signature.
    std::printf("\n\n"
                "-----------------------------"
                "Ephemeris dataset test"
                "-----------------------------\n");

    // Construct the name of the file to be tested and
    // the line header based on the DE model number.
    std::sprintf(testpo, "./testpo.%d", de_model);
    std::sprintf(line_hdr, "%d  ", de_model);

    in_data = 0;
    iline = 0;
    for(itgt = 0; itgt < 13; itgt++)
    {
        for(ictr = 0; ictr < 13; ictr++)
        {
            nrel_checked[itgt][ictr][0] = nrel_checked[itgt][ictr][1] = 0;
        }
    }
    for(itgt = 0; itgt < 2; itgt++)
    {
        nang_checked[itgt][0] = nang_checked[itgt][1] = 0;
    }
    nchecked = nbad = 0;

    testdata = fopen(testpo, "r");
    if(testdata != nullptr)
    {
        while(fgets(buf, sizeof(buf), testdata) != nullptr)
        {
            iline++;

            // Data starts after EOT marker.
            if(!in_data)
            {
                in_data = (strncmp(buf, "EOT", 3) == 0);
            }
            else
            {
                // The line should start with the model number.
                if(strncmp(buf, line_hdr, 5) != 0)
                {
                    fprintf(stderr, "Bad line '%s'\n", buf);
                }
                else
                {
                    // Scan the line.
                    if(sscanf(buf + 16, "%lf %d %d %d %lf", &jdate, &itgt, &ictr, &icoord, &val) != 5)
                    {
                        fprintf(stderr, "Bad line '%s'\n", buf);
                    }
                    else
                    {
                        // Check for valid indices.
                        if((itgt < 1) || (itgt > 15) || (ictr < 0) || (ictr > 15) || (icoord < 1) || (icoord > 6))
                        {
                            fprintf(stderr, "Bad line '%s'\n", buf);
                        }
                        else
                        {
                            // The De4xxEphemeris doesn't support nutations.
                            if(itgt != 14)
                            {
                                // Update the time.
                                jtime = (jdate - jdate_2000) * 86400;
                                time_manager.update(jtime);

                                // Skip test point if date out-of-range.
                                if(de4xx_ephem.time_is_in_range())
                                {
                                    // Update the ephemerides to the specified time.
                                    time_manager.update(jtime);
                                    ephem_manager.update_ephemerides();

                                    // Convert Fortran coordinate to C and determine type (pos or vel).
                                    icoord--;
                                    angle_test = (itgt > 13);
                                    rate_test = (angle_test && (icoord >= 3));
                                    velocity_test = ((!angle_test) && (icoord >= 3));

                                    pleph(pv, ephem_manager, itgt, ictr);

                                    // Scale observation from r, r/s, M, M/s to r, r/day, au, au/day.
                                    if(rate_test)
                                    {
                                        obs = pv[1][icoord - 3] * 86400.0;
                                    }
                                    else if(angle_test)
                                    {
                                        obs = pv[0][icoord];
                                    }
                                    else if(velocity_test)
                                    {
                                        obs = pv[1][icoord - 3] * 86400.0 / constants->au;
                                    }
                                    else
                                    {
                                        obs = pv[0][icoord] / constants->au;
                                    }

                                    // Compute error and test for variance.
                                    // Note: tgt 15, coord 3 hack is per JPL testeph.f.
                                    del = obs - val;
                                    if((itgt == 15) && (icoord == 2))
                                    {
                                        del /= 0.23 * (jdate - 2451545.0);
                                    }

                                    item_passed = !(std::isnan(del) || (del < -1e-13) || (del > 1e-13));
                                    if((verbose > 1) || ((verbose > 0) && (!item_passed)))
                                    {
                                        if(!verbose_hdr_printed)
                                        {
                                            verbose_hdr_printed = true;
                                            std::printf("%4s %3s %3s  %-22s %-22s %-9s\n",
                                                        "line",
                                                        "tgt",
                                                        "ctr",
                                                        "observed",
                                                        "expected",
                                                        "error");
                                        }
                                        std::printf("%4d %3d %3d %22.15e %22.15e %s\n",
                                                    iline,
                                                    itgt,
                                                    ictr,
                                                    obs,
                                                    val,
                                                    color.pass_fail(item_passed, "%9.2e", del).c_str());
                                    }
                                    passed_all = passed_all & item_passed;
                                    if(!item_passed)
                                    {
                                        nbad++;
                                    }
                                    nchecked++;
                                    if(angle_test)
                                    {
                                        nang_checked[itgt - 14][rate_test]++;
                                    }
                                    else
                                    {
                                        nrel_checked[itgt - 1][ictr - 1][velocity_test]++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        fclose(testdata);
    }

    std::printf("\nRelative position tests: "
                "Count of tested pairs, by planet indices\n");
    std::printf("   | ");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf(" %3d", itgt);
    }
    std::printf("\n");
    std::printf("---+-");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf("----");
    }
    std::printf("\n");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf("%2d | ", itgt);
        for(ictr = 1; ictr <= 13; ictr++)
        {
            if(ictr == itgt)
            {
                std::printf("   -");
            }
            else
            {
                std::printf(" %3d", nrel_checked[itgt - 1][ictr - 1][0]);
            }
        }
        std::printf("\n");
    }

    std::printf("\nRelative velocity tests: "
                "Count of tested pairs, by planet indices\n");
    std::printf("   | ");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf(" %3d", itgt);
    }
    std::printf("\n");
    std::printf("---+-");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf("----");
    }
    std::printf("\n");
    for(itgt = 1; itgt <= 13; itgt++)
    {
        std::printf("%2d | ", itgt);
        for(ictr = 1; ictr <= 13; ictr++)
        {
            if(ictr == itgt)
            {
                std::printf("   -");
            }
            else
            {
                std::printf(" %3d", nrel_checked[itgt - 1][ictr - 1][1]);
            }
        }
        std::printf("\n");
    }
    std::printf("\nPlanet indices:\n"
                "   1 = Mercury           8 = Neptune\n"
                "   2 = Venus             9 = Pluto\n"
                "   3 = Earth            10 = Moon\n"
                "   4 = Mars             11 = Sun\n"
                "   5 = Jupiter          12 = Solar system barycenter\n"
                "   6 = Saturn           13 = Earth-Moon barycenter\n"
                "   7 = Uranus\n");

    std::printf("\nNutation, libration tests: "
                "Count of tested items\n");
    std::printf("%-9s | Angle  Rate\n", "Item");
    std::printf("----------|------------\n");
    for(itgt = 0; itgt < 2; itgt++)
    {
        std::printf("%-9s | %5d %5d\n", ang_name[itgt], nang_checked[itgt][0], nang_checked[itgt][1]);
    }

    std::printf("\n\nEphemeris test dataset test results:\n"
                "  # Passed: %d\n"
                "  # Failed: %d\n"
                "  Status  : %s\n\n",
                nchecked - nbad,
                nbad,
                color.pass_fail(passed_all).c_str());

    std::printf("-----------------------------"
                "----------------------"
                "-----------------------------\n");

    return passed_all;
}

/* Numbering for itgt, ictr:
 *   1 = MERCURY           8 = NEPTUNE
 *   2 = VENUS             9 = PLUTO
 *   3 = EARTH            10 = MOON
 *   4 = MARS             11 = SUN
 *   5 = JUPITER          12 = SOLAR-SYSTEM BARYCENTER
 *   6 = SATURN           13 = EARTH-MOON BARYCENTER
 *   7 = URANUS
 */
static void pleph(double pv[2][3], EphemeridesManager & ephem_manager, int itgt, int ictr)
{
    /* Mapping from Fortran/PLEPH bodies to JEOD frames */
    static const char * frame_names[14] = {nullptr, /* Fortran numbering starts at 1 */
                                           "Mercury.inertial",
                                           "Venus.inertial",
                                           "Earth.inertial",
                                           "Mars.inertial",
                                           "Jupiter.inertial",
                                           "Saturn.inertial",
                                           "Uranus.inertial",
                                           "Neptune.inertial",
                                           "Pluto.inertial",
                                           "Moon.inertial",
                                           "Sun.inertial",
                                           "SSBary.inertial",
                                           "EMBary.inertial"};

    // Compute relative state for targets 1-13.
    if(itgt <= 13)
    {
        RefFrame * tgt_frame = ephem_manager.find_ref_frame(frame_names[itgt]);
        RefFrame * ctr_frame = ephem_manager.find_ref_frame(frame_names[ictr]);
        RefFrameState rel_state;
        tgt_frame->compute_relative_state(*ctr_frame, rel_state);
        Vector3::copy(rel_state.trans.position, pv[0]);
        Vector3::copy(rel_state.trans.velocity, pv[1]);
    }

    // Target 15 is lunar orientation ("libration")
    else if(itgt == 15)
    {
        auto * lunar_angles = dynamic_cast<EphemerisZXZOrientation *>(ephem_manager.find_ephem_angle("Moon.pfix"));
        lunar_angles->get_euler_angles(pv[0]);
        lunar_angles->get_euler_rates(pv[1]);
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, EphemeridesMessages::internal_error, "Unsupported item %d", itgt);
    }
}
