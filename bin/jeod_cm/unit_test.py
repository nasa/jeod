#!/usr/bin/env python2

import os, sys, argparse, subprocess
import common
from common import *


cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.", 
                           default='{0}/artifacts/unit_test'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.", 
                           default=numCpus, type=int)

args = cmdLineParser.parse_args()

initArtifactArea(args.output)
initProcessThreads(args.cpus)

buildDirStr = ''
for ii in range(args.cpus):
   buildDirStr += ' -directory {0}/build_{1} '.format(topDir, ii)

#Build JEOD lib for unit tests
outMsg("Building JEOD unit-test compatible library")
ccmd = 'make -f bin/jeod/makefile BUILD_DIR=build TRICK_BUILD=0 ENABLE_UNIT_TESTS=1'
buildOut = os.path.join(args.output, 'lib_build.txt')
ProcessCmd(ccmd, 'JEOD Unit Test Library Build', buildOut).join()

unit_tests = []

# Find all unit tests via directory name and a makefile within it
for root, dirs, files in os.walk('{0}/models'.format(topDir), topdown=True):
   if 'unit_tests' in root:
      for name in files:
         if name == 'makefile':
            unit_tests.append(root)
            break


paths = list(unit_tests)
ut_common_path = get_common_path(paths)

# Build unit tests.
for unit_test in unit_tests:
   ccmd = 'make build'
   ss = unit_test.partition(ut_common_path)
   ss = ss[2]
   subDir = ss.replace('/','_')
   subDirOut = '{0}/BUILD_{1}.txt'.format(args.output, subDir)
   disableParallel()
   thrIdx = getNextAvailThread()
   startProcess(thrIdx, ProcessCmd(ccmd, 'Build {0}'.format(unit_test), subDirOut, execDir=unit_test))
   enableParallel()

waitForAllProcesses()

# Find all gcno files not in build directory name and copy it
for root, dirs, files in os.walk('{0}/models'.format(topDir), topdown=True):
   for name in files:
      if name.endswith('.gcno'):
         fPath = os.path.join(root, name)
         subFile = fPath.partition('/models/')[2]
         destFile = '{0}/build/{1}'.format(topDir, subFile)
         destDir = os.path.dirname(destFile)
         if not os.path.exists(destDir):
            os.makedirs(destDir)
         scmd = 'cp {0} {1}'.format(fPath, destDir)
         os.system(scmd)


for ii in range(args.cpus):
   buildRoot = 'build_{0}'.format(ii)
   outMsg('Copying build directory to {0}'.format(buildRoot))
   if os.path.exists(buildRoot):
      shutil.rmtree(buildRoot)
   os.system('cp -r build {0}'.format(buildRoot))

lcovOut = os.path.join(args.output, 'lcov_output.txt')
initInfo = '{0}/unit_test_base.info'.format(args.output)
ccmd = 'lcov --capture --initial {0} --output-file {1}'.format('-directory {0}/build'.format(topDir), initInfo)
ProcessCmd(ccmd, 'LCOV Init', lcovOut).join()

for unit_test in unit_tests:
   thrIdx = getNextAvailThread()
   ccmd = 'make run'
   ss = unit_test.partition(ut_common_path)
   ss = ss[2]
   subDir = ss.replace('/','_')
   subDirOut = '{0}/RUN_{1}.txt'.format(args.output, subDir)
   numDirsToBuild = len(topDir.split('/'))
   os.environ['GCOV_PREFIX_STRIP']='{0}'.format(numDirsToBuild)
   os.environ['GCOV_PREFIX']='{0}/build_{1}'.format(topDir, thrIdx)
   startProcess(thrIdx, ProcessCmd(ccmd, 'RUN {0}'.format(unit_test), subDirOut, execDir=unit_test))

waitForAllProcesses()

testInfo = '{0}/unit_test_test.info'.format(args.output)
totalInfo = '{0}/unit_test_total.info'.format(args.output)
reducedInfo = '{0}/unit_test_reduced.info'.format(args.output)
ccmd = 'lcov --capture --rc lcov_branch_coverage=1 {0} --output-file {1}'.format(buildDirStr, testInfo)
ProcessCmd(ccmd, 'LCOV Capture Data', lcovOut).join()
ccmd = 'lcov --rc lcov_branch_coverage=1 --add-tracefile {0} --add-tracefile {1} --output-file {2}'.format(initInfo, testInfo, totalInfo)
ProcessCmd(ccmd, 'LCOV Add Tracefiles', lcovOut).join()
ccmd = ['lcov', '--rc', 'lcov_branch_coverage=1', '--extract', totalInfo, '{0}/models/*'.format(topDir), 
        '--output-file', reducedInfo]
ProcessCmd(ccmd, 'LCOV Final Unit Test', lcovOut).join()
   

outMsg("Script complete")
sys.exit(0)
