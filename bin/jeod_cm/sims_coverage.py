#!/usr/bin/env python3

import os, sys, argparse, subprocess, glob, multiprocessing
import common
from common import *

cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.",
                           default='{0}/artifacts/verif_sims'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.",
                           default=numCpus, type=int)
cmdLineParser.add_argument("-a", "--append", dest="append", help="Append coverage with a pattern of directories to rerun",
                           nargs='+')

args = cmdLineParser.parse_args()

exec(open('{0}/regression/verif_sim_list.py'.format(topDir)).read(), globals())

isAtLeastOneSim = False
for modelDir, modelData in verif_sim_list:
   for sim in modelData:
      simSubDir, simData = sim
      simDir = os.path.join(topDir, modelDir, simSubDir)
      if args.append:
         for testDir in args.append:
            if testDir in simDir:
               isAtLeastOneSim = True
               break
      if isAtLeastOneSim:
         break
   if isAtLeastOneSim:
      break

if args.append and not isAtLeastOneSim:
   outMsg("No verif sims for the input directory(ies) {0}. Skipping...".format(args.append))
   sys.exit(0)

if args.append:
   initArtifactArea(args.output, skipClear=True)
else:
   initArtifactArea(args.output, skipClear=False)

initProcessThreads(args.cpus)

buildDirStr = ''
for ii in range(args.cpus):
   buildDirStr += ' -directory {0}/build_{1} '.format(topDir, ii)

shellOut='{0}/shell_output.txt'.format(args.output)

ccmd = 'trick-gte TRICK_HOST_CPU'
hostCpu=subprocess.check_output(ccmd.split()).rstrip()
smain='S_main_{0}.exe'.format(hostCpu)

preExistingTRICK_CFLAGS = ''
preExistingTRICK_CXXFLAGS = ''
if 'TRICK_CFLAGS' in os.environ:
   preExistingTRICK_CFLAGS = os.environ['TRICK_CFLAGS']
   os.environ['TRICK_CFLAGS'] += ' -g -pg -fprofile-arcs -ftest-coverage'
else:
   os.environ['TRICK_CFLAGS'] = '-g -pg -fprofile-arcs -ftest-coverage'

if 'TRICK_CXXFLAGS' in os.environ:
   preExistingTRICK_CXXFLAGS = os.environ['TRICK_CXXFLAGS']
   os.environ['TRICK_CXXFLAGS'] += ' -g -pg -fprofile-arcs -ftest-coverage'
else:
   os.environ['TRICK_CXXFLAGS'] = '-g -pg -fprofile-arcs -ftest-coverage'

#Build JEOD lib for unit tests
ccmd = 'make -f bin/jeod/makefile TRICK_BUILD=1 BUILD_DIR=build ENABLE_UNIT_TESTS=1'
buildOut = os.path.join(args.output, 'lib_build.txt')
ProcessCmd(ccmd, 'JEOD Trick Library Build', buildOut).join()

ccmd = 'make ENABLE_UNIT_TESTS=1'
buildOut = os.path.join(args.output, 'trickified_build.txt')
ProcessCmd(ccmd, 'JEOD Trickified Library Build', buildOut, execDir='{0}/trickified'.format(topDir)).join()

# Build sims
for modelDir, modelData in verif_sim_list:
   for sim in modelData:
      simSubDir, simData = sim
      simDir = os.path.join(topDir, modelDir, simSubDir)
      if args.append:
         isIncluded = False
         for testDir in args.append:
            if testDir in simDir:
               isIncluded = True
               break
         if not isIncluded:
            continue

      ccmd = 'trick-CP TRICKIFIED=1 ENABLE_UNIT_TESTS=1'
      ss = simDir.partition(topDir + '/')
      ss = ss[2]
      subDir = ss.replace('/','_')
      subDirOut = '{0}/BUILD_{1}.txt'.format(args.output, subDir)
      disableParallel()
      thrIdx = getNextAvailThread()
      startProcess(thrIdx, ProcessCmd(ccmd, 'Build {0}'.format(os.path.join(modelDir, simSubDir)), subDirOut,
                                      execDir=simDir))
      enableParallel()

waitForAllProcesses()

# Find all gcno files not in build directory name and copy it
for root, dirs, files in os.walk('{0}'.format(topDir), topdown=True):
   buildDir = '{0}/build'.format(topDir)
   if root != buildDir:
      for name in files:
         if name.endswith('.gcno') and not root.startswith(buildDir):
               fPath = os.path.join(root, name)
               subFileTmp = fPath.partition(topDir+'/')[2]
               subFile = subFileTmp.partition('/')[2]
               destFile = '{0}/build/{1}'.format(topDir, subFile)
               destDir = os.path.dirname(destFile)
               if not os.path.exists(destDir):
                  os.makedirs(destDir)
               if os.path.abspath(fPath) != os.path.abspath(destDir):
                  ccmd = 'cp {0} {1}'.format(fPath, destDir)
                  ProcessCmd(ccmd, 'Copying {0} {1}'.format(fPath, destDir), shellOut).join()

# Copy all .gcno
outMsg('Copying .gcno files to build_0')
build0 = 'build_0'
if os.path.exists(build0):
   shutil.rmtree(build0)
for root, dirs, files in os.walk('{0}/build'.format(topDir), topdown=True):
   for name in files:
      if name.endswith('.gcno'):
         fPath = os.path.join(root, name)
         subFile = fPath.partition('/build/')[2]
         destFile = '{0}/build_0/{1}'.format(topDir, subFile)
         destDir = os.path.dirname(destFile)
         if not os.path.exists(destDir):
            os.makedirs(destDir)
         scmd = 'cp {0} {1}'.format(fPath, destDir)
         os.system(scmd)
for ii in range(1, args.cpus):
   buildRoot = 'build_{0}'.format(ii)
   outMsg('Copying build_0 directory to {0}'.format(buildRoot))
   if os.path.exists(buildRoot):
      shutil.rmtree(buildRoot)
   os.system('cp -r build_0 {0}'.format(buildRoot))

lcovOut = os.path.join(args.output, 'lcov_output.txt')
if args.append:
   initInfo = '{0}/verif_sim_base_append.info'.format(args.output)
else:
   initInfo = '{0}/verif_sim_base.info'.format(args.output)
ccmd = 'lcov --capture --initial {0} --output-file {1}'.format('-directory {0}/build'.format(topDir), initInfo)
ProcessCmd(ccmd, 'LCOV Init', lcovOut).join()

for modelDir, modelData in verif_sim_list:
   for sim in modelData:
      simSubDir, simData = sim
      for runData in simData:
         if len(runData) == 3:
            runpattern, compareList, reqReturn = runData
         else:
            runpattern, compareList = runData
            reqReturn=0
         simDir = os.path.join(topDir, modelDir, simSubDir)
         if args.append:
            isIncluded = False
            for testDir in args.append:
               if testDir in simDir:
                  isIncluded = True
                  break
            if not isIncluded:
               continue

         globpattern = '{0}/SET_test/{1}'.format(simDir,runpattern)
         runs = glob.glob(globpattern)
         if not runs:
            outMsg('Warning: No SET_test/RUNS detected for {0}'.format(simDir))
         for run in runs:
            runSubDir = run.partition(simDir+'/')[2]
            inputFile = '{0}/input.py'.format(runSubDir)
            ccmd = './{0} {1}'.format(smain, inputFile)
            ss = run.partition(topDir+'/')
            ss = ss[2]
            subDir = ss.replace('/','_')
            subDirOut = '{0}/RUN_{1}.txt'.format(args.output, subDir)
            numDirsToBuild = len(topDir.split('/'))
            os.environ['GCOV_PREFIX_STRIP']='{0}'.format(numDirsToBuild)
            os.environ['GCOV_PREFIX']='{0}/build_{1}'.format(topDir, thrIdx)
            thrIdx = getNextAvailThread()
            startProcess(thrIdx, ProcessCmd(ccmd, 'Running {0}'.format(run), subDirOut, execDir=simDir,
                                            reqReturn=reqReturn))

waitForAllProcesses()

#Reset Trick_CFLAGS to original state
os.environ['TRICK_CFLAGS'] = preExistingTRICK_CFLAGS
os.environ['TRICK_CXXFLAGS'] = preExistingTRICK_CXXFLAGS

if args.append:
   testInfo = '{0}/verif_sim_test_append.info'.format(args.output)
   totalInfo = '{0}/verif_sim_total_append.info'.format(args.output)
   reducedInfo = '{0}/verif_sim_append.info'.format(args.output)
else:
   testInfo = '{0}/verif_sim_test.info'.format(args.output)
   totalInfo = '{0}/verif_sim_total.info'.format(args.output)
   reducedInfo = '{0}/verif_sim_reduced.info'.format(args.output)

ccmd = 'lcov --capture --rc lcov_branch_coverage=1 {0} --output-file {1}'.format(buildDirStr, testInfo)
ProcessCmd(ccmd, 'LCOV Capture Data', lcovOut).join()
ccmd = 'lcov --rc lcov_branch_coverage=1 --add-tracefile {0} --add-tracefile {1} --output-file {2}'.format(initInfo, testInfo, totalInfo)
ProcessCmd(ccmd, 'LCOV Add Tracefiles', lcovOut).join()
ccmd = ['lcov', '--rc', 'lcov_branch_coverage=1', '--extract', totalInfo, '{0}/models/*'.format(topDir),
        '--output-file', reducedInfo]
ProcessCmd(ccmd, 'LCOV Final Verif Sim', lcovOut).join()


outMsg("Script complete")
sys.exit(0)
