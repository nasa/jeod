#!/usr/bin/perl -w

################################################################################
#
# run_trans_tests.pl
#
# Purpose:
#   Run the vehicle attitude tests, i.e., the run input files in the
#   directories whose names matches SET_RUNS/RUN*_rot_att*.
#
# Output:
#   Reports the error vectors resulting from comparing the output
#   inertial position and velocity vectors to comparison values constructed
#   by this script.
#
# Syntax:
#   ../../run_trans_tests.pl
#
#
#
################################################################################


use strict;
use Getopt::Long;

our $JEOD_HOME;
our $TRICK_HOME;
BEGIN {
   $JEOD_HOME = $ENV{JEOD_HOME};
   $TRICK_HOME = $ENV{TRICK_HOME};
   unless (defined $JEOD_HOME) {
      warn "Environment variable JEOD_HOME not set.\n";
      chomp ($JEOD_HOME = `pwd`);
      $JEOD_HOME =~ s|/models/dynamics/body_action.*||
         or die "Can't parse working directory = $JEOD_HOME";
      warn "Using $JEOD_HOME.\n"
   }
   unless (defined $TRICK_HOME) {
      warn "Environment variable TRICK_HOME not set.\n";
      $TRICK_HOME = `trick-config --prefix`;
      $TRICK_HOME = chomp($TRICK_HOME);
      warn "Using $TRICK_HOME.\n"
   }
}

use lib "$JEOD_HOME/bin/jeod";
use lib "$TRICK_HOME/bin/pm";

use Trick_Logfile;
use math_utils;
use trick_print;


sub cmp_test ($$);
sub cmp_dirs;
sub sort_test_dirs;
sub test_trans_sim($@);
sub test_orbit_sim($@);
sub print_trans_hdr();
sub test_att_sim($@);
sub print_att_hdr();
sub test_rate_sim($@);
sub print_rate_hdr();
sub run_sim($);
sub color_text($$$);
sub latex_num($$);


################################################################################

my $Pi = 3.14159265358979323846;

my %LaTeX_frame = (
   (map {($_, '\\' . ucfirst $_)} qw(inertial pfix)),
   (map {($_,    '\\' . ucfirst $_,
          "t$_", '\\T'. ucfirst $_)} qw(lvlh ned)),
   (map {($_,    '\\' . ucfirst $_,
          "t$_", '\\T'. ucfirst $_,
          "c$_", '\\'. ucfirst $_)} qw(body struct point)));

my @Class_names = qw (
   Orbit
   TransState
   LvlhTransState
   NedTransState

   RotState
   LvlhRotState
   NedRotState

   LvlhState
   NedState);
my %Active_class;
@Active_class{@Class_names} = (0) x @Class_names;

my @State_names = qw (trans Orbit att rate);
my %Active_state;
@Active_state{@State_names} = (0) x @State_names;
my $LaTeX = 0;


# Process command line options
GetOptions ('latex!' => \$LaTeX)
   or die;

{
my $class_specified = 0;
my $state_specified = 0;

foreach my $key (@ARGV) {
   my $parsed = 0;
   if (defined $Active_class{$key}) {
      $parsed = 1;
      $Active_class{$key} = 1;
      $class_specified = 1;
   }
   if (defined $Active_state{$key}) {
      $parsed = 1;
      $Active_state{$key} = 1;
      $state_specified = 1;
   }
   if (! $parsed) {
      warn "Unknown option '%s'\n";
   }
}
unless ($class_specified) {
   @Active_class{@Class_names} = (1) x @Class_names;
}
unless ($state_specified) {
   @Active_state{@State_names} = (1) x @State_names;
}
$Active_state{full} = 1;
$Active_state{rot} = $Active_state{att} || $Active_state{rate};
}


# Interpret test names based on the names of the SET_test subdirectories
my @Test_dirs = ();
my %Test_dirs = ();
my %Tests = (
   trans => [],
   att   => [],
   rate  => []);
my %Test_type = (
   full  => 15,   # Full test
   trans =>  3,   # Position + velocity
   rot   => 12,   # Attitude + rate
   pos   =>  1,   # Position
   vel   =>  2,   # Velocity
   att   =>  4,   # Attitude
   rate  =>  8);  # Rate

foreach my $dir (<SET_test/RUN_*>) {
   my ($short_name, $long_name, $veh, $test_spec);
   if (! -f "$dir/input.py") {
      ### warn "$dir does not have an input file\n";
      next;
   }
   open RUN_INPUT, '<', "$dir/input.py" or die "Can't open $dir/input.py";
   $_ = <RUN_INPUT>;
   close RUN_INPUT;
   if (/^## Long test name: (RUN_.*)/) {
      $long_name = $1;
   }
   else {
      warn "$dir/input.py does not have a test specification line\n";
      next;
   }
   if ($long_name =~ /^RUN_(PAD_39A|ISS|STS_114)(_.*)/) {
      ($veh, $test_spec) = ($1, $2);
   }
   else {
      warn "$dir/input.py test specification $long_name is invalid\n";
      next;
   }
   ($short_name = $dir) =~ s/SET_test\///;
   my @tests = ();
   my $active = 0;
   my $states = 0;
   while ($test_spec =~
          s/^_((full|trans|rot|att|rate)_
               ([A-Z][A-Za-z]*?)_
               (inertial|pfix|tbody|tstruct|tpoint|t?lvlh|t?ned)_
               (tbody|tstruct|tpoint|cbody|cstruct|cpoint)
               (_set\d+)?)
           //x) {
      my ($id, $type, $class, $src_frame, $tgt_frame, $set) =
         ($1, $2, $3, $4, $5, $6);
      my ($test_states, $ent);
      $test_states = $Test_type{$type};
      $type = 'Orbit' if $class eq 'Orbit';
      $ent = {dir     => $dir,
              type    => $type,
              states  => $test_states,
              id      => $id,
              class   => $class,
              source  => $src_frame,
              target  => $tgt_frame,
              set     => $set};
      push @tests, $ent;
      $active ||= $Active_class{$class} && $Active_state{$type};
      $states |= $test_states;
      warn "Unknown test type $type for $long_name\n"
         unless defined $test_states;
   }
   $test_spec =~ s/^_//;
   if ($test_spec ne '') {
      warn "Test spec $test_spec not fully parsed\n";
   }
   if ($test_spec =~ /(full|trans|rot|att|rate)/) {
      warn "Unhandled test $1 for $dir\n";
   }
   $Test_dirs{$dir} = {dir     => $dir,
                       name    => $short_name,
                       verbose => $long_name,
                       vehicle => $veh,
                       tests   => [@tests],
                       states  => $states,
                       auxinfo => $tests[0]->{set},
                       active  => $active};
   push @Test_dirs, $dir if $active;
}
sort_test_dirs;

printf "Running simulations ...\n";
printf "Index  Directory  Tests\n";
foreach my $dir (@Test_dirs) {
   my $dir_info = $Test_dirs{$dir};

   if ($dir_info->{active}) {
      (my $pname = $dir) =~ s/SET_test\///;
      printf "%5d  %-9s  ", $dir_info->{index}, $pname;
      my $pre = '';
      foreach my $test (@{$dir_info->{tests}}) {
         printf "%s%s\n", $pre, $test->{id};
         $pre = ' ' x 18;
      }
      run_sim $dir;
      foreach my $ent (@{$dir_info->{tests}}) {
         my $type = $ent->{type};
         if ($type eq 'full') {
            push @{$Tests{trans}}, $ent;
            push @{$Tests{att}},   $ent;
            push @{$Tests{rate}},  $ent;
         }
         elsif ($type eq 'rot') {
            push @{$Tests{att}},   $ent;
            push @{$Tests{rate}},  $ent;
         }
         else {
            push @{$Tests{$type}}, $ent;
         }
      }
   }
}

print "\n", '=' x 80, "\n";
foreach my $type (qw(trans Orbit att rate)) {
   my $test_set = $Tests{$type};
   next if ((! $Active_state{$type}) || (scalar @{$test_set} == 0));

   my ($test_fun, @limits);
   if ($type eq 'trans') {
      @limits = (1e-4, 1e-6);
      $test_fun = \&test_trans_sim;
      print "Translation Initialization Tests\n";
      print '=' x 80, "\n\n";
      print_trans_hdr;
   }

   elsif ($type eq 'Orbit') {
      @limits = (1e-3, 1e-5);
      $test_fun = \&test_orbit_sim;
      print "Orbit Initialization Tests\n";
      print '=' x 80, "\n\n";
      print_trans_hdr;
   }

   elsif ($type eq 'att') {
      @limits = (6e-12);
      $test_fun = \&test_att_sim;
      print "Attitude Initialization Tests\n";
      print '=' x 80, "\n\n";
      print_att_hdr;
   }

   elsif ($type eq 'rate') {
      @limits = (1e-14);
      $test_fun = \&test_rate_sim;
      print "Body Rate Initialization Tests\n";
      print '=' x 80, "\n\n";
      print_rate_hdr;
   }

   foreach my $test (@$test_set) {
      &$test_fun ($test, @limits);
   }
   print "\n\n", '=' x 80, "\n";
}
print "\n";
print "\n";


################################################################################

{
our (%Class_order, %Source_order, %Target_order, %Vehicle_order);
BEGIN {
   %Class_order = (
      Orbit          => 0,
      TransState     => 1,
      RotState       => 2,
      LvlhState      => 3,
      LvlhTransState => 4,
      LvlhRotState   => 5,
      NedState       => 6,
      NedTransState  => 7,
      NedRotState    => 8);
   %Source_order = (
      inertial       => 0,
      pfix           => 1,
      ned            => 2,
      lvlh           => 3,
      tbody          => 4,
      tpoint         => 5,
      tstruct        => 6,
      tlvlh          => 7,
      tned           => 8);
   %Target_order = (
      tbody          => 0,
      tstruct        => 1,
      tpoint         => 2,
      cbody          => 3,
      cstruct        => 4,
      cpoint         => 5);
   %Vehicle_order = (
      ISS            => 0,
      STS_114        => 1,
      PAD_39A        => 2);
}


# sub cmp_test: Compares to tests for sorting purposes.
sub cmp_test ($$) {
   my ($test_a, $test_b) = @_;
   return ($test_a->{states} <=> $test_b->{states}) ||
          ($Class_order{$test_a->{class}} <=>
           $Class_order{$test_b->{class}}) ||
          ($Source_order{$test_a->{source}} <=>
           $Source_order{$test_b->{source}}) ||
          ($Target_order{$test_a->{target}} <=>
           $Target_order{$test_b->{target}}) ||
          0;
}


# sub cmp_dirs: Compares two directories for sorting purposes.
sub cmp_dirs {
   my $info_a  = $Test_dirs{$a};
   my $info_b  = $Test_dirs{$b};
   my $tests_a = $info_a->{tests};
   my $tests_b = $info_b->{tests};
   my $cmp = ($info_a->{states} <=> $info_b->{states}) ||
             ($#$tests_a <=> $#$tests_b);

   return $cmp if $cmp != 0;

   for (my $ii = 0; $ii <= $#$tests_a; $ii++) {
      $cmp = cmp_test $tests_a->[$ii], $tests_b->[$ii];
      return $cmp if $cmp != 0;
   }

   return ($Vehicle_order{$info_a->{vehicle}} <=>
           $Vehicle_order{$info_b->{vehicle}}) ||
          ($a cmp $b);
}
}


# sub sort_test_dirs: Sorts and indices the test directories.
sub sort_test_dirs {
   my $index = 1;
   @Test_dirs = sort cmp_dirs @Test_dirs;
   foreach my $dir (@Test_dirs) {
      $Test_dirs{$dir}->{index} = $index++;
   }
}


# sub test_trans_sim: Runs one test.
# A test comprises running the simulation against the input file in the
# supplied run directory and then comparing output value(s) against the
# input rate.
sub test_trans_sim($@) {
   my ($test, $pos_lim, $vel_lim) = @_;
   my ($run_dir, $type, $id, $class, $src_frame, $tgt_frame) =
      @$test{qw(dir type id class source target)};
   my $dir_info  = $Test_dirs{$run_dir};
   my $vehicle   = $dir_info->{vehicle};
   my $ref = $type eq 'full' ? '' : 'trans_';
   my ($veh, $tgt_name, $ref_name);
   my ($source_name, $target_name);
   my $mismatch = 0;

   # Generate target and reference state names.
   $source_name = $src_frame;
   $target_name = $tgt_frame;

   # The reference state has been pre-computed for the target runs.
   if ($tgt_frame =~ /^t/) {
      $veh = 'target';
      if ($src_frame eq 'ned') {
         $mismatch = 1 unless $tgt_frame eq 'tstruct';
         $ref_name = "target.ned_${ref}init";
         $tgt_name = 'target_rel_state.tstruct_wrt_pad39a.rel_state.trans';
      }
      elsif ($src_frame eq 'pfix') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_pfix.rel_state.trans';
         $ref_name = "target.trans_init";
      }
      elsif ($src_frame eq 'inertial') {
         my $tgt_id;
         if ($tgt_frame eq 'tbody') {
            $tgt_id = 'composite_body';
         }
         elsif ($tgt_frame eq 'tstruct') {
            $tgt_id = 'structure';
         }
         else {
            $mismatch = 1;
         }
         $tgt_name = "target.dyn_body.${tgt_id}.state.trans";
         $ref_name = "target.trans_init";
         $source_name = 'inertial';
      }
      else {
         $mismatch = 1;
      }
   }

   # Compute the reference state for the chaser runs.
   elsif ($tgt_frame =~ /^c/) {
      $veh = 'chaser';
      my $rel_name;
      if ($src_frame =~ /^t(body|struct|point)$/) {
         $rel_name = $1;
         $ref_name = 'chaser.trans_init';
      }
      elsif ($src_frame =~ /^t(lvlh|ned)$/) {
         $rel_name = $1;
         $ref_name = "chaser.${rel_name}_${ref}init";
      }
      else {
         $mismatch = 1;
      }

      if ($tgt_frame =~ /^c(body|struct|point)$/) {
         $tgt_name = join '.', 'chaser_rel_state',
                               "${tgt_frame}_wrt_${src_frame}",
                               'rel_state.trans';
      }
      else {
         $mismatch = 1;
      }
   }

   else {
      $mismatch = 1;
   }

   if ($mismatch) {
      warn "Test $run_dir has unhandled translation\nSkipping test\n";
      return;
   }

   my ($log_file,
       $tgt_pos_name, $tgt_vel_name,
       $ref_pos_name, $ref_vel_name,
       $tgt_pos, $tgt_vel,
       $ref_pos, $ref_vel,
       $pos_err, $vel_err,
       $pos_rms_err, $vel_rms_err);

   # Open the log file and read the first (and only) block.
   $log_file = Trick_Logfile->new ("$run_dir/log_${veh}.trk");
   $log_file->read_block;
   $tgt_pos_name = "$tgt_name.position";
   $tgt_vel_name = "$tgt_name.velocity";
   $ref_pos_name = "$ref_name.position";
   $ref_vel_name = "$ref_name.velocity";

   # Extract initialized and reference states.
   $tgt_pos = $log_file->get_vec ($tgt_pos_name);
   $tgt_vel = $log_file->get_vec ($tgt_vel_name);
   $ref_pos = $log_file->get_vec ($ref_pos_name);
   $ref_vel = $log_file->get_vec ($ref_vel_name);

   $log_file->close();
   undef $log_file;


    # Compute RMS errors
   $pos_err     = VmV $tgt_pos, $ref_pos;
   $pos_rms_err = sqrt (VdV $pos_err, $pos_err);
   $vel_err     = VmV $tgt_vel, $ref_vel;
   $vel_rms_err = sqrt (VdV $vel_err, $vel_err);

   # Report (one liner)
   if ($LaTeX) {
      my $run_name = $dir_info->{name};
      my ($pos_str, $vel_str, $status);
      $run_name =~ s/_/\\_/g;
      $target_name =~ s/^t//g;
      $class =~ s/State$//;
      $vehicle =~ s/_/ /g;
      $vehicle =~ s/PAD/Pad/;
      $status = '\passed';
      $pos_str = latex_num $pos_rms_err, $pos_lim;
      $vel_str = latex_num $vel_rms_err, $pos_lim;
      $status  = '\failed' if ($pos_str =~ /\\red/) || ($vel_str =~ /\\red/);
      printf "%-9s & %-10s & %-9s & %-7s & %-7s &\n",
         $run_name, $class,
         $LaTeX_frame{$source_name}, $LaTeX_frame{$target_name},
         $vehicle;
      printf "  %-22s & %-22s & %s \\\\\n",
         $pos_str, $vel_str, $status;
#123456789 & 1234567890 & 123456789 & 1234567 & 1234567 &
#RUN\_0400 & Trans      & \Inertial & \Body   & ISS     &
#  1234567890123456789012 & 1234567890123456789012 & 1234567
#  \green{$7.7\eneg{10}$} & \green{$1.0\eneg{12}$} & \passed \\

   }
   else {
      printf "%-8s  %-14s  %-7s  %-8s  %-7s  %8s  %8s\n",
         $dir_info->{name}, $class, $vehicle,
         $source_name, $target_name,
         color_text ('%8.1e', $pos_rms_err, $pos_lim),
         color_text ('%8.1e', $vel_rms_err, $vel_lim),
   }
}


# sub test_orbit_sim: Runs one test.
# A test comprises running the simulation against the input file in the
# supplied run directory and then comparing output value(s) against the
# input rate.
sub test_orbit_sim($@) {
   my ($test, $pos_lim, $vel_lim) = @_;
   my ($run_dir, $type, $id, $class, $src_frame, $tgt_frame) =
      @$test{qw(dir type id class source target)};
   my $dir_info  = $Test_dirs{$run_dir};
   my $vehicle   = $dir_info->{vehicle};
   my $aux_info  = $dir_info->{auxinfo};
   my $ref = $type eq 'full' ? '' : 'trans_';
   my ($veh, $tgt_name, $ref_name);
   my ($source_name, $target_name, $set_no);
   my $mismatch = 0;

   # Generate target and reference state names.
   $source_name = $src_frame;
   $target_name = $tgt_frame;

   # The reference state has been pre-computed for the target runs.
   if ($tgt_frame =~ /^t/) {
      $veh = 'target';
      if ($class eq 'Orbit') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $ref_name = 'target.expected_state.trans';
         if ($src_frame eq 'pfix') {
            $tgt_name = 'target_rel_state.tbody_wrt_pfix.rel_state.trans';
         }
         elsif ($src_frame eq 'inertial') {
            $tgt_name = 'target.dyn_body.composite_body.state.trans';
         }
         else {
            $mismatch = 1;
         }
         $target_name = 'body';
         ($set_no = $aux_info) =~ s/_set//;
      }
      else {
         $mismatch = 1;
      }
   }

   else {
      $mismatch = 1;
   }

   if ($mismatch) {
      warn "Test $run_dir has unhandled translation\nSkipping test\n";
      return;
   }

   my ($log_file,
       $tgt_pos_name, $tgt_vel_name,
       $ref_pos_name, $ref_vel_name,
       $tgt_pos, $tgt_vel,
       $ref_pos, $ref_vel,
       $pos_err, $vel_err,
       $pos_rms_err, $vel_rms_err);

   # Open the log file and read the first (and only) block.
   $log_file = Trick_Logfile->new ("$run_dir/log_${veh}.trk");
   $log_file->read_block;
   $tgt_pos_name = "$tgt_name.position";
   $tgt_vel_name = "$tgt_name.velocity";
   $ref_pos_name = "$ref_name.position";
   $ref_vel_name = "$ref_name.velocity";

   # Extract initialized and reference states.
   $tgt_pos = $log_file->get_vec ($tgt_pos_name);
   $tgt_vel = $log_file->get_vec ($tgt_vel_name);
   $ref_pos = $log_file->get_vec ($ref_pos_name);
   $ref_vel = $log_file->get_vec ($ref_vel_name);

   $log_file->close();
   undef $log_file;


    # Compute RMS errors
   $pos_err     = VmV $tgt_pos, $ref_pos;
   $pos_rms_err = sqrt (VdV $pos_err, $pos_err);
   $vel_err     = VmV $tgt_vel, $ref_vel;
   $vel_rms_err = sqrt (VdV $vel_err, $vel_err);

   # Report (one liner)
   if ($LaTeX) {
      my $run_name = $dir_info->{name};
      my ($pos_str, $vel_str, $status);
      $set_no += 0;
      $run_name =~ s/_/\\_/g;
      $vehicle =~ s/_/ /g;
      $status = '\passed';
      $pos_str = latex_num $pos_rms_err, $pos_lim;
      $vel_str = latex_num $vel_rms_err, $pos_lim;
      $status  = '\failed' if ($pos_str =~ /\\red/) || ($vel_str =~ /\\red/);
      printf "%-9s & %-9s & %-7s & %2d &\n",
         $run_name, $LaTeX_frame{$source_name}, $vehicle, $set_no;
      printf "  %-22s & %-22s & %s \\\\\n",
         $pos_str, $vel_str, $status;
#0123
#123456789 & 123456789 & 1234567 & 12 &
#RUN\_0001 & \Inertial & ISS     & 01 &
#  1234567890123456789012 & 1234567890123456789012 & 1234567
#  \green{$4.9\eneg{5}$}  & \green{$6.6\eneg{8}$}  & \passed \\
   }
   else {
      printf "%-8s  %-14s  %-7s  %-8s  %-7s  %8s  %8s\n",
         $dir_info->{name}, "Orbit set_$set_no", $vehicle,
         $source_name, $target_name,
         color_text ('%8.1e', $pos_rms_err, $pos_lim),
         color_text ('%8.1e', $vel_rms_err, $vel_lim),
   }
}

sub print_trans_hdr () {
   printf "%-8s  %-14s  %-7s  %-8s  %-7s  %8s  %8s\n" .
          "%-8s  %-14s  %-7s  %-8s  %-7s  %8s  %8s\n",
      'Run',  'Class', 'Vehicle', 'Source', 'Subject', 'Pos Err', 'Vel Err',
      'Name', '',      '',        'Frame',  'Frame',   '(m)',     '(m/s)';
}


################################################################################


# sub test_att_sim: Runs one test.
# A test comprises running the simulation against the input file in the
# supplied run directory and then comparing output value(s) against the
# input rate.
sub test_att_sim($@) {
   my ($test, $err_lim) = @_;
   my ($run_dir, $type, $id, $class, $src_frame, $tgt_frame) =
      @$test{qw(dir type id class source target)};
   my $dir_info = $Test_dirs{$run_dir};
   my $vehicle  = $dir_info->{vehicle};
   my $ref = $type eq 'full' ? '' : 'att_';
   my ($veh, $tgt_name, $ref_name);
   my ($source_name, $target_name);
   my $mismatch = 0;

   # Generate target and reference state names.
   $source_name = $src_frame;
   $target_name = $tgt_frame;

   # Vehicle being tested is the target
   if ($tgt_frame =~ /^t/) {
      $veh = 'target';
      if ($src_frame =~ /lvlh/) {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_tlvlh.rel_state';
         $ref_name = "target.lvlh_${ref}init";
         $source_name = 'tlvlh';
      }
      elsif ($src_frame eq 'ned') {
         $mismatch = 1 unless $tgt_frame eq 'tstruct';
         $tgt_name = 'target_rel_state.tstruct_wrt_pad39a.rel_state';
         $ref_name = "target.ned_${ref}init";
         $source_name = 'pad_ned';
      }
      elsif ($src_frame eq 'tned') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_tned.rel_state';
         $ref_name = "target.ned_${ref}init";
      }
      elsif ($src_frame eq 'pfix') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_pfix.rel_state';
         $ref_name = "target.att_init";
      }
      elsif ($src_frame eq 'inertial') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target.dyn_body.composite_body.state';
         $ref_name = "target.att_init";
      }
      else {
         $mismatch = 1;
      }
   }

   # Vehicle being tested is the chaser
   elsif ($tgt_frame =~ /^c/) {
      $veh = 'chaser';
      my $rel_name;
      if ($src_frame =~ /^t(body|struct|point)$/) {
         $rel_name = $1;
         $ref_name = 'chaser.att_init';
      }
      elsif ($src_frame =~ /^t(lvlh|ned)$/) {
         $rel_name = $1;
         $ref_name = "chaser.${rel_name}_${ref}init";
      }
      else {
         $mismatch = 1;
      }

      if ($tgt_frame =~ /^c(body|struct|point)$/) {
         $tgt_name = join '.', 'chaser_rel_state',
                               "${tgt_frame}_wrt_${src_frame}",
                               'rel_state';
      }
      else {
         $mismatch = 1;
      }
   }

   else {
      $mismatch = 1;
   }

   if ($mismatch) {
      warn "Test $run_dir has unhandled attitude\nSkipping test\n";
      return;
   }

   my ($log_file,
       $tgt_matrix_name, $ref_euler_name,
       $seq, $angles,
       $T_init, $T_euler,
       $att_err, $run_short_name);

   # Open the log file and read the first (and only) block.
   $log_file = Trick_Logfile->new ("$run_dir/log_${veh}.trk");
   $log_file->read_block;
   $tgt_matrix_name = "$tgt_name.rot.T_parent_this";
   $ref_euler_name  = "$ref_name.orientation.euler";

   # Extract initialized and reference states.
   $seq    = $log_file->get_scalar ("${ref_euler_name}_sequence");
   $angles = $log_file->get_vec    ("${ref_euler_name}_angles");
   $T_init = $log_file->get_mat    ($tgt_matrix_name);

   $log_file->close();
   undef $log_file;

    # Form the transformation matrix corresponding to the euler rotation.
   $T_euler = euler_matrix $seq, $angles;

    # The reference matrix T_ref should be equal to T_euler.
    # The product of T_euler and T_ref transpose should be the identity matrix.
    # The single axis rotation angle corresponding to this product is a
    # measure of the error in the computation.
   $att_err = mrot_angle (MxMt $T_euler, $T_init) * 180.0/$Pi;

   # Report (one liner)
   if ($LaTeX) {
      my $run_name = $dir_info->{name};
      my ($att_str, $status);
      $run_name =~ s/_/\\_/g;
      $source_name =~ s/^pad_//;
      $target_name =~ s/^t//g;
      $class =~ s/State$//;
      $vehicle =~ s/_/ /g;
      $vehicle =~ s/PAD/Pad/;
      $status = '\passed';
      $att_str = latex_num $att_err, $err_lim;
      $status  = '\failed' if ($att_str =~ /\\red/);
      printf "%-9s & %-10s & %-9s & %-7s & %-7s &\n",
         $run_name, $class,
         $LaTeX_frame{$source_name}, $LaTeX_frame{$target_name},
         $vehicle;
      printf "  %-22s & %s \\\\\n",
         $att_str, $status;
   }
   else {
      printf "%8s  %-14s  %-7s  %-8s  %-7s  %8s\n",
         $dir_info->{name}, $class, $dir_info->{vehicle},
         $source_name, $target_name,
         color_text ('%8.1e', $att_err, $err_lim),
   }
}

sub print_att_hdr () {
   printf "%-8s  %-14s  %-7s  %-8s  %-7s  %8s\n" .
          "%-8s  %-14s  %-7s  %-8s  %-7s  %8s\n",
      'Run',  'Class', 'Vehicle', 'Source', 'Subject', 'Att Err',
      'Name', '',      '',        'Frame',  'Frame',       '(d)';
}


################################################################################


# sub test_rate_sim: Runs one test.
# A test comprises running the simulation against the input file in the
# supplied run directory and then comparing output value(s) against the
# input rate.
sub test_rate_sim($@) {
   my ($test, $rate_lim) = @_;
   my ($run_dir, $type, $id, $class, $src_frame, $tgt_frame) =
      @$test{qw(dir type id class source target)};
   my $dir_info = $Test_dirs{$run_dir};
   my $vehicle  = $dir_info->{vehicle};
   my $ref = $type eq 'full' ? '' : $type eq 'rot' ? 'att_' : 'rate_';
   my ($veh, $tgt_name, $ref_name);
   my ($source_name, $target_name);
   my $mismatch = 0;

   # Generate target and reference state names.
   $source_name = $src_frame;
   $target_name = $tgt_frame;

   # Vehicle being tested is the target
   if ($tgt_frame =~ /^t/) {
      $veh = 'target';
      if ($src_frame =~ /lvlh/) {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_tlvlh.rel_state';
         $ref_name = "target.lvlh_${ref}init";
         $source_name = 'tlvlh';
      }
      elsif ($src_frame eq 'ned') {
         $mismatch = 1 unless $tgt_frame eq 'tstruct';
         $tgt_name = 'target_rel_state.tstruct_wrt_pad39a.rel_state';
         $ref_name = "target.ned_${ref}init";
         $source_name = 'pad_ned';
      }
      elsif ($src_frame eq 'tned') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_tned.rel_state';
         $ref_name = "target.ned_${ref}init";
      }
      elsif ($src_frame eq 'pfix') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target_rel_state.tbody_wrt_pfix.rel_state';
         $ref_name = "target.${ref}init";
      }
      elsif ($src_frame eq 'inertial') {
         $mismatch = 1 unless $tgt_frame eq 'tbody';
         $tgt_name = 'target.dyn_body.composite_body.state';
         $ref_name = "target.${ref}init";
      }
      else {
         $mismatch = 1;
      }
   }

   # Vehicle being tested is the chaser
   elsif ($tgt_frame =~ /^c/) {
      $veh = 'chaser';
      my $rel_name;
      if ($src_frame =~ /^t(body|struct|point)$/) {
         $rel_name = $1;
         $ref_name = "chaser.${ref}init";
      }
      elsif ($src_frame =~ /^t(lvlh|ned)$/) {
         $rel_name = $1;
         $ref_name = "chaser.${rel_name}_${ref}init";
      }
      else {
         $mismatch = 1;
      }

      if ($tgt_frame =~ /^c(body|struct|point)$/) {
         $tgt_name = join '.', 'chaser_rel_state',
                               "${tgt_frame}_wrt_${src_frame}",
                               'rel_state';
      }
      else {
         $mismatch = 1;
      }
   }

   else {
      $mismatch = 1;
   }

   if ($mismatch) {
      warn "Test $run_dir has unhandled rate\nSkipping test\n";
      return;
   }

   my ($log_file,
       $tgt_rate_name,
       $ref_rate_name,
       $tgt_rate,
       $ref_rate,
       $rate_err,
       $rate_rms_err);

   # Open the log file and read the first (and only) block.
   $log_file = Trick_Logfile->new ("$run_dir/log_${veh}.trk");
   $log_file->read_block;
   $tgt_rate_name = "$tgt_name.rot.ang_vel_this";
   $ref_rate_name = "$ref_name.ang_velocity";

   # Extract initialized and reference states.
   $tgt_rate = $log_file->get_vec ($tgt_rate_name);
   $ref_rate = $log_file->get_vec ($ref_rate_name);

   $log_file->close();
   undef $log_file;


    # Compute RMS errors
   $rate_err     = VmV $tgt_rate, $ref_rate;
   $rate_rms_err = sqrt (VdV $rate_err, $rate_err) * 180.0/$Pi;

   # Report (one liner)
   if ($LaTeX) {
      my $run_name = $dir_info->{name};
      my ($rate_str, $status);
      $run_name =~ s/_/\\_/g;
      $source_name =~ s/^pad_//;
      $target_name =~ s/^t//g;
      $class =~ s/State$//;
      $vehicle =~ s/_/ /g;
      $vehicle =~ s/PAD/Pad/;
      $status = '\passed';
      $rate_str = latex_num $rate_rms_err, $rate_lim;
      $status  = '\failed' if ($rate_str =~ /\\red/);
      printf "%-9s & %-10s & %-9s & %-7s & %-7s &\n",
         $run_name, $class,
         $LaTeX_frame{$source_name}, $LaTeX_frame{$target_name},
         $vehicle;
      printf "  %-22s & %s \\\\\n",
         $rate_str, $status;
   }
   else {
      printf "%8s  %-14s  %-7s  %-8s  %-7s  %8s\n",
         $dir_info->{name}, $class, $dir_info->{vehicle},
         $source_name, $target_name,
         color_text ('%8.1e', $rate_rms_err, $rate_lim),
   }
}

sub print_rate_hdr () {
   printf "%-8s  %-14s  %-7s  %-8s  %-7s  %8s\n" .
          "%-8s  %-14s  %-7s  %-8s  %-7s  %8s\n",
      'Run',  'Class', 'Vehicle', 'Source', 'Subject', 'Rate Err',
      'Name', '',      '',        'Frame',  'Frame',      '(d/s)';
}


################################################################################


# sub run_sim: Run the simulation.
sub run_sim($) {
   my ($run_dir) = @_;
   my $run_input = "$run_dir/input.py";
   my @old_files = <$run_dir/log_*>;

   if (! -f $run_input) {
      die "Missing input file for $run_dir";
   }

return if (-f "$run_dir/run.out");

   foreach my $f (qw(chkpnt_post_init S_job_execution run.err run.out)) {
      push @old_files, "$run_dir/$f" if -f "$run_dir/$f";
   }

   foreach my $f (@old_files) {
      unlink $f or die "Can't delete $f";
   }


   open SAVEOUT, '>&', *STDOUT;
   open SAVEERR, '>&', *STDERR;
   open STDOUT, '>', "$run_dir/run.out" or die 'Can\'t redirect STDOUT';
   open STDERR, '>', "$run_dir/run.err" or die 'Can\'t redirect STDOUT';
   system "./S_main_$ENV{TRICK_HOST_CPU}.exe $run_input";
   open STDOUT, '>&', *SAVEOUT;
   open STDERR, '>&', *SAVEERR;


###   unless (-f "$run_dir/log_${veh}.trk") {
###      die $run_dir . `cat "$run_dir/run.err"`;
###   }
}


sub color_text ($$$) {
   my ($fmt, $val, $lim) = @_;
   my $color;
   $color = (($val < -$lim) || ($val > $lim)) ? 31 : 32;
   return sprintf "\e[%dm${fmt}\e[00m", $color, $val;
}


sub latex_num ($$) {
   my ($val, $lim) = @_;
   my $color;
   $color = (($val < -$lim) || ($val > $lim)) ? '\red' : '\green';
   (my $str = sprintf ("%.1e", $val)) =~ /(-?\d\.\d)e([+-])(\d\d)/ or die;
   my ($man, $esign, $eval) = ($1, $2, $3);
   $eval = $eval + 0;
   if ($eval == 0) {
      $str = "\$$man\$";
   }
   else {
      $esign = ($esign eq '-') ? '\eneg' : '\epos';
      $str = "\$$man$esign\{$eval\}\$";
   }
   return "$color\{$str\}";
}
