use strict;
use Getopt::Long;
use Pod::Usage;

sub make_table (@);
sub make_header();
sub get_files (@);
sub get_file_stats ($);
sub compare_file_names ($$);

my $vertlines = 1;
my $model_home = '../..';


# Get the Trick major and minor release numbers.
my ($Trick_major, $Trick_minor);
my @trick_ver = split /\./, $ENV{TRICK_VER};
$Trick_major = $trick_ver[0];
$Trick_minor = $trick_ver[1];


# Process the command line options.
GetOptions (
   'vertlines!'   => \$vertlines              # Vertical lines in table?
) or pod2usage(2);


chdir $model_home or die;
print make_table qw(include src);



sub make_table (@) {
   my (@dirs) = @_; # i.e. include and src
   # create the header
   my ($table_spec, $table_header) = make_header();
   # get a list of the files for which stats are needed.
   my @files = get_files @dirs;

   my $table = << "   EOF";
      \\begin\{stretchlongtable\}\[c\]${table_spec}
      \\caption\{Coarse Metrics\}
      \\label\{tab:coarse_metrics\} \\\\\[6pt\]

      $table_header
      \\endfirsthead

      \\caption\[\]\{Coarse Metrics (continued from previous page)\} \\\\\[6pt\]
      $table_header
      \\endhead

      \\hline \\multicolumn\{4\}\{r\}\{\{Continued on next page\}\} \\\\
      \\endfoot

      \\hline
      \\endlastfoot
   EOF

   $table =~ s/^      //mg;

   my ($grand_total, $grand_blanks, $grand_comments, $grand_code) =
      (0, 0, 0, 0);

   # for each file, generate the number of blank, comment, and code lines, and
   # the total.  Sum these values into the grand-totals over all files in this
   # model
   foreach my $file (@files) {
      my ($total, $blanks, $comments) = get_file_stats $file;
      my $code = $total - ($blanks + $comments);
      $file =~ s/\//\/\\hspace{0pt}/g;
      $file =~ s/_/\\_\\hspace{0pt}/g;
      my $entry = join ' & ', $file, $blanks, $comments, $code, $total;
      $table .= "\\longentry $entry \\\\\n";
      $grand_total    += $total;
      $grand_blanks   += $blanks;
      $grand_comments += $comments;
      $grand_code     += $code;
   }
   $table .= '\hline ' .
             +(join ' & ',
               '{\bf Total}',
               $grand_blanks, $grand_comments, $grand_code, $grand_total) .
             "\\\\\n";
   $table .= "\\end{stretchlongtable}\n";
   return $table;
}


sub make_header() {
   my $bar = $vertlines ? '|' : '';
   my $space = $vertlines ? '' : '\ ';
   my $dbar = "${bar}${bar}";
   my $total = $vertlines ? 'Total' : '\ \ Total';
   my $table_spec = "\{${dbar}p{2in}${bar}rrrr${bar}\}";
   my $table_header = << "   EOF";
      \\hline
      \\multicolumn\{1\}\{${dbar}l${bar}\} \{\} \&
      \\multicolumn\{4\}\{c${bar}\} \{\\bf Number of Lines\} \\\\
      \\multicolumn\{1\}\{${dbar}l${bar}\} \{\\bf File Name\} \&
      \\multicolumn\{1\}\{r${space}\} \{\\bf Blank\} \&
      \\multicolumn\{1\}\{r${space}\} \{\\bf Comment\} \&
      \\multicolumn\{1\}\{r${space}\} \{\\bf Code\} \&
      \\multicolumn\{1\}\{r${bar}} \{\\bf $total\} \\\\
      \\hline \\hline
   EOF
   $table_header =~ s/^      //;
   $table_header =~ s/\n$//;

   return ($table_spec, $table_header);
}


sub get_files (@) {
   # create a list of files whose address/name includes "src" or "include" but
   # does not include "verif"
   my @files = `git ls-files | grep -v verif | grep -E 'src|include'`;

   return sort compare_file_names @files;
}


# Constants for line categories
sub UNKNOWN {0}
sub PREPROCESSOR {1}
sub STRING {2}
sub COMMENT {3}
sub BLANK {4}
sub CODE {5}

# Constants for the state of the if/elif/else/endif abstract state machine
sub ACTIVE {0}
sub INACTIVE {1}
sub IGNORE {2}

# Preprocessor statement categories
sub NOT_CONDITIONAL {0}
sub IF    {1}
sub ELIF  {2}
sub ELSE  {3}
sub ENDIF {4}


sub get_file_stats ($) {
   my ($fname) = @_;

   my $line_type = UNKNOWN;
   my @line_count = (0) x 6;

   my $continuation = 0;

   my $in_macro = 0;
   my $in_comment = 0;
   my $in_string = 0;
   my $in_special = 0;

   my $if_status = ACTIVE;
   my @if_history = ();
   my %defines = (TRICK_VER => $Trick_major, TRICK_MINOR => $Trick_minor);

   my $lineno = 0;
   my $macro = '';

   open IN, $fname or die "Can't open $fname for input\n";
   while (<IN>) {
      chomp;
      ++$lineno;
      $line_type = UNKNOWN unless $continuation;
      my $line_continues = s/\\$//;
      my $is_blank = /^ *$/;

      if ($in_macro || (! ($continuation || $in_special) && /^ *#/)) {
         $line_type = PREPROCESSOR;

         $macro .= $_;
         if ($line_continues) {
            $in_macro = 1;
         }
         else {
            my $stmt_type = NOT_CONDITIONAL;
            my $if_value = '';

            $macro =~ s/\/\*.*?\*\///g;
            $macro =~ s/\/\/.*//;

            if ($macro =~ /^ *# *include/) {
            }
            elsif ($macro =~ /^ *# *undef +(\w+) *$/) {
               my $defname = $1;
               undef $defines{$defname};
            }

            elsif ($macro =~ /^ *# *define +(\w+) *$/) {
               my $defname = $1;
               $defines{$defname} = 1;
            }

            elsif ($macro =~ /^ *# *define +(\w+) *[\(\)]/) {
               my $defname = $1;
               $defines{$defname} = "+(warn 'Unexpected usage $defname'), undef";
            }

            elsif ($macro =~ /^ *# *define +(\w+) +(.+?) *$/) {
               my ($defname, $defval) = ($1, $2);
               while (my ($key, $val) = each %defines) {
                  $defval =~ s/\b$key\b/$val/g;
               }
               $defines{$defname} = $defval;
            }

            elsif ($macro =~ /^ *# *ifdef *(\w+) *$/) {
               my $defname = $1;
               $stmt_type = IF;
               $if_value = defined $defines{$defname};
            }

            elsif ($macro =~ /^ *# *ifndef *(\w+) *$/) {
               my $defname = $1;
               $stmt_type = IF;
               $if_value = ! defined $defines{$defname};
            }

            elsif ($macro =~ /^ *# *(if|elif) *(.*?) *$/) {
               my ($if, $if_expr) = ($1, $2);
               $stmt_type = $if eq 'if' ? IF : ELIF;

               if (($stmt_type == IF) && ($if_status == ACTIVE) ||
                   ($stmt_type == ELIF) && ($if_status == INACTIVE)) {
                  while ($if_expr =~ s/\bdefined\b\s*
                                       (?:
                                        \(\s*(\w+)\s*\)|
                                        (\w+))
                                      /_RESULT_/x) {
                     my $key = $1 || $2;
                     my $val = defined $defines{$key} ? 1 : 0;
                     $if_expr =~ s/_RESULT_/$val/;
                  }
                  while (my ($key, $val) = each %defines) {
                     $if_expr =~ s/\b$key\b/$val/g;
                     $if_expr =~ s/\$\d+ *(== != < >= > <=) *\d+/1/g;
                  }

                  undef $if_value;
                  $if_value = eval $if_expr;
                  unless (defined $if_value) {
                     warn "Cannot evaluate if expression '$if_expr'\n";
                     $if_value = 0;
                  }
               }
            }

            elsif ($macro =~ /^ *# *else *$/) {
               $stmt_type = ELSE;
            }

            elsif ($macro =~ /^ *# *endif *$/) {
               $stmt_type = ENDIF;
            }

            else {
               $macro =~ s/ *$// or die "Preprocessor error\n";
               warn "Unknown preprocessor '$macro' at $fname line $lineno\n";
            }

            if ($stmt_type == IF) {
               push @if_history, $if_status;
               $if_status = ($if_status == ACTIVE) ?
                            ($if_value ? ACTIVE : INACTIVE) : IGNORE;
            }
            elsif ($stmt_type == ELIF) {
               $if_status = ($if_status == INACTIVE) ?
                            ($if_value ? ACTIVE : INACTIVE) : IGNORE;
            }
            elsif ($stmt_type == ELSE) {
               $if_status = ($if_status == INACTIVE) ? ACTIVE : IGNORE;
            }
            elsif ($stmt_type == ENDIF) {
               $if_status = pop @if_history;
               warn "No #if for #endif at $fname line $.\n"
                  unless defined $if_status;
            }

            $in_macro = 0;
            $macro = '';
         }
      }

      elsif ($if_status == ACTIVE) {
         if ($in_string) {
            $is_blank = 0;
            if (s/^("|.*?[^\\]"|.*?\\\\")/0/) {
               $in_special = $in_string = 0;
            }
            $line_type = STRING if $line_type == UNKNOWN;
         }

         elsif ($in_comment) {
            if (s/^.*?\*\// /) {
               $in_special = $in_comment = 0;
            }
            $line_type = COMMENT if $line_type == UNKNOWN;
         }

         my $parsed = '';
         my $unparsed = $_;

         while (1) {
            if ($in_string) {
               $is_blank = 0;
               if ($unparsed =~ s/^("|.*?[^\\]"|.*?\\\\")//) {
                  $in_special = $in_string = 0;
                  $parsed .= '0';
               }
               else {
                  $unparsed = '';
                  last;
               }
            }
            elsif ($in_comment) {
               if ($unparsed =~ s/^.*?\*\///) {
                  $in_special = $in_comment = 0;
                  $parsed .= ' ';
               }
               else {
                  $unparsed = '';
                  last;
               }
            }

            # Split the unparsed part of the line at the first special char
            if ($unparsed =~ /^(.*?)(\/\*|\/\/|"|')(.*)$/) {
               my ($pre, $special, $post) = ($1, $2, $3);
               $parsed .= $pre;
               $is_blank = 0;

               # Enter comment mode (c++-style)
               if ($special eq '//') {
                  $line_type = COMMENT if $line_type == UNKNOWN;
                  $unparsed = '';
                  last;
               }

               # Enter comment mode (c-style)
               elsif ($special eq '/*') {
                  $line_type = COMMENT if $line_type == UNKNOWN;
                  if ($post =~ s/^.*?\*\// /) {
                     $unparsed = $post;
                  }
                  else {
                     $in_special = $in_comment = 1;
                     $unparsed = '';
                     last;
                  }
               }

               # Enter double quote mode (a string)
               elsif ($special eq '"') {
                  if (($line_type == UNKNOWN) || ($line_type == COMMENT)) {
                     $line_type = STRING;
                  }
                  $unparsed = $post;
                  $in_special = $in_string = 1;
               }

               # Enter single quote mode (a char)
               else {
                  $line_type = CODE;
                  if ($post =~ s/^([^\\]|\\0\d*|\\x\w+|\\[^0x])\'//) {
                     $parsed .= '0';
                     $unparsed = $post;
                  }
                  else {
                     die "Unterminated character constant at $fname:$lineno\n";
                  }
               }
            }
            else {
               $parsed .= $unparsed;
               $unparsed = '';
               last;
            }
         }
         die unless $unparsed eq '';
         $_ = $parsed;
      }

      if ($is_blank) {
         $line_type = BLANK;
      }
      elsif (! /^ *$/) {
         $line_type = CODE
            if (($line_type == UNKNOWN) || ($line_type == COMMENT));
      }
      elsif (($line_type == UNKNOWN)) {
die;
      }

      ++$line_count[$line_type];
      $continuation = $line_continues;
   }
   close IN;

   return ($lineno, $line_count[BLANK], $line_count[COMMENT]);
}


sub compare_file_names ($$) {
   my ($file_a, $file_b) = @_;
   return ((($file_a =~ tr/\///) <=> ($file_b =~ tr/\///)) ||
           ($file_a cmp $file_b));
}
