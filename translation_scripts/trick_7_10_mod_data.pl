#!/usr/bin/perl


sub process_line {
#Comments
#  if( (/^[\/,\s]*\*\/*(.*)/) || (/^\/\/(.*)/ ) ) {
#     return "# $1\n";
#  }
# Blank lines
  if( /^\s*$/ ) {
    return "\n";
  }
#Lines to be fixed:
  if( /^\#FIXME/ ) {
    return "$_";
    next;
  }
# Enumeration assignments:
  if( /\=\s*(\S+)\:\:/ ) {
    $Enum = $1;
    s/$Enum\:\:/trick.$Enum./;
  }
# euler sequence
  if( /euler_sequence\s*=\s*(.*)/ ) {
#    print "$_\n";
    $es = $1;
    s/$es/trick.Orientation.$es/;
#    print "$_\n";
  }
#Everything else
  s/^\s*//;
  return "  $_";
}



$infile = shift @ARGV ;
open FID , "<$infile" or die ;
@in = <FID> ;
close FID ;

$set_name = $infile;
$set_name =~ s/^\.\///;
$set_name =~ s/\.d$//;
$set_name =~ s/\//_/g;
$set_name = "set_$set_name";
print "processing $infile\n";

@object_prefix_list = ();
@fail = ();




#***********************************PROCESS HEADER *****************************
$header_exists = "false";
foreach $line (@in) {
  if( $line =~ /PURPOSE:/ ) {
    $header_exists = "true";
    last;
  }
}

if( $header_exists =~ /false/ ) {
  push @out , "################TRICK HEADER#######################################\n#PURPOSE:\n#  (To define the method $set_name \n####################################################################################\n\n" ;
}
$line_num = 0;
$line = $in[$line_num];
# Bypass any blank lines
while( $line =~ /^\s*$/ ) {
  $line_num++;
  $line = $in[$line_num];
}
# If file starts with a comment, or there is a recognized Trick Header
if( $line =~ /^\s*\/[\/\*]/ ) {
  push @out , "#Auto-generated from the data file with this header:\n";
  if( $line =~ /^\s*\/\*/ ) {
    while( ($line_num <= $#in) && ($line !~ /\*\//) ) {
      push @out , "#$line";
      $line_num++;
      $line = $in[$line_num];
    }
    $line_num++;
  }
  push @out , "#$line\n";
}
#********************************END OF HEADER**********************************



#******************************************PRE-PROCESS FILE*******************
$content_starts = $line_num;

# Join lines that are not comments
$in_comment = "false";
for( $ii = $content_starts ; $ii <= $#in ; $ii++ ) {
  $line = $in[$ii];
  $jj = $ii;
  if( $in_comment =~ /false/ ) {
    if( $line =~ /^\s*\/[\/\*]/) {
      $in[$ii] =~s/^/#/;
      if( ($line =~ /^\s*\/\*/) && ( $line !~ /\*\//) ) {
        $in_comment = "true";
      }
      next;
    }
    #skip blank lines, #s, 
    if( ($line =~ /^\s*$/) || ($line =~/^\s*\#/) ) {
      next;
    }
    # as long as this line does not contain a semi-colon and is not the last
    # line
    while( ($line !~ /\;/) && ($jj < $#in) ) {
      $jj = $jj +1;
      $next_line = $in[$jj];
      chomp($line);
      $line = "$line $next_line\n";
      $in[$ii] = $line;
      $in[$jj] = "";
    }
  }
  else {
    $in[$ii] =~s/^/#/;
    if( $line =~ /\*\// ) {
      $in_comment = "false";
      next;
    }
  }
}
@content = @in[$content_starts..$#in];

# Identify known problems
# change all the following:
#  time" to "jeod_time" 
#  integrator definitions:
#  Yes and No, true and false:
foreach (@content) {

    s/integ.option\s*=\s*/manager_init.sim_integ_opt = trick.sim_services./;
    if( /\.integ\./ ) {
      push @fail, $_;
      s/^/#FIXME:  /;
      next;
    }
    if( /\=\s*alloc/ ) {
      push @fail, $_;
      s/^/#FIXME:  /;
      s/alloc\s*\((.*)\)/trick.sim_services.alloc_type( \1 , "FIXME_TYPE" )/;
      next;
    }
    if( /^\s*sys\./ ) {
      push @fail , $_;
      s/^/#FIXME:  /;
      next;
    }
    s/^\s*time/jeod_time/;
    s/\=\s*Yes/= 1/;
    s/\=\s*true/= True/;
    s/\=\s*On/= True/;
    s/\=\s*No/= 0/;
    s/\=\s*false/= False/;
    s/\=\s*Off/= False/;

    # make all pointer allocations go to the object instead of the address.
    s/\=\s*\&/= /;

    # Edit all 1-d array assignments to new format.
    if( /(.*)\[0\](.*)\=(.*);/ ) {
      $pre_assgt = $1;
      $post_assgt = $2;
      $post_equ = $3;
      # If goes straight to units (or purely space):
      if( ($post_assgt =~ /^\s*\{/) || ($post_assgt =~ /^\s*$/) ) {
        # If multiple assigned values:
        if( $post_equ =~ /,/ ) {
          $pre_equ = $pre_assgt.$post_assgt;
          $post_equ = "[$post_equ]";
        }
        else {
          $pre_equ = $pre_assgt."[0]".$post_assgt;
        }
        s/.*/$pre_equ = $post_equ;/;
      }
      #Otherwise (braces in middle of variable), ignore.
    }

    #Edit unit assignments to new format
    s/(.*)\{(.*)\}\s*\=\s*(.*)\;/\1 = trick.attach_units( "\2",\3)/;
}

# search for #defined values
foreach ( @content ) {
  if( /define/ ) {
  }
  if( /\#define\s+(\S+)\s*(\S+)/ ) {
    $defined_name = $1;
    $defined_sub= $2;
    #delete the line
    s/.*/\n/;
    foreach ( @content ) {
      s/$defined_name/$defined_sub/;
    }
  }
}

#eliminate pointers
foreach ( @content ) {
  s/\-\>/\./g
}

#eliminate calls
foreach (@content) {
  if ( /^\s*call\s*([A-z]*)\.([A-z]*)\.(.*)/ ) {
    print "Call: $_";
    s/call\s*$1\.//;
  }
}

#eliminate "new"
foreach (@content) {
  if( /(\s*\S+)\s*=\s*new\s*(.*)/ ) {
    $local_var_old  = $1;
    $local_var_type = $2;
    $local_var_new = lc($local_var_old)."_local";
    s/.*/$local_var_new = trick\.$local_var_type\(\)\n  $local_var_new\.thisown = 0/;
  }
  foreach (@content) {
    if( (/$local_var_old/) && (! /\=/) ) {
      s/^/\/\//;
    }
    s/$local_var_old/$local_var_new/g;
  }
}

#********************************END OF PRE_PROCESS*****************************




# compile a list of all prefixes
foreach (@content) {
  # on front end (assignee)
  if( /^\s*([A-z][A-z0-9]*)\..*\=/ ) { 
    $object_prefix = $1;
    # if it is a pass-in variable
    if( $object_prefix =~ /^[A-Z]/ ) {
      $new_obj_pref = lc($object_prefix)."_reference";
      foreach( @content) {
        s/$object_prefix/$new_obj_pref/g;
      }
      $object_prefix = $new_obj_pref;
    }
    if( ($object_prefix !~ /^trick$/ ) && ($object_prefix !~ /_local$/) ) {
      $new_object = "true";
      foreach (@arg_list) {
        if( /^$object_prefix$/ ) {
          $new_object = "false";
          last;
        }
      }
      if( $new_object =~ /true/ ) {
        push @arg_list , $object_prefix ;
      }
    }
  }
  # on back end (assignment)
  if( /\=\s*([A-z][A-z0-9]*)\..*/) {
    $object_prefix = $1;
    # if it is a pass-in variable
    if( $object_prefix =~ /^[A-Z]/ ) {
      $new_obj_pref = lc($object_prefix)."_reference";
      s/\=\s*$object_prefix/= $new_obj_pref/;
      $object_prefix = $new_obj_pref;
    }
    if( $object_prefix !~ /^trick$/ ) {
      $new_object = "true";
      foreach (@arg_list) {
        if( /^$object_prefix$/ ) {
          $new_object = "false";
          last;
        }
      }
      if( $new_object =~ /true/ ) {
        push @arg_list , $object_prefix ;
      }
    }
  } 
}



# search for values that have been #defined externally, these need to be passed
# in.
foreach ( @content ) {
  s/\;//;
  $line = $_;
  $temp_line = $line;

  $eval_needed = "false";
  while( ($line =~ /\=/) && ($temp_line =~ /([A-Z_]{3,})/) ) {
   $new_argument = $1;
   $rep_argument = lc($new_argument);
   if( ( ($temp_line =~ /\.$new_argument/) || ($temp_line =~ /$new_argument\./)
   || ($temp_line =~ /\#\($new_argument\)/ ) ) && ( $_ !~ /"$new_argument"/) ) {
      $eval_needed = "true";
      $line =~ s/\#\($new_argument\)/$new_argument/g;
      $line =~ s/$new_argument/' + $new_argument + '/g;
      $line =~ s/\n//;
    
      #check for uniqueness
      $unique_argument = "true";
      foreach $argument ( @arg_list ) {
        if( $argument =~ /^$new_argument$/ ) {
          $unique_argument = "false";
          last;
        }
      }
      #add to argument list if necessary
      if( $unique_argument =~ /true/ ) {
        push @arg_list , $new_argument;
      }
   }
    # temporarily reassign to avoid repetition.
    $temp_line =~ s/$new_argument/$rep_argument/g;
  }
  if( $eval_needed =~ /true/ ) {
    s/.*/exec('$line')/;
  }
}

#Start the function definition:
if( $#arg_list >= 0 ) {
  $argument_list = $arg_list[0];
}
for( $ii = 1 ; $ii <= $#arg_list ; $ii++ ) {
  $argument_list = "$argument_list, $arg_list[$ii]";
}
push @out , "def $set_name($argument_list) :\n";


# Fill out the function definition:
foreach (@content) {
  $output = process_line($_);
  push @out , $output;
}

push @out , "\n  return";


# Fix the body_action address issue:
for( $ii =1 ; $ii <= $#out ; $ii++ ) {
  $line = $out[$ii];
  if( $line =~ /add_body_action\s*\((.*)\)/ ) {
    $body_action_argument = $1;
    #search backward to find the argument
    $found_b_a_r = -1;
    for( $jj = $ii-1; $jj >= 0 ; $jj-- ) {
      $test_line = $out[$jj];
      if( $test_line =~ /$body_action_argument\s*\=\s*(.*)/ ) {
        $body_action_replace = $1;
        $found_b_a_r = $jj;
        last;
      }
    }
    if( $found_b_a_r < 0 ) {
      push @fail , $out[$ii]; 
      $out[$ii] =~ s/^/#FIXME:/
    }
    else {
      $out[$ii] =~ s/$body_action_argument/$body_action_replace/;
      $out[$found_b_a_r] =~ s/.*//;
    }
  }
}








$outfile = $infile;
$outfile =~ s/\.d/\.py/ ;
open FID , ">$outfile" or die ;
print FID @out ;

$failfile = "../translation_fails";
$infile =~ s/\./Modified_data/;
open FAILID , ">>$failfile" or die;
foreach (@fail) {
format FAILID = 
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<: @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  
$infile, $_ 
.
write FAILID;
}
