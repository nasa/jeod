use strict;
sub make_table ($$);
sub get_managed_files;
sub categorize_files($);
sub compare_source_files;
sub compare_non_source_files;

my @tables_info = (
   {key => 'source', label => 'source'},
   {key => 'data',   label => 'data'},
   {key => 'docs',   label => 'documentation'},
   {key => 'verif',  label => 'verification'});


chdir '../..' or die;
my $managed_files = get_managed_files;
my %categorized_files = categorize_files $managed_files;
foreach my $ent (@tables_info) {
   my $ent_files = $categorized_files{$ent->{key}};
   if (scalar @$ent_files) {
      print make_table $ent->{label}, $ent_files;
   }
}


sub make_table ($$) {
   my ($label, $files) = @_;
   my $caption = ucfirst $label;
   my $table;

   # Generate the table header.
   $table = << "   EOF";
      \\begin\{stretchlongtable\}\[c\]\{||p{0.75\\textwidth}|\}
      \\caption\{$caption Files\}
      \\label\{tab:${label}_files\} \\\\\[6pt\]

      \\hline
      \\multicolumn\{1\}\{||c|\} \{\\bf File Name\} \\\\
      \\hline \\hline
      \\endfirsthead

      \\caption\[\]\{$caption Files (continued from previous page)\} \\\\\[6pt\]
      \\hline
      \\multicolumn\{1\}\{||c|\} \{\\bf File Name\} \\\\
      \\hline \\hline
      \\endhead

      \\hline \\multicolumn\{1\}\{r\}\{\{Continued on next page\}\} \\\\
      \\endfoot

      \\hline
      \\endlastfoot
   EOF
   $table =~ s/^      //mg;

   # Add files to the table.
   foreach my $file (@$files) {
      # Make it so the filename can be split at directories and at underscores.
      $file =~ s/\//\/\\hspace{0pt}/g;
      $file =~ s/_/\\_\\hspace{0pt}/g;
      $file =~ s/&/\\&\\hspace{0pt}/g;
      $table .= "\\longentry $file \\tabularnewline\n",
   }

   # Append the environment \end macro to end the table and return the table.
   $table .= "\\end{stretchlongtable}\n\n";
   return $table;
}


sub get_managed_files() {
   my @files = ();
   my @tree = `git ls-tree -r HEAD`;
   foreach my $entry (@tree) {
     #delete everything except the last contiguous block of non-space
     $entry =~ s/.*\s(\S+)/$1/;
     push @files, $entry;
   }
   return [@files];
}


sub categorize_files($) {
   my ($files) = @_;

   my %categorized_files = (
      source => [],
      data   => [],
      docs   => [],
      verif  => []);

   my %sort_function = (
      source => \&compare_source_files,
      data   => \&compare_non_source_files,
      docs   => \&compare_non_source_files,
      verif  => \&compare_non_source_files);

   foreach my $file (@$files) {
      (my $top = $file) =~ s/\/.*//;
      chomp $top;
      my $bin;
      if (($top eq 'verif') || ($file =~ /verif\//)) {
         $bin = 'verif';
      }
      elsif ($top eq 'docs') {
         $bin = 'docs';
      }
      elsif (($top eq 'data') || ($file =~ /^data\//)) {
         $bin = 'data';
      }
      elsif ((($top eq 'include') || ($file =~ /\/include\//)) ||
             (($top eq 'src') || ($file =~ /\/src\//))) {
         $bin = 'source';
      }
      else {
         $bin = 'data';
      }
      push @{$categorized_files{$bin}}, $file;
   }

   foreach my $bin (qw(source data docs verif)) {
      my $files = $categorized_files{$bin};
      my $compare = $sort_function{$bin};
      if (scalar @$files) {
         my @sorted_files = sort $compare @$files;
         $categorized_files{$bin} = [@sorted_files];
      }
   }

   return %categorized_files;
}


sub compare_source_files {
   return ((($a =~ tr/\///) <=> ($b =~ tr/\///)) ||
           ($a cmp $b));
}


sub compare_non_source_files {
   (my $file_a = $a) =~ s/ \(symbolic link.*//;
   (my $file_b = $b) =~ s/ \(symbolic link.*//;
   my @a_path = split /\//, $file_a;
   my @b_path = split /\//, $file_b;
   my $cmp = 0;
   my $min_length = $#a_path < $#b_path ? $#a_path : $#b_path;

   # Compare the directory names leading up to the file name.
   for (my $ii = 0; ($ii < $min_length) && ($cmp == 0); $ii++) {
      $cmp = $a_path[$ii] cmp $b_path[$ii];
   }

   if ($cmp != 0) {
      return $cmp;
   }

   elsif ($#a_path != $#b_path) {
      return $#a_path <=> $#b_path;
   }

   else {
      return $a_path[$#a_path] cmp $b_path[$#b_path];
   }
}
