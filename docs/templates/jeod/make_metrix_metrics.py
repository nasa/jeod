#!/usr/bin/env python3

import os
import sys
import subprocess
import string
import metrixpp

#metrixModuleDir = os.path.dirname(metrixpp.__file__)
#metrixModuleDefLocation = os.path.join(metrixModuleDir, 'metrixpp.py')
metrixModuleDefLocation = os.path.join(os.path.dirname(__file__), 'metrix++.py')

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-t", "--tool", dest="toolLocation", help="Specify location of metrixplusplus package.", default=metrixModuleDefLocation)
(options, args) = parser.parse_args()

model_dir = os.path.realpath(os.path.join(os.getcwd(), '..', '..'))

def processCmd(ccmd, printToScreen=False, fail=True):
   pcmd = subprocess.Popen(ccmd, stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell=True, cwd=model_dir,
                           universal_newlines=True)
   res = pcmd.communicate()
   if printToScreen:
      for pipe in res:
         lines = pipe.split('\n')[:-1]
         lineIdx = 0
         for line in lines:
            print(line)
            lineIdx += 1

   if pcmd.returncode == 0:
     if printToScreen:
        print('Success %s' % ccmd)
   else:
     if fail:
        fp = open('fail.txt', 'w')
        fp.write('FAILURE executing {0}\n\n'.format(ccmd))
        print('FAILURE executing {0}\n'.format(ccmd))
        pipes = ('cout', 'cerr')
        pipeIdx = 0
        for pipe in res:
           lines = pipe.split('\n')[:-1]
           lineIdx = 0
           for line in lines:
              fp.write("{0}, {1}: {2}\n".format(pipes[pipeIdx],lineIdx, line))
              print("{0}, {1}: {2}".format(pipes[pipeIdx],lineIdx, line))
              lineIdx += 1
           pipeIdx += 1
        fp.close()
        print('Error {0}\n'.format(pcmd.returncode))
        sys.exit(pcmd.returncode)
   return (pcmd.returncode, res)


if not options.toolLocation:
  print("Error: No tool location specified")
  parser.print_help()
  sys.exit(-1)

srcFiles = []

ccmd = 'python3 {0} collect --std.code.filelines.comments --std.code.filelines.code --std.code.filelines.total --std.code.complexity.cyclomatic'.format(options.toolLocation)
ccmd += ' --include-files=.*hh'
ccmd += ' --include-files=.*cc'
ccmd += ' --exclude-files=.*S_source.hh'
ccmd += ' --exclude-files=verif.*'
ccmd += ' --exclude-files=data.*'
ccmd += ' --exclude-files=io_src.*'
ccmd += ' --exclude-files=swig.*'
ccmd += ' --exclude-files=trick.* '
ccmd += ' -- .'
rc, res = processCmd(ccmd)
lines = res[1].split('\n')
for line in lines:
   ss = line.partition('Processing: ')
   if ss[2]:
     srcFiles.append(ss[2])

resultsMap = {}
for srcFile in srcFiles:
   ccmd = 'python3 {0} view --format=python -- {1}'.format(options.toolLocation, srcFile)
   rc, res = processCmd(ccmd)
   lines = res[0].split('\n')[:-1]
   for line in lines:
      ss = 'resultsMap["' + srcFile + '"] = ' + line
      exec(ss)

if not srcFiles:
  print("No source files found.")
  sys.exit(-1)

output = '''\\begin{stretchlongtable}[c]{||p{2in} p{2in} r r|}
\caption{Cyclomatic Complexity}
\label{tab:metrix_metrics} \\tabularnewline[6pt]

\hline
{\\bf Method} & {\\bf File} & {\\bf Line} & {\\bf ECC} \\tabularnewline
\hline \hline
\endfirsthead

\caption[]{Cyclomatic Complexity (continued)} \\tabularnewline[6pt]
\hline
{\\bf Method} & {\\bf File} & {\\bf Line} & {\\bf ECC} \\tabularnewline
\hline \hline
\endhead

\hline \multicolumn{4}{r}{{Continued on next page}} \\\\
\endfoot

\hline
\endlastfoot'''

print(output)

for srcFile in srcFiles:
   currentClass = ''
   rmEntry = resultsMap[srcFile]
   fileInfo = rmEntry['view'][0]['data']['info']
   filePath = fileInfo['path']
   with open(os.path.join(model_dir, filePath), 'r') as fp:
      regions = rmEntry['view'][0]['data']['file-data']['regions']
      namespace_regions = []
      class_regions = []
      for region in regions:
         regInfo = region['info']
         if regInfo['name'] == 'JEOD_MAKE_SIM_INTERFACES':
            continue
         if region['info']['type'] == 'namespace':
            namespace_regions.append((regInfo['name'], regInfo['line_begin'], regInfo['line_end']))
         elif region['info']['type'] == 'class':
            class_regions.append((regInfo['name'], regInfo['line_begin'], regInfo['line_end']))
         elif region['info']['type'] != 'function' and region['info']['type'] != 'global' and region['info']['type'] != 'struct':
            print(region)
         else:
            regData = region['data']
            if 'std.code.complexity' in regData:
               fullScopedName = ''
               lineBegin = regInfo['line_begin']
               byteBegin = regInfo['offset_begin']
               fps = fp.seek(byteBegin)
               blockStart = fp.read(regInfo['offset_end'] - byteBegin)
               blockStartOrig = blockStart
               blockStart = ''
               insideCommentBlock = 0
               for chIdx in range(0,len(blockStartOrig)-1):
                  chPair = blockStartOrig[chIdx:chIdx+2]
                  if insideCommentBlock > 0:
                     if insideCommentBlock == 1:
                        insideCommentBlock -= 1
                     elif '*/' == chPair:
                        insideCommentBlock -= 1
                     continue
                  if '/*' == chPair:
                     insideCommentBlock = 2
                  else:
                     blockStart += blockStartOrig[chIdx]
               blockStartOrig = blockStart
               blockStart = ''
               for ln in blockStartOrig.split('\n'):
                  if '//' in ln:
                     ss = ln.partition('//')[0]
                     if ss:
                        blockStart += ss + '\n'
                  else:
                     blockStart += ln + '\n'
                     
               blockStart = ' '.join(blockStart.split())
               if '::' in blockStart:
                  ss = blockStart.partition('::')[0]
                  ss = ss.split()[-1]
                  class_regions.append((ss, regInfo['line_begin'], regInfo['line_end']))
               remStr = blockStart.partition('(')[2]
               sys.stdout.flush()
               args = ''
               numOpenParens = 1
               while numOpenParens:
                  cidx = remStr.find(')')
                  oidx = remStr.find('(') 
                  cidx = sys.maxsize if cidx == -1 else cidx
                  oidx = sys.maxsize if oidx == -1 else oidx
                  if oidx < cidx:
                     noOpenParens = False
                     args += remStr[0:oidx+1]
                     numOpenParens += 1
                     remStr = remStr[oidx+1:]
                  else:
                     args += remStr[0:cidx]
                     numOpenParens -= 1
                     remStr = remStr[cidx:]
               args = ' (' + args.strip().rstrip() + ')'
               args = args.replace('&', '\&')
                  
               for ns in namespace_regions:
                  if lineBegin >= ns[1] and lineBegin <= ns[2]:
                     fullScopedName += ns[0] + '::'
               for cls in class_regions:
                  if lineBegin >= cls[1] and lineBegin <= cls[2]:
                     fullScopedName += cls[0] + '::'
               fullScopedName += regInfo['name']
               lineBreakingScopedName = fullScopedName + args
               for capLet in string.ascii_uppercase:
                  lineBreakingScopedName = lineBreakingScopedName.replace(capLet, '\hspace{0pt}'+capLet)
               lineBreakingScopedName = lineBreakingScopedName.replace('::', '::\hspace{0pt}')
               lineBreakingScopedName = lineBreakingScopedName.replace('_', '\_\hspace{0pt}')
               lineBreakingScopedName = lineBreakingScopedName.replace('(', '\hspace{0pt}(')
               lineBreakingScopedName = lineBreakingScopedName.replace(')', ')\hspace{0pt}')
               lineBreakingScopedName = lineBreakingScopedName.replace('~', '\lower3pt\hbox{\\textasciitilde}')
               origLineBreakingScopedName = lineBreakingScopedName
               lineBreakingScopedName = lineBreakingScopedName.replace('\hspace{0pt}\hspace{0pt}', '\hspace{0pt}')
               while origLineBreakingScopedName != lineBreakingScopedName:
                  origLineBreakingScopedName = lineBreakingScopedName
                  lineBreakingScopedName = lineBreakingScopedName.replace('\hspace{0pt}\hspace{0pt}', '\hspace{0pt}')

               lineBreakingFilePath = filePath.replace('./', '')
               for capLet in string.ascii_uppercase:
                  lineBreakingFilePath = lineBreakingFilePath.replace(capLet, '\hspace{0pt}'+capLet)
               lineBreakingFilePath = lineBreakingFilePath.replace('_', '\_\hspace{0pt}')
               lineBreakingFilePath = lineBreakingFilePath.replace('/', '/\hspace{0pt}')
             
               ss = '\longentry {0} & \longentry {1} & {2} & {3}\\tabularnewline'.format(lineBreakingScopedName, lineBreakingFilePath, regInfo['line_begin'], regData['std.code.complexity']['cyclomatic']+1) 
               print(ss)
print('\end{stretchlongtable}')
