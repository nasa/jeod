use strict;

sub make_refman ();


my $home = $ENV{JEOD_HOME};
my $scripts = "$home/dgh_scripts";
my $jeod_pp = "$scripts/jeod_pp";
my $template = "$scripts/pdf_config.dox";
my $model_name = shift;

make_refman;

sub make_refman () {

   chdir '../..' or die;
   mkdir 'doxygen' unless -e 'doxygen';
   system "sed -e 's/INSERT_MODEL_NAME_HERE/$model_name/' $template " .
          "> doxygen/config.dox"
      and do {
      die "sed failure";
   };
   open FIND, "find . -name '*.[ch]*'|" .
              "grep -E '\.[ch][ch]\$'|" .
              "grep -v '/verif/'|" .
              "sed -e 's/^\\.\\///'|" or do {
      warn "find pipe create failure\n";
      next;
   };
   my @files = <FIND>;
   close FIND;
   scalar @files or do {
      warn "no files found in $ENV{PWD}\n";
      next;
   };
   map chomp, @files;

   foreach my $file (@files) {
      $file =~ /^(.*)\/([^\/]+)/ or die;
      my $dox_path = "doxygen/$1";
      unless (-d $dox_path) {
         warn "mkdir -p $dox_path";
         system "mkdir -p $dox_path" and do {
            warn "Can't create directory $dox_path";
            next;
         };
      }
      system "cp $file doxygen/$file" and do {
         warn "Can't copy $file to doxygen/$file";
         next;
      };
   }

   chdir 'doxygen' or do {
      warn "no such dir doxygen in $ENV{PWD}";
      next;
   };
   system "doxygen config.dox" and do {
      warn "doxygen failure";
      next;
   };
   chdir 'doxygen/latex' or do {
      warn "no such dir doxygen/latex in $ENV{PWD}";
      next;
   };
   system 'make';
   system 'cp refman.pdf ../../../docs' if (-f 'refman.pdf');
   chdir '../../..';
   system "rm -rf doxygen";
}
