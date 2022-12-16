#!/usr/bin/perl
################################################################################
# highlight_output.pl
#
# Synopsis:
#   highlight_output.pl [-q(uiet)] [--] command [command arg] ...
#   perl <path>/highlight_output.pl [-q(uiet)] [--] command [command arg] ...
#
# Description:
#   highlight_output.pl runs a command that nominally prints nothing to stderr,
#   and preferrably nothing to stdout either. Output to stderr from the command
#   is captured and printed in colored text. The text color is yellow if the
#   command status is zero but red for non-zero status.
#
# Options:
#   -q or -quiet
#     Quiet mode. The script nominally echos the command to be performed
#     to standard out. Quiet mode disables this echo.
#
# Usage in make:
#   # highlight command hidden but command itself printed:
#   target : dependencies
#      @ highlight_output.pl command_to_make_target
#
#   # highlight command printed; quiet mode prevents annoying double print:
#   target : dependencies
#      highlight_output.pl -q command_to_make_target
#
#   # Wabbit hunting mode (be vewy vewy quiet!):
#   target : dependencies
#      @ highlight_output.pl -q command_to_make_target
# 
################################################################################

use strict;
my ($dont_print_cmd, $pid, $status, $signal, $exit, $code, @command, @errout);

# Parse the command line: [-q[uiet]] [--] command options ...
# Note: Do not use GetOptions here; it can eat a -q that belongs to the command.
$dont_print_cmd = 0;
if ($ARGV[0] =~ /^-?-q(uiet)?$/) {
   $dont_print_cmd = 1;
   shift @ARGV;
}
# Eat a leading '--' in case someone put that in to stop GetOptions processing.
if ($ARGV[0] eq '--') {
   shift @ARGV;
}

# The remaining options are the command to be performed plus arguments to it.
@command = @ARGV;

# Unbuffer standard out, as best as perl can do that.
$| = 1;

# Print the command unless the quiet option is enabled.
print +(join ' ', @command), "\n" unless $dont_print_cmd;

# Open a pipe. Standard error from the command will be sent to this pipeline.
pipe FROMCMD, CMDERR
   or die "Can't create pipe";

# Fork and split processing based on the fork().
$pid = fork();

# Undefined pid => fork failed.
if (! defined $pid) {
   die "Can't fork";

# pid == 0 => in child.
# Execute the command, with stderr redirected to the pipeline created above.
} elsif ($pid == 0) {
   close FROMCMD;
   close STDERR;
   open STDERR, '>&', *CMDERR
      or die "Can't redirect STDERR";
   exec @command;
   die "Can't exec " . +(join ' ', @command);
}

# From here on were back in the parent (Non-zero pid).

# Collect the error output from the child process.
close CMDERR;
@errout = <FROMCMD>;
close FROMCMD;

# Wait for the child process to complete and get it's exit status.
wait;
$status = $?;

# Split the status into the signal status and exit code.
$signal = $status & 255;
$exit   = $status >> 8;

# Print colorized error output from the child process to standard error:
# Status = 0:  Make error output yellow. 
# Status != 0: Make error output red. 
# Signaled:    Add blinking "Aborted".
if ((scalar @errout) || ($signal != 0)) {
   my $color = ($status == 0) ? "\e[33m" : "\e[31m";
   foreach (@errout) {
      s/^/$color/;
      s/$/\e[00m/;
   }
   push @errout, "\e[05mAborted\e[00m\n" if $signal != 0;
   map {print STDERR $_} @errout;
}

# Exit with status 255 for commands that exited catastrophically,
# exit code of the command for non-catastrophic exits.
$code = ($signal != 0) ? 255 : $exit;
exit $code;
