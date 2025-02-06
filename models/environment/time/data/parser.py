#!/usr/bin/python3

import os, sys, argparse
try:
   from mako.template import Template
   from mako import exceptions
except:
   print("No mako module installed. Suggest running `yum install python-mako`")
   sys.exit(-1)
from collections import namedtuple


leap_second_url = 'https://hpiers.obspm.fr/iers/bul/bulc/Leap_Second.dat'


cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument('-i', '--inputFile', dest='inputFile', help="Specify input file with UTC data", required=True)
cmdLineParser.add_argument('-s', '--skip', dest='skipDownload', help="Flag to skip the leap second data file download", action='store_true', default=False)
args = cmdLineParser.parse_args()

#Download and parse the latest leap seconds updates
if not args.skipDownload:
   ccmd = 'wget {0}'.format(leap_second_url)
   ret = os.system(ccmd)
   if ret != 0:
      print("Unable to download '%s'" % leap_second_url)
      sys.exit(-1)

LeapSecond = namedtuple('LeapSecond', ['mjd', 'day', 'month', 'year', 'numSeconds'])

leapSeconds = []
with open('Leap_Second.dat', 'r') as fpIn, open('src/tai_to_utc.cc', 'w') as fpOut:
  for line in fpIn:
     ln = ' '.join(line.split())
     if ln.startswith('#'):
        continue
     ss = ln.split(' ')
     if len(ss) == 5:
        leapSeconds.append(LeapSecond(float(ss[0]), int(ss[1]), int(ss[2]), int(ss[3]), int(ss[4])))
     else:
        print("Line: '%s'doesn't have 5 columns. Skipping..." % line)
  text = Template(filename='tai_to_utc.mako').render(leapSeconds=leapSeconds)
  fpOut.write(text)

ConversionEntry = namedtuple('ConversionEntry', ['year', 'month', 'day', 'mjd', 'dut'])

def getLeapSecondsFromMjd(mjd):
  global leapSeconds
  retVal = 0
  if mjd < leapSeconds[0].mjd:
     retVal = leapSeconds[0].numSeconds
  elif mjd > leapSeconds[-1].mjd:
     retVal = leapSeconds[-1].numSeconds
  else:
     for lsIndex in range(len(leapSeconds)-1):
        if mjd < leapSeconds[lsIndex+1].mjd:
           retVal = leapSeconds[lsIndex].numSeconds
           break
  return retVal
  

with open(args.inputFile, 'r') as fpIn, open('src/tai_to_ut1.cc', 'w') as fpOut:
  readPastComments = False
  entries = []
  for line in fpIn:
     if not readPastComments:
        if '(0h UTC)' in line:
           readPastComments = True
        continue

     ln = ' '.join(line.split())
     ss = ln.split(' ')
     if len(ss) >= 7:
        try:
           yr = int(ss[0])
           mo = int(ss[1])
           day = int(ss[2])
           mjd = int(ss[3])
           dut = float(ss[6])
           entries.append(ConversionEntry(yr, mo, day, mjd, dut))
        except ValueError as e:
           print(e)
     elif len(ss) > 1:
        print("Line: '%s' doesn't have at least 7 columns. Skipping..." % line)
  text = Template(filename='tai_to_ut1.mako').render(entries=entries, getLeapSecondsFromMjd=getLeapSecondsFromMjd )
  fpOut.write(text)
