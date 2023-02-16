#!/usr/bin/perl -w

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd;
use File::Basename;
use File::Path;
use Data::Dumper qw(Dumper);
use List::Util qw(max);

sub main();
sub find_trick_10_sim_dir();
sub compare_val($);
sub list_runs($$);
sub populate_log_data($);
sub populate_log_hash($$);
sub compare_log_hash($$$);
sub compare_log_hash_key($$$$);
sub compare_values($$);
sub maxResult(@);

# Global variables
my ($Help,
    $Jeod_Trunk,
    $Jeod_2x_Trunk,
    $Val_Name,
    $Path,
    $Curdir,
    $Tolerance,
    $Clean,
    @trick_10_sims,
    @trick_7_sims);

$Curdir = getcwd;
$Jeod_Trunk = 0;
$Jeod_2x_Trunk = 0;
$Path = 0;
$Clean = 0;
$Val_Name = 'SET_test_val';
$Tolerance = '1.0e-10';

# Parse the command line
GetOptions (
   'help'            => \$Help,
   'clean'           => \$Clean,
   'trunk:s'         => \$Jeod_Trunk,
   'trunk_2x:s'      => \$Jeod_2x_Trunk,
   'set_test_val:s'  => \$Val_Name,
   'tolerance:s'     => \$Tolerance,
   'path:s'          => \$Path)
   or pod2usage(2);

pod2usage(-exitstatus => 0, -verbose => 2) if $Help;

# sanity checks
die "Please provide path to JEOD 3.0's JEOD_HOME\n" unless ( $Jeod_Trunk );
die "Please provide path to JEOD 2.2's JEOD_HOME\n" unless ( $Jeod_2x_Trunk );
die "\$TRICK_HOME needs to be set to Trick 13 release."
   unless defined($ENV{'TRICK_HOME'});

main();

#################################
# Main Function
#################################
sub main() {

    die "Please give valid JEOD 2.2 Path\n" unless (-e "$Jeod_2x_Trunk");
    chdir $Jeod_2x_Trunk;
    $Jeod_2x_Trunk = getcwd;
    chdir $Curdir;

    die "Please give valid JEOD 3.0 Path\n" unless (-e "$Jeod_Trunk");
    chdir $Jeod_Trunk;
    $Jeod_Trunk = getcwd;

    # Find all trick 10 simulations 
    &find_trick_10_sim_dir;

    foreach my $simdir (@trick_10_sims) {
        compare_val($simdir);
    }

    chdir $Curdir;
}

#################################
# Find Sim Directories
#################################
sub find_trick_10_sim_dir () {

    my @jeod_sims;
    my $trick_7_sim;

    die "Please provide valid path\n" unless (-d $Path);

    open FIND, "find $Path -name 'SIM_*' -type d | grep -v 'unsupported' | grep -v 'jeod_course' | grep 'SIM_.*10' | sort |"
       or die;
       map chomp, (@jeod_sims = <FIND>);
    close FIND;

    # Store all trick 10 sims that has respective trick 7 sims
    foreach my $sim (@jeod_sims) {
       $trick_7_sim = "$Jeod_2x_Trunk/$sim";
       $trick_7_sim =~ s/_T10//;
       push (@trick_10_sims, "$Jeod_Trunk/$sim") if (-d "$trick_7_sim");
    }
}

#################################
# Create Validate data in Trick 7 for comparison
#################################
sub compare_val($) {
    my @run_dirs = ();
    my $simdir = shift;
    my $trick_7_simdir = $simdir;
    $trick_7_simdir =~ s/$Jeod_Trunk/$Jeod_2x_Trunk/;
    my $simname = basename($simdir);
    my $trick_7_simname = $simname;
    $trick_7_simname =~ s/_T10//;
    my $val_path_10 = "SET_test_val_10";

    chdir "$trick_7_simdir/.."  or die "Cannot change directory to $trick_7_simdir...\n";

    # List run dirs
    if ((-d $simdir) && (-d $trick_7_simname)) {
        @run_dirs = list_runs($simdir, $trick_7_simname);
    } else {
        chdir $Curdir;
        return;
    }

    # copy trick 10 val data and list headers files
    foreach my $run_dir (@run_dirs) {
        my @trick_7_log_files = ();
        my @trick_10_log_files = ();
        my %trick_7_logdata;
        my %trick_10_logdata;
        my $validation_path = "SET_test_val";
        my $trick_7_run_dir = $run_dir;
        $trick_7_run_dir =~ s/$Jeod_Trunk/$Jeod_2x_Trunk/;
        $trick_7_run_dir =~ s/_T10//;
        my $run_dir_name = basename($run_dir);
        my $trick_10_run_dir = $trick_7_run_dir;
        $trick_10_run_dir =~ s/$Val_Name/$val_path_10/;
        $trick_7_run_dir =~ s/$Val_Name/$validation_path/;

        if ($Clean) {
            print "Removing: $trick_7_simname/$val_path_10\n" if (-e "$trick_7_simname/$val_path_10");
            rmtree "$trick_7_simname/$val_path_10" if (-e "$trick_7_simname/$val_path_10");
            next;
        }

        # copy trick 10 val data
        mkdir "$trick_7_simname/$val_path_10" unless (-e "$trick_7_simname/$val_path_10");
        unless (-e "$trick_10_run_dir") {
            print "COPYING: $run_dir to $trick_10_run_dir...\n";
            system("/bin/cp -r $run_dir $trick_10_run_dir");
        }

        print "Comparing RUN: $run_dir\n";
        print "===================================================================================\n";

        # list trick 7 header files
        opendir (RUNDIR, "$trick_7_run_dir") or die $!;
        while (my $logfile = readdir(RUNDIR)) {
            push @trick_7_log_files, "$trick_7_run_dir/$logfile" if ($logfile =~ /\.(csv|trk)$/);
        }
        closedir(RUNDIR);

        # list trick 10 header files
        opendir (RUNDIR, "$trick_10_run_dir") or die $!;
        while (my $logfile = readdir(RUNDIR)) {
            push @trick_10_log_files, "$trick_10_run_dir/$logfile" if ($logfile =~ /\.(csv|trk)$/);
        }
        closedir(RUNDIR);

        #print "TRICK 7 Logfiles: @trick_7_log_files\n";
        # populate log hash from all trick 7 log files
        foreach my $logfile (@trick_7_log_files) {
            my $log_hash = populate_log_data($logfile);
            populate_log_hash(\%trick_7_logdata, $log_hash);
        }

        # populate log hash from all trick 10 log files
        foreach my $logfile (@trick_10_log_files) {
            my $log_hash = populate_log_data($logfile);
            populate_log_hash(\%trick_10_logdata, $log_hash);
        }

        # compare trick 10 and trick 07 log data
        compare_log_hash(\%trick_7_logdata, \%trick_10_logdata, $run_dir);
    }
        
    chdir $Curdir;
}

#################################
# List common run directories between trick 7 and trick 10 sim directory
#################################
sub list_runs($$) {
    my @trick10_runs = ();
    my $validation_path = "SET_test_val";
    my $val_path_10 = "SET_test_val_10";
    my ($trick10_simdir, $trick7_sim) = @_;

    opendir (DIR, "$trick10_simdir/$Val_Name") or die "Cannot open directory $trick10_simdir/$Val_Name, $!";

    while (my $rundir = readdir(DIR)) {
        if (-d "$trick10_simdir/$Val_Name/$rundir") {
            next if ($rundir =~ /^\./);
            if (-d "$trick7_sim/$validation_path/$rundir") {
                push @trick10_runs, "$trick10_simdir/$Val_Name/$rundir";
            } else {
                print "TRICK 7 RUN: $trick7_sim/$validation_path/$rundir does not exist...\n";
            }
        }
    }

    closedir(DIR);
    
    return @trick10_runs;
}

#################################
# Populate log data
#################################
sub populate_log_data($) {
    my $logfile = $_[0];
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
            map s/^jeod_time/time/, $list[$i];
            map s/^sim_time/time/, $list[$i];
            push @{$columns[$i]}, $list[$i];
        }
    }
    close FILE;

    my @headings = map {shift @$_} @columns;
    my @iterator = @{$columns[0]};
    
    foreach (@columns) {
        my %inside_hash;
        @inside_hash{@iterator} = @{$_};
        push @{$new_column[$inc++]}, \%inside_hash;
    }

    $inc = 0;
    foreach (@headings) {
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
# Compare log data hash between trick 7 and trick 10
#################################
sub compare_log_hash($$$) {
    my ($trick7_hash, $trick10_hash, $rundir) = @_;
    for my $t10_key (sort keys %{$trick10_hash}) {
        if (exists $trick7_hash->{$t10_key}) {
            compare_log_hash_key($trick7_hash, $trick10_hash, $t10_key, $rundir) 
       #} else {
       #     print "Key: $t10_key does not exist in Trick 7 log file of RUN: $rundir.\n";
        }
    }
    print "\n";
}

#################################
# Compare log hash variables if exist between both trick 7 and trick 10 log data
#################################
sub compare_log_hash_key($$$$) {
    my ($t7_hash, $t10_hash, $key, $rundir) = @_;
    my $t7_var_hash = $t7_hash->{$key};
    my $t10_var_hash = $t10_hash->{$key};
    my @comparisons=();

    for my $t10_key (keys %{$t10_var_hash}) {
        if (exists $t7_var_hash->{$t10_key}) {
            unless ($t7_var_hash->{$t10_key} eq $t10_var_hash->{$t10_key}) {
                my $comparison_ref = compare_values($t7_var_hash->{$t10_key}, $t10_var_hash->{$t10_key});
                $comparison_ref->{'time'} = $t10_key;
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
        print "$key at t=$worst->{'time'}: $worst->{'diff'} ($worst->{'type'}) (Trick_7: $worst->{'trick_7_value'} <==> Trick_10: $worst->{'trick_10_value'}\n";
        return; 
    } else {
        my $worst = maxResult(@comparisons);
        print "$key at t=$worst->{'time'}: $worst->{'diff'} ($worst->{'type'}) (Trick_7: $worst->{'trick_7_value'} <==> Trick_10: $worst->{'trick_10_value'}\n";
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

    $comparison{'trick_7_value'} = $a;
    $comparison{'trick_10_value'} = $b;

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
A script to copy trick 10 validation into trick 7 sims for comparision purpose

=head1 SYNPOSIS

compare_val_data.pl [options] [path]

[ -help ]
[ -trunk ]
[ -trunk_2x ]
[ -clean ]
[ -tolerance ]
[ -set_test_val ]
[ -path ]

=head1 OPTIONS

=over 8

=item B<-help>

Print this page and exit.

=item B<-trunk>

Path to JEOD 3.0 JEOD_HOME.

=item B<-trunk_2x> 

Path to JEOD 2.2 JEOD_HOME.

=item B<-clean>  (Optional)

If [-clean] flag is added, it will remove all Trick 10 or 13 simulation validation directories from
JEOD 2.2 JEOD_HOME.

=item B<-tolerance> (Optional)

Tolerance limit for comparing values, by default it is 1.0e-10.

=item B<-set_test_val>  (Optional)

Name of SET_test_val directory to look for in JEOD 3.0. If want to compare SET_test data against
trick 7 then provide [-set_test_val SET_test] or else it will default to SET_test_val directory.

=item B<-path> (Optional)

Relative path to a sim from JEOD_HOME to compare data of.

=back

=head1 DESCRIPTION

The compare_val_data.pl scripts copies SET_test or SET_test_val data from Trick 10 to Trick 7 for
comparison purpose.

=cut
