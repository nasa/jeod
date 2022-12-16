use strict;
sub process_file($);
sub disterr($);
sub scale($);
my $pre = shift;
my $r = 6378e3 + 300;
my %by_type = ();

my @type_names = (
   'Euler',
   'Sym. Euler',
   'RK2',
   'RK4',
   'ABM4',
   'Beeman',
   'Midpoint');

my %type_info = (
   'Euler'      => 'Euler & 0 & 1 & 1 & $O(\Delta t)$ &',
   'Sym. Euler' => 'Symplectic Euler & 0 & 1 & 1 & $O(\Delta t)$ &',
   'RK2'        => 'RK2 & 0 & 2 & 2 & $O(\Delta t^2)$ &',
   'RK4'        => 'RK4 & 0 & 4 & 4 & $O(\Delta t^4)$ &',
   'ABM4'       => 'ABM4 & 4 & 2 & 2 & $O(\Delta t^4)$ &',
   'Beeman'     => 'Beeman\'s Algorithm & 1 & 2 & 2 & $O(\Delta t^2)$ &',
   'Midpoint'   => 'Midpoint & 0 & 2 & 2 & $O(\Delta t^2)$ &');


my @files = (<${pre}_[0-9]*.dat>);
@files = sort {my ($na, $nb);
               $a  =~ /${pre}_(\d+)\.dat/;
               $na = $1+0; 
               $b  =~ /${pre}_(\d+)\.dat/;
               $nb = $1+0; 
               $na <=> $nb} @files;
process_file $_ foreach (@files);

foreach my $type (@type_names) {
   my @data = @{$by_type{$type}};
   my ($scale, $units) = scale $data[$#data]->[1];
   print $type_info{$type};
   foreach my $ent (@data) {
      my $val = 0 + sprintf "%.0e", ($ent->[1] * $scale);
      print " \& $val $units";
   }
   print "\n";

   open OUT, '>', "${pre}_leo/plot_${type}.dat" or die;
   print OUT "# $type\n";
   foreach my $ent (@data) {
      print OUT "$ent->[0] $ent->[1]\n";
   }
   close OUT;
}

sub process_file ($) {
   my ($f) = @_;
   my $mode = 0;
   my (@opt, @vals);
   print "\n$f\n";
   $f  =~ /${pre}_(\d+)\.dat/ or die;
   my $nrevs = $1;
   open IN, "scripts/scan_runs.pl -now $f|";
   while (<IN>) {
      chomp;
      if ($mode == 0) {
         if (/^TranslationTestOrbit/) {
            # print;
            $mode = 1;
         }
      }
      elsif ($mode == 1) {
         @opt = split /\t/;
         $mode = 2;
      }
      elsif ($mode == 2) {
         if (/^0\.0562\t/) {
            @vals = split /\t/;
            $mode = 3;
            last;
         }
      }
   }
   close IN;
   last if ($mode < 3);

   shift @opt;
   shift @vals;
###   $_ = disterr($_) foreach @vals;
###   for (my $ii = 0; $ii < scalar @opt; $ii++) {
###      print "$opt[$ii]: $vals[$ii]\n";
###   }
   for (my $ii = 0; $ii < scalar @opt; $ii++) {
      my $opt = $opt[$ii];
      my $val = $vals[$ii] * $r;
      push @{$by_type{$opt}}, [$nrevs, $val];
   }
}


sub disterr($) {
   my ($relerr) = @_;
   my $dist = $relerr * $r;
   my $units;
   $dist = 0+sprintf "%.0e", $dist;
   if ($dist > 300) {
      $dist /= 1000;
      $units = 'km';
   }
   elsif ($dist > 0.1) {
      $units = 'm';
   }
   else {
      $dist *= 1000;
      $units = 'mm';
   }
   return "$dist $units";
}


sub scale($) {
   my ($dist) = @_;
   my ($scale, $units);
   if ($dist > 300) {
      $scale = 1e-3;
      $units = 'km';
   }
   elsif ($dist > 0.1) {
      $scale = 1;
      $units = 'm';
   }
   else {
      $scale = 1e3;
      $units = 'mm';
   }
   return ($scale, $units);
}
