#!/usr/bin/env python3

import os
import pathlib
from common import *

extensions = ['.c', '.cc', '.cpp']

for root, dirs, files in pathlib.Path(topDir).walk():
  if 'CMakeFiles' in root.parts:
     continue
  if 'verif' in root.parts:
     continue
  srcFiles = []
  dataSrcFiles = []
  for testFile in files:
     filePath = pathlib.PurePath(testFile)
     if filePath.suffix in extensions:
        if 'data' in root.parts or 'data_src' in root.parts:
           dataSrcFiles.append(testFile)
        else:
           srcFiles.append(testFile)
  if srcFiles or dataSrcFiles:
     with open(pathlib.PurePath(root, 'cmake_file_list.cmake'), 'w') as fp:
        if 'spice' in root.parts:
           fp.write('if(DEFINED JEOD_SPICE_DIR)\n')
        fp.write('set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})\n')
        if srcFiles:
           fp.write('\nset(SRCS\n')
           for srcFile in srcFiles:
              fp.write(srcFile)
              fp.write('\n')
           fp.write(''')

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
''')

        if dataSrcFiles:
           fp.write('\nset(SRCS\n')
           for srcFile in dataSrcFiles:
              fp.write(srcFile)
              fp.write('\n')
           fp.write(''')

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
''')
        if 'spice' in root.parts:
           fp.write('endif(DEFINED JEOD_SPICE_DIR)\n')
