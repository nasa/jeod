#!/usr/bin/env python3

import os, sys, argparse
import common
from common import *
import pathlib

cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory formatting artifacts.",
                           default='{0}/artifacts/formatting'.format(topDir))
cmdLineParser.add_argument("-f", "--format", dest="format", help="Format the code",
                           action="store_true", default=False)
cmdLineParser.add_argument("-s", "--source", dest="sourceDir", help="Specify the directory to perform formatting",
                           default=topDir)
cmdLineParser.add_argument("-a", "--all", dest="all", help="Flag to enable format checking or processing on all files. Otherwise, only process git diff files",
                           action="store_true", default=False)
cmdLineParser.add_argument("-b", "--branch", dest="branch", help="Specify the branch to diff",
                           default='origin/trunk')

args = cmdLineParser.parse_args()

initArtifactArea(args.output)

configOut = os.path.join(args.output, 'formattingCheck.txt')

extensions = ['.h', '.hh', '.hpp', '.c', '.cc', '.cpp']

retCode = 0

fileList = []
if args.all:
  for root, dirs, files in pathlib.Path(args.sourceDir).walk():
     if 'CMakeFiles' in root.parts:
        continue
     for testFile in files:
        filePath = pathlib.PurePath(testFile)
        if filePath.suffix in extensions:
           fullFilePath = pathlib.PurePath(root, filePath)
           fileList.append(fullFilePath)
else:
  diffOut = os.path.join(args.output, 'gitDiff.txt')
  ProcessCmd('git fetch origin trunk', 'Git Fetch', configOut).join()
  ProcessCmd('git diff --name-only {0} -- {1}'.format(args.branch, args.sourceDir), 'Git Diff', diffOut).join()
  failedFiles = []
  with open(diffOut, 'r') as diffFp:
     for diffFile in diffFp:
        diffFile = diffFile.strip().rstrip()
        diffFilePath = pathlib.PurePath(diffFile)
        if diffFilePath.suffix in extensions and os.path.isfile(diffFilePath):
           fileList.append(diffFilePath)

for file in fileList:
   fileFormatOut = os.path.join(args.output, file.name.replace('/','_'))
   if args.format:
     isSuccess = ProcessCmd('clang-format -i -style=file --verbose {0}'.format(file), 'Formatting {0}'.format(file), fileFormatOut, fail=False).join()
   else:
     isSuccess = ProcessCmd('clang-format -style=file -Werror --dry-run --verbose {0}'.format(file), 'Format Check {0}'.format(file), fileFormatOut, fail=False).join()
   if not isSuccess:
      retCode = 1

outMsg("Script complete")
sys.exit(retCode)
