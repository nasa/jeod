#!/usr/bin/env python2

import os, sys
import common
from common import *

#fileListName = os.sys.argv[1]
cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument("-o", "--output", dest="output", help="Specify output directory unit test artifacts.", 
                           default='{0}/artifacts/'.format(topDir))
cmdLineParser.add_argument("-c", "--cpus", dest="cpus", help="Override MAKEFLAGS setting and set number of cores.", 
                           default=numCpus, type=int)

args = cmdLineParser.parse_args()


initArtifactArea(args.output)

untrackedOut = os.path.join(args.output, 'untracked_files.txt')
ccmd = 'git ls-files --other'
ProcessCmd(ccmd, 'Get Untracked Files List', untrackedOut).join()

mtimeMap = {}

totalFiles = 0
with open(untrackedOut, 'r') as fp:
  fp.readline()
  fp.readline()
  for line in fp:
     totalFiles += 1
     fileName = line.strip()
     mtime = os.path.getmtime(fileName)
     isAdded = False
     while not isAdded:
        if mtime not in mtimeMap:
           mtimeMap[mtime] = fileName
           isAdded = True
        else:
           mtime += mtime/1.0E16

if len(mtimeMap) != totalFiles:
  print "Mismatch of modification file map and total number of files {0} : {1}".format(len(mtimeMap), totalFiles)
  sys.exit(-1)

mtimeSorted = []
for i in sorted(mtimeMap):
  mtimeSorted.append(mtimeMap[i])

print "Updated modification times. See {0} for list of files.".format(untrackedOut)
with open(untrackedOut, 'a+') as fp:
   for fileName in mtimeSorted:
     ccmd = 'touch {0}'.format(fileName)
     os.system(ccmd)
     fp.write(ccmd + '\n')
