################################################################################
# Package math_utils
#
# Provides a (scant) collection of vector, matrix, and quaternion utilities.
#
# David Hammen - OSR - July 2006
#
################################################################################

package math_utils;
require Exporter;
@ISA = qw (Exporter);
@EXPORT = qw( MxV VxS VxV VdV VpV VmV
              VxQ QmQ
              MxM MxMt MtxM MtxMt
              mtrans
              mrot_angle
              euler_matrix );

use POSIX qw(acos asin);
use strict;
sub MxV($$);
sub MtxV($$);
sub VxS($$);
sub VxV($$);
sub VdV($$);
sub VpV($$);
sub VmV($$);
sub VxQ($$);
sub QmQ($$);
sub MxM($$);
sub MxMt($$);
sub MtxM($$);
sub MtxMt($$);
sub mtrans($);
sub mrot_angle($);
sub euler_matrix($$);



sub MxV($$) {
   my ($m, $v) = @_;
   return [
      $m->[0]->[0]*$v->[0] + $m->[0]->[1]*$v->[1] + $m->[0]->[2]*$v->[2],
      $m->[1]->[0]*$v->[0] + $m->[1]->[1]*$v->[1] + $m->[1]->[2]*$v->[2],
      $m->[2]->[0]*$v->[0] + $m->[2]->[1]*$v->[1] + $m->[2]->[2]*$v->[2] ];
}


sub MtxV($$) {
   my ($m, $v) = @_;
   return [
      $m->[0]->[0]*$v->[0] + $m->[1]->[0]*$v->[1] + $m->[2]->[0]*$v->[2],
      $m->[0]->[1]*$v->[0] + $m->[1]->[1]*$v->[1] + $m->[2]->[1]*$v->[2],
      $m->[0]->[2]*$v->[0] + $m->[1]->[2]*$v->[1] + $m->[2]->[2]*$v->[2] ];
}


sub VxS($$) {
   my ($u, $s) = @_;
   return [ $u->[0]*$s, $u->[1]*$s, $u->[2]*$s ];
}


sub VxV($$) {
   my ($u, $v) = @_;
   return [
      $u->[1]*$v->[2] - $u->[2]*$v->[1],
      $u->[2]*$v->[0] - $u->[0]*$v->[2],
      $u->[0]*$v->[1] - $u->[1]*$v->[0] ];
}


sub VdV($$) {
   my ($u, $v) = @_;
   return $u->[0]*$v->[0] + $u->[1]*$v->[1] + $u->[2]*$v->[2];
}

sub VpV($$) {
   my ($u, $v) = @_;
   return [
      $u->[0] + $v->[0],
      $u->[1] + $v->[1],
      $u->[2] + $v->[2] ];
}

sub VmV($$) {
   my ($u, $v) = @_;
   return [
      $u->[0] - $v->[0],
      $u->[1] - $v->[1],
      $u->[2] - $v->[2] ];
}

sub VxQ($$) {
   my ($v, $q) = @_;
   my $qs = $q->[0];
   my $qv = [@$q[1,2,3]];
   my ($ps, $pv);
   $ps = - VdV $v, $qv;
   $pv = VpV (VxS ($v,$qs), VxV ($v,$qv));
   return [$ps, @$pv];
}

sub QmQ($$) {
   my ($u, $v) = @_;
   return [
      $u->[0] - $v->[0],
      $u->[1] - $v->[1],
      $u->[2] - $v->[2],
      $u->[3] - $v->[3] ];
}


sub MxMt($$) {
   my ($m1, $m2) = @_;
   return mtrans [
      MxV ($m1, $m2->[0]),
      MxV ($m1, $m2->[1]),
      MxV ($m1, $m2->[2])];
}


sub MxM($$) {
   my ($m1, $m2) = @_;
   return MxMt $m1, mtrans($m2);
}


sub MtxMt($$) {
   my ($m1, $m2) = @_;
   return mtrans [
      MtxV ($m1, $m2->[0]),
      MtxV ($m1, $m2->[1]),
      MtxV ($m1, $m2->[2])];
}


sub MtxM($$) {
   my ($m1, $m2) = @_;
   return MtxMt $m1, mtrans($m2);
}


sub mtrans($) {
   my ($m) = @_;
   return [
      [$m->[0]->[0], $m->[1]->[0], $m->[2]->[0]],
      [$m->[0]->[1], $m->[1]->[1], $m->[2]->[1]],
      [$m->[0]->[2], $m->[1]->[2], $m->[2]->[2]]];
}

sub mrot_angle($) {
   my ($trans) = @_;
   my $pi = 3.14159265358979323846;
   my ($symdiff, $sint, $cost); 

   $symdiff = [$trans->[1]->[2] - $trans->[2]->[1],
               $trans->[2]->[0] - $trans->[0]->[2],
               $trans->[0]->[1] - $trans->[1]->[0]];
   $sint = sqrt(VdV $symdiff, $symdiff);
   $cost = 0.5 * ($trans->[0]->[0] + $trans->[1]->[1] + $trans->[2]->[2] - 1.0);

   if ($sint == 0.0) {
      return $cost > 0.0 ? 0 : $pi;
   } elsif ($sint < -$cost) {
      return $pi - asin($sint);
   } elsif ($sint < $cost) {
      return asin($sint);
   } else {
      return acos($cost);
   }
}


{
my @seq = ([0,1,2],  # Roll_Pitch_Yaw
           [0,2,1],  # Roll_Yaw_Pitch
           [1,2,0],  # Pitch_Yaw_Roll
           [1,0,2],  # Pitch_Roll_Yaw
           [2,0,1],  # Yaw_Roll_Pitch
           [2,1,0]); # Yaw_Pitch_Roll
my @msingle = (
   sub($$) { my ($c, $s) = @_; [[1, 0, 0], [0, $c, $s], [0, -$s, $c]]; },
   sub($$) { my ($c, $s) = @_; [[$c, 0, -$s], [0, 1, 0], [$s, 0, $c]]; },
   sub($$) { my ($c, $s) = @_; [[$c, $s, 0], [-$s, $c, 0], [0, 0, 1]]; });

sub euler_matrix($$) {
   my ($seq_ind, $angles) = @_;
   my $mat = [[1, 0, 0], [0, 1, 0], [0, 0, 1]];
   my $seq = $seq[$seq_ind];

   die "Bad sequence" unless defined $seq;
   foreach my $ii (2, 1, 0) {
      my $cost = cos $angles->[$ii];
      my $sint = sin $angles->[$ii];
      my $faxis = $msingle[$seq->[$ii]];
      my $maxis = &$faxis($cost, $sint);
      $mat = math_utils::MxM $mat, $maxis;
   }
   return $mat;
}
}


1;
