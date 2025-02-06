"""
Adds the JEOD python library to the python search path.
"""

import sys, os
jeod_python_modules_path = os.getenv("JEOD_HOME") + "/lib/jeod/python"
if not jeod_python_modules_path in sys.path :
 sys.path.append (jeod_python_modules_path)
del jeod_python_modules_path
