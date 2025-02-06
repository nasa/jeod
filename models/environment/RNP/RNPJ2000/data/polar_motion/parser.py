#!/usr/bin/python

import os, sys, argparse
try:
   from mako.template import Template
   from mako import exceptions
except:
   print("No mako module installed. Suggest running `yum install python-mako`")
   sys.exit(-1)
from collections import namedtuple

cmdLineParser = argparse.ArgumentParser()
cmdLineParser.add_argument('-i', '--inputFile', dest='inputFile', help="Specify input file with UTC data", required=True)
cmdLineParser.add_argument('-t', '--type', dest='outputType', help="Specify output frequency type.", choices=['daily', 'monthly', 'both'], default='both')
args = cmdLineParser.parse_args()

ConversionEntry = namedtuple('ConversionEntry', ['year', 'month', 'day', 'mjd', 'xp', 'yp'])

dailyEntries = []
monthlyEntries = []
with open(args.inputFile, 'r') as fpIn:
  readPastComments = False
  for line in fpIn:
     if not readPastComments:
        if '(0h UTC)' in line:
           readPastComments = True
        continue

     ln = ' '.join(line.split())
     ss = ln.split(' ')
     if len(ss) >= 6:
        dailyEntries.append(ConversionEntry(ss[0], ss[1], ss[2], ss[3], ss[4], ss[5]))
        if int(ss[2]) == 1:
           monthlyEntries.append(ConversionEntry(ss[0], ss[1], ss[2], ss[3], ss[4], ss[5]))
     elif len(ss) > 1:
        print("Line: '%s' doesn't have at least 6 columns. Skipping..." % line)


if args.outputType == 'daily' or args.outputType == 'both':
  with open('src/xpyp_daily.cc', 'w') as fpOut:
     text = Template(filename='xpyp.mako').render(entries=dailyEntries, outType='daily')
     fpOut.write(text)
if args.outputType == 'monthly' or args.outputType == 'both':
  with open('src/xpyp_monthly.cc', 'w') as fpOut:
     text = Template(filename='xpyp.mako').render(entries=monthlyEntries, outType='monthly')
     fpOut.write(text)
