#!/usr/bin/perl -w
use strict;
use Getopt::Long;
use IO::Handle;

sub main();
sub get_options();
sub check_file($$);
sub fix_file($$);
sub yorn($);

our $index = 0;
my  $restore = 0;
my  $test = 0;
my  $suffix = 'orig';

main();


################################################################################

sub main () {
   my  @tex_files = ();
   my  @suffixed_tex_files = ();
   my  @need_fix = ();

   get_options();

   open FIND, 'find . -type f -name "*.tex" |' or die;
   @tex_files = <FIND>;
   close FIND;
   map chomp, @tex_files;
   foreach my $file (@tex_files) {
      if (-f "$file.$suffix") {
         push @suffixed_tex_files, [$file, "$file.$suffix"];
      }
   }

   if ($restore) {
      if (! scalar @suffixed_tex_files) {
         print "Nothing to restore!\n";
      }
      elsif ($test) {
         print "The following files need to be restored:\n";
         foreach my $ent (@suffixed_tex_files) {
            print "$ent->[0] from $ent->[1]\n";
         }
      }
      else {
         foreach my $ent (@suffixed_tex_files) {
            rename $ent->[1], $ent->[0]
               or die "Can't rename $ent->[1] to $ent->[0]\n";
         }
      }
   }

   else {
      if (scalar @suffixed_tex_files) {
         print
            "There are some existing files suffixed with .$suffix\n" .
            "Please do something about those files or use a different suffix\n";
      }
      else {
         foreach my $file (@tex_files) {
            push @need_fix, check_file $file, $test;
         }
         if (! scalar @need_fix) {
            print "Nothing to repair!\n";
         }
         elsif ($test) {
            print "The following repairs need to be made:\n";
            foreach my $ent (@need_fix) {
               print "In file $ent->[0], " .
                     "the following lines need to be changed:\n";
               foreach my $subent (@{$ent->[1]}) {
                  printf ("  %4d %s", $subent->[0], $subent->[1]);
               }
               print "The above will be changed to:\n";
               foreach my $subent (@{$ent->[1]}) {
                  printf ("  %4d %s", $subent->[0], $subent->[2]);
               }
               print "\n";
            }
         }
         else {
            printf "%d files need to be repaired.\n", scalar @need_fix;
            if (yorn 'Do you want to proceed') {
               foreach my $file (@need_fix) {
                  fix_file $file, $suffix;
               }
            }
         }
      }
   }
}


sub get_options() {
   my  $help = 0;

   GetOptions (
      'help!'    => \$help,    # Brief help
      'restore!' => \$restore, # Oops mode
      'test!'    => \$test,    # Look, but don't touch
      'suffix=s' => \$suffix)  # Appended suffix to aid in recovery
   or do {
      print "Invalid options.\n";
      $help = 1;
   };

   if ($help) {
      my $message = << '      EOF';
         Purpose: Fix .tex files at or below the current working directory.
         Usage: $0 <options>

         The options are
           -help             Print this message and quit.

           -restore          Restore saved versions of clobbered files when set
                             (Just in case this script is ummm, funky.)

           -test             Just see what needs to be done, but don't do it.
                             This list the files that either need to be changed
                             (-norestore) or restored (-restore).

           -suffix <suffix>  In non-restore mode, the suffix acts as a backup
                             mechanism. Any file that needs to be changed will
                             be renamed to the file name plus this suffix.
                             Prior to proceeding the script will check that
                             such suffixed files do not exist.

                             In restore mode, any files found that are thusly
                             suffixed will be restored to their pristine state
                             from the backup files.
      EOF
      $message =~ s/^         //mg;
      print $message;
      exit;
   }
}


sub check_file ($$) {
   my ($fname, $test) = @_;
   my $needs_repair = 0;
   my $line_no = 0;
   my @fix_these = ();
   my @not_fixed = ();

   open INPUT, '<', $fname or die;
   while (<INPUT>) {
      $line_no++;

      if (/^(\s*)
            (\\hyperdef\{[^{}]*\}\{[^{}]*\})
            \{(\\[a-z]+\{[^{}]+\})\}
            (.*)/x) {
         my ($pre, $hyper, $command, $rest) = ($1, $2, $3, $4);
         if ($test) {
            my $fix = "${pre}${command}${hyper}\{\}${rest}\n";
            push @fix_these, [$line_no, $_, $fix];
         }
         $needs_repair = 1;
      }
      elsif (/hyperdef/) {
         if ((! /\\hyperdef\{[^{}]*\}\{[^{}]*\}\{\}/) &&
             (! /^ *\%/)) {
            push @not_fixed, [$line_no, $_];
         }
      }
   }
   close INPUT;
   if (scalar @not_fixed) {
      print "The following lines in $fname will not be repaired:\n";
      foreach my $ent (@not_fixed) {
         printf ("  %4d %s", @$ent);
      }
      print "\n";
   }
   if ($needs_repair) {
      if ($test) {
         return [$fname, [@fix_these]];
      }
      else {
         return $fname;
      }
   }
   else {
      return ();
   }
}


sub fix_file ($$) {
   my ($fname, $suffix) = @_;
   my $tempfile = "fixed_tex_${$}_${index}.tex";
   my $repaired = 0;
   $index++;
   open INPUT, '<', $fname or die;
   open OUTPUT, '>', $tempfile or die;
   while (<INPUT>) {
      if (s/^(\s*)
             (\\hyperdef\{[^{}]*\}\{[^{}]*\})
             \{(\\[a-z]+\{[^{}]+\})\}
           /$1$3$2\{\}/x) {
         $repaired = 1;
      }
      print OUTPUT;
   }
   close OUTPUT;
   close INPUT;
   if ($repaired) {
      print "$fname repaired; backup saved as $fname.$suffix\n";
      rename $fname , "$fname.$suffix" or die;
      rename $tempfile, $fname or die;
   }
   else {
      print "$fname is OK???\n";
      unlink $tempfile;
   }
}


sub yorn ($) {
   my ($prompt) = @_;
   my $ans;
   open TTY, '+>', '/dev/tty' or die "Can't open /dev/tty\n";
   autoflush TTY 1;
   do {
      print TTY "$prompt (y/n): ";
      $_ = <TTY>;
      if (/^\s*y(es)?\s*$/i) {
         $ans = 1;
      }
      elsif (/^\s*n(o)?\s*$/i) {
         $ans = 0;
      }
   } while (! defined $ans);
   return $ans;
}
