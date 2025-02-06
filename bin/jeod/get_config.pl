#!/usr/bin/perl -w

################################################################################
#
# Purpose:
# Print the make configuration needed to build the JEOD.
#
# Usage:
# get_config [options]
#
# Options:
# The user can provide various options that alter the behavior of this script.
# These options are
#   -trick / -notrick (default)
#      The -trick / -notrick flags indicate whether the JEOD is to be built for
#      use in a Trick or non-Trick environment.
#      The default is to build in a non-Trick environment.
#   -utils <Trick utils source directory>
#   -tricklib <Trick libraries directory>
#
# Environment variables:
#   JEOD_HOME
#   TRICK_UTILS
#   TRICK_LIB
#   TRICK_HOME
#
################################################################################


eval 'exec perl -S $0 ${1+"$@"}'
if 0;

require 5.004;
use strict;
use Getopt::Long;
use FindBin qw($Bin);
use lib "$Bin";
use JeodConfig;

use vars qw(
   $Trick_build
   $Trick_utils
   $Trick_libdir
);


$Trick_build = 0;

GetOptions (
   'trick!'          => \$Trick_build,
   'utils|=s'        => \$Trick_utils,
   'tricklib|=s'     => \$Trick_libdir
) or die;



print get_config($Trick_build, $Trick_utils, $Trick_libdir);
