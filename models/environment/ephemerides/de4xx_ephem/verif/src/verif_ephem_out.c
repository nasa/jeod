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
    ((verif_ephem_out.o))


******************************************************************************/

/* System includes */
//#include <math.h>
#include <stdio.h>

#include "environment/ephem/include/pv_state.h"
#include "environment/ephem/include/ephemerides.h"


/* Entry Point */
int verif_ephem_out(                    /* RETURN: --  Always returns zero                      */
   Ephemerides    * ephem,              /* IN      --  ephemerides structure                    */
   double           rel_pos[3],         /* IN      M   relative position                        */
   PVState        * rel_state )         /* IN      --  relative state                           */

{

double km = 1000.0;  // km in meters

printf("**************************************************************************************\n\n");

printf("Position vector from ephemerides_rel_pos.c routine:\n\n");
printf("Pos km  %25.15E %25.15E %25.15E \n\n\n",rel_pos[0]/km,rel_pos[1]/km,rel_pos[2]/km);

printf("State vector from ephemerides_rel_state.c routine:\n\n");
printf("Pos km  %25.15E %25.15E %25.15E \n",rel_state->pos[0]/km,rel_state->pos[1]/km,rel_state->pos[2]/km);
printf("Vel km/s%25.15E %25.15E %25.15E \n\n",rel_state->vel[0]/km,rel_state->vel[1]/km,rel_state->vel[2]/km);

printf("**************************************************************************************\n");

return(0);

}
