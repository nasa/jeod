#########################################################################
# JEOD_HOME should be set to top level directory path of a JEOD package #
# For Example,                                                          #
#       if your path to JEOD package is /usr/local/jeod/jeod_v2.0, then #
#       setenv JEOD_HOME "/usr/local/jeod/jeod_v2.0"                    #
#########################################################################

if (! $?JEOD_HOME) then
   echo 
   echo "Environment Variable JEOD_HOME is not set"
   echo
endif

set JEOD_CFLAGS="-Wall -I${JEOD_HOME}/models"

if ($?TRICK_VER) then
   set TMAJOR=`echo $TRICK_VER | cut -d. -f1`
   if ($TMAJOR < 04) then
      set TMINOR=`echo $TRICK_VER | cut -d. -f2`
      set JEOD_CFLAGS="-DTRICK_VER=$TMAJOR -DTRICK_MINOR=$TMINOR $JEOD_CFLAGS"
      unset TMINOR
   else if ($TMAJOR < 07) then
      set TMINOR=`echo $TRICK_VER | cut -d. -f2`
      set JEOD_CFLAGS="-DTRICK_MINOR=$TMINOR $JEOD_CFLAGS"
      unset TMINOR
   else
   endif
endif
if ($?TRICK_CFLAGS) then
   setenv TRICK_CFLAGS "$TRICK_CFLAGS $JEOD_CFLAGS"
else
   setenv TRICK_CFLAGS "$JEOD_CFLAGS"
endif

unset JEOD_CFLAGS
unset TRICK_VER
