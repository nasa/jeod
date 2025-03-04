#=============================================================================
# Notices:
#
# Copyright 2025 United States Government as represented by the Administrator
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

#!/usr/bin/bash

import os, glob
from enum import Enum
from VerifFile import VerifFile
from verif_utilities import tprint
#*****************************************************************************
# VerifRun
# Provides all the verification information about any given run.  Exists as an
# element inside a VerifSim
#*****************************************************************************
class VerifRun:

    Status = Enum( 'Status', 'NOT_STARTED RUNNING RUN_FAIL \
                              RUN_SUCCESS COMP_FAIL SUCCESS')
    def __init__(self, full_sim_dir, run_dir, run_command_preamble):
        self.status       = self.Status.NOT_STARTED
        self.full_sim_dir = full_sim_dir
        self.run_command_preamble = run_command_preamble
        self.run_dir     = run_dir # Name of the RUN directory
        self.run_command = ''      # The command to execute this run.
        self.run_base    = ''      # location wrt sim directory of the directory
                                   # containing the runs
        self.verif_base  = ''      # location wrt sim directory of the directory
                                   # containing the verification/regression data
        self.verif_files = []      # VerifFile objects
        self.outcome = 0
        self.unique_id = self.full_sim_dir.replace("/", "__")+"__"+self.run_dir
        self.logName = ''

    #*************************************************************************
    # parse_run_info
    # Parses the run information passed up from VerifSim
    #*************************************************************************
    def parse_run_info( self, run_info, myArgs, make_file_comps):
        if not run_info:
            tprint( "ERROR: run information not provided to "+
                    "parse_run_info method", 'DARK_RED' )
            return False
        if len(run_info) < 2 or len(run_info) > 3:
            tprint("ERROR: malformed run configuration: " + run_info,
                   'DARK_RED')
            return False

        # Grab the outcome if it is specified, this is easy.
        if len(run_info) == 3:
            self.outcome = int(run_info[2])

        self.logName = myArgs.logdir+"/03_run_info_"+self.unique_id+".txt"

        self.run_base = myArgs.run_base
        self.verif_base = myArgs.verif_base

        path_wrt_sim = os.path.join( self.run_base,
                                     self.run_dir)

        # Construct the run-command
        self.run_command = self.run_command_preamble + self.run_dir + \
                       '/input.py; ret=$?; popd; exit $ret'

        # If file comparisons are trivial or not specified, don't add any:
        if not make_file_comps or not run_info[1]:
            return True
        #else:

        #run_info[1] could be a list or a singleton:
        if isinstance( run_info[1], list):
            for file_ in run_info[1]:
                self.add_file_comparisons( file_)
        else: # there's just one name
            self.add_file_comparisons( run_info[1])
        # else: no action, no comparisons for this run

        return True

    #*************************************************************************
    # add_file_comparisons
    # Add instances of VerifFile to this instance of VerifRun
    #*************************************************************************
    def add_file_comparisons( self, filename):

        # filename may include glob characters (e.g. "RUN_*").
        # Construct the full path to allow expansion of the glob and process
        # all matches.
        for full_verif_file in glob.glob( os.path.join( self.full_sim_dir,
                                                        self.verif_base,
                                                        self.run_dir,
                                                        filename)):

            # extract the the glob-expanded filenames one at a time from
            # the set of glob-expanded full-paths
            # (basename extracts the last piece of the full-path)
            verif_file = os.path.basename( full_verif_file)
            self.verif_files.append( VerifFile( os.path.join( self.full_sim_dir,
                                                              self.run_base,
                                                              self.run_dir,
                                                              verif_file),
                                                os.path.join( self.full_sim_dir,
                                                              self.verif_base,
                                                              self.run_dir,
                                                              verif_file)))


    #*************************************************************************
    # execute
    # execute the running of this scenario
    #*************************************************************************
    def execute(self):
        outcome = os.system( self.run_command)
        if os.system( self.run_command) == self.outcome:
            self.status = self.Status.RUN_SUCCESS
        else:
            self.status = self.Status.RUN_FAIL

    #*************************************************************************
    # compare_data
    # execute the comparisons of all instances of VerifFile
    #*************************************************************************
    def compare_data(self):
        if self.status != self.Status.RUN_SUCCESS:
            tprint( "ERROR: called compare_data on a non-successful run.",
                    'DARK_RED')
            return # with existing status.

        for comp in self.verif_files:
            comp.compare_data()

        if any( comp.status is not comp.Status.SUCCESS
                for comp in self.verif_files):
            self.status = self.Status.COMP_FAIL
        else:
            self.status = self.Status.SUCCESS


    #*************************************************************************
    # report
    # Produce the run-specific report data.
    #*************************************************************************
    def report( self):

        color = "DARK_RED"
        if self.status is self.Status.SUCCESS:
            color = "GREEN"
        elif self.status is self.Status.COMP_FAIL:
            color = "DARK_CYAN"
        elif self.status is self.Status.NOT_STARTED:
            color = "DARK_YELLOW"
        tprint("        Run status: "+self.status.name+"  "+self.run_dir, color)
        if self.status is self.Status.RUN_FAIL:
            tprint("      See log file {0}".format(self.logName))

        for comp in self.verif_files:
            comp.report()
