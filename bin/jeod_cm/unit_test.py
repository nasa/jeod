#!/usr/bin/env python3

import os, sys, argparse, subprocess, glob
import common
from common import *
import platform


cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.",
                           default='{0}/artifacts/unit_test'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.",
                           default=numCpus, type=int)
cmdLineParser.add_argument("-p", "--partial", dest="partial", help="Partial coverage with a pattern of directories to rerun",
                           nargs='+')

args = cmdLineParser.parse_args()

if args.partial:
   initArtifactArea(args.output, skipClear=True)
else:
   initArtifactArea(args.output, skipClear=False)

initProcessThreads(args.cpus)

buildDirStr = ''
for ii in range(args.cpus):
   buildDirStr += ' -directory {0}/build_unit_test_{1} '.format(topDir, ii)

unit_tests = []

# Find all unit tests via directory name and a makefile within it
all_unit_tests = []
unit_tests = []
for root, dirs, files in os.walk('{0}/models'.format(topDir), topdown=True):
   if 'unit_tests' in root:
      for name in files:
         if name == 'makefile':
            all_unit_tests.append(root)
            if args.partial:
               isIncluded = False
               for testDir in args.partial:
                  if testDir in root:
                     isIncluded = True
                     break
               if not isIncluded:
                  continue
            unit_tests.append(root)
            break

if args.partial and not unit_tests:
   outMsg("No unit tests for the input directory(ies) {0}. Skipping...".format(args.partial))
   sys.exit(0)

#Build JEOD lib for unit tests
outMsg("Building JEOD unit-test compatible library")
ccmd = 'make -f bin/jeod/makefile BUILD_DIR=build_unit_test INSTALL_DIR=lib_jeod_unit_test TRICK_BUILD=0 ENABLE_UNIT_TESTS=1'
buildOut = os.path.join(args.output, 'lib_build.txt')
ProcessCmd(ccmd, 'JEOD Unit Test Library Build', buildOut).join()

paths = list(all_unit_tests)
ut_common_path = get_common_path(paths)

# Build unit tests.
for unit_test in unit_tests:
   ccmd = 'make SKIP_JEODLIB_BUILD=1 build'
   ss = unit_test.partition(ut_common_path)
   ss = ss[2]
   subDir = ss.replace('/','_')
   subDirOut = '{0}/BUILD_{1}.txt'.format(args.output, subDir)
   thrIdx = getNextAvailThread()
   startProcess(thrIdx, ProcessCmd(ccmd, 'Build {0}'.format(unit_test), subDirOut, execDir=unit_test))

waitForAllProcesses()

if platform.system() != 'Darwin':
   # Find all gcno files not in build directory name and copy it
   for root, dirs, files in os.walk('{0}/models'.format(topDir), topdown=True):
      for name in files:
         if name.endswith('.gcno'):
            fPath = os.path.join(root, name)
            subFile = fPath.partition('/models/')[2]
            destFile = '{0}/build_unit_test/{1}'.format(topDir, subFile)
            destDir = os.path.dirname(destFile)
            if not os.path.exists(destDir):
               os.makedirs(destDir)
            scmd = 'cp {0} {1}'.format(fPath, destDir)
            os.system(scmd)
   
   # Copy all .gcno
   outMsg('Copying .gcno files to build_unit_test_0')
   build0 = 'build_unit_test_0'
   preBuildDirs = glob.glob('{0}/build_unit_test_*'.format(topDir))
   for preBuildDir in preBuildDirs:
      shutil.rmtree(preBuildDir)
   for root, dirs, files in os.walk('{0}/build_unit_test'.format(topDir), topdown=True):
      for name in files:
         if name.endswith('.gcno'):
            fPath = os.path.join(root, name)
            subFile = fPath.partition('/build_unit_test/')[2]
            destFile = '{0}/build_unit_test_0/{1}'.format(topDir, subFile)
            destDir = os.path.dirname(destFile)
            if not os.path.exists(destDir):
               os.makedirs(destDir)
            scmd = 'cp {0} {1}'.format(fPath, destDir)
            os.system(scmd)
         if name.endswith('.gcda'):
            fPath = os.path.join(root, name)
            os.remove(fPath)

   for ii in range(1, args.cpus):
      buildRoot = 'build_unit_test_{0}'.format(ii)
      outMsg('Copying build_unit_test_0 directory to {0}'.format(buildRoot))
      if os.path.exists(buildRoot):
         shutil.rmtree(buildRoot)
      os.system('cp -r build_unit_test_0 {0}'.format(buildRoot))

lcovOut = os.path.join(args.output, 'lcov_output.txt')
if args.partial:
   initInfo = '{0}/unit_test_base_partial.info'.format(args.output)
else:
   initInfo = '{0}/unit_test_base.info'.format(args.output)

if platform.system() != 'Darwin':
   ccmd = 'lcov --capture --initial {0} --output-file {1}'.format('-directory {0}/build_unit_test'.format(topDir), initInfo)
   ProcessCmd(ccmd, 'LCOV Init', lcovOut).join()

for unit_test in unit_tests:
   if args.partial:
      isIncluded = False
      for testDir in args.partial:
         if testDir in unit_test:
            isIncluded = True
            break
      if not isIncluded:
         continue

   thrIdx = getNextAvailThread()
   ccmd = 'make run'
   ss = unit_test.partition(ut_common_path)
   ss = ss[2]
   subDir = ss.replace('/','_')
   subDirOut = '{0}/RUN_{1}.txt'.format(args.output, subDir)
   numDirsToBuild = len(topDir.split('/'))
   if platform.system() != 'Darwin':
      os.environ['GCOV_PREFIX_STRIP']='{0}'.format(numDirsToBuild)
      os.environ['GCOV_PREFIX']='{0}/build_unit_test_{1}'.format(topDir, thrIdx)
   startProcess(thrIdx, ProcessCmd(ccmd, 'RUN {0}'.format(unit_test), subDirOut, execDir=unit_test))

waitForAllProcesses()

if args.partial:
   testInfo = '{0}/unit_test_test_partial.info'.format(args.output)
   totalInfo = '{0}/unit_test_total_partial.info'.format(args.output)
   reducedInfo = '{0}/unit_test_partial.info'.format(args.output)
else:
   testInfo = '{0}/unit_test_test.info'.format(args.output)
   totalInfo = '{0}/unit_test_total.info'.format(args.output)
   reducedInfo = '{0}/unit_test_reduced.info'.format(args.output)

if platform.system() != 'Darwin':
   ccmd = 'lcov --capture --rc lcov_branch_coverage=1 {0} --output-file {1}'.format(buildDirStr, testInfo)
   ProcessCmd(ccmd, 'LCOV Capture Data', lcovOut).join()
   ccmd = 'lcov --rc lcov_branch_coverage=1 --add-tracefile {0} --add-tracefile {1} --output-file {2}'.format(initInfo, testInfo, totalInfo)
   ProcessCmd(ccmd, 'LCOV Add Tracefiles', lcovOut).join()
   ccmd = ['lcov', '--rc', 'lcov_branch_coverage=1', '--extract', totalInfo, '{0}/models/*'.format(topDir),
           '--output-file', reducedInfo]
   ProcessCmd(ccmd, 'LCOV Final Unit Test', lcovOut).join()


outMsg("Script complete")
sys.exit(0)
