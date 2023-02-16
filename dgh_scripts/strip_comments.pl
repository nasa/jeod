#!/usr/bin/perl

use strict;
use Cwd 'abs_path';

my @hfiles;

unless (abs_path('.') eq abs_path("$ENV{JEOD_HOME}")) {
   die "Run script from $ENV{JEOD_HOME}\n";
}

if (! -d 'no_comments_models') {
   printf "Making directory no_comments_models\n";
   mkdir 'no_comments_models' or die;
}

open FIND, "find models -name '*.hh' |" or die;
map chomp, (@hfiles = <FIND>);
close FIND; 
foreach my $hfile (@hfiles) {
   my ($nfile, $ndir);
   ($nfile = $hfile) =~ s/models/no_comments_models/;
   ($ndir  = $nfile) =~ s/\/[^\/]*$//;
   if (! -d $ndir) {
      my @components = split '/', $ndir;
      my $path = '.'; 
      foreach my $ent (@components) {
         $path .= "/$ent";
         if (! -d $path) {
            mkdir $path or die;
         } 
      } 
   } 

   open IN, '<', $hfile or die "open $hfile for input\n";
   open OUT, '>', $nfile or die "open $nfile for output\n";
   my $in_comment = 0;
   while (<IN>) {

      if ($in_comment) {
         if (s/.*\*\/ *//) {
            $in_comment = 0;
            next if (/^$/);
         } elsif (/\*\//) {
            die;
         } else {
            next;
         }
      }

      if (! $in_comment) {
         s/ *\/\/.*$//;
         s/ *\/\*.*?\*\///;
         if (s/ *\/\*.*//) {
            $in_comment = 1;
            next if (/^$/);
         }
      }

      print OUT;
   }
   close IN;
   close OUT;
}

