################################################################################
# Package Trick_Logfile
#
# Provides an object-oriented interface to a Trick 05 binary log file.
# Constructor:
#   $log_file = Trick_Logfile->new($logfile_name);
#     Creates a new Trick_Logfile object.
# Instance methods:
#   $log_file->read_block()
#     Reads the next block of data from a Trick_Logfile object.
#   $log_file->write_block()
#     Write the next block of data from a Trick_Logfile object.
#   $value = $log_file->get_scalar($varname);
#   $value = $log_file->get_vec($varname);
#   $value = $log_file->get_mat($varname);
#   $value = $log_file->get_quat($varname);
#   $value = $log_file->get_nvec($varname);
#     Extract a Trick scalar / 3-vector / 3x3 matrix / quaterion / nvector
#     from the most recently read data block.
#   $log_file->close();
#     Close the log file.
#
# David Hammen - OSR - July 2006
#
################################################################################

package Trick_Logfile;

use strict;
use Carp;
use IO::File;

our %TRICK_07_Logfile_Format;
our %TRICK_10_Logfile_Format;

sub new($;$);
sub close($);
sub read_block($);
sub get_scalar($$);
sub get_nvec($$$);
sub get_vec($$);
sub get_quat($$);
sub get_mat($$);
sub read_log_header($);
sub read_descriptor ($$);
sub read_input ($$$$$);
sub write_output ($$$$$);


%TRICK_07_Logfile_Format = (
     0 => {size => undef, format => 'a'},  # char array
     1 => {size => undef, format => 'a'},  # char array
     3 => {size =>  2,    format => 's'},  # short
     4 => {size =>  2,    format => 'S'},  # unsigned short
     5 => {size =>  4,    format => 'l'},  # int
     6 => {size =>  4,    format => 'L'},  # unsigned int
     7 => {size =>  4,    format => 'l'},  # long ? but size = 4
     8 => {size =>  4,    format => 'L'},  # unsigned long ? but size = 4
     9 => {size =>  4,    format => 'f'},  # float
    10 => {size =>  8,    format => 'd'},  # double
    13 => {size =>  8,    format => 'q'},  # long long
    14 => {size =>  8,    format => 'Q'},  # unsigned long long
    17 => {size =>  1,    format => 'C'},  # Boolean
   102 => {size =>  4,    format => 'l'}   # Enum
);

%TRICK_10_Logfile_Format = (
     1 => {size => undef, format => 'a'},  # char array
     2 => {size => undef, format => 'a'},  # char array
     4 => {size =>  2,    format => 's'},  # short
     5 => {size =>  2,    format => 'S'},  # unsigned short
     6 => {size =>  4,    format => 'l'},  # int
     7 => {size =>  4,    format => 'L'},  # unsigned int
     8 => {size =>  4,    format => 'l'},  # long ? but size = 4
     9 => {size =>  4,    format => 'L'},  # unsigned long ? but size = 4
    10 => {size =>  4,    format => 'f'},  # float
    11 => {size =>  8,    format => 'd'},  # double
    14 => {size =>  8,    format => 'q'},  # long long
    15 => {size =>  8,    format => 'Q'},  # unsigned long long
    17 => {size =>  1,    format => 'C'},  # Boolean
    21 => {size =>  4,    format => 'l'}   # Enum
);


# Trick_Logfile->new($fname) - Create a new Trick_Logfile object
# Returns logfile object.
sub new($;$) {
   my ($class_or_ref, $fname, $mode) = @_;
   my $class = ref ($class_or_ref) || $class_or_ref;
   my $self = {fname  => $fname,
               blkno  => 0,
               at_eof => 0};
   if (defined $mode) {
      $mode =~ /^[rw]$/ or die "Bad mode";
   } else {
      $mode = 'r';
   }

    # Open the specified file.
   $self->{handle} = IO::File->new ($fname, $mode)
      or croak "Can't open log file '$fname'\n";

    # Make the reference a blessed object.
   bless $self, $class;

     # Read the log file header.
   if ($mode eq 'r') {
      $self->read_log_header();
   }

   return $self;
}


# $logfile->close() - Close the log file
sub close($) {
   my ($self) = @_;
   $self->{handle}->close() if defined $self->{handle};
   undef $self->{handle};
}


# $logfile->read_block() - Read the next block from the log file
# Returns 0/1 success code.
sub read_block($) {
   my ($self) = @_;
   my ($handle, $fname, $blksize, $format, $blkid, $buffer,
       @vals, $vars, $vals);

   croak "Already closed" if ! defined $self->{handle};
   croak "Already at eof" if $self->{at_eof};

    # Extract some items from the object.
   ($handle, $fname, $blksize, $format) =
      @$self{qw(handle file blksize format)};

    # Read the next block.
   $self->{blkno}++;
   $blkid = "block $self->{blkno}";
   $buffer = $self->read_input ($blksize, 1, '', $blkid);

    # Extract data from the buffer and return success if a block was read.
   if (defined $buffer) {
      $self->{block} = $buffer;
      @vals = unpack $format, $buffer;
      scalar (@vals) == $self->{nvars}
         or croak "Mismatched log file $self->{fname}";
      $vars = $self->{vars};
      $vals = $self->{vals};
      @$vals{(@$vars)} = @vals;
      return 1;

    # No block read: mark EOF and return failure code.
   } else {
      $self->{at_eof} = 1;
      return 0;
   }
}

# $logfile->get_scalar($vname) - Return logged scalar variable value.
sub get_scalar($$) {
   my ($self, $vname) = @_;
   my $val = $self->{vals}->{$vname};
   defined ($val) or croak "Missing value for $vname";
   return $val;
}

# $logfile->get_nvec($vname, $dim) - Return logged n-vector variable value.
sub get_nvec($$$) {
   my ($self, $vname, $dim) = @_;
   my @vec = ();
   foreach my $ii (0 .. $dim-1) { 
      $vec[$ii] = $self->get_scalar ("$vname\[$ii\]");
   }
   return [@vec]; 
}

# $logfile->get_vec($vname, $dim) - Return logged 3-vector variable value.
sub get_vec($$) {
   my ($self, $vname) = @_;
   $self->get_nvec ($vname, 3);
}

# $logfile->get_quat($vname, $dim) - Return logged quaternion variable value.
sub get_quat($$) {
   my ($self, $vname) = @_;
   $self->get_nvec ($vname, 4);
}
 
# $logfile->get_mat($vname, $dim) - Return logged 3x3 matrix variable value.
sub get_mat($$) {
   my ($self, $vname) = @_;
   my @mat = ();
   foreach my $ii (0 .. 2) { 
      $mat[$ii] = $self->get_vec ("$vname\[$ii\]");
   }
   return [@mat]; 
}


################################################################################
# Internal methods and functions
################################################################################
   

# $logfile->read_log_header - Read the log header.
sub read_log_header($) {
   my ($self) = @_;
   my ($file, $fname) = @$self{qw(handle file)};
   my ($nparams, $blksize, @params, $format);

   tell($file) == 0 or croak "File $fname not positioned correctly";

    # The first ten bytes of the file indicate the file Endian mode.
   $self->{endian_mode} = $self->read_input (10, 0, "a10", "endian mode");

   if ($self->{endian_mode} =~ /Trick.(\d\d).(.)/i) {
      my ($trick_id, $mode) = ($1, $2);
      if ($trick_id eq '07') {
         $self->{logfile_format} = \%TRICK_07_Logfile_Format;
      }
      elsif ($trick_id eq '10') {
         $self->{logfile_format} = \%TRICK_10_Logfile_Format;
      }
      else {
         croak "Unknown format $self->{endian_mode}";
      }
   }
   else {
      croak "Unknown format $self->{endian_mode}";
   }
    # FIXME: Verify the Endian mode.

    # The next four bytes contain the number of parameters recorded.
   $nparams = $self->read_input (4, 0, "l", "number parameters"); 

    # Read the parameter descriptions: name, units, type, size.
   $blksize = 0;
   foreach my $ii (0 .. $nparams-1) {
      $params[$ii] = $self->read_descriptor ($ii);
      $params[$ii]->{offset} = $blksize;
      $blksize += $params[$ii]->{size};
   }

    # Construct the format string to be used use with 'unpack'
    # to unpack the data that comprises a log file data block.
   $format = '';
   foreach my $param (@params) {

       # See Trick source file sim_services/include/parameter_types.h
      my $ref = $self->{logfile_format}->{$param->{type}};
      if ((! defined $ref) ||
          ((defined $ref) &&
           (defined $ref->{size}) &&
           ($ref->{size} != $param->{size}))) {
         croak "Unknown format $param->{type} $param->{size} " .
               "for $param->{name}";
      }

      $format .= $ref->{format};
      if (! defined $ref->{size}) {
         $format .= sprintf "%d", $param->{size};
      }
   }

    # Save the block size, parameter list, and block format.
   @$self{qw(blksize params format)} = ($blksize, [@params], $format);

    # Interpret the header.
   $self->{nvars} = scalar @{$self->{params}};
   $self->{vars}  = [ map {$_->{name}}  @{$self->{params}} ];
   $self->{vals}  = { map {($_, undef)} @{$self->{vars}}   };
}
   

# $logfile->dup_log_info - Generate a log file info based on another log file
sub dup_log_info($$) {
   my ($self, $other) = @_;
   my ($endian_mode, $blksize, $other_params) =
      @$other{qw(endian_mode blksize params)};
   my $nparams = scalar @$other_params;
   my @params = [];

   foreach my $ii (0 .. $nparams-1) {
      my $param = $other_params->[$ii];
      $params[$ii] = { %{$other_params->[$ii]} };
   }

   $self->{endian_mode} = $endian_mode;
   $self->{blksize}     = $blksize;
   $self->{params}      = [ @params ];
}
   

# $logfile->write_log_header - Write the log header.
sub write_log_header($) {
   my ($self) = @_;
   my ($file, $fname, $params) = @$self{qw(handle file params)};
   my $nparams = scalar @$params;

   tell($file) == 0 or croak "File $fname not positioned correctly";

    # The first ten bytes of the file indicate the file Endian mode.
   $self->write_output ($self->{endian_mode}, 10, "a10", "endian mode");

    # The next four bytes contain the number of parameters recorded.
   $self->write_output ($nparams, 4, "l", "number parameters");

    # Write the parameter descriptions: name, units, type, size.
   foreach my $ii (0 .. $nparams-1) {
      $self->write_descriptor ($ii);
   }
}


# $logfile->read_descriptor - Read a parameter descriptor from the log header.
# Returns hash reference to descriptor data.
sub read_descriptor ($$) {
   my ($self, $ii) = @_;
   my $param_id = "param $ii";
   my ($name, $units, $type, $size, $len);

   $len   = $self->read_input (4,    0, "l",     "$param_id name length");
   $name  = $self->read_input ($len, 0, "a$len", "$param_id name");
   $len   = $self->read_input (4,    0, "l",     "$param_id units length");
   $units = $self->read_input ($len, 0, "a$len", "$param_id units");
   $type  = $self->read_input (4,    0, "l",     "$param_id type");
   $size  = $self->read_input (4,    0, "l",     "$param_id size");

   return {name  => $name,
           units => $units,
           type  => $type,
           size  => $size};
}


# $logfile->write_descriptor - Write a parameter descriptor to the log header.
# Returns hash reference to descriptor data.
sub write_descriptor ($$) {
   my ($self, $ii) = @_;
   my $param_id = "param $ii";
   my $param = $self->{params}->[$ii];
   my ($name, $units, $type, $size) = @$param{qw(name units type size)};
   my $len;

   $len = length $name;
   $self->write_output ($len,   4,    "l",     "$param_id name length");
   $self->write_output ($name,  $len, "a$len", "$param_id name");

   $len = length $units;
   $self->write_output ($len,   4,    "l",     "$param_id units length");
   $self->write_output ($units, $len, "a$len", "$param_id units");

   $self->write_output ($type, 4, "l", "$param_id type");
   $self->write_output ($size, 4, "l", "$param_id size");
}

# $logfile->read_input - Read a chunk of data from the log file.
sub read_input ($$$$$) {
   my ($self, $len, $allow_eof, $format, $ident) = @_;
   my ($buff, $chars_read);

   $chars_read = read $self->{handle}, $buff, $len;
   if ($chars_read == 0) {
      if ($allow_eof) {
         return undef;
      } else {
         croak "Unexpected EOF while reading $ident in $self->{fname}\n";
      }

   } elsif ($chars_read < $len) {
      croak "Unexpected EOF while reading $ident in $self->{fname}\n";
   }

   return ($format eq '') ? $buff : unpack $format, $buff;
}

# $logfile->write_output - Write a chunk of data to the log file.
sub write_output ($$$$$) {
   my ($self, $buff, $len, $format, $ident) = @_;
   my ($chars_written);

   if ($format ne '') {
      $buff = pack $format, $buff;
      $len = length $buff;
   } 

   $chars_written = syswrite $self->{handle}, $buff, $len;
   if ($chars_written < $len) {
      croak "Error while writing $ident in $self->{fname}\n";
   }
}


1;
