#!/usr/bin/perl -w
############################################################################
#
# set_ignores: Sets svn ignored on a all arguments directory.
#
############################################################################

use strict;
#use Cwd;
use Getopt::Long;
use IO::Handle;

# global variables
my (
   $Help,            # Help!
   @directories      # set ignore property on this dir
   #$curdir           # current directory this script executed from
   );

$Help = 0;

sub main();
sub usage();

main;

sub main() {

   GetOptions (
      'help'         => \$Help)
      or pod2usage(2);
   pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

   if ($#ARGV < 0) {
      @directories = (".");
   } else {
      @directories  = @ARGV;
   }

   #$curdir = getcwd;

   # change directory to one that needs ignores list set
   foreach my $directory (@directories) {
      system ("set_svn_ignores_per_directory.pl $directory");
   }

   #chdir $curdir;
}

sub usage() {
   print  "\n Usage: \n";
   print  "\tset_ignores.pl <directory_list>\n\n";
   exit 1;
}
