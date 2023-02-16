#!/usr/bin/perl
################################################################################
#
# analyze_models.pl
# Analyze JEOD models from perspective of JEOD coding standards and ensuring
# the models are properly outfitted for use with TRICK.
# Documentation is in pod form. Type 'analyze_models.pl -man' to view.
#
################################################################################

=pod

=head1 NAME

B<analyze_models.pl> -
Analyze JEOD models from perspective of JEOD coding standards and ensuring
the models are properly outfitted for use with TRICK.


=head1 SYNOPSIS

=over 4

=item B<analyze_models.pl>
B<-help>|B<-man>

Provide brief/detailed command summary.

=item B<analyze_models.pl>

Report on all ICGed header files and compiled source files.

=item B<analyze_models.pl>
I<< <file-or-directory> >> ...

Report only on the files or models specified in the command line.

=item B<analyze_models.pl>
B<-debug> B<-verbose>

These options are of interest to the script developer only.

=back


=head1 DESCRIPTION

B<analyze_models.pl>
analyzes the header and source files for coding issues and for compatibility
with Trick. See the I<Discrepancy Report> section below for a list of the
potential issues detected by the script.

=head2 Limitations

B<analyze_models.pl> analyzes on those header files that have been ICGed
and only those source files that have been compiled. Build at least one
simulation prior to invoking B<analyze_models.pl>.

=head1 DISCREPANY REPORT

B<analyze_models.pl> reports on several problems in the header and source
files that comprise the JEOD models.
Some problems are detected simply by scanning the code.
Others require looking at the object files that implement the models.

=over 4

=item B<File contains CRLF line terminators>

The author modified the file in a Windows environment. Repair this by
running dos2unix on the reported file.

=item B<File contains tab characters>

The author left tab chanaracters in the file.

=item B<File contains overlong lines>

The file contains at least one line that exceeds 80 characters in length.

=item B<File contains lines that end with whitespace>

At least one line in the file ends with a space character.

=item B<ifdef TRICK_VER contains non-include statements>

The toplevel #ifdef TRICK_VER if block in a header file exists for one reason,
which is to ensure that the model will work in a Trick environment.
In almost all cases, this means telling Trick to process additional models
that are not specified in the S_define file. The script treats the presence of
anything but #include statements in the toplevel #ifdef TRICK_VER if block
as suspect.

=item B<#ifdef TRICK_VER block not at end of file>

The toplevel #ifdef TRICK_VER if block in a header file should normally be
placed just before the final #endif statement. The #included files within
the toplevel #ifdef TRICK_VER if block should not be needed to make the file
compilable. In other words, the toplevel #ifdef TRICK_VER should not be
essential to an user's understanding. Since this block is superfluous to
understanding and since some files need this block placed at the end of the
file, this block should almost always be placed at the end of the file for
consistency reasons.

=item B<#ifdef TRICK_VER inside #if>

Nesting the toplevel #ifdef TRICK_VER if block inside another #if block
is another sign that something fishy is occurring.

=item B<< C-style typedef <enum|struct> <name> >>

The author ported an old-style C enum or structure definition. JEOD is
supposed to be written in C++, not C.

=item B<< File contains extern <type> <variable_name>; >>

The reported line should probably be moved to a source file.

=item B<< File contains extraneous semicolon at <method_name>() >>

A semicolon after the inline definition of a member function within
a class definition is superfluous.

=item B<< File inlines constructor for <class_name> >>

The author should rethink whether the constructor should be inlined,
particularly if other classes inherit from the class in question.

=item B<< File inlines destructor for <class_name> >>

The author should rethink whether the destructor should be nlined,
particularly if other classes inherit from the class in question.

=item B<< File inlines virtual method <class_name::method_name>() >>

The author should think very strong whether the specified virtual method
should be inlined. An inlined virtual method doesn't make much sense.
The compile will always change such methods to non-inlined.

=item B<< File specifies the following unneeded library dependencies: <list> >>

The specified files do not need to be in the file's "Library
dependencies" list. Remove the specified files from the reported file's
Trick header Library dependencies" list.

=item B<< File omits the following needed library dependencies: <list> >>

The specified files need to be in the header file's "Library
dependencies" list, but are not.  Add the specified files to the reported
file's Trick header Library dependencies" list.

=item B<< File specifies the following unneeded TRICK_VER includes: <list> >>

The specified files do not need to be in the header file's toplevel
#ifdef TRICK_VER if block. Remove the #include commands for the specified files
from the reported file's toplevel #ifdef TRICK_VER if block.

=item B<< File omits the following needed TRICK_VER includes: <list> >>

The specified files need to be in the header file's toplevel #ifdef TRICK_VER
if block. Add a #include command for each specified file to the reported file's
toplevel #ifdef TRICK_VER if block.

=over


=head1 BUGS

The script dies when it gets confused. It shouldn't get confused so easily.
Report any problems to the author.


=head1 AUTHOR

David Hammen, Odyssey Space Research

=cut

################################################################################


use strict;
use warnings;
use Cwd 'abs_path';
use Getopt::Long;
 use Pod::Usage;
use lib "$ENV{TRICK_HOME}/bin/pm";
use html;


sub determine_report_files();
sub analyze_header_files();
sub change_mode($);
sub parse_class($);
sub parse_member($);
sub parse_header_file($);
sub parse_io_src($$);
sub set_external_functions();
sub analyze_source_files();
sub generate_src_report();
sub generate_hdr_report();
sub inherits_from($;$);
sub path_compare;
sub note_inclusion($$$);
sub is_included($$$);
sub unique($$);
sub report_discrepancies($$$$);



sub INIT_MODE ()    {0};
sub MAIN_BODY ()    {2};
sub CLASS_BODY ()   {3};
sub ENUM_BODY ()    {4};
sub MEMBER ()      {10};
sub IGNORE ()      {80};
sub IMMEDIATE ()   {90};
sub TRAILER ()     {99};


my @hdr_files;
my @src_files;
my %functions;
my %class_info;
my %file_info;
my @report_files;
my @src_report_files;
my @hdr_report_files;
my $debug = 0;
my $verbose = 0;
my $help = 0;
my $man = 0;


# Parse the cammand line
GetOptions (
   "debug"   => \$debug,
   "verbose" => \$verbose,
   "help"    => \$help,
   "man"    => \$man)
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
}

@report_files = @ARGV;

#The body of the script must be run with the working set to $JEOD_MODELS/home.
unless (abs_path('.') eq abs_path("$ENV{JEOD_HOME}/models")) {
   chdir "$ENV{JEOD_HOME}/models"
   or die "Can't cd to \$JEOD_HOME/models\n";
}

set_external_functions();
analyze_header_files();
analyze_source_files();

determine_report_files();
generate_hdr_report();
generate_src_report();


################################################################################


sub determine_report_files() {
   if (scalar @report_files) {
      foreach my $fname (@report_files) {
         if (-d $fname) {
            my @find;
            open FIND,
                 "find $fname -name '*.[ch][ch]' | grep -v '/verif/' | sort |"
                 or die;
            map chomp, (@find = <FIND>);
            close FIND;
            push @hdr_report_files, grep {/.hh$/} @find;
            push @src_report_files, grep {/.cc$/} @find;

         } elsif ($fname =~ /\.hh$/) {
            push @hdr_report_files, $fname;

         } elsif ($fname =~ /\.cc$/) {
            push @src_report_files, $fname;

         } else {
            warn "Ignoring $fname, which is not " .
                 "a directory, a .hh file, or a .cc file\n";
         }
      }

   } else {
      @src_report_files = @src_files;
      @hdr_report_files = @hdr_files;
   }
}


sub analyze_header_files() {
   my @io_src;
   open FIND, "find . -name 'io_*.cpp' | grep -v '/verif/' | sort |" or die;
   map {chomp; s/^\.\///} (@io_src = <FIND>);
   close FIND;

   foreach my $io_src (@io_src) {
      (my $hdr_file = $io_src) =~
         s/(.+)\/io_src\/io_(.+)\.cpp$/$1\/include\/$2.hh/ or die;

      if (! -e $hdr_file) {
         die "Cannot find header for $io_src\n";
      }

      push @hdr_files, $hdr_file;

      parse_header_file $hdr_file;
      parse_io_src $io_src, $hdr_file;
   }
}


{
use vars qw(
   $mode
   $if_depth
   $br_depth);

sub change_mode ($) {
   my ($new_mode) = @_;
   warn "Mode change on '$_'\n" .
        "  mode:    $mode -> $new_mode\n" .
        "  ifdepth: $if_depth\n" .
        "  brdepth: $br_depth\n"
      if $debug;
   $mode = $new_mode;
}

sub parse_class ($) {
   my ($code) = @_;
   my $done  = 0;
   my $match = 0;
   my ($parse);

   if ($code =~ /^\s*(class|struct|enum)\s+(\w+)\s*(\:\s*.*?)?\s*(\{)?\s*$/) {
      my ($type, $name, $inheritance, $term) = ($1, $2, $3, $4);

      $inheritance = '' unless defined $inheritance;
      $term = '' unless defined $term;

      $parse = {type        => $type,
                name        => $name,
                inheritance => $inheritance};

      $match = 1;
      $done  = $term eq '{' ? 1 : 0;
   }

   return ($match, $done, $parse);
}

sub parse_member ($) {
   my ($code) = @_;
   my $done  = 0;
   my $match = 0;
   my $error = 0;
   my ($parse, $extra_semi); 
   my ($lpar, $rpar, $eq, $semi_fun, $lbra, $rbra, $semi_data);

   if ($code =~
       /^\s*
         ([^\(\)\{\}\;\=]+)          # type, qualifier, name, all that   1<-id
         (?:                         # split on member data vs function
          (\;) |                     # semicolon terminates data decl    2<-';'
          (?:                        # function group
           (\()                      # start of arg list                 3<-'('
           ([^\(\)\{\}\;\=]*)        # arguments                         4<-args
           (?:                       # args plus def may cross lines
            (\))                     # end of arg list                   5<-')'
            ([^\(\)\{\}\;\=]*)       # const, etc                        6<-qual
            (?:                      # function type group -- decl, ...
             (=)\s*0\s*\; |          # virtual function                  7<-'='
             \s*(\;) |               # function declaration only         8<-';',
             (?:                     # function definition group
              (\:[^\{]*)?            # initializer list                  9<-init
              (?:                    # outer body group
               \s*(\{)               # start of body                    10<-'{'
               (.*?)                 # function body                    11<-body
                (?:                  # function body may cross lines
                (\})                 # end of body                      12<-'}'
                \s*(\;?)             # optional semicolon               13<-';'
               )?                    # end inner body group
              )?                     # end outer body group
             )                       # end function definition group
            )?                       # end function type group
           )?                        # end args to function end group
          )                          # end function group
         )?\s*$/x) {
      my ($id,
          $data_semicolon,
          $open_paren,
          $args,
          $close_paren,
          $qual,
          $virt_eq,
          $fdecl_semicolon,
          $init_list,
          $open_brace,
          $body,
          $close_brace,
          $fdef_semicolon) =
          ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13);
      $match = 1;
      $id =~ s/\s*$//;
      if ((defined $data_semicolon) && ($data_semicolon eq ';')) {
         $done  = 1;
         $parse = {type        => 'data',
                   identifier  => $id};
      } elsif ((defined $virt_eq) && ($virt_eq eq '=')) {
         $done  = 1;
         $parse = {type        => 'pure virtual function',
                   identifier  => $id,
                   arguments   => $args};
      } elsif ((defined $fdecl_semicolon) && ($fdecl_semicolon eq ';')) {
         $done  = 1;
         $parse = {type        => 'function declaration',
                   identifier  => $id,
                   arguments   => $args};
      } elsif ((defined $close_brace) && ($close_brace eq '}')) {
         $done  = 1;
         $parse = {type        => 'function definition',
                   identifier  => $id,
                   arguments   => $args,
                   initializer => $init_list,
                   body        => $body};
         if ((defined $fdef_semicolon) && ($fdef_semicolon eq ';')) {
            $extra_semi = ("$id()");
         }
      } else {
         $done  = 0;
      }
   }

   return ($match, $done, $parse, $extra_semi);
}

sub parse_header_file ($) {
   my ($inc_file) = @_;
   my $in_comment = 0;
   my @issues     = ();
   my @contents   = ();
   my %trick_hdr  = ();
   my @libdep     = ();
   my @includes   = ();
   my @conditional_includes = ();
   my @defines     = ();
   my @transitions = ();
   my @class_heirarchy = ();
   my $ifdef_trickver = 0;
   my $trickver_extra = 0;
   my $trickver_early = 0;

   open INC, '<', $inc_file or die "DIE Can't open $inc_file";
   @contents = <INC>;
   close INC;

   if ($contents[0] =~ /\r/) {
      push @issues, "File contains CRLF line terminators";
      map s/\r$/ /, @contents;
   }

   if (scalar (grep /\t/, @contents)) {
      push @issues, "File contains tab characters";
      map s/\t/ /g, @contents;
   }

   if (scalar (grep {length > 81} @contents)) {
      push @issues, "File contains overlong lines";
   }

   if (scalar (grep / $/, @contents)) {
      push @issues, "File contains lines that end with whitespace";
   }

   %trick_hdr = extract_trick_header $inc_file, +(join '', @contents), '', '';
   if (defined $trick_hdr{libdep}) {
      map {s/\((.*)\)/$1/} (@libdep = ($trick_hdr{libdep} =~ /\([^()]+\)/g));
   } else {
      @libdep = ();
   }


   change_mode INIT_MODE;
   my $line_no = 0;
   foreach my $line (@contents) {
      $line_no++;
      chomp ($_ = $line);
      die if /[\r\t]/;

      # Strip comments
      # Delete up to and including the '*/' if currently in a C-style comment
      if ($in_comment) {
         if (s/.*\*\/ *//) {
            $in_comment = 0;
            if (/\/\*/) {
               die "Multiple C-style comments on one line in $inc_file";
            }
         } else {
            next;
         }
      }

      if (! $in_comment) {
         s/ *\/\/.*$//;
         if (s/ *\/\*.*?\*\///) {
            if (/\/\*/) {
               die "Multiple C-style comments on one line in $inc_file";
            }
         }
         elsif (s/ *\/\*.*//) {
            $in_comment = 1;
         }
      }

      next if /^\s*$/;

      ## At this point we have a non-comment, non-empty line to be parsed.

      # The last queued transitions is the highest priority search.
      # Transitions comprise a set of three function references:
      # - {match}($_)  Determines whether the transition is to be applied
      # - {next}()     Indicates the state to transition to
      # - {action}()   Performs transition-specific side-effects
      if (scalar @transitions) {
         my $tidx = $#transitions;
         my $last = $transitions[$tidx];
         if ($last->{match}($_)) {
            $last->{action}();
            change_mode $last->{next}();
            if (! defined $mode) {
               die "Undefined mode! file=$inc_file line='$_'";
            }
            splice @transitions, $tidx, 1 unless $mode == IMMEDIATE;
            next;
         } elsif ($mode == IMMEDIATE) {
            die "IMMEDIATE must match! file=$inc_file line='$_'";
         }
      }


      # The first two non-comment statements must be a pair of preprocessor
      # statements that protect against double includes.
      if (($mode == INIT_MODE) && (/^#ifndef +(\S*) *$/)) {
         my $protect = $1;
         $if_depth = 1;
         change_mode IMMEDIATE;
         push @transitions, {
              match   => sub {
                            my $l = shift;
                            return (($if_depth == 1) &&
                                    ($l =~ /^#endif *$/)); },
              action  => sub {$if_depth--},
              next    => sub {TRAILER}
            };
         push @transitions, {
              match   => sub {
                            my $l = shift;
                            return (($mode == IMMEDIATE) &&
                                    ($l =~ /^#define $protect *$/)); },
              action  => sub {},
              next    => sub {MAIN_BODY}
            };
         next;
      }

      # Catch non-includes inside an ifdef TRICK_VER block and
      # code after an ifdef TRICK_VER block.
      if (($ifdef_trickver % 2 != 0) &&
          (! /^ *#include/) &&
          (! $trickver_extra)) {
         push @issues, "#ifdef TRICK_VER contains non-include statements";
         $trickver_extra = 1;
      }

      if (($ifdef_trickver > 0) &&
          ($ifdef_trickver % 1 == 0) &&
          (! $trickver_early)) {
         push @issues, "#ifdef TRICK_VER block not at end of file";
         $trickver_early = 1;
      }

      # Handle #ifdef TRICK_VER statements.
      # These must occur after the initial #ifndef and before the final #endif.
      # These should occur right before the final #endif.
      if (($mode == MAIN_BODY) && (/^ *#ifdef +TRICK_VER *$/)) {
         my $curr_mode = $mode;
         if ($if_depth > 1) {
            push @issues, "#ifdef TRICK_VER inside #if";
         }
         $ifdef_trickver++;
         $if_depth++;
         change_mode $mode;
         push @transitions, {
              match   => sub {
                            my $l = shift;
                            return (($if_depth > 1) &&
                                    ($l =~ /^#endif *$/)); },
              action  => sub {$if_depth--; $ifdef_trickver++},
              next    => sub {$curr_mode}
            };
         next;
      }

      # Handle other #if statements.
      # These must occur after the initial #ifndef and before the final #endif.
      if (($if_depth > 0) && (/^ *#if/)) {
         my $curr_mode = $mode;
         $if_depth++;
         change_mode $mode;
         push @transitions, {
              match   => sub {
                            my $l = shift;
                            return (($if_depth > 1) &&
                                    ($l =~ /^#endif *$/)); },
              action  => sub {$if_depth--},
              next    => sub {$curr_mode}
            };
         next;
      }


      # Parse #defines other than the double include #define, which is handled
      # by the immediate transition from the double include #ifndef.
      if (($if_depth > 0) && (/^#define.*?([\\])?$/)) {
         my $continued = $1;
         my $curr_mode = $mode;
         if ((defined $continued) && ($continued eq '\\')) {
            my ($match, $term);
            change_mode IMMEDIATE;
            push @transitions, {
                 match   => sub {
                               my $l = shift;
                               $match = ($l =~ /.*?([\\])?$/);
                               $term  = $1 || '';
                               return (($mode == IMMEDIATE) && $match); },
                 action  => sub {},
                 next    => sub {($term eq '\\') ? IMMEDIATE : $curr_mode}
               };
         } else {
            change_mode $curr_mode;
         }
         next;
      }

      # Parse #includes for the included file.
      if (($mode == MAIN_BODY) && (/^\s*#include +(\S+) *$/)) {
         my $include = $1;
         if ($include =~ s/^"(.*)"$/$1/) {
            if ($if_depth == 1) {
               push @includes, $include;
            } else {
               push @conditional_includes, $include;
            }
         }
         next;
      }


      # Ignore other C preprocessor statements, include #else and #elif
      if (/^\s*#/) {
         next;
      }

      # Parse C++ class/struct/enum statements. 
      if (($mode == MAIN_BODY) || ($mode == CLASS_BODY)) {
         my ($match, $done, $parse) = parse_class ($_);
         if ($match) {
            my $class_code = $_;
            my $curr_mode = $mode;
            my $new_mode;
            my $action = sub {
               $curr_mode = $curr_mode;
               if ($match && $done) {
                  my $type = $parse->{type};
                  my $name = $parse->{name};
                  my $inheritance = $parse->{inheritance};
                  my $full_name = join '::', @class_heirarchy, $name;

                  push @class_heirarchy, $name;
                  push @defines, $full_name;

                  if (defined $class_info{$full_name}->{defined_in}) {
                     die "$type $full_name defined in " .
                         $inc_file  . " and " .
                         $class_info{$full_name}->{defined_in};
                  }
                  $class_info{$full_name}->{defined_in} = $inc_file;
                  $class_info{$full_name}->{type} = $type;
                  $new_mode = ($type eq 'enum') ? ENUM_BODY : CLASS_BODY;

                  if ($type ne 'enum') {
                     map {s/.* //}
                         (@{$class_info{$full_name}->{inherits}} =
                            split /, */, $inheritance);
                  }

                  push @transitions, {
                    match   => sub {
if (! defined $curr_mode) {
die;
}
                                  my $l = shift;
                                  return (($mode == $new_mode) &&
                                          ($br_depth > 0) &&  #{
                                          ($l =~ /^ *} *; *$/)); },
                    action  => sub {
                                  $br_depth--;
                                  pop @class_heirarchy; },
                    next    => sub {$curr_mode}
                  };

                  $br_depth++;
               }
            };

            if ($done) {
               &$action();
               change_mode $new_mode;
               next;

            } else {
               ## warn "curr_mode = $curr_mode" if $debug;
               change_mode IMMEDIATE;
               push @transitions, {
                    match   => sub {
                                  my $l = shift;
                                  $class_code .= " $l";
                                  ($match, $done, $parse) =
                                     parse_class ($class_code);
                                  return ($match);
                               },
                    action  => $action,
                    next    => sub {($done) ? $new_mode : IMMEDIATE}
               };
            }
         }
         next;
      }

      # Parse C-style typedef statements. 
      if ((($mode == MAIN_BODY) || ($mode == CLASS_BODY)) &&
          (/^typedef +(enum|struct) *{ *$/)) {  #}
         my ($type) = ($1);
         my $issues = \@issues;
         my $curr_mode = $mode;
         my $new_mode = ($type eq 'enum') ? ENUM_BODY : CLASS_BODY;
         my $prefix = join '::', @class_heirarchy;

         $br_depth++;
         change_mode $new_mode;
         my ($match, $name);
         push @transitions, {
              match   => sub {
                            my $l = shift;      #{
                            $match = ($l =~  /^ *} *(\S+) *; *$/);
                            $name  = $1;
                            return (($mode == $new_mode) && $match); },
              action  => sub {
                            my $full_name = ($prefix eq '') ?
                                            $name : "$prefix::$name";
                            $br_depth--;
                            if (defined $class_info{$full_name}->{defined_in}) {
                               die "$type $full_name defined in " .
                                   "$inc_file and " .
                                   $class_info{$full_name}->{defined_in};
                            }
                            push @$issues, "C-style typedef $type $name";
                            $class_info{$full_name}->{defined_in} = $inc_file;
                            $class_info{$full_name}->{type} = $type;
                            $class_info{$full_name}->{inherits} = []
                               if ($type ne 'enum');
                            push @defines, $full_name; },
              next    => sub {$curr_mode}
            };
         next;
      }


      # Parse extern "C" statements.
      if (($mode == MAIN_BODY) && (/^extern "C" { *$/)) {
                                                #}
         $br_depth++;
         change_mode IGNORE;
         push @transitions, {
              match   => sub {
                            my $l = shift;
                            return (($mode == IGNORE) &&  #{
                                    ($l =~ /^ *} *$/)); },
              next    => sub {MAIN_BODY},
              action  => sub {$br_depth--}
            };
         next;
      }


      # Ignore external variable declarations.
      # This should not be done in a JEOD header.
      if (($mode == MAIN_BODY) && (/^extern .* *; *$/)) {
         push @issues, "File contains $_";
         warn "$_ in $inc_file\n" if $debug;
         next;
      }


      # Ignore class and friend declarations.
      if ((($mode == MAIN_BODY) || ($mode == CLASS_BODY)) &&
          (/^ *(class +\S+|friend +.+) *; *$/)) {
         ### warn "file=$inc_file mode=$mode line='$_'";
         next;
      }


      # Ignore public/private/protected qualifiers
      if (($mode == CLASS_BODY) &&
          (/^ *(public|protected|private) *: *$/)) {
         ## warn "file=$inc_file mode=$mode line='$_'" if $debug;
         next;
      }


      # Begin parsing member definitions.
      if (($mode == MAIN_BODY) || ($mode == CLASS_BODY)) {
         my ($match, $done, $parse, $extra_semi) = parse_member ($_);
         if ($match) {
            my $member_code = $_;
            my $curr_class  = ($mode == CLASS_BODY) ?
                              $class_heirarchy[$#class_heirarchy] : '';
            my $curr_mode   = $mode;
            my $issues      = \@issues;
            my $action = sub {
               if ($match && $done) {
                  push @$issues, "File contains extraneous semicolon at " .
                                $extra_semi
                     if defined $extra_semi;
                  if ($parse->{type} eq 'function definition') {
                     if ($parse->{identifier} eq $curr_class) {
                        push @$issues,
                             "File inlines constructor for $curr_class";
                     } elsif ($parse->{identifier} =~ /~$curr_class$/) {
                        push @$issues,
                             "File inlines destructor for $curr_class";
                     } elsif ($parse->{identifier} =~ /\bvirtual\b/) {
                        push @$issues,
                             "File inlines virtual method " .
                             "${curr_class}::$parse->{identifier}()";
                     }
                  }
               }
            };

            if ($done) {
               &$action();
               next;

            } else {
               ## warn "curr_mode = $curr_mode" if $debug;
               change_mode IMMEDIATE;
               push @transitions, {
                    match   => sub {
                                  my $l = shift;
                                  $member_code .= " $l";
                                  ## warn "IN: count=$count line=$l" if $debug;
                                  ($match, $done, $parse, $extra_semi) =
                                     parse_member ($member_code);
                                  ## warn "file=$inc_file " .
                                  ##      "mode=$mode count=$count line='$_'"
                                  ##   if $debug;
                                  die "DIE file=$inc_file mode=$mode line='$_'"
                                     if ! $match;
                               return ($mode == IMMEDIATE); } ,
                 action  => $action,
                 next    => sub {($done) ? $curr_mode : IMMEDIATE}
               };
            };
         }
         next;
      }

      # Parse enum members.
      if ($mode == ENUM_BODY) {
         if (! /^\s*\w+\s*(=\s*.+?)?\s*,?\s*$/) {
            die "Bad enum value '$_' in $inc_file";
         }
         next;
      }

      if ($mode == IGNORE) {
         next;
      }

      die "DIE file=$inc_file mode=$mode line='$_'";
   }

   if (scalar @transitions) {
      die "file=$inc_file unused transitions";
   }
   if ($mode != TRAILER) {
      die "file=$inc_file mode=$mode";
   }


   $file_info{$inc_file} = {
      type                 => 'hdr',
      conditional_includes => [@conditional_includes],
      includes             => [@includes],
      defines              => [@defines],
      libdep               => [@libdep],
      issues               => [@issues]
   };
}
}


sub parse_io_src ($$) {
   my ($io_src, $inc_file) = @_;
   my $mode = INIT_MODE;
   my $class;

   open IO, '<', $io_src or die;
   while (<IO>) {

      if (/^ATTRIBUTES attr(\S+)\[\] = \{/) {
         die unless $mode == INIT_MODE;
         ($class = $1) =~ s/__/::/g;
         warn "$inc_file defines $class (ATTRIBUTES)\n" if $debug;
         $mode = MAIN_BODY;
      }

      if (($mode == MAIN_BODY) && /sizeof\((.*?)\)/) {
         my $ref_type = $1;
         push @{$class_info{$class}->{sizeof}}, $ref_type;
      }

      if (/\};\s*$/) {
         $mode = INIT_MODE if $mode == MAIN_BODY;
         undef $class;
      }

###      if (/^void init_attr(\S+)\(void\) \{/) {
###         die unless $mode == INIT_MODE;
###         ($class = $1) =~ s/__/::/g;
###         warn "$inc_file defines $class (init_attr)\n" if $debug;
###         $mode = CLASS_BODY;
###      }

###      if (/^\}\s*$/) {
###         $mode = INIT_MODE if $mode == CLASS_BODY;
###         undef $class;
###      }

   }

   close IO;
}


################################################################################

sub set_external_functions() {
   my @system_functions = (
      '__assert_fail',
      '__ctype_b_loc',
      '__ctype_tolower_loc',
      '__ctype_toupper_loc',
      '__cxa_bad_typeid',
      '__cxa_begin_catch',
      '__cxa_end_catch',
      '__cxa_pure_virtual',
      '__cxa_rethrow',
      '__dynamic_cast',
      '__errno_location',
      '__fxstat',
      '__gxx_personality_v0',
      '_Unwind_Resume',
      'access',
      'acos',
      'acosh',
      'asin',
      'asinh',
      'atan',
      'atanh',
      'atan2',
      'calloc',
      'cos',
      'cosh',
      'erf',
      'exp',
      'fclose',
      'fgets',
      'fileno',
      'floor',
      'fmod',
      'fopen',
      'fprintf',
      'fread',
      'free',
      'fseek',
      'ftell',
      'getenv',
      'log',
      'log10',
      'malloc',
      'memcpy',
      'memmove',
      'memset',
      'operator delete(void*)',
      'operator delete[](void*)',
      'operator new(unsigned int)',
      'operator new[](unsigned int)',
      'pow',
      'pthread_mutex_lock',
      'pthread_mutex_unlock',
      'rewind',
      'sin',
      'sinh',
      'sprintf',
      'sqrt',
      'sscanf',
      'std::_List_node_base::unhook()',
      'std::_List_node_base::hook(std::_List_node_base*)',
      'std::__throw_out_of_range(char const*)',
      'stderr',
      'stdout',
      'strcat',
      'strcmp',
      'strcpy',
      'strdup',
      'strerror',
      'strlen',
      'strncmp',
      'strncpy',
      'strpbrk',
      'tan',
      'tanh',
      'vsprintf');

   my @trick_functions = qw(
      update_alloc_stats
      alloc_tree_by_address
      avl_delete
      dm_invert_symm
      euler_matrix
      exec_terminate
      get_job_cycle
      io_compare_addr
      input_proc_error
      ip_user_alloc_type
      ip_mutex
      ipyyparse
      switch_new_file
      send_hs);


   foreach my $fun (@system_functions) {
      $functions{$fun}->{defined_in} = 'system';
   }
   foreach my $fun (@trick_functions) {
      $functions{$fun}->{defined_in} = 'trick';
   }
}


sub analyze_source_files() {
   my @obj_files;
   open FIND, "find . -name '*.o' | grep -v -E '/(verif|io_|lib_)' | sort |"
      or die;
   map {chomp; s/^\.\///} (@obj_files = <FIND>);
   close FIND;

   foreach my $obj_file (@obj_files) {
      (my $dep_file = $obj_file) =~ s/\/object_$ENV{TRICK_HOST_CPU}\//\//
         or do {
            warn "Skipping $obj_file\n";
            next;
      };
      (my $src_file = $dep_file) =~ s/(.+)\/(.+)\.o$/$1\/src\/$2.cc/ or die;
      my (@contents);
      my (@issues, @libdep, %trick_hdr);
      my (%defines, %constructs);
      my (%references, @references);
      my (%cast_references, @cast_references);
      my (%attr_references, @attr_references);

      if (! -e $src_file) {
         $src_file =~ s/\.cc$/.c/;
         if (! -e $src_file) {
            die "Cannot find source for $obj_file\n";
         }
      }

      open SRC, '<', $src_file or die "DIE Can't open $src_file";
      @contents = <SRC>;
      close SRC;

      if ($contents[0] =~ /\r/) {
         push @issues, "File contains CRLF line terminators";
         map s/\r$/ /, @contents;
      }

      if (scalar (grep /\t/, @contents)) {
         push @issues, "File contains tab characters";
         map s/\t/ /g, @contents;
      }

      if (scalar (grep {length > 81} @contents)) {
         push @issues, "File contains overlong lines";
      }

      if (scalar (grep / $/, @contents)) {
         push @issues, "File contains lines that end with whitespace";
      }

      %trick_hdr = extract_trick_header
                   $src_file, +(join '', @contents), '', '';
      if (defined $trick_hdr{libdep}) {
         map {s/\((.*)\)/$1/} (@libdep = ($trick_hdr{libdep} =~ /\([^()]+\)/g));
      } else {
         @libdep = ();
      }


      push @src_files, $src_file;

      open DEFS, "nm $obj_file | c++filt |" or die;
      while (<DEFS>) {
         chomp;
         ### next if / (for|to) /;
         s/^([0-9a-f]{16}| {16}) // or die "$_";
         /([\w]) (.*)/ or do {
            warn "$_ $src_file";
         };
         my ($t, $fun) = ($1, $2);
         if (($t eq 'T') && ($fun !~ / (for|to) /)) {
            if ((defined $functions{$fun}->{defined_in}) &&
                ($functions{$fun}->{defined_in} ne $dep_file)) {
               die "$fun defined in " .
                   "$dep_file and $functions{$fun}->{defined_in}";
            }
            $defines{$fun} = 1;
            $functions{$fun}->{defined_in} = $dep_file;
            if ($fun =~ /^([A-Za-z]\S+)::[^:]+\(.+$/) {
               my $class = $1;
               $class_info{$class}->{methods}->{$fun} = $dep_file;
               push @{$class_info{$class}->{implementation}->{$dep_file}}, $fun;
               if ($fun eq "${class}::${class}()") {
                  $constructs{$class} = 1;
               }
            }
         }
         elsif ($t eq 'U') {
            if ($fun =~ /^attr(.*)/) {
               my $class = $1;
               $attr_references{$class} = 1;
            } elsif ($fun =~ /(?:vtable|typeinfo) (?:for|to) (.*)/) {
               my $class = $1;
               if ($class =~ /^[A-Za-z]/) {
                  $references{"${class}::${class}()"} = 1;
                  $cast_references{$class} = 1;
               }
               # No else: JEOD classes start with a letter.
               # The __xxx references are g++ entities.

            } else {
               $references{$fun} = 1;
            }
         }
      }
      close DEFS;

      # Weed out the references to system, trick, and functions defined in the
      # subject source file.
      @references = keys %references;
      %references = ();
      foreach my $fun (@references) {
         if ((! defined $functions{$fun}->{defined_in}) ||
             (($functions{$fun}->{defined_in} ne $dep_file) &&
              ($functions{$fun}->{defined_in} ne 'system') &&
              ($functions{$fun}->{defined_in} ne 'trick'))) {
            push @{$functions{$fun}->{referenced_by}}, $dep_file;
            $references{$fun} = 1;
         }
      }
      @references = sort keys %references;
      @cast_references = sort keys %cast_references;
      @attr_references = sort keys %attr_references;

      $file_info{$src_file} = {
         type                 => 'src',
         ### includes             => [@includes],
         constructs           => [sort keys %constructs],
         call_references      => [@references],
         cast_references      => [@cast_references],
         attr_references      => [@attr_references],
         libdep               => [@libdep],
         issues               => [@issues]
      };

      $file_info{$dep_file} = { %{$file_info{$src_file}} };
      $file_info{$dep_file}->{type} = 'obj';
   }
}


sub generate_src_report() {
   my ($not_parsed, $ignore, $comm) = unique \@src_report_files, \@src_files;
   if (@$not_parsed) {
      print "No report generated for the following uncompiled source files:\n";
      print "  $_\n" foreach @$not_parsed;
      print "\n";
      @src_report_files = @$comm;
   }

   foreach my $src_file (@src_report_files) {
      my $finfo = $file_info{$src_file};
      (my $dep_file = $src_file) =~ s|src/(.*).c[c]$|$1.o|;
      (my $dir = $src_file)  =~ s|/src/.*||;
      (my $base = $dep_file) =~ s|$dir/||;
      my @calls      = @{$finfo->{call_references}};
      my @constructs = @{$finfo->{constructs}};


      # Weed out the references to functions co-defined with the files
      # that define the parent class default constructors.
      my %freebies = ();
      foreach my $constructed_class (@constructs) {
         my $inheritance = inherits_from $constructed_class;
         foreach my $parent_class (@$inheritance) {
            my $parent_constructor = "${parent_class}::${parent_class}()";
            $freebies{$functions{$parent_constructor}->{defined_in}} = 1
               if exists ($functions{$parent_constructor}->{defined_in});
         }
      }
      @calls = grep {my $defined_in = $functions{$_}->{defined_in};
                     ((! defined $defined_in) ||
                      (! exists $freebies{$defined_in}))} @calls;

      # Sort the dependencies.
      my %calls;
      $calls{$base} = 0;
      foreach my $fun (@calls) {
         my $where = $functions{$fun}->{defined_in};
         if (defined $where) {
            if ($where =~ s|$dir/||) {
               $calls{$where} = 0;
            } else {
               $calls{$where} = 1;
            }
         }
         else {
            warn "Undefined reference $fun in $src_file\n";
         }
      }
      delete $calls{$base};
      @calls = sort {($calls{$a} <=> $calls{$b}) || ($a cmp $b)} (keys %calls);
      unshift @calls, $base;

      if ($verbose) {
         print "Library dependencies for $src_file:\n";
         print "  ((", (join ")\n   (", @calls), "))\n\n";
      }
      my $report_hdr = "Discrepancy report for $src_file\n";
      if (scalar @{$finfo->{issues}}) {
         print $report_hdr;
         print "Issues detected:\n";
         print "  $_\n" foreach (@{$finfo->{issues}});
         $report_hdr = '';
      }
      report_discrepancies \$report_hdr, 'library dependencies',
                           $finfo->{libdep}, \@calls;
      print "\n" if ($report_hdr eq '');
   }
}


sub generate_hdr_report() {
   my ($not_parsed, $ignore, $comm) = unique \@hdr_report_files, \@hdr_files;
   if (@$not_parsed) {
      print "No report generated for the following unanlyzed header files:\n";
      print "  $_\n" foreach @$not_parsed;
      print "\n";
      @hdr_report_files = @$comm;
   }

   foreach my $hdr_file (@hdr_report_files) {

      (my $model_dir = $hdr_file) =~ s/\/include.*//;
      my $finfo = $file_info{$hdr_file};
      my (@includes, %includes);
      my (@implemented_in, %implemented_in);
      my (@trick_includes, %trick_includes);

      @includes = @{$finfo->{includes}};
      map {note_inclusion \%includes, $model_dir, $_} @includes;

      if ($verbose) {
         if ((defined $finfo->{includes}) && scalar @{$finfo->{includes}}) {
            print "$hdr_file includes the following files:\n";
            print "  $_\n" foreach (@{$finfo->{includes}});
         }

         if ((defined $finfo->{defines}) && (scalar @{$finfo->{defines}} > 0)) {
            print "$hdr_file defines the following:\n";
            foreach my $class (@{$finfo->{defines}}) {
               my $type = $class_info{$class}->{type};
               print "  $type $class\n";
            }
         }
      }

      if ((defined $finfo->{defines}) && (scalar @{$finfo->{defines}} > 0)) {
         foreach my $class (@{$finfo->{defines}}) {
            my $cinfo = $class_info{$class};
            if (defined $cinfo->{sizeof}) {
               foreach my $ref_class (@{$cinfo->{sizeof}}) {
                  my $ref_file = $class_info{$ref_class}->{defined_in};
                  if ((defined $ref_file) && ($ref_file ne $hdr_file)) {
                     $ref_file =~ s|$model_dir/include/||;
                     $trick_includes{$ref_file} = 1
                        unless is_included \%includes, $model_dir, $ref_file;
                  }
               }
            }
            if (defined $cinfo->{implementation}) {
               my @implemented_by = keys %{$cinfo->{implementation}};
               foreach my $obj_file (@implemented_by) {
                  my $oinfo = $file_info{$obj_file};
                  foreach my $ref_class (@{$oinfo->{rtti_references}}) {
                     my $ref_file = $class_info{$ref_class}->{defined_in};
                     if ((defined $ref_file) && ($ref_file ne $hdr_file)) {
                        $ref_file =~ s|$model_dir/include/||;
                        $trick_includes{$ref_file} = 1
                           unless is_included \%includes, $model_dir, $ref_file;
                     }
                  }
                  foreach my $ref_class (@{$oinfo->{attr_references}}) {
                     my $ref_file = $class_info{$ref_class}->{defined_in};
                     if ((defined $ref_file) && ($ref_file ne $hdr_file)) {
                        $ref_file =~ s|$model_dir/include/||;
                        $trick_includes{$ref_file} = 1
                           unless is_included \%includes, $model_dir, $ref_file;
                     }
                  }
                  $obj_file =~ s|$model_dir/||;
                  $implemented_in{$obj_file} = 1;
               }
            }
         }
      }

      @implemented_in = keys %implemented_in;
      @implemented_in = sort path_compare @implemented_in;
      @trick_includes = keys %trick_includes;
      @trick_includes = sort path_compare @trick_includes;

      if ($verbose) {
         if ((defined $finfo->{libdep}) && scalar @{$finfo->{libdep}}) {
            print "$hdr_file lists the following dependencies:\n";
            print "  $_\n" foreach (@{$finfo->{libdep}});
         }

         if (scalar @implemented_in) {
            print "$hdr_file should list the following dependencies:\n";
            print "  $_\n" foreach @implemented_in;
         }

         if ((defined $finfo->{conditional_includes}) &&
             scalar @{$finfo->{conditional_includes}}) {
            print "$hdr_file conditionally includes the following files:\n";
            print "  $_\n" foreach (@{$finfo->{conditional_includes}});
         }
         if (scalar @trick_includes) {
            print "$hdr_file should conditionally include the following:\n";
            print "  $_\n" foreach @trick_includes;
         }
      }

      my $report_hdr = "Discrepancy report for $hdr_file\n";
      if (scalar @{$finfo->{issues}}) {
         print $report_hdr;
         print "Issues detected:\n";
         print "  $_\n" foreach (@{$finfo->{issues}});
         $report_hdr = '';
      }
      report_discrepancies \$report_hdr, 'library dependencies',
                           $finfo->{libdep}, \@implemented_in;
      report_discrepancies \$report_hdr, 'TRICK_VER includes',
                           $finfo->{conditional_includes}, \@trick_includes;
      print "\n" if ($report_hdr eq '');
   }
}


sub inherits_from ($;$) {
   my ($class, $inherits) = @_;
   $inherits = [] unless defined $inherits;
   foreach my $parent_class (@{$class_info{$class}->{inherits}}) {
      my $recorded = 0;
      foreach my $recorded_class (@$inherits) {
         if ($recorded_class eq $parent_class) {
            $recorded = 1;
            last;
         }
      }
      if (! $recorded) {
         push @$inherits, $parent_class;
         inherits_from $parent_class, $inherits;
      }
   }
   return $inherits;
}


sub path_compare {
   ($a =~ m|/|) ?
      +(($b =~ m|/|) ? $a cmp $b :      1   ) :
      +(($b =~ m|/|) ?     -1    : $a cmp $b);
}

sub note_inclusion ($$$) {
   my ($includes, $model_dir, $inc_file) = @_;
   if ($inc_file =~ m|(.*)/include/|) {
      $model_dir = $1;
   } else {
      $inc_file = "$model_dir/include/$inc_file"
   }
   if (! exists $includes->{$inc_file}) {
      $includes->{$inc_file} = 1;
      my $finfo = $file_info{$inc_file};
      if (defined $finfo) {
         foreach my $included_file (@{$finfo->{includes}}) {
            my $inc_dir;
            if ($included_file =~ m|(.*)/include/|) {
               $inc_dir = $1;
            } else {
               $inc_dir = $model_dir;
            }
            note_inclusion $includes, $inc_dir, $included_file;
         }
      }
   }
}

sub is_included ($$$) {
   my ($includes, $model_dir, $inc_file) = @_;
   $inc_file = "$model_dir/include/$inc_file" if ($inc_file !~ m|/|);
   return (exists $includes->{$inc_file});
}

sub unique ($$) {
   my ($list1, $list2) = @_;
   my (%mark1, %mark2);
   my (@uniq1, @uniq2, @comm);
   @mark1{@$list1} = (1) x @$list1;
   @mark2{@$list2} = (1) x @$list2;
   @uniq1 = grep {! exists $mark2{$_}} @$list1;
   @uniq2 = grep {! exists $mark1{$_}} @$list2;
   if ($#$list1 < $#$list2) {
      @comm  = grep {exists $mark2{$_}} @$list1;
   } else {
      @comm  = grep {exists $mark1{$_}} @$list2;
   }
   return (\@uniq1, \@uniq2, \@comm);
}

sub report_discrepancies ($$$$) {
   my ($hdr, $item_name, $file_items, $expected_items) = @_;
   my ($not_in_expected, $not_in_file) = unique $file_items, $expected_items;
   if (scalar @$not_in_expected) {
      print "${$hdr}File specifies the following unneeded ${item_name}:\n";
      print "  $_\n" foreach (@$not_in_expected);
      $$hdr = '';
   }
   if (scalar @$not_in_file) {
      print "${$hdr}File omits the following needed ${item_name}:\n";
      print "  $_\n" foreach (@$not_in_file);
      $$hdr = '';
   }
}
