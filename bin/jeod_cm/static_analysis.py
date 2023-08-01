#!/usr/bin/env python2

import os, sys, argparse, subprocess
import common
from common import *


cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.", 
                           default='{0}/artifacts/static_analysis'.format(topDir))
cmdLineParser.add_argument("-f", "--full", dest="fullAnalysis", help="Do complete analysis of all configurations.", 
                           action="store_true", default=False)
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.", 
                           default=numCpus, type=int)

args = cmdLineParser.parse_args()

initArtifactArea(args.output)
initProcessThreads(args.cpus)


verif_dirs = []
data_dirs = []

# Find all unit tests via directory name and a makefile within it
for root, dirs, files in os.walk('{0}/models'.format(topDir), topdown=True):
   if root.endswith('/verif'):
      verif_dirs.append(os.path.relpath(root, topDir))
   elif root.endswith('/data'):
      data_dirs.append(os.path.relpath(root, topDir))

additionalOptions = [
'warning',  
'style',
'performance', 
'portability', 
'information', 
#'unusedFunction', #This prevents the use of -j making the analysis slower. Most are setters/getters - don't need to be addressed. 
'missingInclude' 
]

includeStrings = ' -I{0}/include -I{0}/trick_source -I{0}/include/trick/compat -DTRICK_VER=19 -UJEOD_MEMORY_DEBUG -I{1}/include -Imodels/'.format(
        os.environ['TRICK_HOME'], os.environ['JEOD_SPICE_DIR'])
cmncppcheckFlags = ' --quiet --error-exitcode=-1 --inline-suppr -j{0} --suppress=*:{1}* --suppress=missingIncludeSystem --suppress=unmatchedSuppression'.format(args.cpus,os.environ['TRICK_HOME'])
if args.fullAnalysis:
   cmncppcheckFlags += ' --force'
if additionalOptions:
   cmncppcheckFlags += ' --enable='
   for additionalOpt in additionalOptions:
      cmncppcheckFlags += '{0},'.format(additionalOpt)
   cmncppcheckFlags = cmncppcheckFlags[:-1]

srcStrings = ' models/'
excludeStrings = ''
for edir in (data_dirs + verif_dirs):
   excludeStrings += ' -i {0}'.format(edir)

# Run config-check for cppcheck
ccmd = 'cppcheck --check-config'
# Add include paths and defines
ccmd += includeStrings
# Add cppcheck switches
ccmd += cmncppcheckFlags
# Add source code directories
ccmd += srcStrings
# Add source code exclusions
ccmd += excludeStrings
configOut = os.path.join(args.output, 'cppcheck_checkconfig.txt')
ProcessCmd(ccmd, 'Check Configuration', configOut).join()

# Run cppcheck
ccmd = 'cppcheck'
# Add include paths and defines
ccmd += includeStrings
# Add cppcheck switches
ccmd += cmncppcheckFlags
# Add source code directories
ccmd += srcStrings
# Add source code exclusions
ccmd += excludeStrings
cppcheckOut = os.path.join(args.output, 'cppcheck_analysis.txt')
ProcessCmd(ccmd, 'Run CppCheck', cppcheckOut).join()

outMsg("Script complete")
sys.exit(0)
