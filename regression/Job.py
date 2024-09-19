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

import logging, os, subprocess, time
from enum import Enum
class Job(object):
    """
    Executes a given command as a subprocess and provides methods
    for getting status information.
    """
    Status = Enum( 'Status', 'NOT_STARTED RUNNING SUCCESS FAILED')
#    Status = variable_server._create_enum(
#      'Status', ['NOT_STARTED', 'RUNNING', 'SUCCESS', 'FAILED'])

    def __init__(self, name, command, log_file, expected_exit_status=0):
        """
        Initialize this instance.

        Parameters
        ----------
        name : str
            The name of this job.
        command : str
            The command to execute when start() is called.
        log_file : str
            The file to which to write log information.
        """
        self.name = name
        self._command = command
        self._log_file_name = log_file
        self._log_file = None
        self._process = None
        self._start_time = None
        self._stop_time = None
        self._exit_status = None
        self._expected_exit_status = expected_exit_status

    def start(self):
        """
        Execute this job.
        """
        logging.debug('Executing command: ' + self._command)
        self._start_time = time.time()
        self._log_file = open(self._log_file_name, 'w', buffering=16)
        self._log_file.write('Output file for Command : {0}\n'.format(self._command))
        self._log_file.flush()
        self._process = subprocess.Popen(
          self._command, executable='/bin/bash', stdout=self._log_file, stderr=self._log_file,
          stdin=open(os.devnull, 'r'), shell=True, preexec_fn=os.setsid,
          close_fds=True)

    def get_status(self):
        """
        Get the status.

        Returns
        -------
        Status
            Status.NOT_STARTED
                This job has yet to be started.
            Status.RUNNING
                This job is running.
            Status.SUCCESS
                This job completed with an exit status of zero.
            Status.FAILED
                This job completed with a non-zero exit status.
        """
        if self._process is None:
            return self.Status.NOT_STARTED

        self._exit_status = self._process.poll()
        if self._exit_status is None:
            return self.Status.RUNNING

        if self._stop_time is None:
            self._log_file.close()
            self._stop_time = time.time()

        return self.Status.SUCCESS if self._exit_status is self._expected_exit_status else self.Status.FAILED

    def get_expected_exit_status(self):
        return self._expected_exit_status

    def get_exit_status(self):
        return self._exit_status

    def get_status_string_line_count(self):
        """
        Get the constant number of lines in the status string.

        Subclasses should override this if they add lines to the status string.

        Returns
        -------
            The number of lines in the status string.
        """
        return 1

    def get_status_string(self):
        """
        Get a string containing status information.

        -------
        str
            Details of this job's progress.
        """
        status = self.get_status()
        if status is self.Status.NOT_STARTED:
            return self._not_started_string()
        if status is self.Status.RUNNING:
            return self._running_string()
        if status is self.Status.SUCCESS:
            return self._success_string()
        return self._failed_string()

    def _not_started_string(self):
        """
        Get a string to display before this Job has been started.

        Returns
        -------
        str
            A string to be displayed when in the NOT_STARTED state.
        """
        return 'Not started yet'

    def _running_string(self):
        """
        Get a string to display while this Job is running.

        Returns
        -------
        str
            A string to be displayed when in the RUNNING state.
        """
        return 'Elapsed Time: {0:7.1f} sec'.format(
          time.time() - self._start_time)

    def _success_string(self):
        """
        Get a string to display when this Job has finished successfully.

        Returns
        -------
        str
            A string to be displayed when in the SUCCESS state.
        """
        return self._done_string()

    def _failed_string(self):
        """
        Get a string to display when this Job has failed.

        Returns
        -------
        str
            A string to be displayed when in the FAILED state.
        """
        return self._done_string()

    def _done_string(self):
        """
        This class uses the same string for SUCCESS and FAILED, but
        subclasses may differentiate.

        Returns
        -------
        str
            A string to be displayed when this Job is done.
        """
        return 'Total Time:   {0:7.1f} sec'.format(
          self._stop_time - self._start_time)


    def report( self ):
        reportStr = "Job name:" + self.name
        reportStr = reportStr + ", exit status:" + str( self._expected_exit_status )
        reportStr = reportStr + "\n"
        return reportStr


    def die(self):
        """
        Immediately kill this Job and all processes in its process group
        by sending them the SIGKILL signal.
        """
        try:
            os.killpg(os.getpgid(self._process.pid), signal.SIGKILL)
            self._process.wait()
            self._log_file.close()
        except:
            pass
