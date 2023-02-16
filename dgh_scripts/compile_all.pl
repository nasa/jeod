my $objdir_name;
my ($compile_cc, $compile_c);
my @src_dirs;

$objdir_name = "object_$ENV{TRICK_HOST_CPU}";
$compile_cc = "g++ $ENV{TRICK_CFLAGS} -I $ENV{TRICK_HOME}/trick_source";
$compile_c  = "gcc $ENV{TRICK_CFLAGS} -I $ENV{TRICK_HOME}/trick_source";


chdir "$ENV{JEOD_HOME}/models" or die;
open FIND, "find . -name 'src' |" or die;
map chomp, (@src_dirs = <FIND>);
close FIND;

foreach my $src_dir (@src_dirs) {
   (my $obj_dir = $src_dir) =~ s/\/src$/\/$objdir_name/;
   opendir SRCDIR, $src_dir or die;
   my @src_files = grep {/^\w.*\.cc?$/} (readdir SRCDIR);
   closedir SRCDIR;
   sleep 0;
   do {mkdir $obj_dir or die} if (! -d $obj_dir);
   foreach my $src_file (@src_files) {
      (my $obj_file = $src_file) =~ s/\.cc?$/.o/ or die;
      my $cc = ($src_file =~ /cc$/) ? $compile_cc : $compile_c;
      $cc .= " -c $src_dir/$src_file -o $obj_dir/$obj_file";
      print "$src_dir/$src_file";
      if (system "$cc >& /dev/null") {
         print " failed\n";
      } else {
         print " OK\n";
      }
   }
}

