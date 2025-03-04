"""
Adds the JEOD python library to the python search path.
"""

import sys, os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.path.dirname(__file__),"../../../"))
jeod_python_modules_path = JEOD_HOME + "/lib/jeod/python"
if not jeod_python_modules_path in sys.path :
 sys.path.append (jeod_python_modules_path)
del jeod_python_modules_path
