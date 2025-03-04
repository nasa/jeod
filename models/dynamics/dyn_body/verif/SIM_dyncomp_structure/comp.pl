################################################################################
# Generates comparison data between SIM_dyncomp and SIM_dyncomp_structure.
#
# Usage:
# Build both simulations and optionally run them on the four SET_test/RUN_10x
# directories. Then run this script. The script generates for files, one for
# each of the four run directories. Each output line contains position
# comparison data of the form
#
#   time delta_x delta_y delta_z delta_r delta_d delta_c delta_mag
#
# All of the delta values are in units of micrometers. The x, y, and z values
# are the differences in the J2000 frame. The r, d, and c values are the
# differences transformed to LVLH (radial, downtrack, crosstrack).
# To generate the plot, run gnuplot on comp.gnu. That script only plots the
# magnitudes of the difference vectors. one can of course use gnuplot manually
# to investigate other presentations of the data.
################################################################################

use warnings;
use strict;
use File::Basename;
use File::Spec;

my $JEOD_HOME = $ENV{JEOD_HOME};
unless (defined $JEOD_HOME) {
    $JEOD_HOME = dirname(dirname(dirname(dirname(dirname(dirname(File::Spec->rel2abs($0)))))));
}

my $Dyncomp = "${JEOD_HOME}/verif/SIM_dyncomp";
chomp (my $Trick_host_cpu = `trick-gte TRICK_HOST_CPU`);
my $S_main = "S_main_$Trick_host_cpu.exe";

sub vcross($$) {
    my ($a, $b) = @_;
    return [
        $a->[1]*$b->[2] - $a->[2]*$b->[1],
        $a->[2]*$b->[0] - $a->[0]*$b->[2],
        $a->[0]*$b->[1] - $a->[1]*$b->[0]];
}

sub vdot($$) {
    my ($a, $b) = @_;
    return $a->[0]*$b->[0] + $a->[1]*$b->[1] + $a->[2]*$b->[2];
}

sub vmag($) {
    my ($a) = @_;
    return sqrt(vdot($a, $a));
}

sub vnorm($) {
    my ($a) = @_;
    my $scale = 1.0/vmag($a);
    return [$a->[0]*$scale, $a->[1]*$scale, $a->[2]*$scale];
}

sub mxv ($$) {
    my ($T, $v) = @_;
    my $p = [0.0, 0.0, 0.0];
    for (my $ii = 0; $ii < 3; $ii++) {
        for (my $jj = 0; $jj < 3; $jj++) {
            $p->[$ii] += $T->[$ii]->[$jj] * $v->[$jj];
        }
    }
    return $p;
}


sub compare ($) {
    my ($rundir) = @_;
    (my $outname = $rundir) =~ s/SET_test\/// or die;

    if (! -f "$rundir/log_state_ASCII.csv")
    {
        print "./$S_main $rundir\n";
        system "./$S_main $rundir/input.py"
             and die "S_main failed: $?";
    }

    if (! -f "$Dyncomp/$rundir/log_state_ASCII.csv")
    {
        system "cd $Dyncomp; ./S_main* $rundir/input.py"
             and die "S_main failed: $?";
    }

    open LOCAL, '<', "$rundir/log_state_ASCII.csv" or die;
    open OTHER, '<', "$Dyncomp/$rundir/log_state_ASCII.csv" or die;
    open OUT, '>', "$outname.dat" or die;

    my $first = 1;
    while ($_ = <LOCAL>) {
        my @ld = split /,/;
        defined ($_ = <OTHER>) or die;
        my @od = split /,/;
        my ($lt, $lpx, $lpy, $lpz) = @ld[0,1,8,15];
        my ($ot, $opx, $ovx, $opy, $ovy, $opz, $ovz) = @od[0,1,2,8,9,15,16];

        my $lp = [$lpx, $lpy, $lpz];
        my $op = [$opx, $opy, $opz];
        my $ov = [$ovx, $ovy, $ovz];

        if ($first) {
            $first = 0;
            next;
        }

        my $rhat = vnorm($op);
        my $zhat = vnorm(vcross($op, $ov));
        my $that = vnorm(vcross($zhat, $rhat));
        my $T = [$rhat, $that, $zhat];
        my $d = [$opx-$lpx, $opy-$lpy, $opz-$lpz];
        my $td = mxv ($T, $d);
        die unless $lt == $ot;
        printf OUT "%d %.16g %.16g %.16g %.16g %.16g %.16g %.16g\n",
            $ot,
            1e6*$d->[0], 1e6*$d->[1], 1e6*$d->[2],
            1e6*$td->[0], 1e6*$td->[1], 1e6*$td->[2],
            1e6*vmag($d);
    }
    close OUT;
}

foreach my $run (qw(A B C D)) {
    my $rundir = "SET_test/RUN_10$run";
    die "Bad run $run (rundir=$rundir)\n" unless (-d $rundir);
    die "Bad dyncomp run $run (dyncomp rundir=$Dyncomp/$rundir)\n"
        unless (-d "$Dyncomp/$rundir");
    compare $rundir;
}
