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

import os
import sys
from VerifModel import VerifModel
from verif_utilities import tprint, parallel_runs
from Job  import Job
from CodeCoverage import CodeCoverage
#*****************************************************************************
# VerifPackage
# Provides all the verification information for an entire package, based on
# a configuration file.
#*****************************************************************************
class VerifPackage:

    def __init__( self, sim_binary, name = '', run_base = '', verif_base = ''):
        self.name       = name
        self.models     = []
        self.sim_binary = sim_binary
        self.run_base   = run_base
        self.verif_base = verif_base



    #*****************************************************************************
    # parse_config_file
    # Reads the configuration file, creating VerifModel instances with the
    # respective information contained therein.
    # Note - creation of the VerifSim, VerifRun, and VerifFile instances cascade
    # from the creation of the VerifModel instances.
    #*****************************************************************************
    def parse_config_file(self, myArgs):

        tprint("Executing regression tests based on definition in " +
                  myArgs.config_file,
               "DARK_CYAN", "INFO")
        exec(compile(open(myArgs.config_file, "rb").read(), myArgs.config_file, 'exec'), globals())

        # If verif_base and run_base are the same, file comparisons will
        # all be comparing the specified file against itself.
        # This would be rather silly.
        make_file_comps = True
        if myArgs.verif_base == myArgs.run_base:
            tprint( "ERROR: verification file locations are identical " +
                    "to run file locations.  No file comparisons will be" +
                    "completed.", 'DARK_RED','ERROR')
            make_file_comps = False

        # If myArgs.cidiff_file is defined, overwrite the myArgs.model
        if myArgs.cidiff_file:
            myArgs.model = "no_model"
            ci_f = open(myArgs.cidiff_file, 'r')
            ci_string = ci_f.read()


        for model_info in verif_sim_list:
            if len(model_info) != 2:
                tprint("ERROR: malformed model configuration: " + model_info,
                       'DARK_RED','ERROR')
                continue

            if myArgs.cidiff_file:
                # Determine the string used to search in the cidiff_file
                if model_info[0] == 'verif' or model_info[0] == 'sims':
                    model_search_string = model_info[0] + '/' + model_info[1][0][0]
                elif model_info[0] == 'docs/Training/Exercises/Solutions/':
                    model_search_string = 'docs/Training/'
                else:
                    model_search_string = model_info[0]
                # search the model string in cidiff_file.
                # if found overwrite the myArgs.model with this model_info[0]
                print(model_search_string)
                if model_search_string in ci_string:
                    myArgs.model = model_info[0]

            # if a specific model is desired, skip if this is not it.
            if myArgs.model and myArgs.model != model_info[0]:
                continue

            model = VerifModel()
            model.parse_model_info (model_info, myArgs, make_file_comps)
            self.models.append(model)

        # if nothing has been added (so models is still empty), there were
        # no matches
        if not self.models:
            if myArgs.cidiff_file:
                tprint ("CI diff file: " + myArgs.cidiff_file + " " +
                        "No changed to the models in the configuration. " +
                        "No sim will be build and run", 'DARK_CYAN', 'INFO')
                sys.exit(0)
            elif myArgs.model:
                tprint("ERROR: unable to find a configuration for "+
                       myArgs.model, 'DARK_RED','ERROR')
                sys.exit(1)
            else:
                tprint("ERROR: Configuration file did not provide any "+
                       "properly configured model descriptions.",
                       'DARK_RED','ERROR')
                sys.exit(1)


    #*************************************************************************
    # report
    # Produce the overall report data.
    #*************************************************************************
    def report( self):
        tprint("Summary of All Sims, All Runs, All Comparisons for "+self.name+
               " package", 'DARK_CYAN')
        for model in self.models:
            model.report()


    #*************************************************************************
    # analyze_data
    # For every VerifRun with a failed data comparison (using md5sum),
    # executes the graphical comparison tool to present the differences
    # in a human-readable form.
    #*************************************************************************
    def analyze_data( self,
                      logdir,
                      file_pattern,
                      num_cpus,
                      run_base = '',
                      verif_base = ''):

        analysis_jobs = [] # list of Job instances
        for sim in [sim for model in self.models
                        for sim in model.sims
                        if any(run.status is run.Status.COMP_FAIL
                          for run in sim.runs)]:
            test_path = os.path.join( sim.full_sim_dir,
                                      run_base)
            verif_path = os.path.join( sim.full_sim_dir,
                                       verif_base)
            #logfile = logdir+"/04_analysis_logging_"+sim.unique_id+".txt"
            command = "$JEOD_HOME/regression/regressionCompare.py "+ \
                      " -r "+verif_path + \
                      " -t "+test_path  + \
                      " -l "+logdir+"/04_data_comp_log_"+sim.unique_id+".txt"+ \
                      " -i "+file_pattern

            job  = Job( "DATA_COMP_"+sim.unique_id,
                        command,
                        logdir+"/04_data_comp_std_"+sim.unique_id+".txt",
                        0)
            analysis_jobs.append( job)

        parallel_runs( analysis_jobs, num_cpus)


    #*************************************************************************
    # code_coverage
    # Produce the code-coverage report on all models tested as part of this
    # package.
    #*************************************************************************
    def code_coverage( coverage_format):
      model_idx = 1
      code_coverage = CodeCoverage( coverage_format)
      for model in models:
        tprint("Code Coverage: %s  ( %d / %d ) ..." % ( model.model_dir,
                                                        model_idx,
                                                        len(models)  ) )
        code_coverage.execute( model.model_dir)
        model_idx += 1
