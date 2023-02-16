#!/usr/bin/perl
################################################################################
#
# translate_logfile.pl
# Translate the names in a Trick logfile. Doing this enables log files with
# disparate names to be compared using standard Trick plotting tools.
# Documentation is in pod form. Type 'translate_logfile.pl -man' to view.
#
################################################################################

=pod

=head1 NAME

B<translate_logfile.pl> -
Generate a Trick logfile with the same data contents as a source logfile but
with the entry names translated according to a user-specified translation table.
The intent is to enable the creation of a new logfile whose contents can be
compared with some other logfile using standard Trick logfile analysis tools.


=head1 SYNOPSIS

B<translate_logfile.pl>
I<< <input_logfile> >>
I<< <translation_table> >>
I<< <output_logfile> >>

Generate the  I<< <output_logfile> >> based on the I<< <input_logfile> >>
but with parameter names translated according to the I<< <translation_table> >>.

=head1 ARGUMENTS

=over 4

=item I<< <input_logfile> >>

The source Trick logfile (.trk format).


=item I<< <translation_table> >>

The name of the file that specifies the translation table.
Each non-comment line in the translation table file must contain two items
separated by whitespace. The first item is a regular expression while the second
is the substitution for parameter names that match the regular expression.
The translation table file may be commented with '#' style comments.


=item I<< <output_logfile> >>

The output Trick logfile (.trk format).

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

=cut

################################################################################

use strict;
use warnings;
use Getopt::Long;
 use Pod::Usage;
use lib "$ENV{JEOD_HOME}/bin/jeod";
use Trick_Logfile;

sub parse_trans_table();
sub generate_header();
sub copy_data();

my ($input_name,  $log_in,
    $output_name, $log_out,
    $trans_name,  @translators,
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

parse_trans_table();

$log_in  = new Trick_Logfile ($input_name, 'r');
$log_out = new Trick_Logfile ($output_name, 'w');

generate_header();
copy_data();


################################################################################


sub generate_header() {
   $log_out->dup_log_info ($log_in);
   foreach my $param (@{$log_out->{params}}) {
      foreach my $trans (@translators) {
         last if (&$trans ($param));
      }
   }
   $log_out->write_log_header();
}

sub copy_data() {
   my ($blkid, $blksize);
   $blksize = $log_in->{blksize};
   while ($log_in->read_block()) {
      $log_out->{blkno}++;
      $blkid = "block $log_out->{blkno}";
      $log_out->write_output ($log_in->{block}, $blksize, '', $blkid);
   }
   $log_in->close();
   $log_out->close();
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
         sub {my $param = shift; $param->{name} =~ s/$pat/$subst/;};
      push @translators, $sub;
   }
   close TRANS;
}
