#!/usr/bin/perl
################################################################################
#
# translate_DP_file.pl
# Translate the names in a Trick data products file. Doing this enables DP
# files to be updates with new variable names.   Documentation is in pod
# form. Type 'translate_DP_file.pl -man' to view.
#
################################################################################

=pod

=head1 NAME

B<translate_DP_file.pl> -
Generate a Trick data products file with the same plot contents as a source
file with the entry names translated according to a user-specified translation
table.  The intent is to transfer DP files from one simulation to another.


=head1 SYNOPSIS

B<translate_DP_file.pl>
I<< <input_DP_file> >>
I<< <translation_table> >>
I<< <output_DP_file> >>

Generate the  I<< <output_DP_file> >> based on the I<< <input_DP_file> >>
but with parameter names translated according to the I<< <translation_table> >>.

=head1 ARGUMENTS

=over 4

=item I<< <input_DP_file> >>

The source Trick DP file (.trk format).


=item I<< <translation_table> >>

The name of the file that specifies the translation table.
Each non-comment line in the translation table file must contain two items
separated by whitespace. The first item is a regular expression while the second
is the substitution for parameter names that match the regular expression.
The translation table file may be commented with '#' style comments.


=item I<< <output_DP_file> >>

The output Trick DP file.

=back

=head1 OPTIONS

=over 4

=item B<-help>
Provide a brief command summary and exit.

=item B<-man>
Provide a detailed command summary and exit.

=back

=head1 AUTHOR

David Hammen, Odyssey Space Research

Edwin Z. Crues, NASA Johnson Space Center

=cut

################################################################################

use strict;
use warnings;
use Getopt::Long;
use Pod::Usage;
use IO::File;

sub parse_trans_table();
sub trans_DP_file();

my ($input_name,  $DP_in,
    $output_name, $DP_out,
    $trans_name,  @translators,
    $line,
    $help,
    $man);


# Parse the cammand line
$help = $man = 0;
GetOptions (
   "help" => \$help,
   "man"  => \$man)
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
} elsif (scalar (@ARGV) != 3) {
   pod2usage (-exitval => 2);
}


($input_name, $trans_name, $output_name) = @ARGV;

# Parse the translation table.
parse_trans_table();

# Open the input DP file.
#$DP_in = IO::File->new ( $input_name, 'r' )
open $DP_in, '<', $input_name
   or die "Can't open $input_name for input\n";

# Open the output DP file.
#$DP_out = IO::File->new ( $output_name, 'w' )
open $DP_out, '>', $output_name
   or die "Can't open $output_name for output\n";

trans_DP_file();

close $DP_in;
close $DP_out;


################################################################################


sub trans_DP_file() {

   while ( $line = <$DP_in> ) {

      # Loop through the translators.
      foreach my $trans (@translators) {
         last if (&$trans ($line));
      }

      print $DP_out $line;

   }

}

sub parse_trans_table() {
   open TRANS, '<', $trans_name
      or die "Can't open $trans_name for input\n";
   while (<TRANS>) {
      next if /^\s*$/ || /^\s*#/;
      /^\s*(\S+)\s*(\S+)\s*$/
         or die "Bad translation table line $_";
      my ($pat, $subst) = ($1, $2);
      my $sub =
         sub {$line =~ s/$pat/$subst/;};
      push @translators, $sub;
   }
   close TRANS;
}
