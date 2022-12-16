#!/usr/bin/perl -w
################################################################################
#
# Purpose:
#   (Add the dependencies produced by gcc -MM to the makefile.)
#
#
#
################################################################################

use strict;
use Cwd;

my $temp_depend = shift;

my (@dependencies, @file_dependencies, %file_dependencies);

open DEPEND, '<', $temp_depend or die "Can't read $temp_depend\n";

my $in_depend = 0;

while (defined (my $line = <DEPEND>)) {
   chomp $line;
   if (! $in_depend) {
      next if $line =~ /^ *$/;
      $line =~ /^(\S+):\s*(.*)/ or die;
      my ($file, $depends) = ($1, $2);
      if ($depends =~ s/ *\\$//) {
         $in_depend = 1;
      } else {
         $in_depend = 0;
      }
      push @dependencies, "\n\$(OBJDIR)\/${file}: \\\n";
      push @file_dependencies, split / /, $depends;
   } else {
      my $depends = $line;
      die "Unexpected entry" if ($depends =~ /^\S/);
      $depends =~ s/^ *//;
      if ($depends =~ s/ *\\$//) {
         $in_depend = 1;
      } else {
         $in_depend = 0;
      }
      push @file_dependencies, split / /, $depends;
      if (! $in_depend) {
         my $source = shift @file_dependencies;
         %file_dependencies = ();
         foreach my $file (@file_dependencies) {
            $file =~ s/^\w+\/\.\.\///;
            $file_dependencies{$file} = 1;
         }
         @file_dependencies = sort keys %file_dependencies;
         unshift @file_dependencies, $source;
         map {$_ = " $_ \\\n"} @file_dependencies;
         $file_dependencies[$#file_dependencies] =~ s/ *\\$//;
         push @dependencies, @file_dependencies;
         @file_dependencies = ();
      }
   }
}

map {s/$ENV{JEOD_HOME}/\${JEOD_HOME}/g} @dependencies;
print @dependencies;
