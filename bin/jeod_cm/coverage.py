#!/usr/bin/env python2

import os, sys, argparse, subprocess
import common
from common import *

cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.", 
                           default='{0}/artifacts/coverage'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.", 
                           default=numCpus, type=int)

args = cmdLineParser.parse_args()

initArtifactArea(args.output)
initProcessThreads(args.cpus)

ccmd = 'rm -rf {0}/build*'.format(topDir)
ProcessCmd(ccmd, 'Cleanup prior build directories', common.TEST_out.name).join()

utOutput='{0}/unit_test'.format(args.output)
simOutput='{0}/verif_sims'.format(args.output)

ccmd = 'bin/jeod_cm/unit_test.py -c {0} -o {1}'.format(args.cpus, utOutput)
ProcessCmd(ccmd, 'Coverage: Unit Test', common.TEST_out.name).join()

ccmd = 'git add artifacts'.format(topDir)
ProcessCmd(ccmd, 'Git Stash Artifacts', common.TEST_out.name).join()

ccmd = 'git clean -xdf'.format(topDir)
ProcessCmd(ccmd, 'git clean everything', common.TEST_out.name).join()

ccmd = 'bin/jeod_cm/sims_coverage.py -c {0} -o {1}'.format(args.cpus, simOutput)
ProcessCmd(ccmd, 'Coverage: Verif Sims', common.TEST_out.name).join()

utInfo='{0}/unit_test_reduced.info'.format(utOutput)
simInfo='{0}/verif_sim_reduced.info'.format(simOutput)
totalInfo='{0}/total_coverage.info'.format(args.output)
lcovOut = '{0}/lcov_output.txt'.format(args.output)
ccmd = 'lcov --rc lcov_branch_coverage=1 --add-tracefile {0} --add-tracefile {1} --output-file {2}'.format(utInfo,
          simInfo, totalInfo)
ProcessCmd(ccmd, 'Coverage: Collect all lcov artifacts', '/dev/null').join()
covResults = '{0}/coverage_results'.format(args.output)
ccmd = 'genhtml {0} --branch-coverage --output-directory {1} -p {2}'.format(totalInfo, covResults, topDir)
ProcessCmd(ccmd, 'Coverage: Finalize and generate HTML at {0}/index.html'.format(covResults), '/dev/null').join()

ccmd = 'git reset HEAD artifacts'.format(topDir)
ProcessCmd(ccmd, 'Git Restore Artifacts to untracked', common.TEST_out.name).join()

outMsg("Script complete")
sys.exit(0)
