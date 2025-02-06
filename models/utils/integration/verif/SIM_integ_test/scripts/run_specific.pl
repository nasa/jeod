#!/usr/bin/perl -w

use strict;
use Getopt::Long;

#open SAVEOUT, ">&STDOUT";
#open STDOUT, ">run.out";
#open STDERR, ">run.err";


my $exec    = "./S_main_$ENV{TRICK_HOST_CPU}.exe";
my $run_dir = 'run_specific';
my $input   = "$run_dir/input.py";


my @classes = qw(
   RotationTestTorqueFreeSphere
   RotationTestTorqueFreeSymTop
   RotationTestTorqueFreeSymTopB
   RotationTestSHOSphere
   TranslationTestOrbit
   TranslationTestSpringDamper);

my @omega_dt = (10,
                map {(5.62*$_, 3.16*$_, 1.78*$_, $_)}
                    (100, 10, 1, 0.1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8));

my @techniques = qw( 
   Euler
   SymplecticEuler
   Beeman
   NystromLear2
   PositionVerlet
   RK2Heun
   RK2Midpoint
   VelocityVerlet
   ModifiedMidpoint4
   AdamsBashforthMoulton4
   RungeKutta4
   RKGill4
   RKFehlberg45
   RKFehlberg78
);
### Add GaussJackson and RKNystrom4 to the above
### when (if) they incorporated into ER7 utils.

my %classes;
my %omega_dt;
my %techniques;

@classes{@classes} = (0 .. $#classes);
@omega_dt{@omega_dt} = (1) x @omega_dt;
@techniques{@techniques} = (0 .. $#techniques);

my %options = ( 
   class       => undef, 
   omega_dt    => undef,
   case        => undef,
   integ_opt   => undef, 
   tests       => 1,
   old_integ   => 0,
   nrevs       => 0,
   log_data    => 0,
   check       => 0,
   debug       => 0,
   trace       => 0,
   report      => 1);

GetOptions (\%options,
            'class=s',
            'omega_dt|wdt=f',
            'case=i',
            'integ_opt=s',
            'tests=i',
            'old_integ!',
            'nrevs=i',
            'log_data',
            'check',
            'debug',
            'trace!',
            'report!')
   or die;
scalar @ARGV == 0 or die;

defined $options{class}
   or die "-class not specified\n";
defined $options{omega_dt}
   or die "-omega_dt not specified\n";
defined $options{case}
   or die "-case not specified\n";
defined $options{integ_opt}
   or die "-integ_opt not specified\n";
defined $classes{$options{class}}
   or die "Bad class $options{class}\n";
defined $omega_dt{$options{omega_dt}+0}
   or die "Bad omega_dt $options{omega_dt}\n";
defined $techniques{$options{integ_opt}}
   or die "Bad integ_opt $options{integ_opt}\n";
($options{case} eq int($options{case})) && ($options{case} >= 0)
   or die "Bad case $options{case}\n";
($options{tests} eq int($options{tests})) && ($options{tests} >= 0)
   or die "Bad tests $options{tests}\n";

if (! -d $run_dir) {
   mkdir $run_dir
      or die "Can't make directory $run_dir";
}
if (-f $input) {
   unlink $input or die "Can't delete old input file";
}
my $source = 'Modified_data/run_env.py';
if ($options{old_integ}) {
   $source =~ s/run_env/run_env_old_integ/;
}   
system 'cp', $source, $input
   and die "Can't create $input";

$ENV{RUN_LOG_DATA}  = $options{log_data};
$ENV{RUN_OMEGA_DT}  = $options{omega_dt};
$ENV{RUN_TEST_ID_0} = $classes{$options{class}};
$ENV{RUN_TEST_ID_1} = -1;
$ENV{RUN_TEST_ID_2} = -1;
$ENV{RUN_TEST_ID_3} = -1;
$ENV{RUN_TEST_ID_4} = -1;
$ENV{RUN_TEST_ID_5} = -1;
$ENV{RUN_INTEG_OPT} = $options{integ_opt};
$ENV{RUN_CASENO}    = $options{case};
$ENV{RUN_NTESTS}    = $options{tests};
$ENV{RUN_NREVS}     = $options{nrevs};
$ENV{RUN_DO_REPORT} = $options{report};
$ENV{RUN_TRACE}     = $options{trace};
$ENV{RUN_REPORT_ID} = 1;

if ($options{check}) {
print "Testing with:\n";
map {print "setenv $_ $ENV{$_}\n"}
    qw(RUN_LOG_DATA
       RUN_OMEGA_DT
       RUN_TEST_ID
       RUN_INTEG_OPT
       RUN_CASENO
       RUN_NTESTS
       RUN_NREVS
       RUN_DO_REPORT
       RUN_TRACE
       RUN_REPORT_ID);
}
elsif ($options{debug}) {
   print "Type run $input after setting desired break points\n";
   system 'gdb', $exec;
}
else {
   system $exec, $input;
}
