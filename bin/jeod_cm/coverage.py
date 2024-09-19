#!/usr/bin/env python3

import os, sys, argparse, subprocess, atexit
import common
from common import *
import platform

cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.",
                           default='{0}/artifacts/coverage'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.",
                           default=numCpus, type=int)
cmdLineParser.add_argument("-p", "--partial", dest="partial", help="Partial coverage with a pattern of directories to rerun",
                           nargs='+')

args = cmdLineParser.parse_args()
shellOut = '{0}/shell_output.txt'.format(args.output)

#Last expected coverage reported
# Summary coverage rate:
#   lines......: 34.0% (5795 of 17060 lines)
#   functions..: 40.1% (3192 of 7959 functions)
#   branches...: 41.6% (1780 of 4275 branches)
lineThreshold = 33.99
funcThreshold = 40.09
branchThreshold = 41.59

for root, dirs, files in os.walk('{0}'.format(topDir), topdown=True):
    for name in files:
       if name.endswith('.gcda'):
          os.remove('{0}/{1}'.format(root, name))

if args.partial:
   stringAppend = ''
   for app in args.partial:
      stringAppend += '{0} '.format(app)
   initArtifactArea(args.output, skipClear=True)
else:
   initArtifactArea(args.output, skipClear=False)
initProcessThreads(args.cpus)

utOutput='{0}/unit_test'.format(args.output)
simOutput='{0}/verif_sims'.format(args.output)

if args.partial:
   ccmd = 'bin/jeod_cm/unit_test.py -c {0} -o {1} -p {2}'.format(args.cpus, utOutput, stringAppend)
   ProcessCmd(ccmd, 'Coverage: Unit Test (Partial)', shellOut).join()
else:
   ccmd = 'bin/jeod_cm/unit_test.py -c {0} -o {1}'.format(args.cpus, utOutput)
   ProcessCmd(ccmd, 'Coverage: Unit Test', shellOut).join()

infoFiles = []
if os.path.exists('{0}/unit_test_reduced.info'.format(utOutput)):
  infoFiles.append('{0}/unit_test_reduced.info'.format(utOutput))
if os.path.exists('{0}/verif_sim_reduced.info'.format(simOutput)):
  infoFiles.append('{0}/verif_sim_reduced.info'.format(simOutput))
if args.partial:
  if os.path.exists('{0}/unit_test_partial.info'.format(utOutput)):
     infoFiles.append('{0}/unit_test_partial.info'.format(utOutput))
  if os.path.exists('{0}/verif_sim_partial.info'.format(simOutput)):
     infoFiles.append('{0}/verif_sim_partial.info'.format(simOutput))
  if os.path.exists('{0}/total_reduced.info'.format(args.output)):
     infoFiles.append('{0}/total_reduced.info'.format(args.output))
totalInfo='{0}/total_coverage.info'.format(args.output)
totalReduced='{0}/total_reduced.info'.format(args.output)
lcovOut = '{0}/lcov_output.txt'.format(args.output)

ret = 0
if platform.system() != 'Darwin':
  ccmd = 'lcov --rc lcov_branch_coverage=1 '
  for infoFile in infoFiles:
     ccmd += '--add-tracefile {0} '.format(infoFile)
  ccmd += '--output-file {0}'.format(totalInfo)
  ProcessCmd(ccmd, 'Coverage: Collect all lcov artifacts', lcovOut).join()

  ccmd = ['lcov', '--rc', 'lcov_branch_coverage=1', '--remove', totalInfo, '{0}/models/*/verif/*'.format(topDir), '{0}/*/experimental/*'.format(topDir), '--output-file', totalReduced]
  ProcessCmd(ccmd, 'Coverage: Remove verif directories from coverage', lcovOut).join()

  genOut = '{0}/genhtml_output.txt'.format(args.output)
  covResults = '{0}/coverage_results'.format(args.output)
  ccmd = 'genhtml {0} --branch-coverage --output-directory {1} -p {2}'.format(totalReduced, covResults, topDir)
  ProcessCmd(ccmd, 'Coverage: Finalize and generate HTML at {0}/index.html'.format(covResults), genOut).join()

  covTextLines = tail(genOut, 4)
  lines = ''
  functions = ''
  branches = ''
  for line in covTextLines:
    line = line.strip().rstrip()
    if line.startswith('lines..'):
       lines = line.partition(': ')[2].partition('%')[0]
    elif line.startswith('functions..'):
       functions = line.partition(': ')[2].partition('%')[0]
    elif line.startswith('branches..'):
       branches = line.partition(': ')[2].partition('%')[0]

  lines = float(lines)
  functions = float(functions)
  branches = float(branches)

  if lines < lineThreshold:
    outMsg("ERROR: Line coverage requirements not met. {0}% required, {1}% actual".format(lineThreshold, lines))
    ret = 1
  else:
    outMsg("PASSED: Line coverage requirements met. {0}% required, {1}% actual".format(lineThreshold, lines))
  if functions < funcThreshold:
    outMsg("ERROR: Function coverage requirements not met. {0}% required, {1}% actual".format(funcThreshold, functions))
    ret = 1
  else:
    outMsg("PASSED: Function coverage requirements met. {0}% required, {1}% actual".format(funcThreshold, functions))
  if branches < branchThreshold:
    outMsg("ERROR: Branch coverage requirements not met. {0}% required, {1}% actual".format(branchThreshold, branches))
    ret = 1
  else:
    outMsg("PASSED: Branch coverage requirements met. {0}% required, {1}% actual".format(branchThreshold, branches))

  if ret != 0:
    outMsg('\n'.join(covTextLines))
else:
  outMsg("No coverage reporting available on Mac yet")

outMsg("Script complete")
sys.exit(ret)
