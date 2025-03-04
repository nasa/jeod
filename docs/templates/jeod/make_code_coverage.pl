#!/usr/bin/perl -w
################################################################################
#
# make_code_coverage.pl: Collect and pretty-print code coverage data.
# Use perl make_code_coverage.pl -man to see documentation.
#
################################################################################


################################################################################

=head1 NAME

make_code_coverage.pl - Generate, collect and pretty-print code coverage data.


=head1 DESCRIPTION

B<make_code_coverage.pl> generates, collects, and prints code coverage data
based on the tests in a model.


=head1 SYNOPSIS

=over

=item perl make_code_coverage.pl -man

Describe B<make_code_coverage.pl> and exit.

=item

=item perl make_code_coverage.pl [options]

Generate, collect, and print code coverage data.

=back
=cut

################################################################################

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd qw(getcwd abs_path);


sub main ();
sub make_table ($);
sub get_coverage ();
sub build_test_spec ();
sub find_local_tests ();
sub build_default_test_spec ();
sub parse_test_spec_file ();
sub expand_local_sim_spec ($);
sub expand_extern_sim_spec ($);
sub expand_unit_test_spec ($);
sub remove_detritus ($);
sub build_coverage ();
sub run_sim($$$);
sub print_message ($);
sub compare_result_entries ($$);

sub LOCAL_SIM  {1}
sub EXTERN_SIM {2}
sub UNIT_TEST  {3}


my $model_home;
my $trick_ver;
my @test_specification = ();
my $have_unit_tests = 0;

my @all_local_sims = ();
my %all_local_sims = ();
my %avail_local_sims = ();

my @all_unit_tests = ();
my %all_unit_tests = ();
my %avail_unit_tests = ();

main;


=head1 OPTIONS

=over

=item B<-dir> I<< directory >>

Specifies the absolute or relative path to the model home directory.
The directory defaults to '../..'; this is the location of the model directory
relative to the docs/tex directory for that model.

=item B<-[no]build>

If set, specifies that code coverage data are to be generated based on the
tests for the model. The file <model_home>/docs/tex/code_coverage.spec, if it
exists, specifies the tests to be performed. Without such a test specification
file, all applicable simulations and all unit tests inside the model directory
are used. The applicable simulations are those that pertain to the current
Trick version (SIM_xxx for Trick 7, SIM_xxx_T10 for Trick 10).

The default setting is B<-nobuild>.

=item B<-[no]clean>

If set, specifies that the existing I<< model_home >>/gcov directory is to
be deleted prior to building code coverage data.
This option is ignored if the B<-build> option is not specified.

The default setting is B<-noclean>.

=item B<-[no]report>

If set, specifies that a LaTeX table that summarizes the code coverage is
to be generated.

The default setting is B<-report>.

=item B<-output> I<< output_file_name >>

Specifies the name of the report file. If left unspecified, the report is
sent to standard output.
This option is ignored if the B<-report> option is not specified.

=item B<-[no]vertlines>

If set, specifies that the generated LaTeX table will have vertical lines
surrounding the table.

The default setting is B<-vertlines>.

=back

=head1 KEY ENVIRONMENT VARIABLES

=over

=item B<JEOD_HOME>

The root JEOD directory, used in a number of places in the script to
identify files located in the JEOD directory tree. The model directory
must be located under the $JEOD_HOME directory.

=item B<TRICK_CFLAGS>

Flags with which test articles are compiled.
This script add "-fprofile-arcs -ftest-coverage" to the TRICK_CFLAGS
to instrument the test articles for coverage.

=item B<TRICK_VER>

The current Trick version. The construction and execution of a Trick 7 versus
a Trick 10 simulation differ in various ways. This script uses the TRICK_VER
environment variable to determine which simulations are to be built and
to determine how to invoke those simulations.

=back

=cut

sub main () {

   my $man = 0;
   my $build = 0;
   my $clean = 0;
   my $report = 1;
   my $output;
   my $vertlines = 1;

   $model_home = '../..';

   # Process the command line options.
   GetOptions (
      'man|help'     => \$man,                   # Man page
      'directory=s'  => \$model_home,            # Model home directory
      'build!'       => \$build,                 # Create coverage?
      'clean!'       => \$clean,                 # Build from scratch?
      'report!'      => \$report,                # Generate output?
      'output=s'     => \$output,                # Output file
      'vertlines!'   => \$vertlines              # Vertical lines in table?
   ) or pod2usage(2);

   pod2usage(-exitstatus => 0, -verbose => 2) if $man;

   # Convert the output (if specified) to an absolute path.
   $output = abs_path $output if defined $output;

   # Script needs to run with model_home as the working directory.
   chdir $model_home or die "Can't chdir to $model_home\n";
   $model_home = getcwd;

   # Get the Trick version.
   ($trick_ver = $ENV{TRICK_VER}) =~ s/\..*//;

   if ($build) {
      remove_detritus $clean;

      build_test_spec;
      my $add = sub($) { my $tag = shift;
                         my $gcov_flags = '-fprofile-arcs -ftest-coverage';
                         return (exists $ENV{$tag}) ?
                            "$gcov_flags $ENV{$tag}" : $gcov_flags; };
      $ENV{TRICK_CFLAGS} = &$add('TRICK_CFLAGS');
      $ENV{TRICK_LDFLAGS} = &$add('TRICK_LDFLAGS');
      build_coverage;
   }

   if ($report) {
      if (defined $output) {
         open OUTPUT, '>', $output or die;
         print OUTPUT make_table $vertlines;
         close OUTPUT;
      }
      else {
         print make_table $vertlines;
      }
   }

   if ($build) {
      remove_detritus 0;
   }
}

=head1 FILES

=over

=item Code coverage report

The report file is in the form of a LaTeX longtable.
The intended use of the script is to create the code coverage table
in the model document.

=cut


sub make_table ($) {
   my ($vertlines) = @_;
   my $bar = $vertlines ? '|' : '';
   my $dbar = "${bar}${bar}";
   my @results = get_coverage;
   my $table_format = "${dbar}p\{2in\}${bar}r\@\{\/\}lr${bar}";

   my $table = << "   EOF";
      \\begin\{stretchlongtable\}\[c\]\{$table_format\}
      \\caption\{Code Coverage\}
      \\label\{tab:code_coverage\} \\\\\[6pt\]

      \\hline
      \\multicolumn\{1\}\{${dbar}l${bar}\} \{\} \&
      \\multicolumn\{3\}\{c${bar}\} \{\\bf Statements Tested\} \\\\
      \{\\bf File Name\} \&
      \{\\bf \\#Tested\} \& \{\\bf Total\} \& \{\\bf Percent\} \\\\
      \\hline \\hline
      \\endfirsthead

      \\caption\[\]\{Code Coverage (continued from previous page)\} \\\\\[6pt\]
      \\hline
      \\multicolumn\{1\}\{${dbar}l${bar}\} \{\} \&
      \\multicolumn\{3\}\{c${bar}\} \{\\bf Statements Tested\} \\\\
      \{\\bf File Name\} \&
      \{\\bf \\#Tested\} \& \{\\bf Total\} \& \{\\bf Percent\} \\\\
      \\hline \\hline
      \\endhead

      \\hline \\multicolumn\{4\}\{r\}\{\{Continued on next page\}\} \\\\
      \\endfoot

      \\hline
      \\endlastfoot
   EOF

   $table =~ s/^      //mg;

   foreach (@results) {
      /^(\S+)\s+(\d+)\/(\d+)\s+(\d+\.\d\d)\s*$/ or die;
      my ($file, $tested, $stmts, $percent) = ($1, $2, $3, $4);
      $file =~ s/\//\/\\hspace{0pt}/g;
      $file =~ s/_/\\_\\hspace{0pt}/g;
      $table .= "\\longentry $file \& $tested \& $stmts \& $percent \\\\\n";
   }
   $table .= "\\end{stretchlongtable}\n";
   return $table;
}


sub get_coverage () {
   my @results;
   open REPORT, "perl $ENV{JEOD_HOME}/dgh_scripts/gcov.pl -report |" or die;
   while (<REPORT>) {
      chomp;
      push @results, $_;
   }
   close REPORT;
   return sort compare_result_entries @results;
}

sub build_test_spec () {

   find_local_tests;

   if (-f 'docs/tex/code_coverage.spec') {
      parse_test_spec_file;
   }
   else {
      build_default_test_spec;
   }
}


sub find_local_tests () {

   my @Trick07_local_sims = ();
   my @Trick10_local_sims = ();

   # Find the local tests.
   foreach my $dir (`find . -name verif | sort`) {
      chomp $dir;
      $dir =~ s/^\.\///;

      if (-d "$dir/unit_tests") {
         foreach (`find $dir/unit_tests -name makefile | sort`) {
            chomp;
            s/\/makefile$//;
            push @all_unit_tests, $_;
         }
      }
      foreach (`find $dir -name S_define | sort`) {
         chomp;
         s/\/S_define$//;
         push @all_local_sims, $_;
      }
   }

   # Note - avail_local_sims allows for deselection down from all_local_sims
   #        However, in this implementation, no down-selection is needed so
   #        they are equivalent.  But note that it is avail_local_sims that
   #        goes on to be used.
   my @avail_local_sims = @all_local_sims;
   @all_local_sims{@all_local_sims}     = (1) x @all_local_sims;
   @avail_local_sims{@avail_local_sims} = (1) x @avail_local_sims;
   @all_unit_tests{@all_unit_tests}     = (1) x @all_unit_tests;
   @avail_unit_tests{@all_unit_tests}   = (1) x @all_unit_tests;
}


sub build_default_test_spec () {
   if (scalar keys %avail_local_sims) {
      push @test_specification, {
         type => LOCAL_SIM,
         targets => [sort keys %avail_local_sims],
         run => '*'};
   }
   if (scalar keys %avail_unit_tests) {
      $have_unit_tests = 1;
      push @test_specification, {
         type => UNIT_TEST,
         targets => [sort keys %avail_unit_tests],
         run => 'run'};
   }
}


=item code_coverage.spec

This file, located in the I<< model_home >>/docs/tex directory,
specifies the tests to be run. The file contains specifications of the
tests to be compiled and run. Three types of tests are supported:

=over

=item local simulations

Local simulations are those simulations that reside in some /verif/
subdirectory of the model directory. Sample usage:

   Local simulations: *

This is the default specification. The asterisk means to use all available
local simulations.

   Local sims: Foo, Bar, Foo_T10, Bar_T10

Here a specific set of simulations are identified. Note that now the user must
explicitly specify the Trick 7 and Trick 10 versions of a simulation.

   Local sims: * - Foo, Bar, Foo_T10, Bar_T10

This is the "All but" specification, which excludes the specified items
from the list (but these can be picked up in a subsequent specification).

Local simulation specifications can be followed by a line that specifies which
simulation runs are to be performed:

   Run directories: *

A similar syntax for the ``Local sims'' specification allows one to identify
specific runs or to exclude specific runs. The default is to perform all
runs, where the runs are the members of the simulation SET_test directory.


=item external simulations

Word

=item unit tests

Word

=back

An example, dynamics/dyn_body/docs/tex/code_coverage.spec:

   Local simulations : *
   External sim: dynamics/body_action/verif/SIM_orbinit
   External sim: dynamics/body_action/verif/SIM_orbinit_T10
   External sim: dynamics/body_action/verif/SIM_verif_frame_switch
   External sim: dynamics/body_action/verif/SIM_verif_frame_switch_T10

Note: If file code_coverage.spec does not exist the default is

   Local simulations : *
   Unit tests : *

=cut


sub parse_test_spec_file () {

   my $test_type = 0;
   my $current_item;

   open SPEC, '<', 'docs/tex/code_coverage.spec' or die;
   while  (<SPEC>) {
      chomp;
      s/ *#.*//;
      s/ +$//;
      next if /^$/;

      my $flush = 0;
      my $new_spec;

      if (/^ *Local sim(?:ulation)?s? *: *(.*)$/) {
         my $sim_spec = $1;
         $new_spec = expand_local_sim_spec $sim_spec;
      }

      elsif (/^ *External sim(?:ulation)?s? *: *(.*)$/) {
         my $sim_spec = $1;
         $new_spec = expand_extern_sim_spec $sim_spec;
      }

      elsif (/^ *Unit tests *: *(.*)$/) {
         my $test_spec = $1;
         $have_unit_tests = 1;
         $new_spec = expand_unit_test_spec $test_spec;
      }


      if (defined $new_spec) {
         if (defined $current_item) {
            push @test_specification, $current_item;
         }
         $current_item = $new_spec;
         $test_type = $new_spec->{type};
      }

      elsif ($test_type == 0) {
         warn "Illegal specification '$_'\n";
      }

      elsif ($test_type == UNIT_TEST) {
         if (/^ *Make targets? *: *(.*)$/) {
            $current_item->{run} = $1;
            $flush = 1;
         }
         else {
            warn "Illegal unit test target '$_'\n";
         }
      }

      elsif (($test_type == LOCAL_SIM) || ($test_type == EXTERN_SIM)) {
         if (/^ *Run dir(?:ectories|ectory) *: *(.*)$/) {
            $current_item->{run} = $1;
            $flush = 1;
         }
         else {
            warn "Illegal simulation target '$_'\n";
         }
      }

      else {
      }

      if ($flush) {
         push @test_specification, $current_item;
         undef $current_item;
         $test_type = 0;
      }
   }
   close SPEC;

   if (defined $current_item) {
      push @test_specification, $current_item;
   }
}


sub expand_local_sim_spec ($) {
   my ($sim_spec) = @_;
   my @new_targets;

   if ($sim_spec =~ /^ *\* *- *(.*)$/) {
      my $sub_spec = $1;
      my @sub_spec = split / *, */, $sub_spec;
      my %new_targets = %avail_local_sims;
      foreach my $sim (@sub_spec) {
         if (exists $avail_local_sims{$sim}) {
            delete $new_targets{$sim};
         }
         elsif (! exists $all_local_sims{$sim}) {
            warn "Illegal local sim spec '$sim_spec'\n";
         }
      }
      @new_targets = sort keys %new_targets;
   }

   elsif ($sim_spec =~ /^ *\* *$/) {
      @new_targets = sort keys %avail_local_sims;
   }

   else {
      my @sim_spec = split / *, */, $sim_spec;
      foreach my $sim (@sim_spec) {
         if (exists $avail_local_sims{$sim}) {
            push @new_targets, $sim;
         }
         elsif (! exists $all_local_sims{$sim}) {
            warn "Illegal local sim spec '$sim_spec'\n";
         }
      }
   }

   foreach my $sim (@new_targets) {
      delete $avail_local_sims{$sim};
   }

   return {
      type => LOCAL_SIM,
      targets => [@new_targets],
      trick_ver => $trick_ver,
      run => '*'};
}


sub expand_extern_sim_spec ($) {
   my ($sim_spec) = @_;

   if ((-d "$ENV{JEOD_HOME}/models/$sim_spec") &&
       (-f "$ENV{JEOD_HOME}/models/$sim_spec/S_define")) {
      return {
         type => EXTERN_SIM,
         targets => [$sim_spec],
         trick_ver => +(($sim_spec =~ /_T10$/) ? 10 : 7),
         run => '*'};
   }

   else {
      warn "Illegal external sim spec '$sim_spec'\n";
      return undef;
   }
}


sub expand_unit_test_spec ($) {
   my ($test_spec) = @_;
   my @new_targets;

   if ($test_spec =~ /^ *\* *- *(.*)$/) {
      my $sub_spec = $1;
      my @sub_spec = split / *, */, $sub_spec;
      my %new_targets = %avail_unit_tests;
      foreach my $test (@sub_spec) {
         if (exists $avail_unit_tests{$test}) {
            delete $new_targets{$test};
         }
         elsif (! exists $all_unit_tests{$test}) {
            warn "Illegal unit test spec '$test_spec'\n";
         }
      }
      @new_targets = sort keys %new_targets;
   }

   elsif ($test_spec =~ /^ *\* *$/) {
      @new_targets = sort keys %avail_unit_tests;
   }

   else {
      my @test_spec = split / *, */, $test_spec;
      foreach my $test (@test_spec) {
         if (exists $avail_unit_tests{$test}) {
            push @new_targets, $test;
         }
         else {
            warn "Illegal unit test spec '$test_spec'\n";
         }
      }
   }

   foreach my $test (@new_targets) {
      delete $avail_unit_tests{$test};
   }

   return {
      type => UNIT_TEST,
      targets => [@new_targets],
      run => 'run'};
}


=item gcov files

Words

=back

=cut

sub perform_command($) {
   my ($cmd) = @_;
   print "$cmd\n";
   system $cmd;
}

sub remove_detritus ($) {
   my ($remove_gcov) = @_;
###if (-d '../../gcov') {
###   if ($remove_gcov) {
###      perform_command 'rm -rf ../../gcov';
###   }
###   else {
###      my @extras = <../../gcov/*.gcov.*>;
###      print "rm ../../gcov/*.gcov.*\n";
###      map unlink, @extras;
###   }
###}
   if ($remove_gcov) {
      perform_command
         'rm -rf `find \$JEOD_HOME -type d -name "gcov"`';
   }
   perform_command
      'rm -rf `find \$JEOD_HOME -type d -name "swig"`';
   perform_command
      'rm -rf `find \$JEOD_HOME -type d -name "obj*\${TRICK_HOST_CPU}"`';
   perform_command
      'rm -rf `find \$JEOD_HOME -type d -name "lib*\${TRICK_HOST_CPU}"`';
}


sub build_coverage () {
   my $TRICK_HOME = $ENV{TRICK_HOME};
   unless (defined $TRICK_HOME) {
      $TRICK_HOME = `trick-config --prefix`;
      $TRICK_HOME = chomp($TRICK_HOME);
   }
   my $CP = "$TRICK_HOME/bin/CP";
   my $gcov = "perl \$JEOD_HOME/dgh_scripts/gcov.pl";

   if ($have_unit_tests) {
      system 'cd $JEOD_HOME; bin/jeod/Configure_jeod -gcov';
   }
   else {
      system 'cd $JEOD_HOME; bin/jeod/Configure_jeod -nob; make makefiles';
   }

   foreach my $item (@test_specification) {
      foreach my $target (@{$item->{targets}}) {
         if ($item->{type} == LOCAL_SIM) {
            print_message "Building $target";
            system "cd $target; $CP";
            run_sim LOCAL_SIM, $target, $item->{run};
            system "$gcov -collect $model_home";
            print_message "Cleaning $target";
            system "cd $target; make spotless";
         }
         elsif ($item->{type} == EXTERN_SIM) {
            if ($item->{trick_ver} == $trick_ver) {
               print_message "Building \$JEOD_HOME/models/$target";
               system "cd \$JEOD_HOME/models/$target; $CP";
               run_sim EXTERN_SIM, $target, $item->{run};
               system "cd \$JEOD_HOME/models/$target; " .
                      "$gcov -collect $model_home";
               system "$gcov -collect $model_home";
               print_message "Cleaning \$JEOD_HOME/models/$target";
               system "cd \$JEOD_HOME/models/$target; make spotless";
            }
         }
         else {
            print_message "Building $target";
            system "cd $target; make build";
         }
      }
   }

   foreach my $item (@test_specification) {
      next if $item->{type} != UNIT_TEST;
      foreach my $target (@{$item->{targets}}) {
         my $make_targets = $item->{run};
         print_message "Running $target $make_targets";
         system "cd $target; make $make_targets";
      }
   }
   system $gcov;
}


sub run_sim($$$) {
   my ($type, $target, $run) = @_;

   my $s_main = "S_main_$ENV{TRICK_HOST_CPU}.exe";
   my $input = ($trick_ver == 7) ? 'input' : 'input.py';

   my $sim_dir = $target;
   $sim_dir =~ s/^/$ENV{JEOD_HOME}\/models\// if ($type == EXTERN_SIM);
   if (-x "$sim_dir/$s_main") {
      chdir $sim_dir or die;
      print_message "Entering " . `pwd`;

      my @run_dirs = ();
      if ($run eq '*') {
         foreach (<SET_test/RUN*/$input>) {
            /^SET_test\/(RUN.*)\/$input$/ or die;
            push @run_dirs, $1;
         }
      }
      else {
         @run_dirs = split / *, */, $run;
      }

      foreach my $run_dir (@run_dirs) {
         print_message "Running $s_main SET_test/$run_dir/$input";
         system "./$s_main SET_test/$run_dir/$input" and warn "$?";
      }
      chdir $model_home;
   }
   elsif (! -d $sim_dir) {
      warn "Missing directory $sim_dir\n";
   }
   elsif (! -e "$sim_dir/$s_main") {
      warn "Missing simulation $sim_dir/$s_main\n";
   }
   else {
      warn "Non-executable simulation $sim_dir/$s_main\n";
   }
}

sub print_message ($) {
   my ($message) = @_;
   print +("\e[032m" . "*"x80 .   "\e[000m\n" .
           "\e[032m" . $message . "\e[000m\n" .
           "\e[032m" . "*"x80 .   "\e[000m\n");
}

sub compare_result_entries ($$) {
   my ($entry_a, $entry_b) = @_;
   $entry_a =~ s/\s.*$//;
   $entry_b =~ s/\s.*$//;
   return ((($entry_a =~ tr/\///) <=> ($entry_b =~ tr/\///)) ||
           ($entry_a cmp $entry_b));
}



__END__
