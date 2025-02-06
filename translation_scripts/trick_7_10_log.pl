#!/usr/bin/perl

$infile = shift @ARGV ;
$filename_core = $infile ;
$filename_core =~ s/\.d$// ;
open FID , "<$infile" or die ;
@in = <FID> ;
close FID ;
print "processing $infile:";


# is there a trick header?
$header_exists = "false";
foreach $line (@in) {
  if( $line =~ /PURPOSE:/ ) {
    $header_exists = "true";
    last;
  }
}

if( $header_exists =~ /false/ ) {
push @out , "################TRICK HEADER#######################################\n#PURPOSE:\n#  (To define the elements to be logged in the logging of $filename_core) \n####################################################################################\n\n" ;
}
else {
  $line_num = 0;
  $line = $in[0];
  push @out , "#Auto-generated from the data file with this header:\n";

  while( ($line_num <= $#in) && ($line !~ /\*\//) ) {
    push @out , "#$line";
    $line_num++;
    $line = $in[$line_num];
  }
  push @out , "#$line\n";
}
# Finished with header


@pound_def_list = ();
$argument_list = " log_cycle ";

# search for #defined values
foreach ( @in ) {
  s/"time\./"jeod_time./;

  if( /#define\s+(\S+)\s*(\S+)/ ) {
    $defined_name = $1;
    $defined_su= $2;
    s/.*/\n/;
    foreach $line ( @in ) {
      $line =~ s/$defined_name/$defined_sub/;
    }
  }
}

# search for values that have been #defined externally, these need to be passed
# in.
foreach $line ( @in ) {
  while( $line =~ /".*\#\(([A-Z,_]+)\)/ ) {
    $new_argument = $1;

#    print "changing $1\n";
    $line =~ s/\#\(([A-Z,_]+)\)/"+$1+"/;

    #check for uniqueness
    $unique_argument = "true";
    foreach $argument ( @pound_def_list ) {
#      print "Testing $argument against $new_argument\n";
      if( $argument =~ /^$new_argument$/ ) {
        $unique_argument = "false";
        last;
      }
    }
    #add to argument list if necessary
    if( $unique_argument =~ /true/ ) {
      push @pound_def_list , $new_argument;
      $argument_list = "$argument_list, $new_argument ";
    }
  }
  if( $line =~ /^\s*sys\.exec.*name.*=(.*);/ ) {
    print "  Group Name $1\n";
    $group_name = $1;
  }
}


push @out , "def log_$filename_core ($argument_list) :\n" ;

push @out , "  recording_group_name = $group_name";
push @out , "\n" ;
push @out , "  dr_group = trick.sim_services.DRBinary(recording_group_name)\n  dr_group.thisown = 0\n  dr_group.set_cycle(log_cycle)\n  dr_group.freq = trick.sim_services.DR_Always\n" ;

foreach $line ( @in ) {

   if( $line =~ /^\s*"\S+"/) {
      if( $line =~ /^\s*("\S+\[)\s*([0-9]+)\s*\-\s*([0-9]+)\s*(\])/) {
         push @out , "  for ii in range($2,$3) :\n    dr_group.add_variable($1".'" + str(ii) + "'.']" )'."\n";
      }
      else {
         $line =~ s/^(\s*"\S+").*/  dr_group.add_variable($1)/ ;
         push @out , $line ;
      }
   }
}

push @out , "\n  data_record.drd.add_group(dr_group)\n\n  return\n" ;

$outfile = "log_$filename_core.py" ;
open FID , ">$outfile" or die ;
print FID @out ;
