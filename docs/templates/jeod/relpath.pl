#!/usr/bin/perl

#####################################################################
# Purpose:
#    Determine path of first directory relative to second,
#    the latter of which defaults to the current working directory.
#    The second directory must be subsidiary to the first.
#####################################################################

use strict;
use Cwd 'abs_path';
use Getopt::Long;


my $down = 0;

# Process the command line options.
GetOptions (
   'down!' => \$down
) or die "Usage: $0 [-down] <parent> <child>\n";


my $target = shift;
my $source = shift;
my $relpath;

$source = '.' unless (defined $source);

die "Usage: $1 <target> [<source>]\n"
  unless (defined $target) && (-d $target) && (-d $source);

$target = abs_path $target;
$source = abs_path $source;

if ($source eq $target) {
   print ".\n";
}
elsif (($relpath = $source) =~ s|^$target/||) {
   $relpath =~ s|[^/][^/]*|..|g unless $down;
   print "$relpath\n";
} else {
   die "Can't find $source in $target\n";
}
