#!/usr/bin/perl

# Temporary fix:  Trick10 will not currently handle $JEOD_HOME.
# So, $JEOD_HOME is passed into this converter as the second argument (ARGV[1]),
# and expanded in the include statement to provide the full path. See FIXME
# below.


$infile = @ARGV[0] ;
open FID , "<$infile" or die ;
@in = <FID> ;
close FID ;



@this_object = () ;
$this_object[0] = 0 ;
@this_sim_object = () ;
$this_sim_object[0] = 0 ;
@next_object = () ;
$next_object[0] = $#in ;
@next_sim_object = () ;
$next_sim_object[0] = $#in ;
@class_prefixi_array = () ;
%class_name = ();
%object_object_ref = () ;
@ordered_object_list = () ;
$delete_method = "false";


#identify first sim object
for( $ii = 0 ; $ii<=$#in ; $ii++ ) {
   if( $in[$ii] =~ /^\s*sim_object\s*{/ ) {
      $next_sim_object[0] = $ii;
      last ;
   }
}

#Identify end of preamble
$lastdef = -1 ;
for( $ii = 0 ; $ii <= $next_sim_object[0] ; $ii++ ) {
   if( $in[$ii] =~ /^#define/ ) {
      $lastdef = $ii ;
      $next_object[0] = $ii+1 ;
   }
}

push @out , "//===========================TRICK HEADER=====================\n// PURPOSE:\n" ;

for( $ii = 0 ; $ii <= $lastdef ; $ii++ ) {
   push @out , $in[$ii] ;
}
push @out , "\n // Include the default system classes:\n" ;
push @out , '#include "sim_objects/default_trick_sys.sm"' ;
push @out , "\n" ;
#FIXME THIS SHOULD WORK, according to the Trick documentation, but it doesn't
#$jeod_home = os.getenv("JEOD_HOME");
#$line = '#include "'.$jeod_home.'/sims/shared/Trick10/S_modules/jeod_sys.sm"' ;
$line = '#include "JEOD_S_modules/jeod_sys.sm"' ;
push @out , $line;
push @out , "\n\n" ;

$sim_object_id = 0;
while( $next_sim_object[$sim_object_id] < $#in ) {
 $sim_object_id++ ;
 $this_object[$sim_object_id] = $next_object[$sim_object_id-1] ;
 $this_sim_object[$sim_object_id] = $next_sim_object[$sim_object_id-1 ] ;
 $next_object[$sim_object_id] = $#in ;
 $next_sim_object[$sim_object_id] = $#in +1;
 
 # look for next sim_object
 for( $ii = $this_sim_object[$sim_object_id]+1 ; $ii<=$#in ; $ii++ ) {
   if( $in[$ii] =~ /^\s*sim_object\s*{/ ) {
      $next_sim_object[$sim_object_id] = $ii ;
      last;
   } #note - if none found, then $next_sim_object = $#in+1, and this is last time through.
 }
   
 #Rewind
 for( $jj = $next_sim_object[$sim_object_id]-1 ; $jj > $this_sim_object[$sim_object_id] ; $jj-- ) {
    $line = $in[$jj] ;
    if( $line =~ /^\s*}\s*[A-z]/ ) {
       $next_object[$sim_object_id] = $jj+1 ;
       $class_prefix = $line ;
       $class_prefix =~ s/^\s*}\s*([A-z]+).*\n/$1/ ;
       if( $class_prefix =~/^time$/ ) {
         $class_prefix = "jeod_time";
       }
       $class_prefix_array[$sim_object_id] = $class_prefix ;
       $class_name{$class_prefix} = $class_prefix ;
       if ($class_name{$class_prefix} =~ /([a-z])(.*)/ ) {
          $class_name{$class_prefix} = uc($1).$2."SimObject" ;
       }
       last;
    }
 }
}

# identify all object-object references
for( $id = 1 ; $id <=$#class_prefix_array ; $id++ ) {
   $class_prefix = $class_prefix_array[$id];
   # Avoid sys and jeod_sys objects.
   if ( $class_prefix !~ /sys/ ) {
     print "Evaluating $class_prefix\n";

      for( $iline = $this_sim_object[$id] ; $iline < $next_object[$id] ; $iline++) {
         $line = $in[$iline] ;
         #check on function object-homes
         # merge 2-line entries into 1-line entries
         if ( ($line =~ /.*\).*:/ ) && ($line !~ /.*\).*:\s*[a-z]+\./) ) {
            $line =~ s/\n/ / ;
            $line = $line.$in[$iline+1] ;
         }
         # set default:
         $obj_prefix = $class_prefix;
         # extract function object-home if line represents a function call
         if( $line =~ /.*\).*:\s*([a-z,_,\/]+)\s*\..*/ ) {
            $obj_prefix = $1 ;
         }
         # extract argument object-home if line is part of an argument list
         if( $line =~  /[a-z].*=[\s,\&]*([a-z]+)\..*/ ) {
            $obj_prefix = $1 ;
         }

         #Check for the "time" conflict.
         if( $obj_prefix =~/^time$/ ) {
           $obj_prefix = "jeod_time";
         }
         
         #if object is foreign
         if ($class_prefix !~ $obj_prefix) {
            #if no record for this class prefix exists, create one
            if( !exists($object_object_ref{$class_prefix})){
#               print "First foreign object in $class_prefix, $this_sim_object[$id], $next_object[$id]\n";
               $object_object_ref{$class_prefix} = [] ;
            }
#            print "Found a foreign object, $obj_prefix in $class_prefix.";
            #check for uniqueness
            $unique_entry = "true";
            foreach $entry (@{$object_object_ref{$class_prefix}}) {
               if ($entry =~ /^$obj_prefix$/) {
                   $unique_entry = "false";
                   last;
               }
            }
            #if it is a unique foreign object, record it.
            if( $unique_entry =~ /true/ ) {
               print "Found a foreign object, $obj_prefix.\n";
#               print "  First instance \n";
               push( @{$object_object_ref{$class_prefix}} , $obj_prefix) ;
            }
            else {
#               print "  repeated.  Ignoring\n";
            }
         }
      }
   }
}

# Order objects to take care of object-object-dependencies
@ordered_object_list_test_array = ();
$ordered_object_list_test_array[0] = "true";

for( $id = 1 ; $id <=$#class_prefix_array ; $id++ ) {
  push @ordered_object_list_test_array , "false";
}

$cycle_permitted = "true";
$cycle_needed = "true";
print "\nOrdering sim objects:\n\n";
while( ($cycle_needed =~/true/) && ($cycle_permitted =~/true/) ) {
  print "Cycling\n";
  $cycle_permitted = "false";
  $cycle_needed = "false";

  
  # Cycle through all objects
  for( $id = 1 ; $id <=$#class_prefix_array ; $id++ ) {
    # If the object is not already in the list:
    if( $ordered_object_list_test_array[$id] =~ /false/ ) {
       $class_prefix = $class_prefix_array[$id];
       print "Testing class $id, $class_prefix \n";
       # Avoid sys and jeod_sys objects.
       if ( $class_prefix =~ /sys/ ) {
         print "Ignoring system class, $class_prefix \n";
         #Avoid testing this one again
         $ordered_object_list_test_array[$id] = "true";
       }
       else {
         # If there are no dependencies, add this class to the list at the first
         # available slot.
         if( !exists($object_object_ref{$class_prefix})){
           push @ordered_object_list , $id;
           $ordered_object_list_test_array[$id] = "true";
           $cycle_permitted = "true";
           print "No dependencies, OK to process class $class_prefix \n";
         }
         else {
           # Assume that there are no dependencies not already in the list.
           $unprocessed_dependencies = "false";
           # for each dependency:
           foreach $foreigner (@{$object_object_ref{$class_prefix}}) {
             # Assume it is not in the list (seems counter to first assumption, but it
             # will make sense).
             $in_list = "false";
             # test each entry in the ordered list for comparison with the dependency.
             # If it matches, the dependency is already on the list and will be
             # processed first.
             foreach $list_entry (@ordered_object_list) {
                if( $class_prefix_array[$list_entry] =~ /^$foreigner$/ ) {
                  $in_list = "true";
                }
             }
             # If the dependency was not on the list, do not add it to the list.
             if ($in_list =~/false/ ) {
               $unprocessed_dependencies = "true";
             }
           }
           # Now, having processed all dependencies, if $unprocessed_dependencies has
           # NOT been reset, can add this one to the list and set the cycle_again flag
           # (this allows the search to go again if needed).  Otherwise set the
           # cycle_needed flag to indicate that the algorithm needs to cycle again to
           # try to place this one.
           if( $unprocessed_dependencies =~/false/ ) {
             push @ordered_object_list , $id;
             $ordered_object_list_test_array[$id] = "true";
             $cycle_permitted = "true";
             print "Dependencies accounted for, OK to process class $class_prefix \n";
           }
           else {
             print "Dependencies not accounted for, will revisit class $class_prefix \n";
             $cycle_needed = "true";
           }
         } # end of all un-entered non-sys object
       } # end of all un-entered objects
     } # end of all objectsa
   } # end of for-loop

   if( ($cycle_needed =~ /true/) && ( $cycle_permitted =~/false/ ) ) {
     print "**************\nWARNING: Circular dependencies.  Will need to sort by hand\n****************\n";
     # Go ahead and add the rest anyway.
     for( $id = 1 ; $id <=$#class_prefix_array ; $id++ ) {
       # If the object is not already in the list:
       if( $ordered_object_list_test_array[$id] =~ /false/ ) {         
         push @ordered_object_list , $id;
         print "Class $class_prefix_array[$id] has the following dependencies:\n";
         foreach $foreigner (@{$object_object_ref{$class_prefix_array[$id]}}) {
           print "  $foreigner \n";
         }
       }
     }
   }
} #end while loop.
     

print "\nProcessing sim objects.\n\n";
# Process each object, one at a time according to the order just determined. 
for( $id_order = 0 ; $id_order <=$#ordered_object_list ; $id_order++ ) {
 $integrator_include = "false";
 $integ_name = "QQQQQQ";
 $id = $ordered_object_list[$id_order];
 $class_prefix = $class_prefix_array[$id];
 # Avoid sys and jeod_sys objects.
 if ( $class_prefix !~ /sys/ ) {
   print "processing class $class_prefix\n";
    
#   $class_name =~ s/([a-z])/uc($1)/ ;
   push @out , "//***************************************************************\n// CLASS: ($class_name{$class_prefix}) \n// PURPOSE: \n" ;  

   #find first text line in header
   $first_head_line = $this_sim_object[$id] ;
   for( $ii = $this_object[$id]+1 ; $ii < $this_sim_object[$id]-1 ; $ii++ ){
      $line = $in[$ii] ;
      if( ($line =~ /[A-z]/ ) && ($line !~ /SIM_OBJECT/ ) ) {
         $first_head_line = $ii ;
         last;
      }
   }
   #find last text line in header
   $last_head_line = $this_object[$id] ;
   for( $ii = $this_sim_object[$id]-1 ; $ii > $this_object[$id]+1 ; $ii-- ) {
      $line = $in[$ii] ;
      if( ($line =~ /[A-z]/ ) && ($line !~ /SIM_OBJECT/ ) ){
         $last_head_line = $ii ;
      }
   }

   #output header
   if( $first_head_line < $last_head_line ) {
      $line = $in[$first_head_line] ;
      $line =~ s/\/\///;
      push @out, "//  (".$line ;
      for( $ii = $first_head_line+1 ; $ii <= $last_head_line-1 ; $ii++ ) {
         push @out , $in[$ii] ;
      }
      $line = $in[$last_head_line] ;
      $line =~ s/$/)/;
      push @out , $line ;
   }
   else {
      if( $first_head_line == $last_head_line ) {
         $line = $in[$first_head_line] ;
         $line =~ s/\/\///;
         $line =~ s/$/)/;
         push @out, "//  (".$line ;
      }
   }
   push @out , "//***************************************************************\n\n" ;

   # find the data structures and the default data
   @store = () ;
   @store_dd = () ;
   $num_store = -1 ;
   $num_store_dd = -1;
   for( $ii = $this_sim_object[$id]+1 ; $ii<=$next_object[$id] ; $ii++ ) {
      if( $in[$ii] =~ /^\s*[a-z,A-Z,0-9,\/,_]+\s*:/ ) {
         $num_store++ ;
         $store[$num_store] = $ii;
      }
      else {
         if( $in[$ii] =~ /\(.*\.d/ ) {
            $num_store_dd++ ;
            $store_dd[$num_store_dd] = $ii ;
         }
      }
   }



   # generate the ##include lines from data structures
   if( $num_store >= 0 ) {
      push @out , "\n// Include headers for classes that this class contains:\n" ;
      for( $ii = 0 ; $ii <= $#store ; $ii++ ) {
         $line = $in[$store[$ii]] ;
         # check for split lines
         if( ( $line  =~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:/ ) && ( $line  !~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:\s*(\S+).*/ ) ) {
            $line =~ s/\n/ /;
            $line = $line.$in[$store[$ii]+1];
         }
         if ( $line  =~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:\s*(\S+).*/ ) {
            $address = $1;
            $class_type = $2;
            while ( $class_type  =~ /(.*)([a-z,0-9])([A-Z]+)(.*)/ ) {
               $class_type = $1.$2."_".lc($3).$4 ;
            }
            while ( $class_type  =~ /(.*)([A-Z]+)(.*)/ ) {
               $class_type = $1.lc($2).$3 ;
            }
            $line = '##include "'.$address.'/include/'.$class_type.'.hh"' ;
         }
         push @out , $line."\n" ;
      }
   }

   #generate the ##include lines from the default data
   if( $num_store_dd >= 0 ) {
      push @out , "\n// Include default data classes:\n" ;
      for( $ii = 0 ; $ii <= $#store_dd ; $ii++ ) {
         $line = $in[$store_dd[$ii]] ;
         if ( $line  =~ /^\s*\((.*)\/(.*)\.d/ ) {
            $line = '##include "'.$1.'/include/'.$2.'.hh"' ;
         }
         push @out , $line."\n" ;
      }
   }


   
   #start the class
   push @out , "\nclass $class_name{$class_prefix}: public Trick::SimObject {\n\n   public:\n" ;


   #generate the data elements of the class
   if( $num_store >= 0 ) {
      for( $ii = 0 ; $ii <= $#store ; $ii++ ) {
         $line = $in[$store[$ii]] ;
         # check for split lines
         if( ( $line  =~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:/ ) && ( $line  !~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:\s*(\S+).*/ ) ) {
            $line =~ s/\n/ /;
            $line = $line.$in[$store[$ii]+1];
         }
         if( $line  =~ /^\s*([a-z,A-Z,0-9,\/,_]+)\s*:\s*(\S+).*/ ) {
            # strip the addressing off the data structures:
            $line =~ s/^\s*[a-z,A-Z,0-9,\/,_]*:\s+(.*)/    $1/ ;
            # And add a newline character
            if( $line !~ /;\s*$/ ) {
               $line =~ s/\s*$/;\n/ ;
            }
            if( $line =~ /INTEGRATOR\s*(\S+);/ ) {
              $integ_name = $1;
              push @out , "\n    Trick::Integrator     * integ_ptr;\n";
              $integrator_include = "true";
            }
            else {
              push @out , $line ;
            }
         }
      }
   }
   push @out , "\n// Instances for matching to other sim objects: \n";

   #generate the data elements associated with links to other sim objects:
   foreach $foreign_object (@{$object_object_ref{$class_prefix}}) {
      push @out , "    $class_name{$foreign_object} * internal_$foreign_object;\n"
   }

   #generate the data elements associated with default data:
   if( $num_store_dd >= 0 ) {
      push @out , "\n// Instances for default data:\n";
      for( $ii = 0 ; $ii <= $#store_dd ; $ii++ ) {
         $line = $in[$store_dd[$ii]] ;
         $dd_instance = $in[$store_dd[$ii]-1] ;
         # grab the second entry (the class name) and third entry (the instance
         # name)
         if ($dd_instance =~ /^\s*\S*\s*(\S*)\s*(\S*)/ ) {
            $dd_class = $1 ;
            $dd_instance = $2 ;
         }
         #read the filename 
         if ( $line  =~ /^\s*\(.*\/(.*)\.d/ ) {
            $line = $dd_class."_".$1."_default_data    ".$dd_instance."_default_data;" ;
            push @out , "    $line\n" ;
         }
      }
   }


   #start the constructor
   $constructor_args = "" ;
   $n_f_obj = 0;
   #compile the arguments for the constructor, based on object_object_ref
   foreach $foreign_object (@{$object_object_ref{$class_prefix}}) {
      $n_f_obj++;
      if( $n_f_obj > 1 ) {
         $constructor_args = $constructor_args."," ;
      }
      $constructor_args = $constructor_args."\n           ".$class_name{$foreign_object}." & ext_".$foreign_object ;
   }

   push @out , "\n//Constructor\n    $class_name{$class_prefix}($constructor_args) {\n\n" ;

   # assign the internal variables to the values passed into the constructor.
   foreach $foreign_object (@{$object_object_ref{$class_prefix}}) {
      push @out , "    internal_$foreign_object = & ext_$foreign_object;\n"
   }

   #set the integ_ptr if necessary
   if( $integrator_include =~ /true/ ) {
     push @out , "    integ_ptr = Trick::getIntegrator( Runge_Kutta_4, 12, DYNAMICS);\n";
   }

   # generate the default data calls
   if( $num_store_dd >= 0 ) {
      push @out , "   //\n   //Default data jobs\n   //\n";
      for( $ii = 0 ; $ii <= $#store_dd ; $ii++ ) {
         $dd_instance = $in[$store_dd[$ii]-1] ;
         #grab the third entry
         if ($dd_instance =~ /^\s*\S*\s*\S*\s*(\S*)/ ) {
            $dd_instance = $1 ;

            $line = '      ("default_data") '.$dd_instance."_default_data.initialize ( &".$dd_instance." );" ;
            push @out , $line."\n" ;

         }
      }
      push @out , "\n" ;
   }

   #generate the rest of the calls
   for( $ii = $this_sim_object[$id]+1 ; $ii<$next_object[$id]-1 ; $ii++ ) {
      $line = $in[$ii] ;
      # Push all comments directly.
      if( ( $line =~ /^\s*\/\// ) && ($line !~ /Data struc/) ) {
         push @out, $line ;
         next;
      }

      # if line contains close-paren, comma, or single equals sign and is not a
      # data file (data files end with "name.d)", "name.d ", or "name.dd")
      if( ( $line =~ /[\),\,^==^=]/ ) && ( $line !~ /^\s*\(.*\.d[d,\s,\)]/ ) ) {
        # Enclose all trick job-classes in quotes - these are lines that have
        # text enclosed in parentheses.  
           # First get the non-scheduled classes
           # extract the contents in the parentheses, and reinsert
           # enclosed in quotes.
         if( $line =~ /^(.*)\(\s*([a-z]+)\s*\)(.*)/ ) {
           $line = $1.' ("'.$2.'") '.$3;
         }
           # Now get the scheduled classes
           # extract the contents after the comma in the parentheses, and reinsert
           # enclosed in quotes.
         if( $line =~ /^(.*),\s*([a-z]+)\s*\)(.*)/ ) {
           $line = $1.', "'.$2.'") '.$3;
         }
         
       # Handle the function calls first:
         # merge 2-line function calls entries into 1-line entries
         if ( ($line =~ /^\s*(.*\))\s*[a-z,A-Z,0-9,_,\/]+\s*:/ ) && ($line !~
         /^\s*(.*\))\s*[a-z,A-Z,0-9,_,\/]+\s*:\s*[a-z]+\./) ) {
            $line =~ s/\n/ / ;
            $line = $line.$in[$ii+1] ;
         }

         # divide function call lines into 3 components:
         if( $line =~ /^\s*(.*\))\s*[a-z,A-Z,0-9,_,\/]+\s*:\s*([a-z,_]+)\.(.*\(.*)/ ) {
            $preamble = $1;
            $obj_name = $2;
            $function_call = $3;

            #Check for the "time" conflict.
            if( $obj_prefix =~/^time$/ ) {
              $obj_prefix = "jeod_time";
            }
         
            if ($class_prefix =~ $obj_name ) {
               $obj_name = "";
            }
            else {
               $obj_name = "internal_".$obj_name."->";
            }
            $line = "      $preamble $obj_name$function_call \n";
            if( $preamble =~ /0\.0\s*\,/) {
               $delete_method = "true";
               $line = "//$line";
            }
              

#i Add the extra stuff to the integration class jobs:
            $line =~ s/"integration"\)/"integration", DUMMY_INTEGRATION_POINTER) trick_ret =/ ;
            push @out , $line;
            next;
         }

       # Now handle the function arguments:
         if ($delete_method =~ /true/ ) {
            push @out , "//$line";
            if( $line =~/;\s*$/ ) {
               $delete_method = "false";
            }
            next;
         }

         # merge 2-line argument entries into 1-line entries
         if( ($line =~  /=/ ) && ($line !~ /[\,\;]\s*$/ ) ) {
            $line =~ s/\n/ / ;
            $line = $line.$in[$ii+1] ;
         }

         # divide function arguments into 2 components:
         if( $line =~  /=\s*(\S+.*)/ ) {
           $argument = $1;
           if( $argument =~ /^[0-9]/ ) {
             $obj_name = "";
           }
           elsif( $argument =~ /^(.*?)\.(.*)/ ) {
             $obj_name = $1;
             $argument = $2;
             
           }
           else {
             $obj_name = "";
           }
            # make a note of whether passing an an address or reference
            if( $obj_name =~ /&/ ) {
               $addresser = "&";
               $obj_name =~ s/[&, ]//g;
            }
            else {
               $addresser = "";
            }

            #Check for the "time" conflict.
            if( $obj_name =~/^time$/ ) {
              $obj_name = "jeod_time";
            }
         
            # check for integrator pass
            if( ( $argument =~ /^$integ_name/ ) && ($addresser =~ /&/ ) ) {
               $addresser = "";
               $argument =~ s/$integ_name/integ_ptr/;
            }

            if( ($class_prefix =~ $obj_name ) || ($obj_name =~ /^$/) ) {
               push @out , "          $addresser$argument\n";
               next;
            }
            else {
               push @out , "          $addresser internal_$obj_name->$argument\n";
               next;
            }
         }
         #Grab any exec_ function calls as arguments
         if( $line =~  /.*=\s*(exec_.*)/) {
            $argument = $1;
            push @out , "          $argument\n";
            next;
         }
         #Grab any numericals used as arguments
         if( $line =~  /.*=\s*([0-9].*)/) {
            $argument = $1;
            push @out , "          $argument\n";
            next;
         }



      }
   }
   # Add the copy constructor and operator = definition (or lack thereof)
   push @out , "    }\n\n  private:\n    $class_name{$class_prefix} (const $class_name{$class_prefix}&);\n    $class_name{$class_prefix} & operator = (const $class_name{$class_prefix}&);\n\n};";

   # Instantiate the class
   $instantiation_args = "" ;
   $n_f_obj = 0;
   #compile the arguments for the constructor, based on object_object_ref
   foreach $foreign_object (@{$object_object_ref{$class_prefix}}) {
      $n_f_obj++;
      if( $n_f_obj > 1 ) {
         $instantiation_args = $instantiation_args."," ;
      }
      $instantiation_args = $instantiation_args." ".$foreign_object ;
   }
   if( $instantiation_args =~ /[a-z]/ ) {
     $instantiation_args = "($instantiation_args)";
   }
   push @out , "\n$class_name{$class_prefix} $class_prefix $instantiation_args;\n\n\n" ;
 }
}
#Add the collect mechanism
#Positioning of collect statements is not uniform across simulations; some
#simulations have them distributed, others collected at the end.  So, it is
#necessary to scan the entire file again.  This could be done in each sim
#object, but then the inconsistency would remain.  Instead, I'm collecting all
#collects at the end of the file., regardless of where they appear in the
#original S_define.

for( $ii = $lasdef+1 ; $ii <= $#in ; $ii++ ) {
   $line = $in[$ii];
   if( $line =~ /^\s*vcollect/ ) {
      push @out , $line;
      #if collect spans multiple lines, output them all
      if( $line !~ /\}/) {        
         $jj = 1;
         $line = $in[$ii + $jj];
         #output all lines that do not end the vcollect
         while ($line !~ /\}/ ) {
            push @out , $line;
            $jj++;
            $line = $in[$ii + $jj];
         }
         #output the line that DOES end the vcollect
         push @out , $line;
      }
   }
}


#ALTERNATIVELY, 
#Find the instances of vcollect, 
#read the first class_prefix after vcollect (e.g.  sv_dyn)
#(On the first hit, add some comment line about going to collects, and ##include
#    the SVCollectSimObject class header file
#For the first hit on each class_prefix (or should this be on the first hit
#    only?), instantiate a SVCollectSimObject:
#    SVCollectSimObject <class_prefix>_collect( <class_prefix> );
#Output current line, and subsequent lines until $line =~ /}/, output this line
#    and return to search for next vcollect, until reach end of file.




# Add the integrate statement
for( $ii = $next_object[$#next_object] ; $ii <= $#in ; $ii++ ) {
   $line = $in[$ii];
   if( $line =~ /integrate/ ) {
      $line =~ s/integrate/IntegLoop sim_integ_loop /;
      push @out , "\n $line \n";
   }
}

#Is this necessary?
#Add the create_connections tool
#FIXME



#One of a kind fixes
$integ_name = "QQQQQ";
for( $ii = 1 ; $ii<=$#in ; $ii++ ) {
   $out[$ii] =~ s/de4xx_ephemeris\.hh/de4xx_ephem.hh/;
   $out[$ii] =~ s/rnpj2000\.hh/rnp_j2000.hh/;
   $out[$ii] =~ s/metatmosphere\.hh/MET_atmosphere.hh/;
   $out[$ii] =~ s/simple6_dof_dyn_body\.hh/simple_6dof_dyn_body.hh/;
   $out[$ii] =~ s/atmosphere_state\.hh/atmosphere.hh/;
   $out[$ii] =~ s/aerodynamic_drag\.hh/aero_drag.hh/;
   $out[$ii] =~ s/.*sim_services.*integrator\.hh"//;
   $out[$ii] =~ s/Idynamics//;

   $out[$ii] =~ s/DUMMY_INTEGRATION_POINTER/&integ_ptr/
}




$outfile = "S_define" ;
open FID , ">$outfile" or die ;
print FID @out ;



