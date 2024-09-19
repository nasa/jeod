use strict;
use Cwd qw(getcwd abs_path);
use Getopt::Long;

sub setup ();
sub process_gcda_files ();
sub process_merged_files ();
sub get_gcov ($$$);
sub save_gcov_file ($$$);
sub merge_contents ($$);
sub read_gcov_file ($);

my $verbose;
my @collect;
my $report;
my $jeod_objdir;
my $trick_objdir;
my $basedir;


setup;
process_gcda_files if scalar @collect;
process_merged_files if $report;


################################################################################


sub setup () {
   my $status;
   my $makefile = "$ENV{JEOD_HOME}/makefile";

   die "Bad environment\n" if (!defined $ENV{TRICK_HOST_CPU});
   die "No makefile\n" unless -f $makefile;

   $basedir = getcwd;

   $trick_objdir = "object_$ENV{TRICK_HOST_CPU}";
   chomp ($jeod_objdir = `grep '^JEOD_OBJDIR' $makefile | sed -e 's/.*= *//'`);
   die if (!defined $jeod_objdir) || ($jeod_objdir eq '');

   $verbose = $report = 0;

   $status = GetOptions ('verbose+'  => \$verbose,
                         'collect=s' => \@collect,
                         report      => \$report)
      or die "Bad options\n";

   if ((scalar @collect == 0) && (! $report)) {
      @collect = ($basedir);
      $report = 1;
   }
   elsif (scalar @collect != 0) {
      my $have_base = 0;
      foreach my $dir (@collect) {
         $dir = abs_path $dir;
         $dir =~ s/\/$//;
         if ($dir eq $basedir) {
            $have_base = 1;
         }
         push @collect, $basedir unless $have_base;
      }
   }
   

   foreach my $dir (@collect) {
      mkdir "$dir/gcov" unless -d "$dir/gcov";
   }

}


sub process_gcda_files () {
   my @gcda_files;
   open FIND, "find . -name '*.gcda'|" or die;
   @gcda_files = <FIND>;
   close FIND;
   map chomp, @gcda_files;


   foreach my $gcda_file (@gcda_files) {
      $gcda_file =~ /^\.((?:\/.*)?)\/([^\/]+)\/([^\/]+)\.gcda$/ or die;
      my ($parent, $objdir, $prefix) = ($1, $2, $3);
      $parent =~ s/^\///;
      $parent = '.' if $parent eq '';
      my $srcfile;

      if (-f "$parent/io_src/$prefix.cpp") {
         $srcfile = "io_src/$prefix.cpp";
      }

      elsif (-f "$parent/swig/$prefix.cpp") {
         $srcfile = "swig/$prefix.cpp";
      }

      elsif ($objdir eq $jeod_objdir) {
         $srcfile = "src/$prefix.cc";
      }

      elsif ($prefix eq 'main') {
         $srcfile = 'main.cc';
         $parent = "$parent/$objdir";
         $objdir = '.';
      }

      elsif ($prefix eq 'S_source') {
         $srcfile = 'S_source.cpp';
      }

      else {
         $srcfile = "$prefix.cc";
         $parent = "$parent/src";
         $objdir = "../$objdir";
      }
      if (! -f "$parent/$srcfile") {
         warn "Couldn't find source file corresponding to $gcda_file\n";
         next;
      }
    
      print "Processing $parent/$srcfile\n" if $verbose;
      chdir $parent;
      get_gcov $parent, $srcfile, $objdir;
      chdir $basedir;
   }
}


sub process_merged_files () {
   if ($verbose) {
      print "\n" . ('#' x 72) . "\n\n";
      print "Merged results:\n\n";
   }
   foreach my $file (<$basedir/gcov/*.gcov>) {
      my $n_statements = 0;
      my $n_executed   = 0;
      open IN, '<', $file or die;
      while (<IN>) {
         if (/^ *[#\d]/) {
            $n_statements++;
            $n_executed++ if /^ *\d/;
         }
      }
      close IN;
      my ($path, $ratio, $percent);
      ($path = $file) =~ s/\.gcov$//;
      $path =~ s/.*\///;
      $path =~ s/#/\//g;
      $ratio =  sprintf "%4d/%4d", $n_executed, $n_statements;
      $ratio =~ s/\/( *)(\d*)/\/$2$1/;
      if ($n_executed == $n_statements) {
         $percent = sprintf '100.00';
      }
      else {
         $percent = sprintf " %5.2f", 100.0*$n_executed/$n_statements;
      }
      printf "%-48s %s %s\n", $path, $ratio, $percent;
   }
}


sub get_gcov ($$$) {
   my ($parent, $srcfile, $objdir) = @_;
   my @gcov_files = ();
   my $mode = 0;
   open GCOV, "gcov -o $objdir $srcfile |" or die;
   while (<GCOV>) {
      if (/(.*):creating '(.*)'/) {
         my ($path, $gcov) = ($1, $2);
         $path = abs_path $path;
         next if ($path =~ /\/(verif|io_src|swig)\//) ||
                 ($path =~ /^\/(usr|sw)\//);
         foreach my $dir (@collect) {
            if ($path =~ /^\Q$dir\/\E(.*)/) {
               push @gcov_files, [$gcov, $dir, $1];
               last;
            }
         }
      }
      elsif ($verbose > 1) {
         print;
      }
   }
   close GCOV;
   foreach my $ent (@gcov_files) {
      save_gcov_file $ent->[0], $ent->[1], $ent->[2];
   }
   foreach my $file (<*.gcov>) {
      unlink $file or die;
   }   
}


sub save_gcov_file ($$$) {
   my ($gcov, $parentdir, $relpath) = @_;
   my ($savedir, $savename);
   ! -f $gcov and die;
   if (! -f "$parentdir/$relpath") {
      warn "No such file '$parentdir/$relpath'";
      return;
   }
   ($savename = $relpath) =~ s/\//#/g;
   $savename .= ".gcov";
   $savedir = "$parentdir/gcov";
   my $merged_file = "$savedir/$savename";
   my $new_contents = read_gcov_file $gcov;
   if (-f $merged_file) {
      my $old_contents = read_gcov_file $merged_file;
      $new_contents = merge_contents $old_contents, $new_contents;
   }
   open MERGE, '>', $merged_file or die;
   foreach my $ent (@$new_contents) {
      print MERGE +(join ':', @$ent) . "\n";
   }
   close MERGE;
   my @existing = <$savedir/$savename.*>;
   my $suffix = 1 + scalar @existing;
   rename $gcov, "$savedir/$savename.$suffix" or die;
}


sub merge_contents ($$) {
   my ($old_contents, $new_contents) = @_;
   my @merged_contents;
   die unless scalar @$old_contents == scalar @$new_contents;
   for (my $ii = 0; $ii <= $#$old_contents; $ii++) {
      my $old_ent = $old_contents->[$ii];
      my $new_ent = $new_contents->[$ii];
      my $merged_ent = [@$old_ent];
      die unless ($old_ent->[1] eq $new_ent->[1]) &&
                 ($old_ent->[2] eq $new_ent->[2]);
      if ($old_ent->[0] =~ /[#-]/) {
         $merged_ent->[0] = $new_ent->[0];
      } elsif ($new_ent->[0] !~ /[#-]/) {
         $merged_ent->[0] = sprintf "%9d", $old_ent->[0]+$new_ent->[0];
      }
      push @merged_contents, $merged_ent;
   }
   return [@merged_contents];
}


sub read_gcov_file ($) {
   my ($gcov_file) = @_;
   open GCOV, '<', $gcov_file or die;
   my $prev_line = 0;
   my @entries = ();
   while (<GCOV>) {
      /(.{9}):(.{5}):(.*)/ or die;
      my ($count, $lineno, $line) = ($1, $2, $3);
      if ($lineno > 0) {
         warn "Bad count" if ($lineno != $prev_line+1);
         $prev_line = $lineno;
         push @entries, [$count, $lineno, $line];
      }
   }
   close GCOV;
   return [@entries];
}
