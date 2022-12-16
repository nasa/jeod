#!/usr/bin/perl -w

################################################################################
#
# validate.pl
#
# Purpose:
# Compare data from the SET_test directories to corresponding
# data from the SET_test_val directories
#
# Output:
# A report on the errors from each directory.
#
# Syntax:
# validate.pl (from the sim directory)
# Assumes corresponding SET_test and SET_test_val directory trees
#
#
################################################################################


use strict;

sub max($ @) {
my ($first, @rest) = @_;
my $r = $first;

foreach (@rest) {
$r = $_ if ($_ > $r);
} # and foreach

return $r;
} # end sub

sub maxResult (@) {
my ($first, @rest) = @_;
my $r = $first;

foreach (@rest) {
$r = $_ if ($_->{'difference'} > $r->{'difference'});
} # end foreach

return $r;
} # end sub

sub compare ($$) {
my ($a, $b) = @_;
my %comparison=();
my $d = abs($a-$b);
my $scale = max(abs($a), abs($b));

if ($scale > 1.0) {
$comparison{'type'} = 'relative';
$comparison{'difference'} = abs($d)/$scale;
} # end if
else {
$comparison{'type'} = 'absolute';
$comparison{'difference'} = abs($d);
} # end else

return \%comparison;
} # end sub

sub printComparison ($) {
my $r = $_[0];
my %result=%{$r};
my @k = sort (keys(%result));

foreach(@k) {
print "$_: $result{$_}\n";
} # end foreach
} # end sub

sub splitCsv($) {
my $line = $_[0];
my @fields = split(/\s*,\s*/, $line);

return \@fields;
} # end sub

sub readFile($) {
my $cmd=($_[0] =~ /\.csv$/)? 'cat':
($ENV{TRICK_VER} =~ /^16\./)? 'trick-trk2csv': 'trk2csv';
my @lines = `$cmd $_[0]`;
my %result=();
my ($header, @data) = @lines;
my @parsedData=();

$result{'header'} = $header;
$result{'header_fields'} = splitCsv($header);

foreach(@data) {
next unless (/\S+/);

my $fields = splitCsv($_);

push @parsedData, $fields;
} # end foreach

$result{'data'} = \@parsedData;

return \%result;
} # end sub

sub print_file($) {
my $r=$_[0];
my $h = $r->{'header_fields'};
my $d = $r->{'data'};

print "Headers:\n";

foreach (@{$h}) {
print "$_\n";
} # end foreach

my $i=1;

foreach (@{$d}) {
my $l = $_;

print "Line $i:\n";
$i++;
foreach(@{$l}) {
print "$_\n";
} # end foreach
} # end foreach
} # end sub

sub compareFile($ $) {
my ($f1, $f2) = @_;
my $r1 = readFile($f1);
my $r2 = readFile($f2);
my @comparisons=();
my @headers = @{$r1->{'header_fields'}};
my @data1 = @{$r1->{'data'}};
my @data2 = @{$r2->{'data'}};

my $row = 0;
my %result=();

$f1 =~ s/.*?\///;
$f1 =~ s/\.(trk)|(csv)$//;
$result{'file_name'} = $f1;

if ($r1->{'header'} ne $r2->{'header'}) {
$result{'error'} ='Headers do not match';
return \%result;
} # end if

if ($#data1 != $#data2) {
$result{'warning'} = 'Data sizes are different.';
} # end if

my $lastData = ($#data1<$#data2)? $#data1: $#data2;

for ($row = 0; $row <= $lastData; $row++) {
my $col = 0;

foreach (@headers) {
my $comparison = compare ($data1[$row]->[$col], $data2[$row]->[$col]);

$comparison->{'data_values'} = "$data1[$row]->[$col], $data2[$row]->[$col]";
$comparison->{'field_name'} = $_;
push @comparisons, $comparison;
$col++;
} # end foreach
$row++;
} # end for $row

my $worst = maxResult(@comparisons);
my @k = keys(%{$worst});

foreach (@k) {
$result{$_} = $worst->{$_};
} # end foreach

return \%result;
} # end compareFile

################################################################################
# main program
################################################################################

my $testDir = 'SET_test';
my $valDir = 'SET_test_val';
my @testFiles = ();
my @fileComps=();

$valDir = shift @ARGV if (@ARGV);
$testDir = shift @ARGV if (@ARGV);

@testFiles = <$testDir/*/*.trk $testDir/*/*.csv>;

foreach (@testFiles) {
chomp;

my $testFile = $_;
my $valFile = $testFile;
my $runDir='';

if ($valFile =~ /^.*\/(.*)\/(.*)$/) {
($runDir, $valFile) = ($1, $2);
} # end if
else {
die "File ${testFile} didn't parse correctly."
} # end else

$valFile = "${valDir}/${runDir}/${valFile}";

unless (-e $valFile) {
print "No validation data for ${testFile}\n";
next;
} # end unless

my $r = compareFile($testFile, $valFile);

# printComparison($r);
push @fileComps, $r;
} # end foreach

my @sortedFileComps = sort {$b->{'difference'} <=> $a->{'difference'}} @fileComps;

printComparison($_) foreach(@sortedFileComps);
