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
from VerifRun import VerifRun
from verif_utilities import tprint
#*****************************************************************************
# VerifSim
# Provides all the verification information about any given sim.  Exists as an
# element inside a VerifModel
#*****************************************************************************
class VerifSim:

    Status = Enum( 'Status', 'NOT_STARTED NO_EXECUTIVE BUILT RUN_FAIL SUCCESS')

    def __init__(self, model_dir):
        self.model_dir = model_dir  # model directory wrt to top level of project
        self.sim_dir = ""           # Path to sim directory wrt model_dir
        self.full_sim_dir = ""      #Path to sim dir from top level
        self.runs = []              # VerifRun objects
        self.commands = []
        self.status = self.Status.NOT_STARTED
        self.compile_command = ''
        self.unique_id = ''
        self.logName = ''

    #*************************************************************************
    # parse_sim_info
    # Parses the sim information passed up from VerifModel
    #*************************************************************************
    def parse_sim_info( self, sim_info, myArgs, make_file_comps):
        if not sim_info:
            tprint( "ERROR: sim information not provided to "+
                    "parse_sim_info method", 'DARK_RED' )
            return False
        if len(sim_info) != 2:
            tprint("ERROR: malformed sim configuration: " + sim_info,
                   'DARK_RED')
            return False

        self.sim_dir = sim_info[0]
        self.full_sim_dir = os.path.join( self.model_dir,
                                          self.sim_dir)
        self.unique_id = self.full_sim_dir.replace("/", "__")
        self.compile_command = 'pushd ' + self.full_sim_dir + '; ' + \
                               myArgs.build_command + '; ' +\
                               'ret=$?; popd; exit $ret'

        run_command_preamble = 'pushd ' + self.full_sim_dir + '; ' + \
                               myArgs.prerun_cmd + \
                               './'+myArgs.sim_binary + \
                               ' '+myArgs.run_base+'/'

        self.logName = myArgs.logdir+"/02_build_info_"+self.unique_id+".txt"

        for run_info in sim_info[1]:
            # The first value in sim_info is the run name, but this may
            # include glob characters (e.g. "RUN_*").
            # Expand the glob, and process all matches.
            if not run_info:
                tprint( "ERROR: run information not available ",'DARK_RED' )
                continue
            if len(run_info) < 2 or len(run_info) > 3:
                tprint("ERROR: malformed run configuration: " + run_info,
                       'DARK_RED')
                continue
            for path in glob.glob( os.path.join( self.full_sim_dir,
                                                 myArgs.run_base,
                                                 run_info[0])):
                # get the glob-expanded run directory
                run_dir = os.path.basename( path)
                run = VerifRun( self.full_sim_dir,
                                run_dir,
                                run_command_preamble )
                if run.parse_run_info( run_info, myArgs, make_file_comps):
                    self.runs.append(run)
        return True

    #*************************************************************************
    # report
    # Produce the sim-specific report data.
    #*************************************************************************
    def report( self):

        color = "DARK_RED"
        if self.status is self.Status.SUCCESS:
            color = "GREEN"
        elif self.status is self.Status.RUN_FAIL:
            color = "DARK_CYAN"
        tprint("    Sim status: "+self.status.name+"  "+self.sim_dir, color)
        if self.status is self.Status.NO_EXECUTIVE:
            tprint("      See log file {0}".format(self.logName))

        for run in self.runs:
            run.report()
