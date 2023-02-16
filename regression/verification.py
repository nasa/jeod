#!/usr/bin/env python3
#!/usr/bin/bash

import sys, os
import logging
from argparse import ArgumentParser
from Job import Job
from VerifPackage import VerifPackage
from CodeCoverage import CodeCoverage
from verif_utilities import sys_cmd, tprint, parallel_runs, validateOutputPath

startupDir = os.getcwd()
argv = sys.argv[1:]

def getArgs():
    parser = ArgumentParser(description='Build, run, and compare all \
    validation/verification sims as specified in verif_sim_list.py. \
    Returns zero if all tests pass, nonzero otherwise.')

    parser.add_argument( "-a",
                         "--analyze",
                         dest="analyze_data_deltas",
                         action="store_true",
                         default=False,
                         help="Instruction to generate data-delta plots")

    parser.add_argument( "--analysis_pattern",
                         dest="analysis_pattern",
                         default="log_.*\..*[kv]",
                         help="Regex for which data files to use for data comparisons")

    parser.add_argument( "-b",
                         "--build-command",
                         dest="build_command",
                         default="$TRICK_HOME/bin/trick-CP",
                         help="Command to build the simulation.")

    parser.add_argument( "--build-none",
                         dest="build_none",
                         action="store_true",
                         default=False,
                         help="Instruction to build none of the identified sims")

    parser.add_argument( "--build-all",
                         dest="build_all",
                         action="store_true",
                         default=False,
                         help="Instruction to build all of the identified sims")

    parser.add_argument( "-c",
                         "--cpus",
                         dest="cpus",
                         default=3,
                         type=int,
                         help="Number of simultaneous cpu's to use when building/running in parallel. Defaults to 3.")

    parser.add_argument( "-f",
                         "--config-file",
                         dest="config_file",
                         default=str(os.path.join("regression","verif_sim_list.py")),
                         help="file providing the verification configuration data.")

    parser.add_argument( "-L",
                         "--log-dir",
                         dest="logdir",
                         default=str(os.getcwd())+"/runLogs/",
                         help="Path to dir where all simulation logs will be written"  )

    parser.add_argument( "-m",
                         "--model",
                         dest="model",
                         metavar="model_directory",
                         default="",
                         help="Specify the directory of the model to test. Default is to test all the models listed in the given config file.")

    parser.add_argument( "-n",
                         "--name",
                         dest = "name",
                         default = "",
                         help="Name of the VerifPackage")

    parser.add_argument( "-p",
                         "--repo-path",
                         dest="repo_path",
                         default=str(os.getcwd()),
                         help="path to top level of repo, defaults to this directory" )
    parser.add_argument( "--prerun",
                         dest = "prerun_cmd",
                         default = "",
                         help="Instruction to be sent prior to calling the sim executable.  Note - if this is a standalone command it must be completed with semi-colon; this is not needed if it is a prepend to the executable.")

    parser.add_argument( "-r",
                         "--run-base",
                         dest="run_base",
                         default="",
                         help="path to location of RUN* directories from top level of sim." )

    parser.add_argument( "--run-none",
                         dest="run_none",
                         action="store_true",
                         default=False,
                         help="flag to block execution of sims; script ends "+\
                              "with build phase.")

    parser.add_argument( "-s",
                         "--sim-binary",
                         dest="sim_binary",
                         default='S_main_{0}.exe'.format('4.8_x86_64' if 'TRICK_HOST_CPU' not in os.environ else os.environ['TRICK_HOST_CPU']),
                         help="Name of sim compiled executable file."  )

    parser.add_argument( "-v",
                         "--verif-base",
                         dest="verif_base",
                         default="verif_data",
                         help="path to location of RUN* directories that contain the verification data, relative to top level of sim.")

    parser.add_argument( "-w",
                         "--coverage-html",
                         dest="coverage_html",
                         action="store_true",
                         default=False,
                         help="Enable the code coverage analysis and output the result in html format for the given models by option '--model' or for all models. Default is disabled.")

    parser.add_argument( "-x",
                         "--coverage-xml",
                         dest="coverage_xml",
                         action="store_true",
                         default=False,
                         help="Enable the code coverage analysis and output the result in xml format for the given models by option '--model' or for all models. Default is disabled.")

    parser.add_argument( "--cidiff-file",
                         dest="cidiff_file",
                         default="",
                         help="CI diff file. if this argument is defined, the models to be tested will be overwritten by the information in the file.")

    return parser, parser.parse_args()


def no_path_error( path ):
    tprint("ERROR: Can't find path: " + str(path) + " !")
    sys.exit(1)



#*************************************************************************
# The main function is now defined:
#*************************************************************************
def main():
    """

    Returns
    -------
    bool
        True if any build, run, or comparison failed.
        False if all elements completed successfully.
    """
    # Grab info out of argparser
    myParser, myArgs = getArgs()

    # cd to top level of repo.
    #FIXME check validity
    os.chdir(myArgs.repo_path)

    # Validate requested output files, ensure we have a full path
    myArgs.config = validateOutputPath(myArgs.config_file, 'isFile')
    myArgs.logdir = validateOutputPath(myArgs.logdir, 'isDir')

    # Set up logging of the output of this script.
    logging.basicConfig(filename = myArgs.logdir+"/01_top_level.txt",
                        filemode='w',
                        level=logging.DEBUG)


    # generate the output format of code coverage
    # 0: no coverage report
    # 1: report in xml only
    # 2: report in html only
    # 3: report in xml and html
    code_coverage_format = 0
    if myArgs.coverage_xml:
        code_coverage_format += 1
    if myArgs.coverage_html:
        code_coverage_format += 2

    # Create a package isntance and process the config file.
    verif_package = VerifPackage( sim_binary = myArgs.sim_binary,
                                  name       = myArgs.name,
                                  run_base   = myArgs.run_base,
                                  verif_base = myArgs.verif_base)

    verif_package.parse_config_file( myArgs)


    header = (
      'Top level project path: ' + myArgs.repo_path + '\n' +
      '  Config file:\n' +
      '    ' + myArgs.config_file + ' \n'
      '  Logging for this script:\n'
      '    ' +  myArgs.logdir + '\n'
      '  stdout/stderr for the simulation builds:\n'
      '    ' +  myArgs.logdir + '\n')

    tprint (header, 'DARK_CYAN')

  ############################################################################
  # Create a list of sims that need building, and those which do not.
  # Flags passed in with arguments:
  # - build_none. Do not build even in cases where executable files do not exist
  # - build-all.  Build (or rebuild) all sims, overwriting existing executables
  # - <default>.  Use existing executables where they exist, build sims where
  #                executables do not exist
  ############################################################################
    all_sims = [sim for model in verif_package.models
                    for sim in model.sims]
    build_sims = []
    no_build_sims = []
    if myArgs.build_none:
        no_build_sims = all_sims
    elif myArgs.build_all:
        build_sims = all_sims
    else:
        for sim in all_sims:
            if os.path.isfile( os.path.join( sim.full_sim_dir,
                                             myArgs.sim_binary)):
                no_build_sims.append(sim)
            else:
                build_sims.append(sim)

  ############################################################################
  # Build all sims identified in the build_sims list in parallel
  # Set the status of *every* sim (including those not in the build_sims
  #   list) according to whether it has an executable.
  ############################################################################
    exec_jobs = [ Job ( "Build "+ sim.full_sim_dir,
                        "pushd $JEOD_HOME/"+sim.full_sim_dir+"; "+myArgs.build_command+"; ret=$?; popd; exit $ret",
                        myArgs.logdir+"/02_build_info_"+sim.unique_id+".txt",
                        0)
                  for sim in build_sims]
    # execute the build commands in parallel
    parallel_runs( exec_jobs, myArgs.cpus)

    # Check on which sims have an executable and adjust the status accordingly.
    for sim in all_sims:
        if os.path.isfile( os.path.join( sim.full_sim_dir,
                                         myArgs.sim_binary)):
            sim.status = sim.Status.BUILT
        else:
            sim.status = sim.Status.NO_EXECUTIVE



  ############################################################################
  # - Generate a set of run-commands for all runs associated with those sims
  #   that now have an executable.  Skip sims that failed to build.
  # - Create a list of executable jobs,  with each job representing one run and
  # - Execute them in parallel.
  # - Adjust the status of the VerifRun instances according to the status of
  #   the completed corresponding Job instances.
  # - launch the data comparison on all runs that have completed.
  ############################################################################
    all_runs = [run for sim in all_sims
                    for run in sim.runs]
    if myArgs.run_none:
        all_exec_runs = []
    else:
        all_exec_runs = [run for sim in all_sims
                             if sim.status is sim.Status.BUILT
                             for run in sim.runs]

    # Create the list of executable jobs
    exec_jobs = [ Job( run.run_dir,      # job-name
                       run.run_command,  # job-command
                       myArgs.logdir+"/03_run_info_"+run.unique_id+".txt",
                       run.outcome)     # expected exec-return
                  for run in all_exec_runs]
    # execute them in parallel
    parallel_runs( exec_jobs, myArgs.cpus)

    # assign the status exec_jobs and the corresponding all_exec_runs have
    # matching indices so can use enumerate capability.
    for ii,job in enumerate(exec_jobs):
        run = all_exec_runs[ii]
        if job.get_status() is job.Status.NOT_STARTED:
            run.status = run.Status.NOT_STARTED
        elif job.get_status() is job.Status.FAILED:
            run.status = run.Status.RUN_FAIL
        elif job.get_status() is job.Status.SUCCESS:
            run.status = run.Status.RUN_SUCCESS
            # perform the data verification on all successful runs
            # note - this will push the status to either COMP_FAIL or SUCCESS
            run.compare_data()



  ############################################################################
  # Start collecting the metadata
  # Need counts of what has been succesful and what has not.
  # Work from fine detail (file comparison success) to coarse (model success)
  ############################################################################

    ####
    # Generate 4 counts of all file comparisons that:
    # - were known to the VerifPackage
    # - were compared and matched with baseline data
    # - were compared and failed to match with baseline data
    # - were not compared -- because of an earlier fail in the sim or run
    ####
    file_comparisons =[comp for run in all_runs
                            for comp in run.verif_files]
    count_comp = len(file_comparisons)
    count_comp_success = len([comp for comp in file_comparisons
                                    if comp.status is comp.Status.SUCCESS])
    count_comp_failures = len([comp for comp in file_comparisons
                                    if comp.status is comp.Status.FAIL])
    count_comp_noexec = count_comp - count_comp_success - count_comp_failures

    ####
    # Generate a count of all runs that -
    # - are known to the VerifPackage
    # - ran and generated matching regression data
    # - ran and generated non-matching regression data
    # - failed to run
    # - were not attempted -- because of a failed sim-build
    ####
    count_run = len(all_runs)
    count_run_success = len ([run for run in all_exec_runs
                                  if run.status is run.Status.SUCCESS])
    count_run_comp_fail = len ([run for run in all_exec_runs
                                    if run.status is run.Status.COMP_FAIL])
    count_run_failures = len ([run for run in all_exec_runs
                                   if run.status is run.Status.RUN_FAIL])
    count_run_no_start = len ([run for run in all_exec_runs
                                   if run.status is run.Status.NOT_STARTED])
    count_run_noexec = count_run - len (all_exec_runs)

    ####
    # Generate a count of all sims that -
    # - are known to the VerifPackage
    # - built as part of the process
    # - had pre-existing executables and were not rebuilt
    # - failed to build
    # - were blocked from rebuilding and had no pre-existing executable
    # - built and ran completely successfully
    # - built but had a failure in one of the runs or file comaprisons
    ####
    count_sim = len(all_sims)
    # before finalizing sim status, count the number of sims with the interim
    # BUILT status.
    count_sim_build_success = len ([sim for sim in build_sims
                                        if sim.status is sim.Status.BUILT])
    count_sim_build_preexisting = len ([sim for sim in no_build_sims
                                            if sim.status is sim.Status.BUILT])
    count_sim_build_failures = len ([sim for sim in build_sims
                                         if sim.status is sim.Status.NO_EXECUTIVE])
    count_sim_build_blocked = len ([sim for sim in no_build_sims
                                        if sim.status is sim.Status.NO_EXECUTIVE])

    # propagate run-failure flags up to the BUILT sims
    # a sim can only indicate succesful regression testing if all of its
    # runs were successful.
    for sim in all_sims:
        if sim.status is sim.Status.BUILT:
            if myArgs.run_none:
                sim.status = sim.Status.SUCCESS
            elif all( run.status is run.Status.SUCCESS
                      for run in sim.runs):
                sim.status = sim.Status.SUCCESS
            else:
                sim.status = sim.Status.RUN_FAIL
    # Now count the instances of these 2 final status levels
    count_sim_success =  len ([sim for sim in all_sims
                                  if sim.status is sim.Status.SUCCESS])
    count_sim_run_fail = len ([sim for sim in all_sims
                                    if sim.status is sim.Status.RUN_FAIL])



    ####
    # Generate a count of all models that -
    # - are known to the VerifPackage
    # - were completely successful
    ####
    count_model = len(verif_package.models)

    # Propagate sim-failure flags up to the models
    # a model can only indicate succesful regression testing if all of its
    # sims were successful.
    for model in verif_package.models:
        if all( sim.status is sim.Status.SUCCESS
                for sim in model.sims):
            model.status = model.Status.SUCCESS
        else:
            model.status = model.Status.SIM_FAIL

    count_model_success =  len ([model for model in verif_package.models
                                       if model.status is model.Status.SUCCESS])


  ############################################################################
  # Create the report and print out the metadata.
  # - The report descends through the layers adding information at each layer.
  # - The summary goes to stdout
  ############################################################################
    verif_package.report()

    # Print a summary
    tprint( "\nSUMMARY:", 'DARK_CYAN' )
    tprint( "Of " + str(count_model) + " models:",
            "DARK_CYAN")
    if count_model == count_model_success:
        tprint( "all tested successfully\n",
                'GREEN',
                indent=1)
    else:
        # Model-level data:
        tprint( str(count_model_success) + " tested successfully",
                'GREEN',
                indent = 1)
        tprint( str(count_model - count_model_success) + " failed\n",
                'DARK_RED',
                indent = 1)

        # Sim-level data:
        tprint( "Of " + str(count_sim) + " sims:",
                "DARK_CYAN")
        tprint( str(count_sim_success) + " tested successfully",
                'GREEN',
                indent = 1)
        tprint( str(count_sim - count_sim_success) + " failed",
                'DARK_RED',
                indent = 1)
        count_fail_build = count_sim_build_failures + count_sim_build_blocked
        if count_fail_build == 0:
            tprint( "all sims had executables",
                    'GREEN',
                    indent = 2)
            tprint( str(count_sim_build_success) + " built succesfully",
                    'GREEN',
                    indent = 3)
            tprint( str(count_sim_build_preexisting) + " were already compiled",
                    'DARK_YELLOW',
                    indent = 3)
        else:
            tprint( str(count_fail_build) + " had no executable",
                    'DARK_YELLOW',
                    indent = 2)
            tprint( str(count_sim_build_failures) + " failed to build",
                    'DARK_RED',
                    indent = 3)
            tprint( str(count_sim_build_blocked) +
                    " were blocked with no existing compilation",
                    'DARK_YELLOW',
                    indent = 3)


        tprint( str(count_sim_run_fail) + " had run failures\n",
                'DARK_RED' if count_sim_run_fail > 0 else 'DARK_YELLOW',
                indent = 2)

        # Run-level data:
        tprint( "Of " + str(count_run) + " runs:",
                "DARK_CYAN")
        tprint( str(count_run_noexec) + " were not attempted (no executable)",
                'DARK_YELLOW',
                indent = 1)
        tprint( str(count_run_success) + " tested successfully",
                'GREEN',
                indent = 1)

        count_fail = count_run - count_run_success - count_run_noexec
        tprint( str(count_fail) + " failed",
                'DARK_RED' if count_fail > 0 else 'DARK_YELLOW',
                indent = 1)
        if (count_fail > 0):
            tprint( str(count_run_no_start) + " were not started",
                    'DARK_YELLOW',
                    indent = 2)
            tprint( str(count_run_failures) + " failed to run",
                    'DARK_RED',
                    indent = 2)
            tprint( str(count_run_comp_fail) + " failed data comparison\n",
                    'DARK_RED',
                    indent = 2)
        else:
            tprint( "")

        tprint( "Of " + str(count_comp) + " comparisons:",
                "DARK_CYAN")
        tprint( str(count_comp_noexec) + " were not tested (run not completed)",
                'DARK_YELLOW',
                indent = 1)
        tprint( str(count_comp_success) + " matched",
                'GREEN',
                indent = 1)

        tprint( str(count_comp_failures) + " failed to match\n",
                'DARK_RED' if count_comp_failures > 0 else 'DARK_YELLOW',
                indent = 1)

  ############################################################################
  # Add optional calls to code-coverage and data analysis
  ############################################################################
    if myArgs.analyze_data_deltas:
        tprint( "ANALYZING DATA",'DARK_CYAN')
        if count_model == count_model_success:
            tprint( "All comparisons agreed",
                    'GREEN',
                    indent = 1)
        elif count_comp_failures == 0:
            tprint( "All completed comparisons agreed",
                    'DARK_YELLOW',
                    indent = 1)
        else:
            verif_package.analyze_data( myArgs.logdir,
                                        myArgs.analysis_pattern,
                                        myArgs.cpus,
                                        myArgs.run_base,
                                        myArgs.verif_base)


    #Do the code coverage
    #FIXME this has not been tested.
    if 0<code_coverage_format:
      tprint( "Processing code-coverage",'DARK_CYAN')
      verif_package.code_coverage( code_coverage_format)

  ############################################################################
  # return:
  # - 1 : something failed
  # - 0 : everything passed
  # - 6/10/2019: Morris reversed the 0/1 to reflect what CI expects
  ############################################################################
    return not all(model.status is model.Status.SUCCESS
                   for model in verif_package.models)


if __name__ == "__main__":
    sys.exit(main())
