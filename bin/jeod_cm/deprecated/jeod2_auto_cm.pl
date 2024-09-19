#!/usr/bin/perl -w
######################################################################
#
# jeod2_auto_cm.pl: JEOD CM Scritps for JEOD 2.x
#
######################################################################

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd;
use File::Basename;
use File::Compare;
use File::Copy;

sub main();
sub set_environment();
sub print_environment();
sub jeod_sim_level_tasks();
sub find_sim_dir();
sub find_tex_dir();
sub compile();
sub remove_trick_stuff();
sub clean_run;
sub check_and_run;
sub check_sendhs;
sub wait_for_fork;
sub fork_run;
sub get_zombie_list;
sub kill_zombie_list;
sub run;
sub get_sim_runs;
sub make_jeod_lib();
sub unit_test();
sub find_unit_test_dir();
sub find_unit_tests;
sub clean_unit_test;
sub fork_unit_test;
sub build_pdf_docs();
sub find_tex_dirs();
sub build_pdf;
sub clean_pdf;
sub build_jeod_api();
sub verify();
sub get_run_data;
sub make_validation_data();
sub pretty_error_files();
sub process_cp_error;
sub process_run_error;
sub process_unit_error;

# Global variables
my (
    # Constants
    $Help,                         # -help
    $Environment,                  # -env
    $Jeod_home,                    # Top level JEOD directory
    $Trick_home,                   # Trick Home
    $Path,                         # Path
    $Trick10,                      # Trick 10 flag
    $Curdir,                       # Current working directory
    $Os_name,                      # Operating System type
    $CP,                           # Compile Sims flag
    $Run,                          # Run JEOD SET_test runs
    $Lib,                          # Build JEOD Library
    $Unit,                         # Build and run unit tests
    $Docs,                         # Build PDF docs
    $Api,                          # Build JEOD Doxygenate API Doc
    $Clean,                        # clean flag to do specific task
    $Verify,                       # Verify SET_test vs SET_test_val
    $Valflag,                      # Flag indicating to use machine specific data
    $Valpath,                      # Path to machine specific val data
    $EnvPath,                      # Global environment PATH variable
    $Make_val,                     # Make validation data for SET_test
    @Forked_run_pids              # forked pids of all the runs
   );

my $Start = time();
my $Finish;
my $Total_forks = 0;
my $Max_Processes = 0;
my $Total_CP = 0;
my $Failed_CP = 0;

$Curdir = getcwd;
$Os_name = $^O;
$EnvPath = $ENV{'PATH'};

# Set default command line options
$Help         = 0;
$Environment  = 0;
$CP           = 0;
$Run          = 0;
$Lib          = 0;
$Unit         = 0;
$Docs         = 0;
$Api          = 0;
$Clean        = 0;
$Verify       = 0;
$Valflag      = 0;
$Make_val     = 0;
$Trick10      = 0;
$Path         = 0;


# Parse the command line
GetOptions (
   'help'            => \$Help,
   'env'             => \$Environment,
   'trick10'         => \$Trick10,
   'path:s'          => \$Path,
   'cp'              => \$CP,
   'run:s'           => \$Run,
   'lib'             => \$Lib,
   'unit'            => \$Unit,
   'pdfdocs'         => \$Docs,
   'apidoc'          => \$Api,
   'clean'           => \$Clean,
   'verify:s'        => \$Verify,
   'valflag'         => \$Valflag,
   'mkval:s'         => \$Make_val)
    or pod2usage(2);

pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

# set global variable based on user option and arguments to
# this script

# Sanity checks
set_environment if $Environment;

die "\$JEOD_HOME needs to be set to top level directory of JEOD software."
   unless defined($ENV{'JEOD_HOME'});

unless ( $Environment ) {
   $Jeod_home = $ENV{'JEOD_HOME'};
   $Trick_home = $ENV{'TRICK_HOME'};
}


if ( $Path ) {
   chdir $Jeod_home;
   chdir $Path or die "Cannot chdir to $Path: $!\n";
   $Path = getcwd;
} else  {
   $Path = $Jeod_home;
   chdir $Jeod_home;
}

$Run = 'RUN_\*' if ( $Run eq '' );
$Verify = 'RUN_\*' if ( $Verify eq '' );
$Make_val = 'RUN_\*' if ( $Make_val eq '' );
$Valpath = $Jeod_home . '/' . $Os_name . '/';

main();

######################################################################
# Main function
######################################################################

sub main() {

   jeod_sim_level_tasks if ( $CP or $Run or $Verify or $Make_val );
   make_jeod_lib if $Lib;
   unit_test if $Unit;
   build_pdf_docs if $Docs;
   build_jeod_api if ( $Api && ( $Os_name eq 'darwin' ));
   pretty_error_files;

   $Finish = time();
   my $duration = $Finish - $Start;
   my @time_parts = gmtime($duration);
   printf("Completed in %2d hours %2d minutes and %2d seconds\n", @time_parts[2,1,0]);

   chdir $Curdir or die "Cannot chdir from which this script was executed: $!\n";

}

sub set_environment() {


   $ENV{'JEOD_HOME'}               = "$ENV{'HOME'}/nightly_build/trunk" unless ( defined $ENV{'JEOD_HOME'} );
   $ENV{'TRICK_VER'}               = '07.21.3' unless $Trick10;
   $ENV{'TRICK_VER'}               = '10.9.0' if $Trick10;
   $ENV{'TRICK_HOME'}              = "$ENV{'HOME'}/trick/$ENV{'TRICK_VER'}";
   $ENV{'TRICK_CFLAGS'}            = " -Wall -I$ENV{'JEOD_HOME'}/models";
   $ENV{'TRICK_CATALOG_HOME'}      = "$ENV{'HOME'}/nightly_build/catalog_$ENV{'TRICK_VER'}";
   $ENV{'TRICK_CAT_MGR_HOME'}      = "$ENV{'TRICK_HOME'}/catalog";
   $ENV{'TRICK_CC'}                = "cc";
   $ENV{'TRICK_CPPC'}              = "c++";
   $ENV{'MAKEFLAGS'}               = '-j 7';
   $ENV{'TRICK_DEBUG'}             = 0;
   $ENV{'TRICK_EDITOR'}            = "nedit";
   $ENV{'TRICK_USER_HOME'}         = $ENV{'JEOD_HOME'};
   $ENV{'TRICK_USER_LINK_LIBS'}    = ' -ltk -ltcl -ldl';
   my $trick_host_cpu              = (`$ENV{'TRICK_HOME'}/bin/gte TRICK_HOST_CPU`);
   chomp $trick_host_cpu;
   $ENV{'TRICK_HOST_CPU'}          = $trick_host_cpu;
   $ENV{'TRICK_PATH'}              = "$ENV{'TRICK_HOME'}/bin_$ENV{'TRICK_HOST_CPU'}:$ENV{'TRICK_HOME'}/bin";
   $ENV{'PATH'}                    = "$ENV{'JEOD_HOME'}/bin/jeod_cm:$EnvPath:$ENV{'TRICK_PATH'}";

   $Jeod_home  = $ENV{'JEOD_HOME'};
   $Trick_home = $ENV{'TRICK_HOME'};

   mkdir $ENV{'TRICK_CATALOG_HOME'} unless ( -d $ENV{'TRICK_CATALOG_HOME'} );

   if ( $Os_name eq "darwin" ) {
      my $doxygen_path = "/Applications/Doxygen.app/Contents/Resources";
      $ENV{'PATH'} = "$doxygen_path:$ENV{'PATH'}";
   }

   if ( $Os_name eq "linux" ) {
      my $tex_path = "$ENV{'HOME'}/jeod/texlive/2010/bin/x86_64-linux";
      $ENV{'PATH'} = "$tex_path:$ENV{'PATH'}";
   }

}

sub print_environment() {

   print "\nJEOD_HOME=$ENV{'JEOD_HOME'}";
   print "\nTRICK_CATALOG_HOME=$ENV{'TRICK_CATALOG_HOME'}";
   print "\nTRICK_CAT_MGR_HOME=$ENV{'TRICK_CAT_MGR_HOME'}";
   print "\nTRICK_CC=$ENV{'TRICK_CC'}";
   print "\nTRICK_CFLAGS=$ENV{'TRICK_CFLAGS'}";
   print "\nTRICK_CPPC=$ENV{'TRICK_CPPC'}";
   print "\nTRICK_DEBUG=$ENV{'TRICK_DEBUG'}";
   print "\nTRICK_EDITOR=$ENV{'TRICK_EDITOR'}";
   print "\nTRICK_VER=$ENV{'TRICK_VER'}";
   print "\nTRICK_HOME=$ENV{'TRICK_HOME'}";
   print "\nTRICK_HOST_CPU=$ENV{'TRICK_HOST_CPU'}";
   print "\nTRICK_MAKE=$ENV{'TRICK_MAKE'}";
   print "\nTRICK_PATH=$ENV{'TRICK_PATH'}";
   print "\nTRICK_USER_HOME=$ENV{'TRICK_USER_HOME'}";
   print "\nTRICK_USER_LINK_LIBS=$ENV{'TRICK_USER_LINK_LIBS'}";
   print "\nPATH=$ENV{'PATH'}";
   print "\n\n";

}

######################################################################
# Wrapper function to appropriately direct to specific function based
# on user input
######################################################################

sub jeod_sim_level_tasks() {

   my @jeod_sims = find_sim_dir;

   foreach my $sim (@jeod_sims) {

      chdir $Path or die "Cannot chdir to path $Path\n";
      chdir $sim or die "Cannot chdir to sim $sim: $!\n";

      compile if $CP;
      run if $Run;
      verify if $Verify;
      make_validation_data if $Make_val;

      my $forked_run = scalar (@Forked_run_pids);
      if ( $Run and $forked_run ) {
         my @zombie_list = get_zombie_list($0);
         foreach my $zombie_pid (@zombie_list) {
            waitpid($zombie_pid,0);
         }
      }

   }

   my $forked_run = scalar (@Forked_run_pids);
   if ( $Run  and $forked_run ) {
      foreach my $pid (@Forked_run_pids) {
         print "\n\n\t\tPID MATCHED TO THIS SCRIPT\n\n" if ( $pid == $$ );
         next if ( $pid == $$ );
         kill_zombie_process($pid);
      }
   }

}

sub get_zombie_list {

   my @pslist;
   my @zombie_pid_list;
   my $scriptname = shift;

   open ZOMBIE, "ps -e -o state,pid,user | grep '^Z' | grep $ENV{'USER'} |" or die "Cannot use ps command: $!\n";
   map chomp, (@pslist = <ZOMBIE>);
   close ZOMBIE;


   foreach my $list (@pslist) {
      my ($stat, $pid) = split('\s+', $list);
      push (@zombie_pid_list, $pid) if ( $stat =~ /^Z/ );
   }

   return @zombie_pid_list;

}

sub kill_zombie_process {

   my $pid = shift;
   my $stat;

   open STAT, "ps -p $pid -o stat,command | grep -vi STAT |" or die "Cannot use ps stat command: $!\n";
   $stat = <STAT>;
   close STAT;

   waitpid($pid,0) if $stat;

}

######################################################################
# Find all the sims under a user given path as an argument
######################################################################

sub find_sim_dir() {        # list of jeod sims

   my @jeod_sims;
   my $basename = basename($Path);
   my $grep_option = '';
   $grep_option = '-v' unless $Trick10;

   if ($basename =~ /SIM\_.*/) {
      @jeod_sims  = '.'
   } else {
      open FIND,
         "find . -name 'SIM_*' -type d | grep -v 'unsupported' | grep -v 'jeod_course' | grep $grep_option 'SIM_.*10' | sort |"
            or die;
      map chomp, (@jeod_sims = <FIND>);
      close FIND;
   }
   return @jeod_sims;

}

######################################################################
# Compile JEOD sims specified by user given path as an argument
######################################################################

sub compile() {

   my $simpath = getcwd;
   $simpath =~ s/$Jeod_home\///;

   if ( $Clean ) {
      #system ('make spotless >& /dev/null') if ( -e 'S_source.cpp' or -e 'S_source.c' );
      return if ( not (-e "S_main_$ENV{'TRICK_HOST_CPU'}.exe" or -e 'Makefile' ) );
      print "Make spotless SIM $simpath ...\n";
      system ('make spotless >& /dev/null');
   } else {
      # check if sim is already have been compiled
      return if ( -e "S_main_$ENV{'TRICK_HOST_CPU'}.exe" );

      system ('catalog -i >& /dev/null');
      sleep 1;
      remove_trick_stuff;
      system ('make spotless >& /dev/null') if ( -e 'S_source.cpp' or -e 'S_source.c' );
      sleep 1;
      $Total_CP += 1;
      print "Compiling SIM $simpath ...\n";
      system ("$Trick_home/bin/CP >& /dev/null");
      if ( ! -e "S_main_$ENV{'TRICK_HOST_CPU'}.exe" ) {
         $Failed_CP += 1;
         my $error_file = 'CP_error';
         open CPERROR, ">>$Jeod_home/$error_file";
         print CPERROR "SIM $simpath Failed ...\n";
         close CPERROR;
      }
   }
}

sub remove_trick_stuff() {

    print "Removing Trick's autogenerated swig, io_src, xml and object_* directories ...";
    system("find $Jeod_home -name swig -o -name io_src -o -name object_$ENV{'TRICK_HOST_CPU'} -o -name xml  | xargs rm -rf");
    print " Done\n";

}

######################################################################
# RUN JEOD sim's SET_test specified by user given path as an argument
######################################################################

sub run {

   my @non_restart_pids;
   my @restart_runs;
   my $have_restart_runs = 0;

   my $simpath = getcwd;
   $simpath =~ s/$Jeod_home\///;
   my $error_file = 'run_error';

   if ( ! -e "S_main_$ENV{'TRICK_HOST_CPU'}.exe" and !$Clean ) {
      open RUNERROR, ">>$Jeod_home/$error_file";
      print RUNERROR "No executable found for sim $simpath\n";
      close RUNERROR;
      return;
   }

   my @jeod_sim_runs = get_sim_runs($Run);

   foreach my $run (@jeod_sim_runs) {
      $have_restart_runs = 1 if ( $run =~ /restart/i );
   }

   foreach my $run (@jeod_sim_runs) {
      ( clean_run($run) and next ) if $Clean;

      if ( $Trick10 ) {
         ( push (@restart_runs, $run) and next ) if ( $run =~ /restart/i );
         check_and_run($run);
         push(@non_restart_pids, pop(@Forked_run_pids)) if $have_restart_runs;
      } else {
         check_and_run($run);
      }
   }


   if ( $Trick10 and !$Clean) {

      if ( $have_restart_runs ) {
         foreach my $pid (@non_restart_pids) {
            kill_zombie_process($pid) if $pid;
         }
      }

      foreach my $run (@restart_runs) {
         check_and_run($run);
      }
   }

}

sub get_sim_runs {

   my $runname = shift;
   my @jeod_runs;

   open FIND, "find SET_test -name $runname -type d | sort |" or die;
   map chomp, (@jeod_runs = <FIND>);
   close FIND;

   return @jeod_runs;

}

sub check_and_run {

   my $rundir = shift;

   unless ( check_sendhs($rundir) ) {
      if ( $Os_name eq 'darwin' ) {
         $Total_forks++;
         wait_for_fork if ( ($Total_forks % 20) == 0 );
      }
      my $run_pid = fork_run($rundir);
      push (@Forked_run_pids, $run_pid);
   }

}

sub wait_for_fork() {

   my $num_proc = 0;
   do {
      open PSOUT, "ps -e | wc -l|" or die "Cannot run ps command: $!\n";
      $num_proc = <PSOUT>;
      chomp $num_proc;
      close PSOUT;
   } while ( $num_proc =~ /^[^\d]+$/ );

   if ( $num_proc > $Max_Processes ) {
      $Max_Processes = $num_proc;
      print "New Max Process: $Max_Processes ...\n";
   }

   while ( $num_proc > 150 ) {
      foreach my $zombie_id (get_zombie_list($0)) {
         waitpid($zombie_id, 0);
         $num_proc--;
      }
      sleep 10;
   }
}

sub check_sendhs {  # return 1 if no need to run and 0 if needed to run

   my $rundir = shift;
   my $already_ran;

   if ( -e "$rundir/send_hs" ) {
      open SENDHS, "$rundir/send_hs" or die "Cannot open $rundir/send_hs: $!\n";
      $already_ran = grep { /reached/i and /termination/ and /time/ } <SENDHS>;
      close SENDHS;
   }

   return $already_ran;

}

sub fork_run {   # run a run and if fails, reports it to run_error

   my $rundir = shift;

   my $simpath = getcwd;
   $simpath =~ s/$Jeod_home\///;
   my $inputfile = 'input';
   $inputfile = 'input.py' if $Trick10;
   my $error_file = 'run_error';

   my $smain_num;
   open PSOUT, "ps -e -o user,comm | grep $ENV{'USER'} | grep S_main | grep -v grep | wc -l|" or die "Cannot run ps command: $!\n";
   $smain_num = <PSOUT>;
   chomp $smain_num;
   close PSOUT;

   my $pid = fork();

   if ( $pid == 0 ) {

      while ( $smain_num > 3 ) {
         sleep 5;
         open PSOUT1, "ps -e -o user,comm | grep $ENV{'USER'} | grep S_main | grep -v grep | wc -l|" or die "Cannot run ps command: $!\n";
         #open PSOUT1, "ps -e -o comm | grep S_main | wc -l|" or die "Cannot run ps command: $!\n";
         $smain_num = <PSOUT1>;
         chomp $smain_num;
         close PSOUT1;
      }

      print "Running $simpath/$rundir ...\n";
      system ("./S_main_$ENV{'TRICK_HOST_CPU'}.exe $rundir/$inputfile >& /dev/null");

      unless ( check_sendhs($rundir) ) {
         open RUNERROR, ">>$Jeod_home/$error_file";
         print RUNERROR "RUN $simpath/$rundir Failed ...\n";
         close RUNERROR;
      }

      exit 0;
   } elsif ( $pid ) {
      return $pid;
   } else {
      print "Resources not available to fork run process\n";
   }

}

sub clean_run {
   # check if log data exists
   my $localdir = getcwd;
   my $relpath = $localdir;
   $relpath =~ s/$Jeod_home\///;
   my $rundir = shift;
   chdir $rundir;
   my @files = <*>;
   # remove run data
   my $data_exist = map m/log.*\.trk/,@files;
   print "Cleaning RUN $relpath/$rundir ...\n" if $data_exist;
   foreach my $filename (@files) {
     (unlink $filename and next) if ( $filename =~ /log/ );
     (unlink $filename and next) if ( $filename =~ /S\_/ );
     #(unlink $filename and next) if ( $filename =~ /chkpnt/ );
     (unlink $filename and next) if ( $filename =~ /\.out/ );
     (unlink $filename and next) if ( $filename =~ /\.err/ );
     (unlink $filename and next) if ( $filename =~ /send_hs/ );
   }
   chdir $localdir;
}

######################################################################
# Creates JEOD Library used for unit tests
######################################################################

sub make_jeod_lib() {

   unless ( $Clean ) {
      print "Compiling JEOD library ...\n";
      system ("bin/jeod/Configure_jeod >& /dev/null");
   } else {
      print "Removing JEOD library ...\n";
      system ("make real_clean >& /dev/null");
   }

}

######################################################################
# Run JEOD Unit Tests under user specified path
######################################################################

sub unit_test() {

   my @jeod_unit_tests_dir = find_unit_test_dir;
   my $unit_pid;
   my @forked_unit_pids;

   foreach my $unit_test (@jeod_unit_tests_dir) {
      my @test_cases = find_unit_tests($unit_test);

      foreach my $test (@test_cases) {
         unless ( $Clean ) {
            $unit_pid = fork_unit_test($test);
             push (@forked_unit_pids, $unit_pid) if $unit_pid;
         } else {
            clean_unit_test($test);
         }
      }

     my $forked_unit = scalar(@forked_unit_pids);

     if ( $forked_unit ) {
        my @zombie_units = get_zombie_list($0);
        foreach my $zombie_pid (@zombie_units) {
           waitpid($zombie_pid,0);
        }
     }

   }

   my $forked_unit = scalar(@forked_unit_pids);

   if ( $forked_unit ) {
      foreach my $pid (@forked_unit_pids) {
         kill_zombie_process($pid);
      }
   }

}

sub find_unit_test_dir() {

   my @unit_tests;
   open FIND, "find $Path/ -name 'unit_tests' -type d | sort |"
      or die;
   map chomp, (@unit_tests = <FIND>);
   map s/$Jeod_home\///, @unit_tests;
   close FIND;

   return @unit_tests;

}

sub find_unit_tests {

   my $unit_test_dir = shift;
   $unit_test_dir =~ s/^\///;
   my @test_cases;
   chdir $Jeod_home;

   # list all files and directories under unit test directory
   opendir ( DIR, $unit_test_dir ) || die "Error in opening dir $unit_test_dir\n";
   my @subdir = readdir (DIR);
   closedir (DIR);

   # only list unit_test directories that have makefile
   foreach my $dir (@subdir) {
      next if ($dir =~ /^\.+/);
      next unless ( -d "$unit_test_dir/$dir" );
      next unless ( -e "$unit_test_dir/$dir/makefile" );

      push(@test_cases, "$unit_test_dir/$dir");
   }
   return @test_cases;

}

sub clean_unit_test {

   my $test_case = shift;
   chdir "$Jeod_home/$test_case";
   return if ( ! -e 'test_program' );
   print "Cleaning $test_case unit test ...\n";
   system ("make clean >& /dev/null");
}

sub fork_unit_test {

   my $test_case = shift;
   my $error_file = 'unit_error';
   chdir "$Jeod_home/$test_case";
   return if ( -e 'test_program' );

   my $pid = fork();

   if ( $pid == 0) {

      my @unitlist;


      open MAKE, "ps -o pid,command | grep make | grep -v grep|" or die "Cannot use ps command: $!\n";
      map chomp, (@unitlist = <MAKE>);
      close MAKE;

      my $forked_unit = scalar(@unitlist);

      while ( $forked_unit > 8 ) {
         sleep 2;

         open MAKE, "ps -o pid,command | grep make | grep -v grep|" or die "Cannot use ps command: $!\n";
         map chomp, (@unitlist = <MAKE>);
         close MAKE;

         $forked_unit = scalar(@unitlist);
      }

      print "Building $test_case unit test ...\n";
      my $make_build = system ("make build >& /dev/null");
      if ($make_build) {
         # error in building unit_test
         open UNITERROR, ">>$Jeod_home/$error_file";
         print UNITERROR "Unit test $test_case failed to build ...\n";
         close UNITERROR;
         exit 0;
      }

      print "Running $test_case unit test ...\n";
      my $make_test = system ("make run >& /dev/null");
      if ($make_test) {
         # error in running unit_test
         open UNITERROR, ">>$Jeod_home/$error_file";
         print UNITERROR "Unit test $test_case failed to run ...\n";
         close UNITERROR;
         exit 0;
      }
      exit 0;
   } elsif ( $pid ) {
      return $pid;
   } else {
      print "Resources not available to fork unit process\n";
   }

}

######################################################################
# Verify JEOD sim's SET_test data with SET_test_val (validation data)
######################################################################

sub verify() {

   my $simpath = getcwd;
   $simpath =~ s/$Jeod_home\///;
   my $error_file = 'verif_error';
   my @rundir_content;
   my $set_val_name = 'SET_test_val';

   $set_val_name = $Valpath . $simpath . '/' . $set_val_name if ($Valflag);

   # Check if SET_test exists
   unless ( -d 'SET_test') {
      open VERIFERROR, ">>$Jeod_home/$error_file";
      print VERIFERROR "No SET_test in $simpath ...\n";
      close VERIFERROR;
      return;
   }

   # Check if SET_test_val exists
   unless ( -d "$set_val_name" ) {
      open VERIFERROR, ">>$Jeod_home/$error_file";
      if ($Valflag) {
         print VERIFERROR "No Validation data in $Valpath" . $simpath . " ...\n";
      } else {
         print VERIFERROR "No Validation data in $simpath ...\n";
      }
      close VERIFERROR;
      return;
   }

   my @set_test_runs = get_sim_runs($Verify);

   foreach my $run (@set_test_runs) {

      opendir ( DIR, $run ) || die "Error in opening run $run\n";
      @rundir_content = readdir (DIR);
      closedir (DIR);

      my @headers;
      my @ascii_binaries;
      my @run_data;

      my $set_val_run = $run;
     $set_val_run =~ s/SET_test/SET_test_val/;

     $set_val_run = $Valpath . $simpath . '/' . $set_val_run if ($Valflag);

      @run_data = get_run_data(@rundir_content);

      foreach my $file (@run_data) {
         push (@headers,$file) if ( $file =~ /\.header$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.trk$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.csv$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.out$/ );
      }

      unless (scalar(@ascii_binaries)) {
         open VERIFERROR, ">>$Jeod_home/$error_file";
         print VERIFERROR "RUN $simpath/$run has NO data ...\n";
         close VERIFERROR;
         next;
      }

      foreach my $ascii_binary_out_file (@ascii_binaries) {
         # Check if SET_test_val/RUN has this file to compare against
         unless ( -e "$set_val_run/$ascii_binary_out_file" ) {
            open VERIFERROR, ">>$Jeod_home/$error_file";
            if ($Valflag) {
               print VERIFERROR "Needed $set_val_run/$ascii_binary_out_file to compare against ...\n";
            } else {
               print VERIFERROR "Needed $simpath/$set_val_run/$ascii_binary_out_file to compare against ...\n";
            }
            close VERIFERROR;
            next;
         }

         # compare SET_test and SET_test_val
         if ( compare("$run/$ascii_binary_out_file","$set_val_run/$ascii_binary_out_file") != 0 ) {
            open VERIFERROR, ">>$Jeod_home/$error_file";
            if ($Valflag) {
               print VERIFERROR "Result for $set_val_run/$ascii_binary_out_file does not match ...\n";
            } else {
               print VERIFERROR "Result for $simpath/$set_val_run/$ascii_binary_out_file does not match ...\n";
            }
            close VERIFERROR;
            next;
         }

      }

   }
}

sub get_run_data {

   my @run_files = @_;
   my @important_files;

   foreach my $file (@run_files) {
      next if ( $file =~ /^\./ );
      next if ( $file =~ /send_hs/ );
      next if ( $file =~ /input/ );
      next if ( $file =~ /log_mem_stats/ );
      next if ( $file =~ /log_frame/ );
      next if ( $file =~ /log_jobs/ );
      next if ( $file =~ /^S_.*/ );
      next if ( $file =~ /Title/ );

      push (@important_files,$file);
   }

   return @important_files;
}

######################################################################
# Build JEOD PDF Documents
######################################################################

sub build_pdf_docs() {
   chdir $Jeod_home;
   my @tex_dirs = find_tex_dirs;
   my @pdf_pidlist;
   my $pdf_pid;

   foreach my $tex_dir (@tex_dirs) {
      unless ( $Clean ) {
         build_pdf($tex_dir);
      } else {
         clean_pdf($tex_dir) if ( $Clean );
         sleep 1;
      }
   }
}

sub find_tex_dirs() {        # list of jeod sims

   my @dirs;
   open FIND, "find $Path -name 'tex' -type d | grep -v 'jeod_course' | sort |"
      or die;
   map chomp, (@dirs = <FIND>);
   map s/$Jeod_home\///, @dirs;
   close FIND;

   return @dirs;

}

sub build_pdf {

   my $tex_dir = shift;
   my $error_file = 'doc_error';
   chdir "$Jeod_home/$tex_dir" or die "Cannot chdir to $tex_dir: $!\n";
   return unless ( -e 'makefile' );

   my $pdf_build = system ("make >& /dev/null");

   if ($pdf_build) {
      # error in building pdf document
      open DOCERROR, ">>$Jeod_home/$error_file";
      print DOCERROR "PDF document from $tex_dir failed to build ...\n";
      close DOCERROR;
   } else {
      print "Generating PDF from $tex_dir ...\n";
      system ("make install >& /dev/null");
   }

}

sub clean_pdf {

   my $tex_dir = shift;
   chdir "$Jeod_home/$tex_dir" or die "Cannot chdir to $tex_dir: $!\n";
   print "Cleaning PDF in $tex_dir ...\n";
   system ("make real_clean >& /dev/null");

}

######################################################################
# Build JEOD Doxygentated API
######################################################################

sub build_jeod_api() {
   chdir $Jeod_home;
   system ('dgh_scripts/doxygenate.csh');
}

######################################################################
# Creats a validation data for new sims
# One need to have SET_test data for a run to create a validation data
######################################################################

sub make_validation_data() {

   my $simpath = getcwd;
   $simpath =~ s/$Jeod_home\///;
   my $error_file = 'val_error';
   my @rundir_content;

   # Check if SET_test exists
   unless ( -d 'SET_test') {
      open VALERROR, ">>$Jeod_home/$error_file";
      print VALERROR "No SET_test in $simpath ...\n";
      close VALERROR;
      return;
   }

   # Check if SET_test_val exists and if not create it using svn command
   system ("svn mkdir SET_test_val") unless ( -d 'SET_test_val' );

   my @set_test_runs = get_sim_runs($Make_val);

   foreach my $run (@set_test_runs) {

      opendir ( DIR, $run ) || die "Error in opening run $run\n";
      @rundir_content = readdir (DIR);
      closedir (DIR);

      my @run_data;
      my @headers;
      my @ascii_binaries;

      my $set_val_run = $run;
      $set_val_run =~ s/SET_test/SET_test_val/;

      @run_data = get_run_data(@rundir_content);

      foreach my $file (@run_data) {
         push (@headers,$file) if ( $file =~ /\.header$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.trk$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.csv$/ );
         push (@ascii_binaries,$file) if ( $file =~ /\.out$/ );
      }

      unless (scalar(@ascii_binaries)) {
         open VALERROR, ">>$Jeod_home/$error_file";
         print VALERROR "RUN $simpath/$run has NO date to host in SET_test_val ...\n";
         close VALERROR;
         next;
      }

      system ("svn mkdir --parents $set_val_run") unless ( -d "$set_val_run" );

      foreach my $file (@ascii_binaries,@headers) {
         # Check if SET_test_val/RUN has this file to compare against
         unless ( -e "$set_val_run/$file" ) {
            print "Copying $simpath/$run/$file to $simpath/$set_val_run ...\n";
            copy("$run/$file","$set_val_run/$file")
               or die "Copy failed from $simpath/$run/$file to $simpath/$set_val_run\n";
            system ("svn add $set_val_run/$file");
            next;
         }

         # compare SET_test and SET_test_val
         if ( compare("$run/$file","$set_val_run/$file") != 0 ) {
            print "Copying $simpath/$run/$file to $simpath/$set_val_run ...\n";
            copy("$run/$file","$set_val_run/$file")
               or die "Copy failed from $simpath/$run/$file to $simpath/$set_val_run\n";
            next;
         }

      }

   }

}

######################################################################
# Pretty Error Files
######################################################################

sub pretty_error_files() {

   chdir $Jeod_home;
   my @error_files;
   open FIND, "find . -maxdepth 1 -name '*_error' -type f | sort |"
      or die;
   map chomp, (@error_files = <FIND>);
   close FIND;

   foreach my $file (@error_files) {
      my @file_content;

      process_cp_error($file) if ( $file =~ /cp/i );
      process_run_error($file) if ( $file =~ /run/ );
      process_unit_error($file) if ( $file =~ /unit/ );
      process_doc_error($file) if ( $file =~ /doc/ );

   }
}

sub process_cp_error  {
   my $file_name = shift;
   my @file_content;

   if ( $Failed_CP > 0 ) {
      push (@file_content, "**************************************************************\n");
      push (@file_content, "   Trick $ENV{TRICK_VER} Compilation Status: (Failed $Failed_CP of $Total_CP)\n");
      push (@file_content, "**************************************************************\n");
   }
   open CPERROR, "$file_name" or die "Cannot read from $file_name: $!\n";
   while (<CPERROR>) {

      next if /^$/;
      next if /^=.*$/;
      push (@file_content, $_);

   }
   close CPERROR;
   unlink $file_name;

   return if ( scalar(@file_content) == 0 );
   push (@file_content, "==============================================================\n");

   open CPERROR, ">$file_name" or die "Cannot create $file_name for writing: $!\n";
   foreach my $line (@file_content) {
      print CPERROR $line;
   }
   close CPERROR;
}

sub process_run_error  {
   my $file_name = shift;
   my @file_content;

   open RUNERROR, "$file_name" or die "Cannot read from $file_name: $!\n";
   while (<RUNERROR>) {

      next if /^$/;
      next if /^=.*$/;
      next if /RUN_fail/i;
      next if /SIM_message_handler.*\/RUN_test/i;
      push (@file_content, $_);

   }
   close RUNERROR;
   unlink $file_name;

   return if ( scalar(@file_content) == 0 );
   push (@file_content, "==============================================================\n");

   open RUNERROR, ">$file_name" or die "Cannot create $file_name for writing: $!\n";
   foreach my $line (@file_content) {
      print RUNERROR $line;
   }
   close RUNERROR;
}

sub process_unit_error  {
   my $file_name = shift;
   my @file_content;

   open UNITERROR, "$file_name" or die "Cannot read from $file_name: $!\n";
   while (<UNITERROR>) {

      next if /^$/;
      next if /^=.*$/;
      next if /unit_tests\/fail/i;
      push (@file_content, $_);

   }
   close UNITERROR;
   unlink $file_name;

   return if ( scalar(@file_content) == 0 );
   push (@file_content, "==============================================================\n");

   open UNITERROR, ">$file_name" or die "Cannot create $file_name for writing: $!\n";
   foreach my $line (@file_content) {
      print UNITERROR $line;
   }
   close UNITERROR;
}

sub process_doc_error  {
   my $file_name = shift;
   my @file_content;

   open DOCERROR, "$file_name" or die "Cannot read from $file_name: $!\n";
   while (<DOCERROR>) {

      next if /^$/;
      next if /^=.*$/;
      push (@file_content, $_);

   }
   close DOCERROR;
   unlink $file_name;

   return if ( scalar(@file_content) == 0 );
   push (@file_content, "==============================================================\n");

   open DOCERROR, ">$file_name" or die "Cannot create $file_name for writing: $!\n";
   foreach my $line (@file_content) {
      print DOCERROR $line;
   }
   close DOCERROR;
}

__END__

=head1 NAME

jeod_cm.pl -
A script to test everything on JEOD package

=head1 SYNPOSIS

jeod_cm.pl [options] [path]

[ -help ]
[ -cp ]
[ -run ]
[ -lib ]
[ -unit ]
[ -pdfdoc ]
[ -apidoc  ]
[ -verify ]
[ -valflag ]
[ -mkval ]
[ -trick10 ]
[ -path ]
[ -clean ]

=head1 OPTIONS

=over 8

=item B<-help>

Print this page and exit.

=item B<-cp> [-path [path argument]] [-clean]

Compiles JEOD sims under specified path. $JEOD_HOME is the default path.
If B<-clean> option is applied, it will clean the affected JEOD sims (make spotless).

=item B<-run [runname]> [-path [path argument]] [-clean]

Runs JEOD SET_test runs under a specified path. $JEOD_HOME is the default path.
If B<-clean> option is applied, it will clean the affected SET_test runs.

=item B<-lib> [-clean]

Builds JEOD Library, mainly used for unit test.
if B<-clean> option is applied, it will clean the library content from JEOD.

=item B<-unit> [-path [path argument]] [-clean]

Build and runs JEOD unit test under a specified path. $JEOD_HOME is the default path.
if B<-clean> option is applied, it will clean the affected unit test.

=item B<-pdfdoc> [-path [path argument]] [-clean]

Builds PDF from LaTex source code under a specified path. $JEOD_HOME is the default path.
If B<-clean> option is applied, it will clean the affected LaTex source director[y|ies].

=item B<-apidoc> [-clean]

Buils Top Level JEOD Doxygenated API Document.
if B<-clean> option is applied, it will remove the API Document from JEOD.

=item B<-verify> [-path [path argument]]

Verifies all the sims under a specified path if the SET_test run's data matches that of the
validation date. $JEOD_HOME is the default path.

=item B<-valflag> [-path [path argument]]

Verifies all the sims under a specified path if the SET_test run's data matches that of the
machine specific validation date. This option is used in conjuction with [-verify] option.
$JEOD_HOME is the default path.

=item B<-mkval> [-path [path argument]]

Creats validation data (SET_test_val) for all the sims under a specified path if the SET_test run's data
exists. $JEOD_HOME is the default path.

=item B<-path [path argument]>

Specify a relative path under which one need to use a specific JEOD related task. $JEOD_HOME is the default path.

=item B<-clean>

Clean option will clean either a SIM or Run or Unit Test or Document based on with which option this is being
used with. So if this option is used with B<-cp>, it will clean respective sims under B<-path> (make spotless)

=back

=head1 ENVIRONMENT VARIABLES

=over 8

=item B<$JEOD_HOME>

Top level JEOD directory path.

=back

=head1 DESCRIPTION

The jeod_cm.pl scripts does most of the checks of JEOD suite for everyday testing.

=cut
