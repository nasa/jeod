#!/usr/bin/perl

sub process_line {
#Comments
#  if( (/^[\/,\s]*\*\/*(.*)/) || (/^\/\/(.*)/ ) ) {
#     return "# $1";
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
  return "$_";
}




#***************************************************************************************
#***************************************************************************************
#***************************************************************************************




$infile = shift @ARGV ;
$directory = $infile ;
$directory =~ s/input$// ;
open FID , "<$infile" or die ;
@in = <FID> ;
close FID ;
print "processing $directory/input\n";
@fail = ();
@out = ();

#***********************************PROCESS HEADER *****************************
$header_exists = "false";
foreach $line (@in) {
  if( $line =~ /PURPOSE:/ ) {
    $header_exists = "true";
    last;
  }
}

if( $header_exists =~ /false/ ) {
  push @out , "################TRICK HEADER#######################################\n#PURPOSE:\n#  (To define the input data for simulation $directory \n####################################################################################\n\n" ;
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

  while( ($line_num <= $#in) && ($line !~ /\*\//) ) {
    push @out , "#$line";
    $line_num++;
    $line = $in[$line_num];
  }
  push @out , "#$line\n";
}

#********************************END OF HEADER**********************************

#******************************************PRE-PROCESS FILE*******************
$content_starts = $line_num + 1;

# Comment comments, and join incomplete lines that are not comments
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
    #skip  blank lines, #s
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

#********************************END OF PRE_PROCESS*****************************

push @out , "\ntrick.sim_services.exec_set_trap_sigfpe(1)\n";

push @out , "\n#############  NUMERICAL DEFAULT DATA  #################\n";
@pound_def_list = ();
%pound_def_replacement = ();
# Record the #defines:
foreach( @content ) {
  if( /#define\s+(\S+)\s+(\S+)/) {
    $def_var = $1;
    $def_val = $2;
    # If it is a number, record it.
    if( $def_val =~ /^[0-9,\.]+$/ ) {
      push @out , "$def_var = $def_val\n";
    }
    #otherwise, store them for later editing
    else {
      push @pound_def_list , $def_var;
      $pound_def_replacement{$def_var} = $def_val;
    }
  }
}

# Log data includes
push @out , "\n#############  LOG DATA  #################\n";
foreach( @content ) {
  if( /include.*Log_data\/(.*)\.d"/ ) {
     $log_filename = $1;
     
     # search for the intended log cycle:
     $data_file = "Log_data/$1.d";
     open FID , "<$data_file" or die ;
     @log_data = <FID> ;
     close FID ;
     foreach $data_line ( @log_data ) {
       if ($data_line =~ /sys\.exec\.record\..*\.cycle\s*=\s*(\S+);/ ) {
         $data_log_cycle = $1;
         last;
       }
     }
     # search for the argument list - Note - this assumes that the Log data
     # file has already been processed:
     $data_file = "Log_data/log_$log_filename.py";
     open FID , "<$data_file" or die ;
     @log_data = <FID> ;
     close FID ;
#     print "Grabbing argument list for log_$log_filename.\n";
     foreach $data_line( @log_data ) {
       if ($data_line =~ /def\s*log_$log_filename\s*\((.*)\)\s*:/ ) {
         $argument_list = $1;
         last;
       }
     }
     $argument_list =~ s/log_cycle/$data_log_cycle/;

     $line = 'execfile( "Log_data/log_'.$log_filename.'.py")'."\nlog_$log_filename($argument_list)\n";
     push @out , $line;
     next;
  }
}



# Assignments:
$in_read_block = "false";
push @out , "\n#############  ASSIGNMENTS, CALLS, and CONTROL  #################\n";
foreach( @content ) {
  s/\;//;
# Modified Data includes
  if( !/\/\//) {
  if( /include.*Modified_data\/(.*)\.d"/ ) {
     $address_name = $1;
     $name = "set_$1";
     $name =~ s/\//_/g;

     # search for the argument list - Note - this assumes that the Modified data
     # file has already been processed:
     $data_file = "Modified_data/$address_name.py";
     open FID , "<$data_file" or die "Could not open $data_file";
     @mod_data = <FID> ;
     close FID ;

     foreach (@mod_data) {
       if ( /def/ ) {
       }
       if ( /def\s*$name\s*\((.*)\)\s*:/ ) {
         $argument_list = $1;
         last;
       }
     }
     $line = 'execfile( "Modified_data/'.$address_name.'.py")'."\n$name($argument_list)\n";
     push @out , $line;
     next;
   }
  }
  if( /^\s*call/ ) {
    s/call\s*//;
    s/[A-z0-9]+?\.//;

    push @out , "#CALL\n$_\n";
    next;
  }

#reads
  if( /^\s*read\s*=\s*(.*);/ ) {
    $line = "trick_ip.ip.add_read($1,  ".'"""'."\n";
    if( $in_read_block =~ /true/ ) {
      push @out , '""")'."\n\n";
    }
    push @out , "\n$line";
    $in_read_block = "true";
    next;
  }
# terminate
  if( /stop\s*=\s*([0-9]+)/ ) {
    $line = "\ntrick.sim_services.exec_set_terminate_time($1)";
    if( $in_read_block =~ /true/ ) {
      push @out , '""")'."\n\n";
    }
     push @out , $line;
     next;
  }
# General processing
  $output = process_line($_);
  push @out , $output;
}



#Edit the whole file to eliminate the pound defs
foreach $def_var ( @pound_def_list ) {
print $def_var;
  $def_val = $pound_def_replacement{$def_var};
#  print "Subbing $def_val for $def_var\n"; 
  foreach $line ( @out ) {
    # change the  #(VALUE) to value_replace
    if( $line =~ /\#\($def_var\)/ ) {
       $line =~ s/\#\($def_var\)/$def_val/g;
    }
    # change the arguments to the log and set functions that match VALUE.  Pass
    # these in as strings.
    if( ($line =~ /log_.*\(.*$def_var.*\)/) || ($line =~ /set_.*\(.*$def_var.*\)/) ){  
      $line =~ s/$def_var/"$def_val"/g;
    }
    # change the arguments to the log and set functions that require a reference
    # to the object associated with VALUE.
    $def_var_ref = lc($def_var)."_reference";
    if( ($line =~ /log_.*\(.*$def_var_ref.*\)/) || ($line =~ /set_.*\(.*$def_var_ref.*\)/) ){  
      $line =~ s/$def_var_ref/$def_val/g;
    }
    while( $line =~ /[\s\.\(]*$def_var[\s\.\)\;]/ ) {
         $line =~ s/$def_var/$def_val/ ;
    }
  }
}

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
    
  
$outfile = $directory."input.py" ;
open FID , ">$outfile" or die ;
print FID @out ;
close FID;

$failfile = "translation_fails";
open FAILID , ">>$failfile" or die;
foreach (@fail) {
format FAILID = 
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<: @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  
$infile, $_ 
.
write FAILID;
}
close FAILID;
