#!/usr/bin/perl -w

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd;
use File::Basename;
use File::Compare;
use File::Path;
use Data::Dumper qw(Dumper);
use List::Util qw(max);

sub main();
sub find_sim_dir();
sub compare_val($);
sub list_runs($);
sub populate_log_data($);
sub populate_log_hash($$);
sub compare_log_hash($$$);
sub compare_log_hash_key($$$$);
sub compare_values($$);
sub maxResult(@);

# Global variables
my ($Help,
    $Jeod_Home,
    $TestDir,
    $ValDir,
    $Path,
    $Curdir,
    $Tolerance,
    @sims);

$Curdir    = getcwd;
$Jeod_Home = 0;
$Path      = '.';
$TestDir   = 'SET_test';
$ValDir    = 'SET_test_val';
$Tolerance = '1.0e-10';

# Parse the command line
GetOptions (
   'help'            => \$Help,
   'jeod_home:s'     => \$Jeod_Home,
   'set_test:s'      => \$TestDir,
   'set_test_val:s'  => \$ValDir,
   'tolerance:s'     => \$Tolerance,
   'path:s'          => \$Path)
   or pod2usage(2);

pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

# sanity checks
if ($Jeod_Home) {
    $ENV{'JEOD_HOME'} = $Jeod_Home;
} else {
    die "\$Please provide path to JEOD_HOME.\n"
        unless defined($ENV{'JEOD_HOME'});
}
    
die "\$TRICK_HOME needs to be set to Trick 13 release."
   unless defined($ENV{'TRICK_HOME'});

main();

#################################
# Main Function
#################################
sub main() {

    die "Please give valid JEOD 3.0 Path\n" unless defined($ENV{'JEOD_HOME'});
    chdir $ENV{'JEOD_HOME'};
    $Jeod_Home = getcwd;
    print "JEOD_HOME = $ENV{'JEOD_HOME'}\n";

    # Find all simulations 
    &find_sim_dir;

    foreach my $simdir (@sims) {
        compare_val($simdir);
    }

    chdir $Curdir;
}

#################################
# Find Sim Directories
#################################
sub find_sim_dir () {

    my @jeod_sims;

    die "Please provide valid path\n" unless (-d $Path);

    open FIND, "find $Path -name 'SIM_*' -type d | grep -v 'unsupported' | grep -v 'jeod_course' | grep -v 'jeod_tutorial' | sort |"
       or die;
       map chomp, (@jeod_sims = <FIND>);
    close FIND;

    # Store all sims
    foreach my $sim (@jeod_sims) {
       push (@sims, "$Jeod_Home/$sim");
    }
}

#################################
# Create Validate data in Trick 7 for comparison
#################################
sub compare_val($) {
    my @run_dirs = ();
    my $simdir = shift;
    my $simname = basename($simdir);

    chdir "$simdir/.."  or die "Cannot change directory to $simdir...\n";

    # List run dirs
    if (-d $simdir) {
        @run_dirs = list_runs($simdir);
    } else {
        chdir $Curdir;
        return;
    }

    # list log files
    foreach my $run_dir (@run_dirs) {
        my @log_files = ();
        my @val_log_files = ();
        my %logdata_val;
        my %logdata_test;
        my $run_name = basename($run_dir);
        my $val_run_dir = $run_dir;
        $val_run_dir =~ s/$TestDir/$ValDir/;

        print "Validating RUN: $run_dir\n";
        print "===================================================================================\n";

        # list trick log files
        opendir (RUNDIR, "$run_dir") or die $!;
        while (my $logfile = readdir(RUNDIR)) {
            if ($logfile =~ /\.(csv|trk)$/) {
                push @log_files, "$run_dir/$logfile" 
            }
        }
        closedir(RUNDIR);

        if (scalar(@log_files) == 0) {
             print "No Log file for RUN: $run_dir...\n\n";
             next; 
        }

        # list trick validation log files
        opendir (VALRUNDIR, "$val_run_dir") or die $!;
        while (my $vallogfile = readdir(VALRUNDIR)) {
            if ($vallogfile =~ /\.(csv|trk)$/) {
                push @val_log_files, "$val_run_dir/$vallogfile" 
            }
        }
        closedir(VALRUNDIR);

        if (scalar(@val_log_files) == 0) {
             print "No Log file for RUN: $val_run_dir...\n\n";
             next; 
        }

        # populate log hash from test log files
        foreach my $logfile (@log_files) {
            my $log_hash = populate_log_data($logfile);
            populate_log_hash(\%logdata_test, $log_hash);
        }

        # populate log hash from validation log files
        foreach my $logfile (@val_log_files) {
            my $log_hash = populate_log_data($logfile);
            populate_log_hash(\%logdata_val, $log_hash);
        }

        # compare log data
        compare_log_hash(\%logdata_test, \%logdata_val, $run_dir);
    }
        
    chdir $Curdir;
}

#################################
# List common run directories of the sim directory
#################################
sub list_runs($) {
    my @runs = ();
    my $simdir = shift;

    opendir (DIR, "$simdir/$TestDir") or die "Cannot open directory $simdir/$TestDir, $!";
    while (my $rundir = readdir(DIR)) {
        if (-d "$simdir/$TestDir/$rundir") {
            next if ($rundir =~ /^\./);
            push @runs, "$simdir/$TestDir/$rundir";
        }
    }
    closedir(DIR);
    
    return @runs;
}

#################################
# Populate log data
#################################
sub populate_log_data($) {
    #my $logfile = $_[0];
    my $logfile = shift;
    my %hash;
    my @new_column = ();
    my $inc = 0;
    my @columns = ();

    if ($logfile =~ /\.trk$/) {
        open FILE, "$ENV{TRICK_HOME}/bin/trk2csv $logfile|" or die "Cannot trk2csv $logfile, $!\n";
    } else {
        open FILE, $logfile or die "Cannot open $logfile, $!\n";
    }

    while (<FILE>) {
        chomp;
        my $index = 0;
        my @list = split /,/;
        for my $i ($index .. $#list) {
            my $temp = $list[$i];
            map s/\s//g, $list[$i];
            map s/{.*?}//, $list[$i];
            if (index($logfile, $ValDir) != -1) {
                #if (index($logfile, "log_VehState.trk") != -1 ||
                #    index($logfile, "log_rot_state.trk") != -1 ||
                #    index($logfile, "environment") != -1 ||
                #    index($logfile, "interactions") != -1 ||
                #    index($logfile, "utils") != -1) {
                # 
                #} else {
                if (index($logfile, "Integrated_Validation") != -1 ||
                    index($logfile, "SIM_dyncomp") != -1) {
                    map s/\.utc\./\.time_utc\./, $list[$i];
                    map s/\.ut1\./\.time_ut1\./, $list[$i];
                    map s/\.tai\./\.time_tai\./, $list[$i];
                    map s/\.tt\./\.time_tt\./, $list[$i];
                    map s/\.manager\./\.time_manager\./, $list[$i];
                    map s/\.conv_/\.time_converter_/, $list[$i];
                    map s/sv_dyn\./vehicle\./, $list[$i];
                    map s/\.body\./\.dyn_body\./, $list[$i];
                    map s/\.atmos\./\.atmos_state\./, $list[$i];
                }
            }
            push @{$columns[$i]}, $list[$i];
        }
    }
    close FILE;

    my @headings = map {shift @$_} @columns;
    my @iterator = @{$columns[0]};

    #foreach (@iterator) {
    #    print "$_\n";
    #}
    
    foreach (@columns) {
        my %inside_hash;
        @inside_hash{@iterator} = @{$_};
        push @{$new_column[$inc++]}, \%inside_hash;
    }

    $inc = 0;
    foreach (@headings) {
        #print "$_\n";
        $hash{$_} = $new_column[$inc++][0];
    }

    return \%hash;
}

#################################
# Populate log hash from log data hash
#################################
sub populate_log_hash($$) {
   my ($log_hash, $new_data_hash) = @_;
   
   for my $key (keys %{$new_data_hash}) {
       unless (exists $log_hash->{$key}) {
           $log_hash->{$key} = $new_data_hash->{$key};
       }
   }
}

#################################
# Compare log data hash between SET_test and SET_test_val
#################################
sub compare_log_hash($$$) {
    my ($hash_test, $hash_val, $rundir) = @_;
    for my $hash_val_key (sort keys %{$hash_val}) {
        if (exists $hash_test->{$hash_val_key}) {
            compare_log_hash_key($hash_test, $hash_val, $hash_val_key, $rundir) 
       } else {
            print "Key: $hash_val_key does not exist in SET_test log file of RUN: $rundir.\n";
        }
    }
    print "\n";
}

#################################
# Compare log hash variables if exist between both SET_test and SET_test_val log data
#################################
sub compare_log_hash_key($$$$) {
    my ($hash_test, $hash_val, $key, $rundir) = @_;
    my $var_hash_test = $hash_test->{$key};
    my $var_hash_val  = $hash_val->{$key};
    my @comparisons=();

    for my $hash_val_key (keys %{$var_hash_val}) {
        if (exists $var_hash_test->{$hash_val_key}) {
            unless ($var_hash_test->{$hash_val_key} eq $var_hash_val->{$hash_val_key}) {
                my $comparison_ref = compare_values($var_hash_test->{$hash_val_key}, $var_hash_val->{$hash_val_key});
                $comparison_ref->{'time'} = $hash_val_key;
                $comparison_ref->{'run'} = $rundir;
                #print "At t=$t10_key of $key: $comparison_ref->{'diff'} ($comparison_ref->{type})\n";
                push @comparisons, $comparison_ref if ($comparison_ref->{'diff'} >= $Tolerance);
            }
        }
    }

    if (scalar @comparisons eq 0) {
        return;
    } elsif (scalar @comparisons eq 1) {
        my $worst = $comparisons[0];
        print "$key at t=$worst->{'time'}: $worst->{'diff'} ($worst->{'type'}) ";
        print "(Test Data: $worst->{'test_value'} <==> Validation Data: $worst->{'val_value'}\n";
        return; 
    } else {
        my $worst = maxResult(@comparisons);
        print "$key at t=$worst->{'time'}: $worst->{'diff'} ($worst->{'type'}) ";
        print "(Test Data: $worst->{'test_value'} <==> Validation Data: $worst->{'val_value'}\n";
        return;
    }
}

#################################
# Compare two values
#################################
sub compare_values($$) {
    my ($a, $b) = @_;
    my %comparison;
    my $difference = ($b-$a);
    my $scale = max(0, abs($a)); 

    $comparison{'test_value'} = $a;
    $comparison{'val_value'} = $b;

    if ($scale > 1.0) {
        $comparison{'type'} = 'relative';
        $comparison{'diff'} = $difference/abs($a);
    } else {
        $comparison{'type'} = 'absolute';
        $comparison{'diff'} = $difference;
    }

    return \%comparison;
}

#################################
# Return Max difference
#################################
sub maxResult (@) {
    my ($first, @rest) = @_;
    my $r = $first;

    foreach (@rest) {
        $r = $_ if (abs($_->{'diff'}) > abs($r->{'diff'}));
    } # end foreach

    return $r;
} # end sub
__END__

=head1 NAME

compare_val_data.pl -
A script to compare test data against validation data.

=head1 SYNPOSIS

compare_val_data.pl [options] [path]

[ -help ]
[ -jeod_home ]
[ -tolerance ]
[ -set_test ]
[ -set_test_val ]
[ -path ]

=head1 OPTIONS

=over 8

=item B<-help>

Print this page and exit.

=item B<-jeod_home>

Path to JEOD 3.0 JEOD_HOME.

=item B<-clean>  (Optional)

=item B<-tolerance> (Optional)

Tolerance limit for comparing values, by default it is "1.0e-10".

=item B<-set_test>  (Optional)

Name of test directory to look for in JEOD_HOME's simulation directory, by default
it will default to "SET_test" directory.

=item B<-set_test_val>  (Optional)

Name of validation directory to look for in JEOD_HOME's simulation directory, by default
it will default to "SET_test_val" directory.

=item B<-path> (Optional)

Relative path to a simulation relative to JEOD_HOME to compare data of.

=back

=head1 DESCRIPTION

The compare_val_data.pl script compares test data against validation data.

=cut
