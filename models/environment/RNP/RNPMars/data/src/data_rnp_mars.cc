/*******************************************************************************

Purpose:
(Mars RNP matrix parameter initialization.)

Reference:
(((Konopliv, Alex S., et al)
(A global solution for the Mars static and seasonal gravity, Mars
orientation, Phobos and Deimos masses, and Mars ephemeris)
(Icarus, Volume 182, Issue 1, Pages 23-50) (May 2006))
((Konopliv, Alex S., et al)
(Mars high resolution gravity fields from MRO, Mars season gravity, and
other dynamical parameters)
(Icarus, Volume 211, Issue 1, Pages 401-428) (January 2011)))

Assumptions and limitations:
((Mars specific))

 
 *******************************************************************************/

class RNPMars;
#define JEOD_FRIEND_CLASS RNPMars_rnp_mars_default_data


// System includes
#include <cmath>

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "environment/RNP/GenericRNP/include/planet_rnp.hh"
#include "environment/RNP/RNPMars/include/rnp_mars.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/rnp_mars.hh"


//! Namespace jeod
namespace jeod {

void
RNPMars_rnp_mars_default_data::initialize (
		RNPMars * RNPMars_ptr)
{

	// NOTE: All values from Konopliv 2010 unless otherwise noted

	// Calculate degrees to radians conversion locally
	double deg2rad_local = M_PI / 180.0;

	// Calculate conversion from degrees per day to radians per second
	double degday2radsec = deg2rad_local * 1.0 / 86400.0;

	// Local conversion from milliarcseconds to radians
	double mas2rad_local = deg2rad_local/(1000.0*3600.0);

	// Set top-level default data values
	RNPMars_ptr->name = "Mars";
	RNPMars_ptr->rnp_type = PlanetRNP::FullRNP;
	RNPMars_ptr->enable_polar = false;
	RNPMars_ptr->planet_omega = 350.891985303 * degday2radsec;

	// Set rotation and precession default data values
	RNPMars_ptr->RMars.phi_at_j2000 = 133.38462 * deg2rad_local;

	RNPMars_ptr->PMars.psi_at_j2000 = 81.9683671267 * deg2rad_local;
	RNPMars_ptr->PMars.psi_dot = -0.000005756 * degday2radsec;
	RNPMars_ptr->PMars.N = 3.37919183 * deg2rad_local;
	RNPMars_ptr->PMars.J = 24.67682669 * deg2rad_local;


	// Set nutation default data values
	RNPMars_ptr->NMars.I_at_j2000 = 25.1893984585 * deg2rad_local;
	RNPMars_ptr->NMars.I_dot = -0.000000005 * degday2radsec;
	RNPMars_ptr->NMars.mean_motion = 0.52403958828 * degday2radsec;
	RNPMars_ptr->NMars.mean_anomaly_j2000 = 19.3564 * deg2rad_local;
	RNPMars_ptr->NMars.q_angle_j2000 = 142.00 * deg2rad_local;  // Konopliv 2006


	// Set up matrices required for nutation correction calcs
	RNPMars_ptr->NMars.I_m_orig =
		JEOD_ALLOC_PRIM_ARRAY (10, double);  // from Konopliv 2006
	RNPMars_ptr->NMars.I_m_orig[0] =  -1.4*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[1] =  -0.4*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[2] =   0.0*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[3] =   0.0*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[4] = -49.1*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[5] = 515.7*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[6] = 112.8*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[7] =  19.2*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[8] =   3.0*mas2rad_local;
	RNPMars_ptr->NMars.I_m_orig[9] =   0.4*mas2rad_local;


	RNPMars_ptr->NMars.psi_m_orig =
		JEOD_ALLOC_PRIM_ARRAY (10, double);  // from Konopliv 2006
	RNPMars_ptr->NMars.psi_m_orig[0] =    0.0*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[1] = -632.6*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[2] =  -44.2*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[3] =   -4.0*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[4] = -104.5*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[5] = 1097.0*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[6] =  240.1*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[7] =   40.9*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[8] =    6.5*mas2rad_local;
	RNPMars_ptr->NMars.psi_m_orig[9] =    1.0*mas2rad_local;


	RNPMars_ptr->NMars.int_to_double =
		JEOD_ALLOC_PRIM_ARRAY (10, double);
	RNPMars_ptr->NMars.int_to_double[0] = 0.0;
	RNPMars_ptr->NMars.int_to_double[1] = 1.0;
	RNPMars_ptr->NMars.int_to_double[2] = 2.0;
	RNPMars_ptr->NMars.int_to_double[3] = 3.0;
	RNPMars_ptr->NMars.int_to_double[4] = 4.0;
	RNPMars_ptr->NMars.int_to_double[5] = 5.0;
	RNPMars_ptr->NMars.int_to_double[6] = 6.0;
	RNPMars_ptr->NMars.int_to_double[7] = 7.0;
	RNPMars_ptr->NMars.int_to_double[8] = 8.0;
	RNPMars_ptr->NMars.int_to_double[9] = 9.0;

}
} // End JEOD namespace
