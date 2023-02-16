#!/usr/local/bin/perl -w

use strict;
use Getopt::Long;
use Pod::Usage;
use Cwd;

my $fail_only = 0;
my $help = 0;
my $man = 0;


# Parse the cammand line
GetOptions (
   "fail_only!" => \$fail_only,
   "help"       => \$help,
   "man"        => \$man)
or pod2usage (-exitval => 2);

if ($man) {
   my $pager;
   $pager = $ENV{MANPAGER};
   $pager = $ENV{PAGER}     unless defined $pager;
   $pager = 'less'          unless defined $pager;
   system "pod2man $0 | nroff -man | $pager";
   exit 0;
} elsif ($help) {
   pod2usage (-exitval => 1);
}

scalar @ARGV or die "Syntax: test_includes.pl fname ...\n";

my $cwd = getcwd;
my $trick_ver = $ENV{TRICK_VER};
my ($trick_major, $trick_minor) = $trick_ver =~ /(\d+)\.([^\.]+)/;
my $cflags = "$ENV{TRICK_CFLAGS} -I$ENV{TRICK_HOME}/trick_source " .
             '-Wno-write-strings';
my $tflags = "-DTRICK_VER=$trick_major -DTRICK_MINOR=$trick_minor";


# cleanup: Get rid of the junk files.
# FIXME: This needs to be called in a END block
sub cleanup ($) {
   my ($dir) = @_;
   foreach my $suff (qw (cc hh o nm)) {
      if (-f "$dir/junk.$suff") {
         unless (unlink "$dir/junk.$suff") {
            die "Can't delete $dir/junk.$suff exists\n";
         }
      }
   }
   return 1;
}



# strip_comments: get rid of comments in code.
sub strip_comments ($$) {
   my ($line, $in_comment) = @_;
   my $unprocessed = $line;
   my $code = '';

   while (1) {
      if (! $in_comment) {
         if ($unprocessed =~ /^(.*?)\/([\*\/])(.*)/) {
            my ($pre, $ctype, $rest) = ($1, $2, $3);
            $code .= $pre . ' ';
            if ($ctype eq '/') {
               $unprocessed = '';
               last;
            }
            else {
               $unprocessed = $rest;
               $in_comment = 1;
            }
         }
         else {
            $code .= $unprocessed;
            $unprocessed = '';
            last;
         }
      }
      else {
         if ($unprocessed =~ /^.*?\*\/(.*)/) {
            $unprocessed = $1;
            $in_comment = 0;
         }
         else {
            $unprocessed = '';
            last;
         }
      }
   }
   $code =~ s/ +$//;

   return ($code, $in_comment);
}


# collect_includes: Determine the files that are #included by some file.
sub collect_includes ($) {
   my ($file_name) = @_;
   my $is_header = $file_name =~ /\.hh?$/;
   my @contents;

   open IN, '<', $file_name or die "Can't open $file_name\n";
   @contents = <IN>;
   close IN;

   my @includes = ();
   my $mode = $is_header ? 0 : 3;
   # 0 = before protect block
   # 1 = just after ifndef NAME
   # 2 = after define NAME
   # 3 = after endif
   # 9 = bad mojo
   my $in_comment = 0;
   my $ifdepth = 0;
   my $active = 1;
   my $activate_at = 0;
   my $top_blocks = 0;
   my $protect;
   my $lineno;


   $lineno = 0;
   foreach my $line (@contents) {
      $lineno++;
      chomp $line;
      $line =~ s/\r$//;

      # Deal with comments.
      ($line, $in_comment) = strip_comments $line, $in_comment;

      # Deal with double-include protection.
      if ($is_header) {
         if (($mode == 0) && ($line !~ /^$/)) {
            if ($line =~ /^#ifndef +(\w+)$/) {
               $protect = $1;
               $mode = 1;
               $ifdepth = 1;
               next;
            }
            else {
               warn "First code line is not #ifndef NAME " .
                    "at line $lineno in $file_name\n";
               $mode = 9;
               last;
            }
         }
         elsif ($mode == 1) {
            if ($line =~ /^$/) {
               warn "#define $protect does not follow #ifndef $protect " .
                    "at line $lineno in $file_name\n";
               next;
            }
            elsif ($line =~ /^#define +(\w+)$/) {
               my $name = $1;
               if ($name ne $protect) {
                  warn "Mismatch protect block name $name versus $protect " .
                       "at line $lineno in $file_name\n";
               }
               $mode = 2;
               next;
            }
            else {
               warn "Line after #ifndef $protect is not #define $protect " .
                    "at line $lineno in $file_name\n";
               $mode = 9;
               last;
            }
         }
         elsif ($mode == 2) {
            if (($ifdepth == 1) &&
                ($line =~ /^ *# *endif$/)) {
               $ifdepth = 0;
               $mode = 3;
               next;
            }
         }
         elsif ($mode == 3) {
            if ($line !~ /^$/) {
               warn "Code after end of protect block " .
                    "at line $lineno in $file_name\n";
               $mode = 9;
               last;
            }
         }
      }

      if ($line =~ /^$/) {
         ;
      }
      elsif ($line =~ /^ *# *if(.*)/) {
         my $if = $1;
         $ifdepth++;

         if ($active) {
            if ($if =~ /\(? *0 *\)?/) {
               warn "#if 0 " .
                    "at line $lineno in $file_name\n";
               $active = 0;
               $activate_at = $ifdepth;
            }
            elsif ($if =~ /def +TRICK_VER$/) {
               $active = 0;
               $activate_at = $ifdepth;
            }
         }
      }

      elsif ($line =~ /^ *# *el(.*)/) {
         my $else = $1;
         if (!$active && ($activate_at == $ifdepth)) {
            $active = 1;
         }
      }

      elsif ($line =~ /^ *# *endif$/) {
         if (!$active && ($activate_at == $ifdepth)) {
            $active = 1;
         }
         $ifdepth--;
      }

      elsif ($line =~ /^ *# *include *(.*)/) {
         my $inc = $1;
         if ($active) {
            push @includes, $inc;
         }
      }

      else {
         ;
      }
   }

   if ($mode == 3) {
      return @includes;
   }
   else {
      if ($mode == 0) {
         warn "$file_name is an empty header\n";
      }
      elsif (($mode == 1) || ($mode == 2)) {
         warn "$file_name has un-terminated #ifndef\n";
      }
      return ();
   }
}


# file_contains: Determine if the file contains some string.
sub file_contains ($$) {
   my ($file_name, $string) = @_;
   my @contents;

   open IN, '<', $file_name or die "Can't open $file_name\n";
   @contents = <IN>;
   close IN;

   my $in_comment = 0;

   foreach my $line (@contents) {
      chomp $line;
      $line =~ s/\r$//;

      # Deal with comments.
      ($line, $in_comment) = strip_comments $line, $in_comment;

      if (($line !~ /^ *#/) &&
          ($line =~ /$string/)) {
         return 1;
      }
   }
   return 0;
}


# compile: Compile file to generate messages
sub compile ($;$$) {
   my ($dir, $oflags, $suff) = @_;
   my $flags = defined $oflags ? "$cflags $oflags" : $cflags;
   my %cc = (c => 'gcc', cc => 'g++');
   my $cc;

   $suff = 'cc' unless defined $suff;
   $cc   = $cc{$suff};
   defined $cc or die "compile $dir $suff\n";

   system "$cc -o $dir/junk.o -c $dir/junk.$suff $flags";
}


# test_compile: Does file compile?
# Returns true if files compiles without warnings.
sub test_compile ($;$$) {
   my ($dir, $oflags, $suff) = @_;
   my $flags = defined $oflags ? "$cflags $oflags" : $cflags;
   my %cc = (c => 'gcc', cc => 'g++');
   my $cc;

   $suff = 'cc' unless defined $suff;
   $cc   = $cc{$suff};
   defined $cc or die "compile $dir $suff\n";

   if (-f "$dir/junk.o") {
      unlink "$dir/junk.o" or die "Can't delete $dir/junk.o\n";
   }
   my $ec = `$cc -o $dir/junk.o -c $dir/junk.$suff $flags 2>&1 | wc -l`;
   chomp $ec;
   return (-f "$dir/junk.o") && ($ec == 0);
}


# test_include: Is include file needed?
# Returns true if test compile excluding the include file fails.
sub test_include ($$$$) {
   my ($fname, $dir, $suff, $include) = @_;
   my $csuff = ($suff eq 'hh') ? 'cc' : $suff;
   system "grep -v '#include  *$include' $fname > $dir/junk.$suff"
      and die "Can't build $dir/junk.$suff\n";
   system "cmp -s $fname $dir/junk.$suff"
      or die "Internal error excluding $include from $fname\n";
   return ! test_compile $dir, undef, $csuff;
}

# build_nm: Build defalt symbol table.
sub build_nm ($$) {
   my ($dir, $suff) = @_;
   my @commands = ("nm -g $dir/junk.o");
   my $nm;

   if ($suff eq 'cc') {
      push @commands,
        'grep -v \'\.eh$\'',
        'sed -e \'s/^................ //\'',
        'c++filt';
   }
   else {
      push @commands,
        'sed -e \'s/^................ //\'';
   }
   $nm = join ' | ', @commands;
   ## system "nm -C $dir/junk.o > $dir/junk.nm"
   ##    and die "Can't build $dir/junk.nm\n";
   system "$nm > $dir/junk.nm"
      and die "Can't build $dir/junk.nm\n";
}

# test_nm: Another test of whether include file is needed.
# Returns true if symbol table differs from the default table.
sub test_nm ($) {
   my ($dir) = @_;
   my $nm = join ' | ',
            "nm -g $dir/junk.o",
            'grep -v \'\.eh$\'',
            'sed -e \'s/^................ //\'',
            'c++filt';
   # my $ec = `nm -C $dir/junk.o | diff - $dir/junk.nm | wc -l`;
   my $ec = `$nm | diff - $dir/junk.nm | wc -l`;
   chomp $ec;
   return ($ec != 0);
}


my $last_was_ok = 1;
foreach my $fname (@ARGV) {
   my ($dir, $base, @includes, %includes);
   if ($fname =~ /^(.*)\/(.*)$/) {
      ($dir, $base) = ($1, $2);
   } else {
      ($dir, $base) = ($cwd, $fname);
   }

   if (! -f $fname) {
      warn "No such file '$fname'\n";
      next;
   }

   @includes = collect_includes $fname;
   @includes{@includes} = @includes;

   if ($fname =~ /\.hh$/) {
      if (! cleanup $dir) {
         next;
      }
      
      open JUNKCC, '>', "$dir/junk.cc";
      print JUNKCC "#include \"junk.hh\"\nint main() {return 0;}\n";
      close JUNKCC;

      system "cat $fname > $dir/junk.hh" and die;
      if (! test_compile $dir) {
         warn "\n$fname\nTest program does not compile clean. Skipping tests\n";
         compile $dir;
      }

      elsif (! test_compile $dir, $tflags) {
         warn "\n$fname\nTest program does not compile clean. Skipping tests\n";
         compile $dir, $tflags;
      }

      elsif ($fail_only) {
         print "$fname OK\n";
      }

      else {
         my $inline = $fname;
         my @not_needed = ();
         $inline =~ s/.*\///;
         $inline =~ s/\.hh$/_inline.hh/;
         foreach my $include (@includes) {
            if (($include ne "\"$inline\"") &&
                ($include ne '"class_declarations.hh"') &&
                ($include ne '"utils/misc/include/jeod_class.hh"') &&
                (! test_include $fname, $dir, 'hh', $include)) {
               my $needed = 0;
               if ((($include eq '"utils/math/include/vector3.hh"') &&
                    file_contains $fname, 'Vector3::') ||
                   (($include eq '"utils/math/include/matrix3x3.hh"') &&
                    file_contains $fname, 'Matrix3x3::') ||
                   (($include eq '<cstddef>') &&
                    file_contains $fname, 'NULL')) {
                  $needed = 1;
               }
               push @not_needed, $include unless $needed;
            }
         }
         if (@not_needed) {
            print "\n$fname may include unneccessary files:\n" .
                  (join "\n", (map {"   $_"} @not_needed), '');
            $last_was_ok = 0;
         } else {
            print "\n" unless $last_was_ok;
            print "$fname OK\n";
            $last_was_ok = 1;
         }
      }
      cleanup $dir;
   }

   elsif ($fname =~ /\.(cc?)$/) {
      my $suff = $1;

      if (! cleanup $dir) {
         next;
      }
      
      system "cat $fname > $dir/junk.$suff" and die;
      if (! test_compile $dir, undef, $suff) {
         warn "\n$fname\nDoes not compile clean. Skipping tests\n";
         compile $dir;
      }

      elsif (! test_compile $dir, $tflags, $suff) {
         warn "\n$fname\nDoes not compile clean. Skipping tests\n";
         compile $dir, $tflags, $suff;
      }

      elsif ($fail_only) {
         print "$fname OK\n";
      }

      else {
         build_nm $dir, $suff;
         my @not_needed = ();
         foreach my $include (@includes) {
            ## (my $inline = $include) =~ s/\.hh/_inline.hh/;
            if (## (($inline eq $include) || (! defined $includes{$inline})) &&
                (! (test_include $fname, $dir, 'cc', $include)) &&
                (! (test_nm $dir))) {
               my $needed = 0;
               if ((($include eq '"utils/math/include/vector3.hh"') &&
                    file_contains $fname, 'Vector3::') ||
                   (($include eq '"utils/math/include/matrix3x3.hh"') &&
                    file_contains $fname, 'Matrix3x3::') ||
                   (($include eq '<cstddef>') &&
                    file_contains $fname, 'NULL')) {
                  $needed = 1;
               }
               push @not_needed, $include unless $needed;
            }
         }
         if (@not_needed) {
            print "\n$fname may include unneccessary files:\n" .
                  (join "\n", (map {"   $_"} @not_needed), '');
            $last_was_ok = 0;
         } else {
            print "\n" unless $last_was_ok;
            print "$fname OK\n";
            $last_was_ok = 1;
         }
      }
      cleanup $dir;
   }
   else {
      warn "File '$fname' is not a header or source file\n";
   }
}
