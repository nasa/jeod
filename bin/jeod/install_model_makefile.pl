#!/usr/bin/perl -w
################################################################################
#
# Purpose:
#   (Establish a relative path link to the template model makefile.
#
#  
#
################################################################################

use strict;
use Cwd;

my $cwd = getcwd;
my ($type, $name);
if ($cwd =~ /^\Q$ENV{JEOD_HOME}\E\/models\/([^\/]+)\/([^\/]+)\/docs\/tex$/) {
   ($type, $name) = ($1, $2);
}
else {
   die "Working directory is not a JEOD model document directory.\n";
}


if (-e 'makefile') {
   die "A makefile already exists. Aborting.\n";
}

print "Linking in the standard model document makefile.\n";
symlink '../../../../../docs/templates/jeod/model_makefile', 'makefile'
   or die "Couldn't link the model makefile.\n";

if (! -e 'model_name.mk') {
   print "Creating the default model_name.mk for this directory.\n";
   print "The machine-generated names may not be correct. Please check.\n";
   open OUT, '>', 'model_name.mk' or die;
   print OUT "MODEL_TYPE  = $type\n";
   print OUT "MODEL_NAME  = $name\n";
   print OUT "MODEL_TITLE = '\u$name Model'\n";
   close OUT;
}
