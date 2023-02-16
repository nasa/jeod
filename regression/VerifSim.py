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

        for run in self.runs:
            run.report()

