#!/usr/bin/env python2

import os, sys, socket, glob, inspect, shutil, argparse, time, atexit, signal, fcntl, tempfile
import subprocess
import socket
import threading
import multiprocessing

scriptStartTime = time.time()
 
thisFile = os.path.abspath(__file__)
thisDir = os.path.dirname(thisFile)
topDir = os.path.abspath(os.path.join(thisDir, '..', '..'))
os.environ['JEOD_HOME']=topDir

numCpus=-1
origMakeFlags = ''
if 'MAKEFLAGS' in os.environ:
   origMakeFlags = os.environ['MAKEFLAGS']
   if '-j' in origMakeFlags:
      ss = origMakeFlags.partition('j')
      ss = ss[2].partition('-')[0]
      try:
         numCpus = int(ss)
      except:
         numCpus = multiprocessing.cpu_count()

if numCpus < 1:
   numCpus = multiprocessing.cpu_count() * 3 / 4
   if 'MAKEFLAGS' in os.environ:
      os.environ['MAKEFLAGS']+=' -j{0}'.format(numCpus)
   else:
      os.environ['MAKEFLAGS']='-j{0}'.format(numCpus)


threadObjs = {}
outputDir = '{0}/artifacts'.format(topDir)
isOutputInitialized = False
TEST_out = None
procs = None

token = None

def getGitlabToken():
   global token
   if token == None:
      token_path = os.path.join(os.environ['HOME'], '.config', 'jeod', 'api_token')
      with open(token_path, 'r') as fp:
        token_str_raw = fp.readline()
        token = token_str_raw.strip().rstrip()
   return token

def enableParallel():
  if numCpus > 1:
     os.environ['MAKEFLAGS']= '{0} -j{1}'.format(origMakeFlags, numCpus)

def disableParallel():
  os.environ['MAKEFLAGS']= '{0} -j1'.format(origMakeFlags)

def initArtifactArea(setOutputDir):
  global outputDir, isOutputInitialized, TEST_out
  if not isOutputInitialized:
     outputDir = setOutputDir
     isOutputInitialized = True
     if os.path.exists(outputDir):
        shutil.rmtree(outputDir)
     if not os.path.exists(outputDir):
        os.makedirs(outputDir)

     TEST_out = open('{0}/TEST_OUTPUT.txt'.format(outputDir), 'w', buffering=0)
  elif outputDir != setOutputDir:
     outMsg('TEST SETUP FAILURE: Output directory is already initialized but a call to initArtifactArea with a '
            'different directory was initiated.')
     sys.exit(-1)

def initProcessThreads(numProcs):
  global procs
  procs = [None] * numProcs


def getNextAvailThread():
  global procs
  while True:
     for ii in range(len(procs)):
        if procs[ii] != None:
           ret = procs[ii].poll()
           if ret != None:
              procs[ii].join()
              procs[ii] = None
              return ii
        else:
           return ii
     time.sleep(0.001)

def startProcess(thrIdx, proc):
  global procs
  if procs[thrIdx] != None:
     outMsg("Error: Process instance is not NULL. Exiting...")
     sys.exit(-1)
  else:
     procs[thrIdx] = proc

def waitForAllProcesses():
  global procs
  for ii in range(len(procs)):
     if procs[ii] != None:
        procs[ii].join()
        procs[ii] = None

def outMsg( msg ):
  global TEST_out
  print( msg )
  if TEST_out != None:
     TEST_out.write(msg + "\n")
  sys.stdout.flush()
  sys.stderr.flush()

def kill_threads():
  for name, threadObj in threadObjs.iteritems():
     if threadObj != None:
        if threadObj.isAlive():
           threadObj.killThread = True
           threadObj.join(5.0)
  endTime = time.time()
  outMsg('Script processing time = {0} seconds'.format(endTime-scriptStartTime))

atexit.register(kill_threads)

class ProcessCmd():
   def __init__(self, ccmd, label, outFile, execDir=os.getcwd(), fail=True, numRetries=0, stdin=subprocess.PIPE, reqReturn=0):
      if isinstance(ccmd, list):
         self.ccmd = ccmd
      else:
         self.ccmd = ccmd.split()
      self.label = label
      self.execDir = execDir
      self.outFile = open(outFile, 'a+', buffering=0)
      if stdin != subprocess.PIPE:
         try:
            self.inputStr = ' < {0}'.format(stdin.name)
         except:
            self.inputStr = ' < {0}'.format(stdin)
      else:
         self.inputStr = ''
      outMsg('Starting process {0}'.format(self.label))
      self.outFile.write('Output file for command "{0}" executed from directory {1}\n\tCommand: {2}{3}\n'.format(self.label, 
                         self.execDir, ' '.join(self.ccmd), self.inputStr))
      self.execDir = execDir
      self.fail = fail
      self.numRetries = numRetries
      self.reqReturn = reqReturn
      self.process = subprocess.Popen(self.ccmd, stdin=stdin, stdout=self.outFile, stderr=self.outFile, 
                                      cwd=self.execDir, universal_newlines=True)

   def __del__(self):
      self.numRetries=0
      if self.process != None:
         if self.process.poll() == None:
            self.process.terminate()
            self.join()

   def retry(self):
      self.outFile.write('\n\n{0}\nRetrying command. Retries left {1}\n{0}\n\n'.format('*'*20, self.numRetries))
      self.process = subprocess.Popen(self.ccmd, stdin=subprocess.PIPE, stdout=self.outFile, stderr=self.outFile, 
                                      cwd=self.execDir, universal_newlines=True)

   def terminate(self):
      if self.process != None:
         self.process.terminate()
         return self.join()
      return False

   def kill(self):
      if self.process != None:
         self.process.kill()
         return self.join()
      return False

   def poll(self):
      self.process.poll()
      if self.process.returncode != None:
         return self.join()
      else:
         return None

   def convertSignalToString(self, value):
      if value == signal.SIGABRT:
         return 'SIGABRT'
      elif value == signal.SIGFPE:
         return 'SIGFPE'
      elif value == signal.SIGILL:
         return 'SIGILL'
      elif value == signal.SIGINT:
         return 'SIGINT'
      elif value == signal.SIGSEGV:
         return 'SIGSEGV'
      elif value == signal.SIGTERM:
         return 'SIGTERM'
      else:
         return 'Unknown signal {0}'.format(value)

   def join(self):
      self.process.communicate()
      if self.process.returncode != self.reqReturn:
         if self.process.returncode == -1:
            self.outFile.write('Process ended with {0} return code\n'.format(self.process.returncode))
         else:
            self.outFile.write('Process ended with {0} return code "{1}"\n'.format(self.process.returncode, 
                               self.convertSignalToString(self.process.returncode)))
         if self.numRetries > 0:
            self.numRetries -= 1
         else:
            outMsg('ERROR executing {0}\n\tSee {1} for more details.'.format(self.label, self.outFile.name))
            if self.fail:
               sys.exit(-1)
         return False
      outMsg('SUCCESS executing {0}'.format(self.label))
      return True


def shutdownAll():
  global threadObjs
  for target, threadObj in threadObjs.iteritems():
     if threadObj != None:
        if threadObj.isAlive():
           threadObj.killThread = True
           threadObj.join(5.0)
        del threadObj
        threadObjs[target] = None

def get_common_path(paths):
    common = []
    common_size = -1

    for p in paths:
        s = p.split('/')
        s.pop()
        if common_size == -1:
            common_size = len(s)
            common = s
            continue
        while common_size > 0 and s[0:common_size] != common:
            common_size -= 1
            common.pop()

    return '/'.join(common) + '/'
