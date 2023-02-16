#!/usr/bin/perl -w
############################################################################
#
# set_svn_ignores_per_directory.pl: Sets svn ignored on a given directory
#                                   as an argument.
#
############################################################################

use strict;
use Cwd;
use Getopt::Long;
use IO::Handle;

# global variables
my (
   $Svn,              # svn path
   $Help,             # Help!
   $directory,        # set ignore property on this dir
   $curdir,           # current directory this script executed from
   @prev_list,        # previous ignore lists
   @new_list,         # new ignore lists
   @ignore_list,      # newly formed list including old and new lists
   );

$Help = 0;
$Svn = `which svn`;
chomp $Svn;

sub main();
sub usage();

main;

sub main() {

   GetOptions (
      'help'         => \$Help,
      'svn=s'        => \$Svn)
      or pod2usage(2);
   pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

   if ($#ARGV < 0) {
      $directory = ".";
   } else {
      $directory = "$ARGV[0]";
   }

   $curdir = getcwd;
   # change directory to one that needs ignores list set
   chdir $directory or die "Can't cd to path:$directory \n";

   open LIST, "$Svn propget svn:ignore .|" or die;
   @prev_list = <LIST>;
   close LIST;

   open NEW, "$Svn status | grep '^?' | awk '{print \$2}'|" or die;
   @new_list = <NEW>;
   close NEW;

   if (scalar(@new_list) == 0) {
      print "No new ignore files are in $directory\n";
      print "$directory is clean of ignore list\n";
      exit 2;
   }

   @ignore_list = (@prev_list, @new_list);
   my $host_cpu = $ENV{TRICK_HOST_CPU};

   open TEMP, ">temp.txt" or die $!;
   foreach my $file (@ignore_list) {
   chomp $file;
      if ($file eq "" or $file eq "temp.txt") {
         next;
      }
      if ($file =~ m/$host_cpu/) {
         $file =~ s/$host_cpu/\*/;
      }

   print TEMP "$file\n";
   }

   system ("$Svn propset svn:ignore -F temp.txt .");
   unlink "temp.txt";

   chdir $curdir;
}

sub usage() {
   print  "\n Usage: \n";
   print  "\tset_svn_ignores_per_directory.pl <directory_path>\n\n";
   exit 1;
}
