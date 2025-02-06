/******************************** TRICK HEADER ********************************
PURPOSE:
    (Display to screen the output of the ephemeris routines for verification
     purposes.)

REFERENCE:
    ((none))

ASSUMPTIONS AND LIMITATIONS:
    ((none))

CLASS:
    (scheduled)

LIBRARY DEPENDENCY:
    ((verif_ephemeris_sun_moon_out.o))


******************************************************************************/

/* System includes */
#include <stdio.h>

#include "environment/earth/include/ephemeris.h"
#include "environment/earth/include/univ_time.h"
#include "sim_services/include/exec_proto.h"


/* Entry Point */
int verif_ephemeris_sun_moon_out(  /* RETURN: --  Always returns zero                      */
   UTIME          * utime,         /* IN      --  time structure                           */
   double           del_time,      /* IN      --  delta time (sys exec time)               */
   EPHEM          * old_ephem)     /* IN      --  ephemeris structure                      */

{

double km = 1000.0; // km in meters

printf("**************************************************************************************\n\n");

printf("Date and Time: \n");
printf("JED           %17.9f \n",utime->out.jed);
printf("sim exec time %17.9f \n\n",del_time);

printf("Sun position vector from ephemeris_sun.c routine:\n\n");
printf("Pos km  %25.15E %25.15E %25.15E \n\n\n",old_ephem->Sun.inrtl[0]/km,
                                                old_ephem->Sun.inrtl[1]/km,
                                                old_ephem->Sun.inrtl[2]/km);

printf("Moon position vector from ephemeris_moon.c routine:\n\n");
printf("Pos km  %25.15E %25.15E %25.15E \n\n",old_ephem->moon.inrtl[0]/km,
                                              old_ephem->moon.inrtl[1]/km,
                                              old_ephem->moon.inrtl[2]/km);

printf("**************************************************************************************\n");

return(0);

}
