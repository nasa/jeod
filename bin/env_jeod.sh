#########################################################################
# JEOD_HOME should be set to top level directory path of a JEOD package #
# For Example,                                                          #
#       if your path to JEOD package is /usr/local/jeod/jeod_v2.0, then #
#       JEOD_HOME="/usr/local/jeod/jeod_v2.0"                           #
#       export JEOD_HOME                                                #
#########################################################################

if [ -z "$JEOD_HOME" ]; then
   JEOD_HOME=$(dirname $(dirname $(realpath "${BASH_SOURCE[0]}")))

   echo
   echo "Environment Variable JEOD_HOME is not set. Using relative location."
   echo
fi

JEOD_CFLAGS="-Wall -I${JEOD_HOME}/models"

if [ -n "$TRICK_VER" ] ; then
   TMAJOR=`echo $TRICK_VER | cut -d. -f1`
   if [ $TMAJOR -lt 04 ] ; then
      TMINOR=`echo $TRICK_VER | cut -d. -f2`
      JEOD_CFLAGS="-DTRICK_VER=$TMAJOR -DTRICK_MINOR=$TMINOR $JEOD_CFLAGS"
   elif [ $TMAJOR -lt 07 ] ; then
      TMINOR=`echo $TRICK_VER | cut -d. -f2`
      JEOD_CFLAGS="-DTRICK_MINOR=$TMINOR $JEOD_CFLAGS"
   fi
fi

if [ -n "$TRICK_CFLAGS" ] ; then
   export TRICK_CFLAGS="$TRICK_CFLAGS $JEOD_CFLAGS"
else
   export TRICK_CFLAGS="$JEOD_CFLAGS"
fi

unset JEOD_CFLAGS
unset TRICK_VER
