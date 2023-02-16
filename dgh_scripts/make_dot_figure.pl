use strict;
my $dot_file = shift;
my $pdf_file = shift;

my ($width, $height);

open DOT, "dot -Tps2 $dot_file |"
   or die "dot is not available on this machine\n";
open PS2PDF, "| ps2pdf - $pdf_file"
   or die "ps2pdf is not available on this machine\n";

my @ps2lines = <DOT>;
close DOT;

foreach (@ps2lines) {
   if (/CropBox \[\d+ \d+ (\d+) (\d+)\]/) {
      ($width, $height) = ($1, $2);
      last;
   }
}

foreach (@ps2lines) {
   if (s/PageSize \[\d+ \d+\]/PageSize [$width $height]/) {
      last;
   }
}

print PS2PDF @ps2lines;
close PS2PDF;
