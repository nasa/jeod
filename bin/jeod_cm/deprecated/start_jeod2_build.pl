#!/usr/bin/perl -w
######################################################################
#
# start_jeod2_build.pl: JEOD Nightly Build Process script for JEOD 2.x
#
######################################################################

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd 'abs_path';
use File::Path;
use File::Copy;
use IO::Handle;

sub main();
sub need_checkout();
sub checkout();
sub set_environment();
sub print_environment();
sub compile_sims();
sub uncompile_sims();
sub run_sims();
sub compile_and_run();
sub clean_runs();
sub compile_jeod_lib();
sub run_unit_tests();
sub build_docs();
sub archive_build_status();

# Global variables
my (
    # Constants
    $Start_time,                   # start time of this script
    $End_time,                     # end time of this script
    $Help,                         # -help
    $Path,                         # -path
    $Clean,                        # -clean
    $Checkout,                     # -checkout
    $Cp,                           # -cp
    $Run,                          # -run
    $Lib,                          # -lib
    $Unit,                         # -unit
    $Docs,                         # -docs
    $Archive,                      # -archive
    $Svn,                          # Svn command path
    $Trunk_url,                    # JEOD trunk location on Nexus
    $Home_dir,                     # User home dir
    $Working_dir,                  # Nightly Build working directory
    $Archive_dir_name,             # Directory name of nightly build status archive
    $Archive_dir,                  # Directory to archive nighly build status
    $Webserver,                    # Hostname of webserve (nexus)
    $Webserver_user,               # Username of webserver
    $Webserver_user_dir,           # User home of webserver
    $Trick10,                      # Trick 10 Flag
    $Trick_home,                   # Trick home
    $Trick_catalog,                # Trick catalog dir
    $Jeod_home,                    # Top level JEOD directory
    $Current_day,                  # Day of build execution
    $Current_month,                # Month of build execution
    $Current_year,                 # Year of build execution
    $Os_name                       # Operating system type
   );

$Start_time = time();

#FIXME: update the script for Git
$Svn = `which svn`;
chomp $Svn;
$Trunk_url = "https://github.com/nasa/jeod/tree/main";
$Home_dir = "$ENV{HOME}";
$Working_dir = "$Home_dir/nightly_build";
$Archive_dir_name = 'nightly_build_archive';
$Archive_dir = "$Home_dir/$Archive_dir_name";
$Webserver = 'https://github.com';
$Webserver_user = '';
$Webserver_user_dir = "/users/$Webserver_user";
$Os_name = $^O;

# Set default command line options
$Help         = 0;
$Checkout     = 1;
$Cp           = 1;
$Run          = 1;
$Lib          = 1;
$Unit         = 1;
$Docs         = 1;
$Archive      = 1;
$Trick10      = 0;
$Path         = 0;
$Clean        = 0;

# Parse the command line
GetOptions (
   'help'             => \$Help,
   'clean'            => \$Clean,
   'path:s'           => \$Path,
   'trick10'          => \$Trick10,
   'checkout!'        => \$Checkout,
   'cp!'              => \$Cp,
   'run!'             => \$Run,
   'lib!'             => \$Lib,
   'unit!'            => \$Unit,
   'docs!'            => \$Docs,
   'archive!'         => \$Archive)
    or pod2usage(2);

pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

main;

######################################################################
# Main function
######################################################################

sub main() {

   ($Current_day, $Current_month, $Current_year) = (localtime time)[3,4,5];
   $Current_month++;    # Month are starting from 0 to 11 so need to increment
   $Current_year += 1900; # Year is listed since 1900 so needs to add 1900

   $Current_day   = sprintf("%02d", $Current_day);  # formatted to two digit number
   $Current_month = sprintf("%02d", $Current_month); # formatter to two digit number

   checkout if ( $Checkout );

   print "Setting environment\n";
   set_environment;

   do {
      if ( $Cp and $Run ) {
         print "Compiling and Running sims\n";
         compile_and_run;
      } else {
         if ( $Cp )  {
            if ( $Clean ) {
               print "Cleaning compiled sims\n";
               uncompile_sims;
            } else {
               print "Compiling sims\n";
               compile_sims;
            }
         }

         if ( $Run ) {
            if ( $Clean ) {
               print "Cleaning SIM runs\n";
               clean_runs;
            } else {
               print "Running Sim runs\n";
               run_sims;
            }
         }
      }

      unless ( $Trick10 ) {
         print "Cleaning compiled sims\n";
         uncompile_sims;
      }

      # toggle Trick10 variable
      $Trick10 = $Trick10 ? 0 : 1;

   } while ( $Trick10 );

   if ( $Lib ) {
      print "Compiling JEOD library\n";
      compile_jeod_lib;
   }

   if ( $Unit ) {
      print "Running JEOD unit tests\n";
      run_unit_tests;
   }

   if ( $Docs ) {
      print "Making Documents\n";
      build_docs;
   }

   if ( $Archive ) {
      print "Archiving Nightly Build status\n";
      archive_build_status;
   }

   print "Done with Nightly Build.\n";

   $End_time = time();
   my $total_time = $End_time - $Start_time;
   my @time_parts = gmtime($total_time);
   printf("Completed Nightly Build in %2d hours %2d minutes and %2d seconds\n", @time_parts[2,1,0]);

   exit 0;
}


######################################################################
# Check if Build is necessary
######################################################################

sub need_checkout() {

   my $current_log = (`$Svn log -ql 1 $Trunk_url`);
   $current_log =~ m/^-.*\n(.*?-(\d+)-(\d+).*)\n/;
   my ($trunk_month, $trunk_day) = ($2,$3);

   if (($trunk_month == $Current_month) && ($trunk_day == $Current_day)) {
      return 1;
   } else {
      return 0;
   }
}

######################################################################
# Checkout of JEOD trunk from Subversion
######################################################################

sub checkout() {
   if ( ! need_checkout ) {
      print "No need to checkout\n";
      exit 1;
   }

   print "Checking out trunk on $Current_month-$Current_day\n";

  # Remove previous build if exist
  rmtree $Working_dir if -d $Working_dir;

  # Checkout a clean trunk in working directory
  mkdir $Working_dir;
  chdir $Working_dir;
  system ("$Svn checkout $Trunk_url >& /dev/null");
}

######################################################################
# Set Environment Variables
######################################################################

sub set_environment() {

   $Jeod_home        = "$Working_dir/trunk";
   $ENV{'JEOD_HOME'} = $Jeod_home;
   $ENV{'PATH'} = "$Jeod_home/bin/jeod_cm:$ENV{'PATH'}";

   my $os = $^O;
   if ( $os eq "darwin" ) {
      my $doxygen_path = "/Applications/Doxygen.app/Contents/Resources";
      $ENV{'PATH'} = "$doxygen_path:$ENV{'PATH'}";
   }

}

sub print_environment() {

   my @environments = qw(JEOD_HOME
                         PATH);

   foreach my $env_var (@environments) {
      print "\$$env_var \t= " . $ENV{"$env_var"} . "\n";
   }

}

######################################################################
# Compiles all the JEOD sims
######################################################################

sub compile_sims() {

   if ( $Path ) {
      system ("jeod_auto_cm.pl -env -cp -path $Path") unless $Trick10;
      system ("jeod_auto_cm.pl -env -cp -trick10 -path $Path") if $Trick10;
   } else {
      system ("jeod_auto_cm.pl -env -cp") unless $Trick10;
      system ("jeod_auto_cm.pl -env -cp -trick10") if $Trick10;
   }
}

######################################################################
# Uncompiles all the JEOD sims
######################################################################

sub uncompile_sims() {
   if ( $Path ) {
      system ("jeod_auto_cm.pl -env -cp -clean -path $Path") unless $Trick10;
      system ("jeod_auto_cm.pl -env -cp -clean -trick10 -path $Path") if $Trick10;
   } else {
      system ('jeod_auto_cm.pl -env -cp -clean') unless $Trick10;
      system ('jeod_auto_cm.pl -env -cp -clean -trick10') if $Trick10;
   }
}

######################################################################
# Compile and Run JEOD Simulations
######################################################################

sub compile_and_run() {

   if ( $Path ) {
      system ("jeod_auto_cm.pl -env -cp -run -path $Path") unless $Trick10;
      system ("jeod_auto_cm.pl -env -cp -run -trick10 -path $Path") if $Trick10;;
   } else {
      system ('jeod_auto_cm.pl -env -cp -run') unless $Trick10;
      system ('jeod_auto_cm.pl -env -cp -run -trick10') if $Trick10;;
   }
}

######################################################################
# Run JEOD Simulations
######################################################################

sub run_sims() {

   if ( $Path ) {
      system ("jeod_auto_cm.pl -env -run -path $Path") unless $Trick10;
      system ("jeod_auto_cm.pl -env -run -trick10 -path $Path") if $Trick10;;
   } else {
      system ('jeod_auto_cm.pl -env -run') unless $Trick10;
      system ('jeod_auto_cm.pl -env -run -trick10') if $Trick10;;
   }
}

######################################################################
# Clean JEOD Simulation RUNS
######################################################################

sub clean_runs() {
   if ( $Path ) {
      system ("jeod_auto_cm.pl -env -run -clean -path $Path") unless $Trick10;
      system ("jeod_auto_cm.pl -env -run -clean -trick10 -path $Path") if $Trick10;
   } else {
      system ('jeod_auto_cm.pl -env -run -clean') unless $Trick10;
      system ('jeod_auto_cm.pl -env -run -clean -trick10') if $Trick10;
   }
}

######################################################################
# Compile JEOD Library
######################################################################

sub compile_jeod_lib() {
   system ('jeod_auto_cm.pl -env -lib');
}

######################################################################
# Run JEOD Unit tests
######################################################################

sub run_unit_tests() {
   system ('jeod_auto_cm.pl -env -unit');
}

######################################################################
# Build Documents (including pdfs and doxygen api)
######################################################################

sub build_docs() {
   system ('jeod_auto_cm.pl -env -pdfdocs');
   system ('jeod_auto_cm.pl -env -apidoc');
}

######################################################################
# Archive JEOD Nightly Build status
######################################################################

sub archive_build_status() {
   mkdir $Archive_dir unless -d $Archive_dir;
   mkdir $Archive_dir . "/" . $Os_name unless -d "$Archive_dir/$Os_name";

   my $current_build_dir = $Current_month . "-" . $Current_day . "-" . $Current_year;

   mkdir "$Archive_dir/$Os_name/$current_build_dir" unless -d "$Archive_dir/$Os_name/$current_build_dir";

   chdir $Jeod_home;

   open FIND, 'find . -maxdepth 1 -name "*_error"|' or die;
   my @error_files = <FIND>;
   close FIND;

   map chomp, @error_files;
   map s/\.\///, @error_files;

   foreach my $file (@error_files) {
      copy($file, "$Archive_dir/$Os_name/$current_build_dir/$file") or die "Cannot copy $file to $Archive_dir/$Os_name/$current_build_dir\n";
   }

   system ("scp -r $Archive_dir/$Os_name/$current_build_dir $Webserver_user\@$Webserver:$Webserver_user_dir/$Archive_dir_name/$Os_name/.");
}
######################################################################

__END__

=head1 NAME

start_jeod_build.pl -
A script to do JEOD nightly build

=head1 SYNPOSIS

start_jeod_build.pl [options]
[ -checkout | -nocheckout ]
[ -cp | -nocp ]
[ -run | -norun ]
[ -lib | -nolib ]
[ -unit | -nounit ]
[ -docs | -nodocs ]
[ -archive | -noarchive ]

=head1 OPTIONS

=over 2

=item

-help

Print this page and exit.

=item

-checkout (default) / -nocheckout

Check out JEOD trunk to do nightly build, a must have option to go any further.

=item

-cp (default) / -nocp

Compiles all JEOD sims.

=item

-run (default) / -norun

Runs all the simulation specific runs by default.

=item

-lib (default) / -nolib

Build JEOD library by default.

=item

-unit (default) / -nounit

Runs all the unit tests by default.

=item

-docs (default) / -nodocs

Only build document in linux and will also build JEOD API if on Mac machine
by default.

=back

=head1 DESCRIPTION

The start_jeod_build.pl scripts does the nightly build tast when trunk's content changes

=cut
