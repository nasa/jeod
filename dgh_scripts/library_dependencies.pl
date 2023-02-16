use strict;
use Cwd 'abs_path';

sub set_external_functions();
sub analyze_object_files();
sub generate_report();

my @src_files;
my %functions;
my %lib_references;

unless (abs_path('.') eq abs_path("$ENV{JEOD_HOME}/models")) {
   die "Run script from $ENV{JEOD_HOME}/models\n";
}

my @report_files = @ARGV;

set_external_functions();
analyze_object_files();
unless (scalar @report_files) {
   @report_files = @src_files;
}
generate_report();

######################################################################## #######

sub set_external_functions() {
   my @system_functions = (
      '__assert_fail',
      '__ctype_b_loc',
      '__ctype_tolower_loc',
      '__ctype_toupper_loc',
      '__cxa_bad_typeid',
      '__cxa_begin_catch',
      '__cxa_end_catch',
      '__cxa_pure_virtual',
      '__cxa_rethrow',
      '__dynamic_cast',
      '__errno_location',
      '__fxstat',
      '__gxx_personality_v0',
      '_Unwind_Resume',
      'access',
      'acos',
      'asin',
      'atan',
      'atan2',
      'calloc',
      'cos',
      'erf',
      'exp',
      'fclose',
      'fgets',
      'fileno',
      'floor',
      'fopen',
      'fprintf',
      'fread',
      'free',
      'fseek',
      'ftell',
      'getenv',
      'malloc',
      'memcpy',
      'memmove',
      'memset',
      'operator delete(void*)',
      'operator delete[](void*)',
      'operator new(unsigned int)',
      'operator new[](unsigned int)',
      'pthread_mutex_lock',
      'pthread_mutex_unlock',
      'rewind',
      'sin',
      'sprintf',
      'sqrt',
      'sscanf',
      'std::_List_node_base::unhook()',
      'std::_List_node_base::hook(std::_List_node_base*)',
      'std::__throw_out_of_range(char const*)',
      'stderr',
      'stdout',
      'strcat',
      'strcmp',
      'strcpy',
      'strdup',
      'strerror',
      'strlen',
      'strncmp',
      'strncpy',
      'strpbrk');

   my @trick_functions = qw(
      update_alloc_stats
      alloc_tree_by_address
      avl_delete
      dm_invert_symm
      euler_matrix
      exec_terminate
      io_compare_addr
      input_proc_error
      ip_user_alloc_type
      ip_mutex
      ipyyparse
      switch_new_file
      send_hs);


   foreach my $fun (@system_functions) {
      $functions{$fun}->{defined_in} = 'system';
   }
   foreach my $fun (@trick_functions) {
      $functions{$fun}->{defined_in} = 'trick';
   }
}


sub analyze_object_files() {
   open FIND, "find . -name '*.o' | grep -v -E '/(io|lib)_' | sort |" or die;
   my @obj_files = <FIND>;
   close FIND;
   map {chomp; s/^\.\///} @obj_files;
   sleep 0;
   foreach my $obj_file (@obj_files) {
      my ($src_file, $dep_file);
      my (%references, @references);
      ($src_file = $obj_file) =~ s/object_$ENV{TRICK_HOST_CPU}/src/;
      $src_file =~ s/\.o$/.cc/;
      if (! -e $src_file) {
         $src_file =~ s/\.cc$/.c/;
         if (! -e $src_file) {
            warn "Cannot find source for $obj_file\n";
         }
      }
      ($dep_file = $obj_file) =~ s/object_$ENV{TRICK_HOST_CPU}\///;

      push @src_files, $src_file;

      open DEFS, "nm -C $obj_file |" or die;
      while (<DEFS>) {
         chomp;
         ### next if / (for|to) /;
         s/^([0-9a-f]{8}| {8}) // or die "$_";
         /([\w]) (.*)/ or do {
            warn "$_ $src_file";
         };
         my ($t, $fun) = ($1, $2);
         if (($t eq 'T') && ($fun !~ / (for|to) /)) {
            if ((defined $functions{$fun}->{defined_in}) &&
                ($functions{$fun}->{defined_in} ne $dep_file)) {
               die "$fun defined in $dep_file and $functions{$fun}->{defined_in}";
            }
            $functions{$fun}->{defined_in} = $dep_file;
         }
         elsif (($t eq 'U') &&
                ($fun !~ /^attr/)) {
            if ($fun =~ /(vtable|typeinfo) (for|to) (.*)/) {
               $fun = "$3::$3()";
               if ($fun =~ /^[A-Z][a-z]/) {
                  $references{$fun} = 1;
               }
            } else {
               $references{$fun} = 1;
            }
         }
      }
      close DEFS;

      @references = keys %references;
      %references = ();
      foreach my $fun (@references) {
         if ((! defined $functions{$fun}->{defined_in}) ||
             (($functions{$fun}->{defined_in} ne $dep_file) &&
              ($functions{$fun}->{defined_in} ne 'system') &&
              ($functions{$fun}->{defined_in} ne 'trick'))) {
            push @{$functions{$fun}->{referenced_by}}, $dep_file;
            $references{$fun} = 1;
         }
      }
      @references = sort keys %references;
      $lib_references{$dep_file} = [@references];
   }
}


sub generate_report() {
   my %src_files;
   @src_files{@src_files} = @src_files;
   foreach my $src_file (@report_files) {
      if (! exists $src_files{$src_file}) {
         warn "$src_file was not compiled; skipping file\n";
         next;
      }

      my $dep_file = $src_file;
      my $dir = $src_file;
      my $base;
      $dep_file =~ s/src\///;
      $dep_file =~ s/\.cc$/.o/;
      $dir =~ s/\/src\/.*//;
      ($base = $dep_file) =~ s|$dir/||;
      my %calls;
      my @calls;
      $calls{$base} = 0;
      foreach my $fun (@{$lib_references{$dep_file}}) {
         my $where = $functions{$fun}->{defined_in};
         if (defined $where) {
            if ($where =~ s|$dir/||) {
               $calls{$where} = 0;
            } else {
               $calls{$where} = 1;
            }
         }
         else {
            warn "Undefined reference $fun in $src_file\n";
         }
      }
      delete $calls{$base};
      @calls = sort {($calls{$a} <=> $calls{$b}) || ($a cmp $b)} (keys %calls);
      unshift @calls, $base;
      print "Library dependencies for $src_file:\n";
      print "  ((", (join ")\n   (", @calls), "))\n\n";
   }
}
