#!/usr/bin/perl -w

use strict;
use IO::Handle;
use Getopt::Long;
use POSIX ":sys_wait_h";

sub main ();
sub set_unhandled_classes ();
sub make_run_dirs ();
sub make_omega_dt_list ();
sub start_available ($$$$$$);
sub start_sim ($$$$$$);
sub collect_ready ($);
sub collect_ready_output($$);
sub collect_finished_output($$$);
sub print_report ($$$);
sub reap_hung_processes ($);
sub make_select (@);
sub add_handle($$);
sub del_handle($$);
sub can_read($$);

my $Exec = "./S_main_$ENV{TRICK_HOST_CPU}.exe";
my @Classes = qw(
   RotationTestTorqueFreeSphere
   RotationTestTorqueFreeSymTop
   RotationTestTorqueFreeSymTopB
   RotationTestSHOSphere
   TranslationTestOrbit
   TranslationTestSpringDamper);
my %Classes;
@Classes{@Classes} = (0 .. $#Classes);

my @Techniques = qw(
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

   GaussJackson2
   GaussJackson4
   GaussJackson6
   GaussJackson8
   GaussJackson10
   GaussJackson12
);
### Add RKNystrom4 to the above
### when (if) it is incorporated into ER7 utils.

my %Techniques;
@Techniques{@Techniques} = (0 .. $#Techniques);

my %Handles = ();
foreach my $technique (@Techniques) {
   my %handles = ();
   @handles{@Classes} = (1) x scalar @Classes;
   $Handles{$technique} = \%handles;
}

my %Options = (
   class        => [],
   max_logwdt   => 1,
   min_logwdt   => -4.5,
   del_logwdt   => 0.25,
   integ_opt    => [],
   skip_integ   => [],
   rotation     => 0,
   translation  => 0,
   old_integ    => 0, 
   ncpus        => 2,
   nrevs        => 1,
   prefix       => 'run_cases',
   start        => 0,
   ncases       => 20,
   run_size     => 20,
   report       => 1,
   dot          => 0,
   dry_run      => 0);

my $Syntax = "Syntax: $0 [Options]";

my %Pid_info = ();
my %Fid_info = ();

my %Rundirs = (free  => {},
               inuse => {},
               names => []);

# Number of lines in each summary report
# NOTE: Change this if the report changes
my %Report_lines = (
   RotationTest    => 18,
   TranslationTest => 16);
my $Last_time = 0;


my %Report;
my ($Navail, $Nactive);

my $Npending = 0;
$SIG{CHLD} = sub {$Npending++};
main();



################################################################################

sub main () {
   GetOptions (\%Options,
               'class=s@',
               'max_logwdt=f',
               'min_logwdt=f',
               'del_logwdt=f',
               'integ_opt=s@',
               'skip_integ=s@',
               'rotation!',
               'translation!',
               'old_integ!',
               'ncpus=i',
               'nrevs=i',
               'prefix=s',
               'start|begin=i',
               'ncases|end=i',
               'run_size=i',
               'report!',
               'dot!',
               'dry_run')
      or die;
   ($Options{del_logwdt} > 0)
      or die "Bad del_logwdt $Options{del_logwdt}";
   (($Options{ncpus} > 0) && ($Options{ncpus} < 16))
      or die "Bad ncpus $Options{ncpus}";
   (($Options{ncases} > 0) && ($Options{ncases} < 2000))
      or die "Bad ncases $Options{ncases}";
   (($Options{run_size} >= 0) && ($Options{run_size} < 2000))
      or die "Bad run_size $Options{run_size}";
   (($Options{start} >= 0) && ($Options{start} < $Options{ncases}))
      or die "Bad start $Options{start}";
   ($Options{prefix} =~ /^\S+$/)
      or die "Bad prefix $Options{prefix}";

   my @omega_dt = make_omega_dt_list;
   my %mode_classes = ();
   my @mode_classes = ();
   push @mode_classes, grep /^Rotation/, @Classes    if $Options{rotation};
   push @mode_classes, grep /^Translation/, @Classes if $Options{translation};
   foreach my $class (@mode_classes, @{$Options{class}}) {
      if (defined $Classes{$class}) {
         $mode_classes{$class} = 1;
      }
      else {
         die "Bad class $class";
      }
   }
   @mode_classes = sort {$Classes{$a} <=> $Classes{$b}} keys %mode_classes;
   scalar @mode_classes
      or die "No tests are enabled";

   my @integ_opt = @{$Options{integ_opt}};
   @integ_opt = @Techniques unless scalar @integ_opt;
   foreach my $integ_opt (@integ_opt) {
      die "Bad integration technique $integ_opt"
         unless defined $Techniques{$integ_opt};
   }

   if (scalar @{$Options{skip_integ}}) {
      my %skip = ();
      @skip{@{$Options{skip_integ}}} = (1) x scalar @{$Options{skip_integ}};
      foreach my $integ_opt (@{$Options{skip_integ}}) {
         warn "Ignoring bad skip option '$integ_opt'\n"
            unless defined $Techniques{$integ_opt};
      }
      @integ_opt = grep {! exists $skip{$_}} @integ_opt;
   }

   set_unhandled_classes();

   my $open_mode;
   if (-f "$Options{prefix}.dat") {
      $open_mode = '>>';
   }
   else {
      $open_mode = '>';
   }
   open SAVOUT, ">&STDOUT";
   open SAVERR, ">&STDERR";
   SAVOUT->autoflush(1);
   SAVERR->autoflush(1);
   open STDOUT, $open_mode, "$Options{prefix}.out";
   open STDERR, $open_mode, "$Options{prefix}.err";
   open RUNDAT, $open_mode, "$Options{prefix}.dat";
   %Report = (
      report => *RUNDAT,
      stdout => *STDOUT,
      stderr => *STDERR);
   STDOUT->autoflush (1);
   STDERR->autoflush (1);
   RUNDAT->autoflush (1);

   my $select = make_select();

   $Navail  = $Options{ncpus};
   $Nactive = 0;
   make_run_dirs;

   for (my $ii = 0; $ii < 7; $ii++) {
      $ENV{"RUN_TEST_ID_$ii"} = -1;
   }

   # Make the run proceed from hardest/longest to easiest/fastest
   @integ_opt = sort {$Techniques{$b} <=> $Techniques{$a}} @integ_opt;
   @omega_dt = reverse @omega_dt;

   my $total_runs = 0;
   $Npending = 0;
   foreach my $omega_dt (@omega_dt) {
      for (my $itest = $Options{start};
           $itest < $Options{ncases};
           $itest += $Options{run_size}) {
         foreach my $integ_opt (@integ_opt) {
            my @run_classes = grep {$Handles{$integ_opt}->{$_}} @mode_classes;
            next unless scalar @run_classes;
            my $nruns = $Options{run_size};
            $nruns = $Options{ncases} - $itest
            if ($itest + $nruns > $Options{ncases});
            if (! $Options{dry_run}) {
               start_sim $select, \@run_classes, $integ_opt, $omega_dt,
                         $itest, $nruns;
               $total_runs += scalar @mode_classes;
               print SAVOUT '.' if $Options{dot};
            }
            else {
               print +(join ' ',
                            "start_sim",
                            "select=$select",
                            ("run_classes=" . (join ',', @run_classes)),
                            "integ_opt=$integ_opt",
                            "omega_dt=$omega_dt",
                            "itest=$itest",
                            "nruns=$nruns\n");
            }
         }
      }
      print SAVOUT '+' if $Options{dot};
   }
   while (scalar keys %Pid_info) {
      collect_ready $select;
   }
   print SAVOUT "\nTotal runs = $total_runs\n";
}


################################################################################

sub set_unhandled_classes() {
   my @missing_lie_integrator = qw(
         Beeman
         NystromLear2
         ModifiedMidpoint4
         AdamsBashforthMoulton4
         RKGill4
         GaussJackson2
         GaussJackson4
         GaussJackson6
         GaussJackson8
         GaussJackson10
         GaussJackson12);
   if (!$Options{old_integ}) {
      foreach my $class (@Classes) {
         if ($class =~ /^Rotation/) {
            foreach my $technique (@missing_lie_integrator) {
               $Handles{$technique}->{$class} = 0;
            }
         }
      }
   }
}


sub make_run_dirs() {
   my $source = 'Modified_data/run_env.py';
   my $run_dir_parent = "$Options{prefix}_run";
   if ($Options{old_integ}) {
      $source =~ s/run_env/run_env_old_integ/;
   }
   if (! -d $run_dir_parent) {
      mkdir $run_dir_parent
         or die "Can't make directory $run_dir_parent";
   }
   for (my $ii = 0; $ii < $Navail; $ii++) {
      my $run_dir = sprintf "%s/run_%02d", $run_dir_parent, $ii;
      if (-d $run_dir) {
         system "rm -rf $run_dir"
            and die "Can't delete old $run_dir";
      }
      mkdir $run_dir
         or die "Can't make directory $run_dir";
      system 'cp', $source, "$run_dir/input.py"
         and die "Can't create $run_dir/input.py";
      $Rundirs{names}->[$ii] = $run_dir;
      $Rundirs{free}->{$ii} = 1;
   }
}


sub make_omega_dt_list () {
   my $inv_del = int(1.0/$Options{del_logwdt}+0.5);
   my @pow10 = map {int(100*10**($_/$inv_del)+0.5)/100} (0..$inv_del-1);
   my ($log_min, $log_max);
   my @result;
   my $x;

   $x = $Options{min_logwdt};
   $log_min = $x != 0 ? int($x*$inv_del +0.5*$x/abs($x)) : 0;
   $x = $Options{max_logwdt};
   $log_max = $x != 0 ? int($x*$inv_del +0.5*$x/abs($x)) : 0;
   for (my $ii = $log_min; $ii <= $log_max; $ii++) {
      my $mod = $ii % $inv_del;
      my $base = +('1e' . +(($ii - $mod) / $inv_del)) + 0;
      push @result, $base * $pow10[$mod];
   }
   return reverse @result;
}


sub start_sim ($$$$$$) {
   my ($select, $classes, $integ_opt, $omega_dt, $case, $nruns) = @_;

   foreach my $class (@$classes) {
      $Handles{$integ_opt}->{$class} or die "Internal error.";
   }

   my @class_ids = map {$Classes{$_}} @$classes;

   local $^F = 9999; # Arggh.

   while ($Navail == 0) {
      collect_ready $select;
   }

   my $free = $Rundirs{free};
   my @free = sort keys %$free;
   scalar @free or die;
   my $runidx = $free[0];
   delete $free->{$runidx};
   my $rundir = $Rundirs{names}->[$runidx];
   my $input = "${rundir}/input.py";

   my $report_to_parent  = new IO::Handle;
   my $report_from_child = new IO::Handle;
   pipe $report_from_child, $report_to_parent;

   my $stdout_to_parent  = new IO::Handle;
   my $stdout_from_child = new IO::Handle;
   pipe $stdout_from_child, $stdout_to_parent;

   my $stderr_to_parent  = new IO::Handle;
   my $stderr_from_child = new IO::Handle;
   pipe $stderr_from_child, $stderr_to_parent;

   my $pid = fork();
   if (!defined $pid) {
      die "Fork failed\n";
   }

   # Child
   if ($pid == 0) {
      my $report_fid = fileno ($report_to_parent);
      close SAVOUT;
      close SAVERR;
      close RUNDAT;

      close $report_from_child;
      autoflush $report_to_parent 1;

      close $stdout_from_child;
      autoflush $stdout_to_parent 1;
      close STDOUT;
      open STDOUT, '>&', $stdout_to_parent;
      autoflush STDOUT 1;

      close $stderr_from_child;
      autoflush $stderr_to_parent 1;
      close STDERR;
      open STDERR, '>&', $stderr_to_parent;
      autoflush STDERR 1;
      print STDERR "\nStarting simulation pid=$$\n",
                   "run dir    = $rundir\n",
                   "integ opt  = $integ_opt\n",
                   "omega dt   = $omega_dt\n",
                   "test type  = @class_ids\n",
                   "start case = $case\n",
                   "end case   = $nruns\n",
                   "report fid = $report_fid\n";

      $ENV{RUN_LOG_DATA}  = 0;
      $ENV{RUN_TRACE}     = 0;
      $ENV{RUN_OMEGA_DT}  = $omega_dt;
      $ENV{RUN_INTEG_OPT} = $integ_opt;
      $ENV{RUN_CASENO}    = $case;
      $ENV{RUN_NTESTS}    = $nruns;
      $ENV{RUN_NREVS}     = $Options{nrevs};
      $ENV{RUN_DO_REPORT} = $Options{report};
      $ENV{RUN_REPORT_ID} = $report_fid;
      for (my $ii = 0; $ii <= $#$classes; $ii++) {
         $ENV{"RUN_TEST_ID_$ii"} = $class_ids[$ii];
      }

      foreach my $key (sort keys %ENV) {
         if ($key =~ /^RUN/) {
            print STDERR "setenv $key $ENV{$key}\n";
         }
      }

### system $Exec, $input, '-d';
      exec $Exec, $input;
      die;
   }

   $report_to_parent->close();
   $stdout_to_parent->close();
   $stderr_to_parent->close();

   my @file_info = (
      {name => 'report', handle => $report_from_child},
      {name => 'stdout', handle => $stdout_from_child},
      {name => 'stderr', handle => $stderr_from_child});

   foreach my $file_info (@file_info) {
      my $fd = fileno ($file_info->{handle});
      if (defined $Fid_info{$fd}) {
         die "Fid_info entry $fd is in use";
      }

      $file_info->{fdesc}  = $fd;
      $file_info->{buffer} = '';
      $file_info->{offset} = 0;
      $file_info->{pid}    = $pid;
      $file_info->{done}   = 0;
      $file_info->{mark}   = 0;

      $Fid_info{$fd} = $file_info;

      add_handle $select, $file_info->{handle};
   }

   $Pid_info{$pid} = {
      runidx => $runidx,
      killed => 0,
      report => $file_info[0]->{fdesc},
      stdout => $file_info[1]->{fdesc},
      stderr => $file_info[2]->{fdesc}};

   $Rundirs{inuse}->{$pid} = $runidx;

   $Nactive++;
   $Navail--;
}


sub collect_ready ($) {
   my ($select) = @_;
   my $pid;

   # reap_hung_processes $select;

   while (($pid = waitpid -1, WNOHANG) > 0) {
      collect_finished_output $select, $pid, 1e-6;
      select undef, undef, undef, 1e-6;
   }

   # Nothing available yet.
   # Collect output, with a not-so-small hang between checks.
   if ($Navail != $Options{ncpus}) {
      collect_ready_output $select, 1e-2;

      while (($pid = waitpid -1, WNOHANG) > 0) {
         collect_finished_output $select, $pid, 1e-6;
         select undef, undef, undef, 1e-6;
      }
   }
}


sub collect_ready_output ($$) {
   my ($select, $hang) = @_;
   my @reaped = ();

   my $done = 0;
   while (! $done) {
      $done = 1;
      my ($ready, $error) = can_read $select, $hang;
      my %reap = ();
      foreach my $fh (@$ready) {
         if (ref($fh) ne 'IO::Handle') {
            warn "Got something goofy from can_read\n";
            ## $select->remove ($fh);
            next;
         }
         my ($fd, $info, $len);
         $fd   = $fh->fileno();
         $info = $Fid_info{$fd};
      die "No finfo for $fd" unless defined $info;
         $len  = $fh->sysread ($info->{buffer}, 64, $info->{offset});
         if ((defined $len) && ($len > 0)) {
            $info->{offset} += $len;
            if ($info->{mark} == 0) {
               $info->{mark} = time();
            }
         }
         else {
            $info->{done} = 1;
            $reap{$fd} = $fh;
         }
         $done = 0;
      }

      foreach my $fh (@$error) {
         if (ref($fh) ne 'IO::Handle') {
            warn "Got something goofy from can_read\n";
            ## $select->remove ($fh);
            next;
         }
         my ($fd, $info, $len);
         $fd   = $fh->fileno();
         $info = $Fid_info{$fd};
      die "No finfo for $fd" unless defined $info;
         warn "Error on $fd ($info->{name})\n";
         $info->{done} = 1;
         $reap{$fd} = $fh;
      }

      foreach my $fd (keys %reap) {
         my $fh = $reap{$fd};
         del_handle $select, $fh;
         push @reaped, $fh;
      }
   }

   return @reaped;
}


sub collect_finished_output ($$$) {
   my ($select, $pid, $hang) = @_;
   my $info = $Pid_info{$pid};

   my %collect = ();
   my @collect = ();

   foreach my $id (qw(report stdout stderr)) {
      my $fd = $info->{$id};
      my $finfo = $Fid_info{$fd};
      if (! $finfo->{done}) {
         my $fh = $finfo->{handle};
         push @collect, $fh;
         $collect{$fd} = $fh;
         del_handle $select, $fh;
      }
   }

   if (@collect) {
      my $subselect = make_select (@collect);
      while (@collect) {
         my @reaped = collect_ready_output ($subselect, $hang);
         foreach my $fh (@reaped) {
            delete $collect{$fh->fileno()};
         }
         @collect = keys %collect;
      }
   }

   foreach my $id (qw(report stdout stderr)) {
      my $report = $Report{$id};
      my $fd = $info->{$id};
      my $finfo = $Fid_info{$fd};
warn "$finfo->{name} for $pid is not done\n" unless $finfo->{done};
      close $finfo->{handle};
      if ($id eq 'report') {
         print_report $report, $pid, $finfo->{buffer};
      }
      else {
         print $report $finfo->{buffer};
      }
      delete $Fid_info{$fd};
   }

   delete $Rundirs{inuse}->{$pid};
   $Rundirs{free}->{$Pid_info{$pid}->{runidx}} = 1;
   delete $Pid_info{$pid};

   $Nactive--;
   $Npending--;
   $Navail++;
}


sub print_report ($$$) {
   my ($report, $pid, $buffer) = @_;
   my $pid_info = $Pid_info{$pid};
   my @reports;

   if ($buffer !~ /^(?:Rotation|Translation)Test/x) {
      if ($buffer =~ s/^(.*?)(^(?:Rotation|Translation)Test)/$2/msx) {
         my $junk = $1;
         warn "Discarding JUNK before start of report for $pid\n";
         warn "$junk\n";
      }
      else {
         warn "Buffer is garbage for $pid\n";
         warn "$buffer\n";
         $buffer = "\n";
      }
   }
   if (! chomp $buffer) {
      warn "Buffer was not terminated for $pid\n";
   }
   $buffer .= "\nRotationTest\n";
   while ($buffer =~ s/(^((?:Rotation|Translation)Test).*?)
                       (^(?:Rotation|Translation)Test)/$3/msx) {
      push @reports, [$1, $2];
   }
   for (my $ii = 0; $ii <= $#reports; $ii++) {
      my ($buf, $typ) = @{$reports[$ii]};
      my $line_count = $Report_lines{$typ} or die;
      my $lines = 0;
      $lines++ while $buf =~ /\n/g;
      if ($lines == $line_count) {
         print $report $buf;
      }
      else {
         my $which = $ii == $#reports ? "FINAL" : "INTERMEDIATE";
         warn "Discarding $which partial report for $pid\n";
      }
   }
}


sub reap_hung_processes ($) {
   my ($select) = @_;

   my $time = time();

   unless (defined $Last_time) {
      die;
   }

   if ($Last_time < $time) {
      $Last_time = $time;

      foreach my $pid (keys %Pid_info) {
         my $pid_info = $Pid_info{$pid};
         next if $pid_info->{killed};

         my $report_fd   = $pid_info->{report};
         my $stdout_fd   = $pid_info->{stdout};
         my $report_info = $Fid_info{$report_fd};
         my $stdout_info = $Fid_info{$stdout_fd};
         if ((($report_info->{mark} > 0) &&
              ($report_info->{mark} + 40 < $time)) ||
             (($stdout_info->{mark} > 0) &&
              ($stdout_info->{mark} + 40 < $time))) {
            my $stderr_fd   = $pid_info->{stderr};
            my $stderr_info = $Fid_info{$stderr_fd};
            $pid_info->{killed} = 1;
            kill 9, $pid;

            del_handle $select, $report_info->{handle}
               unless $report_info->{done};
            del_handle $select, $stdout_info->{handle}
               unless $stdout_info->{done};
            del_handle $select, $stderr_info->{handle}
               unless $stderr_info->{done};

            warn "\n\nKilled pid=$pid\n" .
                 "report_info->{mark} = $report_info->{mark}\n" .
                 "stdout_info->{mark} = $stdout_info->{mark}\n" .
                 "time = $time\n\n";
         }
      }
   }
}


################################################################################


sub make_select (@) {
   my (@handles) = @_;
   my $select = {handles => [], vector  => ''};
   foreach my $fh (@handles) {
      add_handle $select, $fh;
   }
   return $select;
}


sub add_handle ($$) {
   my ($select, $handle) = @_;
   if (ref($handle) ne 'IO::Handle') {
      die "Bad handle to add_handle";
   }
   my $fd = $handle->fileno();
   if (defined $select->{handles}->[$fd]) {
      die "Duplicate handle to add_handle";
   }
   $select->{handles}->[$fd] = $handle;
   vec ($select->{vector}, $fd, 1) = 1;
}


sub del_handle ($$) {
   my ($select, $handle) = @_;
   if (ref($handle) ne 'IO::Handle') {
      die "Bad handle to del_handle";
   }
   my $fd = $handle->fileno();
   if (! defined $select->{handles}->[$fd]) {
      die "Handle not registered in del_handle";
   }
   $select->{handles}->[$fd] = undef;
   vec ($select->{vector}, $fd, 1) = 0;
}


sub can_read ($$) {
   my ($select, $hang) = @_;
   my $handles  = $select->{handles};
   my $rout     = $select->{vector};
   my $eout     = $rout;
   my @can_read   = ();
   my @had_errors = ();
   if (select ($rout, undef, $eout, $hang) > 0) {
      for (my $fd = 0; $fd < scalar @$handles; $fd++) {
         if (vec ($eout, $fd, 1)) {
            my $fh = $handles->[$fd];
            if (! defined $fh) {
               die "Handle not registered in can_read";
            }
            push @had_errors, $fh;
         }
         if (vec ($rout, $fd, 1)) {
            my $fh = $handles->[$fd];
            if (! defined $fh) {
               die "Handle not registered in can_read";
            }
            push @can_read, $fh;
         }
      }
   }
   return ([@can_read], [@had_errors]);
}
