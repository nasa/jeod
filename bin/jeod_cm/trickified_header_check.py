#!/usr/bin/env python3

import sys, re, os
import pathlib
import common
from common import *

ignoredDirectories = ['verif', 'experimental']
ignoredFiles = ['_inline.hh',
                'class_declarations.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_first_order_ode_integrator.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_generalized_second_order_ode_integrator.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_integrator_base.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_integrator_base_first.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_integrator_base_second.hh',
                'utils/integration/gauss_jackson/include/gauss_jackson_simple_second_order_ode_integrator.hh',
                'utils/integration/gauss_jackson/include/two_d_array.hh',
                'utils/sim_interface/include/config_test_harness.hh']



#Assemble list of all *.hh files
headerFileList = []
for root, dirs, files in os.walk("{0}/models".format(os.getcwd())):
    for testFile in files:
        filePath = pathlib.PurePath(testFile)
        if filePath.suffix == '.hh':
            fullFilePath = pathlib.PurePath(root, filePath)
            relativeFilePath = os.path.relpath(fullFilePath, "{0}/models".format(os.getcwd()))
            headerFileList.append(relativeFilePath)

#Find and remove ignored files and directories
ignoredFileList = []
for file in headerFileList:
    for dir in ignoredDirectories:
        if dir in file.split(os.path.sep):
            ignoredFileList.append(file)
    for fileName in ignoredFiles:
        if fileName in file:
            ignoredFileList.append(file)

cleanedHeaderFileList = list(set(headerFileList) - set(ignoredFileList))


#Extract current list of trickified files
file = open("trickified/S_source.hh", 'r')
matches = re.findall(r'#include ".*"', file.read())
stripped = [entry.replace('#include ','') for entry in matches]
currentTrickifiedFileList = [entry.replace('"','') for entry in stripped]

#Compare lists
notInTrickifiedFileList = list(set(cleanedHeaderFileList) - set(currentTrickifiedFileList))

if notInTrickifiedFileList:
    outMsg("Header files are missing from trickified/S_source.hh. Either paste this list to the file or add them to the list of ignored files in bin/jeod_cm/trickified_header_check.py")
    for file in notInTrickifiedFileList:
        outMsg('#include "' + file + '"')
    retCode = 1
else:
    retCode = 0

sys.exit(retCode)
