#=============================================================================
# Notices:
#
# Copyright 2023 United States Government as represented by the Administrator
# of the National Aeronautics and Space Administration.  All Rights Reserved.
#
#
# Disclaimers:
#
# No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
# ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
# TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
# FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
# FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
# SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
# ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
# RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
# RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
# DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
# IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
#
# Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
# LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
# INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
# USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
# ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
# AGREEMENT.
#
#=============================================================================

import curses, logging, os, stat, subprocess, sys, time
from ColorStr import ColorStr

#*****************************************************************************
#Runs a shell command
#*****************************************************************************
def sys_cmd(cmd_str, assert_flag=True):
  cmd_pipe = subprocess.Popen(cmd_str, shell=True, stdout=subprocess.PIPE, universal_newlines=True)
  cmd_res = cmd_pipe.stdout.read()
  if assert_flag:
    assert cmd_pipe.wait() == 0
  else:
    cmd_pipe.wait()
  return cmd_res

#*****************************************************************************
# Write to both the log and std out, with optional color for stdout
#*****************************************************************************
printer = ColorStr()
def tprint(content, color='ENDC', verbosity='INFO', indent=0):
    """
    Utility method for writing something to both log and stdout

    Parameters
    ----------
    line
    String to be written

    color
    Desired color of output string

    verbosity
    How line will be output or logged
    """
    if type(content) is list or type(content) is set:
        for line in content:
            tprint( line, color, verbosity, indent)
        return

    #else:
    line = ("  " * indent) + str(content)
    colorLine = printer.colorstr(line, color)
    sys.stdout.write(colorLine+"\n")
    if verbosity == 'INFO':
        logging.info(line)
    elif verbosity == 'DEBUG':
        logging.debug(line)
    elif verbosity == 'ERROR':
        logging.error(line)
    elif verbosity == 'WARNING':
        logging.warning(line)
    else:
        pass


#*****************************************************************************
# Join 2 path segments, ensuring that one and only one standard separator
# is used between path components.
#*****************************************************************************
def smartJoin( *args):
    path = ""
    sep  = os.path.sep
    for segment in args:
        segment = str( segment )
        if len( segment ) > 0:
            if len(path) == 0:
                path = str(segment)
            else:
                # ensure that path ends with exactly one directory separator
                if path[-1] != sep:
                    path = path + sep
                else:
                    while ( len(path) > 1 ) and ( path[-2:] == ( sep + sep ) ):
                        path = path[:-2]
                # ensure that segment doesn't start with a path separator
                while ( len(segment) > 0 ) and ( segment[0] == sep ):
                    segment = segment[1:]

                path = path + segment
    return path

#*******************************************************************************
# Tests whether a file or directory is writeable
#*******************************************************************************
def isUserWriteable(filepath):
    st = os.stat(filepath)
    return bool(st.st_mode & stat.S_IWUSR)


#*******************************************************************************
# Tests whether a file or directory intended to contain some output can be
# created; returns the full path to that file or directory if only given the
# relative path
#*******************************************************************************
def validateOutputPath(inputname, fileordir, exitOnError=True):

    newFileName = ''
    # Make sure whatever we've been given, we turn it into a full path
    if os.path.dirname(inputname)[0] == '/':  # inputname starts with /
                                             # Full path specified
        newFileName = inputname
    else: # relative path provided from current directory
        newFileName = smartJoin( os.getcwd(), inputname )

    # If given path exists and is writeable (file or directory)
    if os.path.exists(newFileName) and isUserWriteable(newFileName):
        pass

    # If path to file's directory exists and is writeable
    elif fileordir == 'isFile':
        dirname = os.path.dirname(newFileName)
        if not os.path.isdir(dirname):
            try:
               os.mkdir(dirname)
            except:
                tprint ("ERROR: There's a problem creating output directory " +
                          dirname + ", exiting.",
                        'DARK_RED',
                        'ERROR')
                sys.exit(2)

    # If path to directory's parent exists and is writeable
    elif fileordir == 'isDir':
        try:
           os.mkdir(newFileName)
        except:
            tprint ("ERROR: There's a problem creating output directory " +
                      newFileName + ", exiting.",
                    'DARK_RED',
                    'ERROR')
            sys.exit(2)

    # Something went wrong, maybe bad input?
    else:
        tprint("ERROR: Unable to validate requested output file: " +
                 inputname + ", permission issue?",
               'DARK_RED',
               'ERROR')
        if exitOnError is True:
            sys.exit(1)
    return newFileName


#*****************************************************************************
# A progress bar provides information on the progress of long jobs.
#*****************************************************************************
def create_progress_bar(fraction, text):
    text = ' ' + text + ' '
    length = len(text)
    bar = list('[{0:<78}]'.format('=' * min(78, int(round(fraction * 78)))))
    index = (len(bar) - length) / 2
    bar[index : index + length] = text
    return ''.join(bar)

#*****************************************************************************
# parallel_runs
# manages parallel execution of a set of Jobs
#****************************************************************************/
def parallel_runs(jobs, max_concurrent=None, header=None):
    """
    Run jobs, blocking until all have returned.

    Parameters
    ----------
    jobs : iterable of Job
        The jobs to run.
    max_concurrent : int
        The maximum number of jobs to execute simultaneously.
    header : str
        Header text.

    Returns
    -------
    bool
        True if any job failed or was not run.
        False if all jobs completed successfully.
    """
    if not os.environ.get('TERM'):
        tprint(
          'The TERM environment variable must be set when the command\n'
          'line option --quiet is not used. This is usually set by one\n'
          "of the shell's configuration files (.profile, .cshrc, etc).\n"
          'However, if this was executed via a non-interactive,\n'
          "non-login shell (for instance: ssh <machine> '<command>'), it\n"
          'may not be automatically set.', 'DARK_RED','WARNING')
        return True

    num_jobs = len(jobs)
    if max_concurrent is None or max_concurrent < 1:
        max_concurrent = num_jobs

    if header:
        header += '\n'
    else:
        header = ''

    header += (
      'Executing {0} total jobs, running up to {1} simultaneously.\n'
      .format(num_jobs, max_concurrent) +
      'Press CTRL+C to terminate early.\n')

    logging.info(header)

    # Define the meat of this function in an inner function.
    # This inner function will be called via curses.wrapper if
    # status output is enabled. Otherwise, it will be called
    # directly. See below.
    def execute(stdscr=None):

        # stdscr is passed via curses.wrapper
        if stdscr:
            # Turn off the cursor. Not all terminals may support
            # this.
            try:
                curses.curs_set(False)
            except curses.error:
                pass

            # Configure colors. Not all terminals may support
            # this.
            try:
                curses.start_color()
                curses.use_default_colors()
                curses.init_pair(1, curses.COLOR_RED, -1)
                curses.init_pair(2, curses.COLOR_GREEN, -1)
                use_colors = True
            except curses.error:
                use_colors = False

            # Cause getch to be non-blocking. The arrow keys and
            # mouse wheel are used to scroll the pad. We don't
            # want to hang if the user doesn't type anything.
            stdscr.timeout(0)

            # Nothing will be displayed without an initial call
            # to refresh.
            stdscr.refresh()

            # Create a pad for the header. It must have enough
            # lines to contain all the content we intend to
            # write. Text longer than the width wraps, consuming
            # extra lines, so pick a realy big width that isn't
            # likely to cause wrapping. We also need a final
            # additional line for the cursor to end on.
            header_pad = curses.newpad(header.count('\n') + 1, 1000)
            header_pad.addstr(header)

            # Create a pad for the status.
            # The total line count is:
            #   all job status strings
            # + a line for each job name
            # + a blank line after each status string
            # + a final line for the cursor to end on
            status_pad = curses.newpad(
              sum(job.get_status_string_line_count() for job in jobs) +
                2 * len(jobs) + 1,
              1000)

            # The top visible status pad line.
            # Used for scrolling.
            top_line = 0
            header_height = header_pad.getmaxyx()[0]
            status_height = status_pad.getmaxyx()[0]

        while any(job.get_status() in
          [job.Status.NOT_STARTED, job.Status.RUNNING]
          for job in jobs):

            # Start waiting jobs if cpus are available
            waitingJobs = [job for job in jobs
              if job.get_status() is job.Status.NOT_STARTED]

            if waitingJobs:
                available_cpus = max_concurrent - sum(1 for job in jobs
                  if job.get_status() is job.Status.RUNNING)

                for i in range(min(len(waitingJobs), available_cpus)):
                    waitingJobs[i].start()

            # display the status if enabled
            if stdscr:
                status_pad.erase()
                for i, job in enumerate(jobs):
                    # print the name
                    status_pad.addstr('Job {0:{width}d}/{1}: '.format(
                      i + 1, num_jobs, width=len(str(num_jobs))))
                    status_pad.addstr(job.name + '\n', curses.A_BOLD)

                    # print the status string
                    if use_colors:
                        # color the status string
                        status = job.get_status()
                        if status is job.Status.FAILED:
                            color = curses.color_pair(1)
                        elif status is job.Status.SUCCESS:
                            color = curses.color_pair(2)
                        else:
                            color = curses.color_pair(0)
                        status_pad.addstr(
                          job.get_status_string() + '\n\n', color)
                    else:
                        status_pad.addstr(
                          job.get_status_string() + '\n\n')

                # handle scrolling
                while True:
                    key = stdscr.getch()
                    if key == -1:
                        # no input
                        break
                    if key == curses.KEY_UP:
                        top_line -= 1
                    elif key == curses.KEY_DOWN:
                        top_line += 1

                # prevent scrolling beyond the bounds of status_pad
                screen_height, screen_width = stdscr.getmaxyx()
                top_line = max(
                  0,
                  min(top_line,
                      status_height - 2 - (screen_height - header_height)))

                # Resizing the terminal can cause the actual
                # screen width or height to become smaller than
                # what we already got from getmaxyx, resulting
                # in a curses.error in these calls. Note that
                # even calling getmaxyx again right here isn't
                # fool-proof. Resizing is asynchronous (curses
                # responds to it via a signal handler), so the
                # size can always change between when we get it
                # and when we use it. Best to just use what we
                # have and ignore errors.
                try:
                    header_pad.noutrefresh(
                      0, 0, 0, 0, screen_height - 1, screen_width - 1)
                    status_pad.noutrefresh(
                      top_line, 0, header_height, 0,
                      screen_height - 1, screen_width - 1)
                except curses.error:
                    pass
                curses.doupdate()

            # take a nap
            time.sleep(0.1)

    try:
      #  if not self.quiet:
      #      # wrapper takes care of initializing the terminal and
      #      # restores it to a useable state regardless of how
      #      # execute exits (even via exception)
      #  curses.wrapper(execute)
      #  else:
      #      # not using curses, just call execute
        execute()

    except BaseException as exception:
        logging.exception('')
        tprint(
          'An exception occurred. See the log for details.\n\n'
          '    ' + repr(exception) + "\n\n"
          'Terminating all jobs. Please wait for cleanup to finish. '
          'CTRL+C may leave orphaned processes.', 'DARK_RED',
          'ERROR')

        # kill all the jobs
        for job in jobs:
            job.die()
        tprint('All jobs terminated.\n', 'DARK_RED')

    # print summary
    summary = 'Job Summary\n'
    for i, job in enumerate(jobs):
        summary += 'Job {0:{width}d}/{1}: {2}\n{3}\n'.format(
          i + 1, num_jobs, job.name, job.get_status_string(),
          width=len(str(num_jobs)))

    logging.info(summary)

    for job in jobs:
        text, color = {
            job.Status.NOT_STARTED: ('was not run', 'GREY40'),
            job.Status.SUCCESS: ('succeeded', 'DARK_GREEN'),
            job.Status.FAILED: ('failed', 'DARK_RED')
        }[job.get_status()]

        text = job.name + ' ' + text

        # Print the summary status even if self.quiet is True
        tprint(text, color)

    return any(job.get_status() is not job.Status.SUCCESS for job in jobs)
