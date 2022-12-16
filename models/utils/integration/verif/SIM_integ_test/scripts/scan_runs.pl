#!/usr/bin/perl -w

use strict;
use Getopt::Long;

sub main ();
sub scan_sim_output ($);
sub collate_data ($);
sub generate_report ($$);
sub log10_round ($);

sub TABLE       {0}
sub GNUPLOT     {1}
sub FIXER_UPPER {2}

my @Class_names = qw(
   RotationTestTorqueFreeSphere
   RotationTestTorqueFreeSymTop
   RotationTestTorqueFreeSymTopB
   RotationTestSHOSphere
   TranslationTestOrbit
   TranslationTestSpringDamper);

my %Integ_opts_by_name = (
   Euler                  =>   1, # Basic Euler
   SymplecticEuler        =>   2, # Symplectic Euler

   Beeman                 =>  11, # Beeman's algorithm
   NystromLear2           =>  14, # Second order Nystrom-Lear
   PositionVerlet         =>  15, # Position verlet
   RK2Heun                =>  16, # Heun's method
   RK2Midpoint            =>  17, # Midpoint method
   VelocityVerlet         =>  19, # Velocity verlet

   ModifiedMidpoint4      =>  21, # Modified midpoint

   AdamsBashforthMoulton4 =>  31, # Fourth order Adams-Bashforth-Moulton
   RungeKutta4            =>  34, # Fourth order Runga Kutta
   RKGill4                =>  35, # Fourth order Runga Kutta Gill
   RKNystrom4             =>  36, # Fourth order Runga Kutta Nystrom

   RKFehlberg45           =>  44, # Runge-Kutta Fehlberg 4/5

   RKFehlberg78           =>  74, # Runge-Kutta Fehlberg 7/8

   GaussJackson2          => 122, # 2nd order Gauss-Jackson
   GaussJackson4          => 124, # 4th order Gauss-Jackson
   GaussJackson6          => 126, # 6th order Gauss-Jackson
   GaussJackson8          => 128, # 8th order Gauss-Jackson
   GaussJackson10         => 130, # 10th order Gauss-Jackson
   GaussJackson12         => 132, # 12th order Gauss-Jackson
);
my %Integ_opts_by_number = reverse %Integ_opts_by_name;

my @Integ_opts = sort {$a <=> $b} keys %Integ_opts_by_number;
my @Integ_names = map {$Integ_opts_by_number{$_}} @Integ_opts;
my %Integ_names = ();
@Integ_names{@Integ_opts} = @Integ_names;

my %Integ_opts = ();

my %Class_index;
@Class_index{@Class_names} = (0 .. $#Class_names);

my %Selected_classes;
my %Selected_integs;

my %Options = (
   mode           => TABLE,
   class          => [],
   integ_opt      => [],
   output_prefix  => undef,
   xrange         => undef,
   yrange         => '1e-14:1',
   plots_output   => 'pdf',
   plots_per_page => 3,
   ncpus          => 2,
   warnings       => 1);


my $Prefix;
my $Nrevs = -1;

main;


################################################################################


sub main () {
   GetOptions ('table|excel'   => sub {$Options{mode} = TABLE},
               'latex|gnuplot' => sub {$Options{mode} = GNUPLOT},
               'fixer_upper'   => sub {$Options{mode} = FIXER_UPPER},
               'aqua|xterm'    => sub {@Options{qw(mode plot_output)} =
                                       (GNUPLOT, 'aqua')},
               'class=s@'      => \$Options{class},
               'integ_opt=s@'  => \$Options{integ_opt},
               'prefix=s'      => \$Options{output_prefix},
               'xrange=s'      => \$Options{xrange},
               'yrange=s'      => \$Options{yrange},
               'plots=s'       => \$Options{plots_per_page},
               'ncpus=i'       => \$Options{ncpus},
               'warnings!'     => \$Options{warnings})
   or die "Syntax: $0 [options] file_name\n";
   die "Syntax: $0 [options] file_name\n" if (scalar @ARGV != 1);
   my $fname = $ARGV[0];
   ($Prefix = $fname) =~ s/\.dat$//;

   if (scalar @{$Options{class}}) {
      foreach my $class (@{$Options{class}}) {
         if (defined $Class_index{$class}) {
            $Selected_classes{$class} = 1;
         }
         else {
            die "Bad class $class";
         }
      }
   }
   else {
      @Selected_classes{@Class_names} = (1) x scalar @Class_names;
   }

   if (scalar @{$Options{integ_opt}}) {
      foreach my $integ_opt (@{$Options{integ_opt}}) {
         if (defined $Integ_opts_by_name{$integ_opt}) {
            $Selected_integs{$Integ_opts_by_name{$integ_opt}} = 1;
         }
         else {
            die "Bad integ_opt $integ_opt";
         }
      }
   }
   else {
      @Selected_integs{@Integ_opts} = (1) x scalar @Integ_opts;
   }

   my $sim_data = scan_sim_output $fname;
   my ($collated_data, $nruns) = collate_data $sim_data;
   generate_report $collated_data, $nruns;
}


################################################################################

sub scan_sim_output ($) {
   my ($fname) = @_;
   my $run_data = [];

   open IN, "<$fname" or die "Can't open $fname";
   my $line;
   while (! eof(IN)) {
      while (defined ($line = <IN>)) {
         chomp $line;
         last if defined $Class_index{$line};
         warn "$line\n";
      }
      unless (defined $line) {
         warn "Unexpected EOF while looking for chunk\n";
         last;
      }
      my @chunk = ($line);
      my $error = 0;
      my $nlines = $line =~ /^Rot/ ? 18 : 16;
      foreach (my $iline = 1; $iline < $nlines; $iline++) {
         if (eof(IN)) {
            $error = 1;
            last;
         }
         chomp ($line = <IN>);
         chomp $line;
         push @chunk, $line;
      }
      if ($error) {
         warn "Unexpected EOF while reading for chunk\n";
         last;
      }
      push @$run_data, [@chunk];
   }
   close IN;
   return $run_data;
}


sub collate_data ($) {
   my ($run_data) = @_;
   my $collated_data = {};
   my $nruns = 0;

   foreach my $chunk (@$run_data) {
      my ($class, $integ, $wdt, $test, $cycles, $error);
      ($class, $integ, $wdt, $test, $cycles, $error) =
         @{$chunk}[0, 1, 2, 4, 7, 8];
      $integ =~ /integ option        = (\d+)$/ or die $integ;
      $integ = $1 + 0;
      $wdt =~   /omega_dt        .d. = (\S+)$/ or die $wdt;
      $wdt = $1;
      $test =~  /test number         = (\d+)$/ or die $test;
      $test = $1 + 0;
      $cycles =~  /# cycles            = (\S+)$/ or die $cycles;
      $cycles = int ($1 + 0.5);
      if ($cycles > $Nrevs) {
         warn "Shift in # cycles from $Nrevs to $cycles\n"
            unless $Nrevs < 0;
         $Nrevs = $cycles;
      }
      elsif ($cycles < $Nrevs) {
         # warn ("Test needs redo!\n");
         next;
      }
      if ($class =~ /^Rot/) {
         $error =~ /max angle err   .d. = (\S+)$/ or die $error;
         $error = $1 + 0;
      }
      elsif ($class =~ /^Trans/) {
         $error =~ /max pos err    .... = (\S+)$/ or die $error;
         $error = $1 + 0;
      }
      else {
         die $class;
      }

      next unless (exists $Selected_classes{$class} &&
                   exists $Selected_integs{$integ});

      $Integ_opts{$integ} = 1;
      if (! exists $collated_data->{$class}) {
         $collated_data->{$class} = {};
      }
      my $class_data = $collated_data->{$class};
      if (! exists $class_data->{$wdt}) {
         $class_data->{$wdt} = {};
      }
      my $wdt_data = $class_data->{$wdt};
      if (! exists $wdt_data->{$integ}) {
         $wdt_data->{$integ} = {series => [], max => 0, count => 0};
      }
      my $integ_data = $wdt_data->{$integ};
      my $wdt_series = $integ_data->{series};
      $wdt_series->[$test] = $error;
      $integ_data->{max} = $error if $integ_data->{max} < $error;
      $integ_data->{count}++;
      $nruns = $integ_data->{count} if $nruns < $integ_data->{count};
   }

   return ($collated_data, $nruns);
}


sub generate_report ($$) {
   my ($collated_data, $nruns) = @_;
   my @integ_opts = sort {$a <=> $b} keys %Integ_opts;
   my @missing = ();
   my @plots = ();

   foreach my $class (@Class_names) {
      my $class_data = $collated_data->{$class};
      next unless defined $class_data;
      my %gnuplot_output;

      if ($Options{mode} == TABLE) {
         print "$class\n";
         print +((join "\t", 'wdt', map {$Integ_names{$_}} @integ_opts), "\n");
      }

      foreach my $wdt (sort {$b <=> $a} (keys %$class_data)) {
         print $wdt if ($Options{mode} == TABLE);

         my $wdt_data = $class_data->{$wdt};
         my $log_wdt  = log10_round $wdt;
         foreach my $integ (@integ_opts) {
            unless (defined $wdt_data->{$integ}) {
               my $msg = "Missing series: $class $integ $wdt";
               if ($Options{mode} == FIXER_UPPER) {
                  push @missing, {
                     message   => $msg,
                     class     => $class,
                     integ_opt => $integ,
                     min       => $log_wdt,
                     max       => $log_wdt,
                     start     => 0,
                     end       => $nruns};
               }
               else {
                  warn "$msg\n" if $Options{warnings};
                  print "\t" if $Options{mode} == TABLE;
               }
               next;
            }
            my $gnuplot_fh;
            if ($Options{mode} == GNUPLOT) {
               if (! defined $gnuplot_output{$integ}) {
                  my $fname = +(join '_',
                                     $Options{output_prefix},
                                     $Integ_names{$integ},
                                     $class) . '.dat';
                  open $gnuplot_fh, '>', $fname
                     or die "Can't create gnuplot data file '$fname'";
                  $gnuplot_output{$integ} = {
                     handle => $gnuplot_fh,
                     name   => $fname};
               }
               else {
                  $gnuplot_fh = $gnuplot_output{$integ}->{handle};
               }
            }
            my $integ_data = $wdt_data->{$integ};
            my $run_series = $integ_data->{series};
            if ($integ_data->{count} != $nruns) {
               my $first = -1;
               my $last = -1;
               for (my $ii = 0; $ii < $nruns; $ii++) {
                  if (defined $run_series->[$ii]) {
                     if ($last >= 0) {
                        my $msg = "Missing: $class $integ $wdt $first:$last";
                        if ($Options{mode} == FIXER_UPPER) {
                           push @missing, {
                              message   => $msg,
                              class     => $class,
                              integ_opt => $integ,
                              min       => $log_wdt,
                              max       => $log_wdt,
                              start     => $first,
                              end       => $last+1};
                        }
                        else {
                           warn "$msg\n" if $Options{warnings};
                        }
                     }
                     $first = $last = -1;
                  }
                  else {
                     if ($first < 0) {
                        $first = $ii;
                     }
                     $last = $ii;
                  }
               }
               if ($last >= 0) {
                  my $msg = "Missing: $class $integ $wdt $first:$last";
                  if ($Options{mode} == FIXER_UPPER) {
                     push @missing, {
                        message   => $msg,
                        class     => $class,
                        integ_opt => $integ,
                        min       => $log_wdt,
                        max       => $log_wdt,
                        start     => $first,
                        end       => $last+1};
                  }
                  else {
                     warn "$msg\n" if $Options{warnings};
                  }
               }
            }
            if ($Options{mode} == TABLE) {
               print "\t$integ_data->{max}";
            }
            elsif ($Options{mode} == GNUPLOT) {
               print $gnuplot_fh "$wdt $integ_data->{max}\n";
            }
         }
         print "\n" if ($Options{mode} == TABLE);
      }
      if ($Options{mode} == TABLE) {
         print "\n\n";
      }
      elsif ($Options{mode} == GNUPLOT) {
         my ($range, $ylabel, $height, $spacing, $cycle, $freq, $fh);
         my $plural = $Nrevs == 1 ? '' : 's';
         my $xunits = '{/Symbol w}{/Symbol D}t, degrees';
         my $prefix = "$Options{output_prefix}_$class";
         my $fname  = "$prefix.gnu";
         if ($class =~ /^Rot/) {
            $ylabel = 'Angular Error (degrees)';
            $cycle  = 'Rotation';
            $freq   = 'Frequency';
         }
         elsif ($class =~ /Orbit/) {
            $ylabel = 'Position Error (relative)';
            $cycle  = 'Orbit';
            $freq   = 'Orbital Rate';
         }
         else {
            $ylabel = 'Position Error (relative)';
            $cycle  = 'Undamped Oscillation';
            $freq   = 'Undamped Oscillator Frequency';
         }
         if ($Options{plots_per_page} == 4) {
            $height = "2in";
            $spacing = '';
         } elsif ($Options{plots_per_page} == 3) {
            $height = "2.65in";
            $spacing = "spacing 1.1";
         }
         $range = '';
         if (defined $Options{xrange}) {
            $range = "set xrange \[$Options{xrange}\]; ";
         }
         $range .= "set yrange \[$Options{yrange}\]";
         my $hdr = << "            EOF";
               set logscale
               set title '3{/Symbol s} Error After $Nrevs $cycle$plural'
               set ylabel '$ylabel' rotate by 90
               $range
               set key Left reverse outside right center \\
                 width -0.5 $spacing samplen 1.2 title 'Legend' box
               set xlabel '$freq {/Symbol \\264} Step Size ($xunits)'
               set termoption font "Helvetica,8"
            EOF
         my $prev = "plot \\\n";
         $hdr =~ s/^               //mg;
         $hdr .= "set output \"${prefix}_revs_${Nrevs}_monte_err.pdf\"\n" .
                 "set term pdf enhanced size 5.8in,$height\n"
            if $Options{plot_output} eq 'pdf';
         open $fh, '>', $fname or die;
         print $fh $hdr;
         foreach my $integ (@integ_opts) {
            my $entry = $gnuplot_output{$integ};
            next unless defined $entry;
            print $fh "$prev  \"$entry->{name}\" " .
                      "title \"$Integ_names{$integ}\" with lines";
            $prev = ", \\\n";
            close $entry->{handle};
         }
         print $fh "\n";
         close $fh;
         push @plots, $fname;
      }
   }

   if ($Options{mode} == GNUPLOT) {
      foreach my $plot (@plots) {
         system "gnuplot $plot";
      }
   }
   elsif ($Options{mode} == FIXER_UPPER) {
      print "#!/usr/bin/perl\n";
      if (@missing) {
         print +(join "\n",
                 'use strict;',
                 'sub run(@);',
                 'my @Exec = (\'./scripts/run_cases.pl\',',
                 "            -ncpus  => $Options{ncpus},",
                 "            -prefix => '$Prefix',",
                 "            -nrev   => $Nrevs);",
                 '',
                 '');
         foreach my $missing (@missing) {
            my $msg = delete $missing->{message};
            my $pre = "run ";
            my $str = '';
            my $size = $missing->{end} - $missing->{start};
            my $runs = int (($size+$Options{ncpus}-1)/$Options{ncpus});
            $missing->{run} = $runs;
            $missing->{class} = "'$missing->{class}'";
            print "\n\# $msg\n";
            while (my ($key, $value) = each %$missing) {
               $str .= "$pre-$key => $value,";
               $pre = "\n    ";
            }
            $str =~ s/,$//;
            print "$str;\n";
         }
      }
      print +(join "\n",
              '',
              '',
              'sub run(@) {',  #}
              '   print "running @_\n";',
              '   system (@Exec, @_);',
              '}',
              '');
   }
}


sub log10_round ($) {
   my ($wdt) = @_;
   my $log_wdt = log($wdt) / log(10);
   if ($log_wdt > 0) {
      $log_wdt = int(4*$log_wdt+0.5)/4;
   }
   elsif ($log_wdt < 0) {
      $log_wdt = int(4*$log_wdt-0.5)/4;
   }
   return $log_wdt;
}
