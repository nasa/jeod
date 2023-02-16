my $objdir_name;
my ($compile_cc, $compile_c);
my @inc_dirs;


chdir "$ENV{JEOD_HOME}/models" or die;
open FIND, "find . -name 'include' |" or die;
map chomp, (@inc_dirs = <FIND>);
close FIND;

foreach my $inc_dir (@inc_dirs) {
   (my $iosrc_dir = $inc_dir) =~ s/\/include$/\/io_src/;
   opendir INCDIR, $inc_dir or die;
   my @inc_files = grep {/^\w.*\.hh?$/} (readdir INCDIR);
   closedir INCDIR;
   sleep 0;
   do {mkdir $iosrc_dir or die} if (! -d $iosrc_dir);
   foreach my $inc_file (@inc_files) {
      print "$inc_dir/$inc_file\n";
      system "ICG $inc_dir/$inc_file >& /dev/null";
   }
}

