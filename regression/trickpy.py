#=============================================================================
# Notices:
#
# Copyright 2025 United States Government as represented by the Administrator
# of the National Aeronautics and Space Administration.  All Rights Reserved.
#
#
# Disclaimers:
#
# No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
# ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
# TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
# FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
# FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
# SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
# ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
# RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
# RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
# DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
# IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
#
# Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
# LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
# INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
# USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
# ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
# AGREEMENT.
#
#=============================================================================

"""
Load data record, data collection, and monte_runs files.

The trickpy module allows you to easily load output data from FAST (and other
Trick sims).  At a minimum, trickpy is able to load .trk and .csv data record
files and monte_runs dispersion files.  If the h5py module is available,
trickpy is also able to load .h5 data record files and FAST data collection
files.


Data Record Files
-----------------

To load a single data record file, use trickpy.load_file:

  >>> data = trickpy.load_file('/path/to/log_file.trk')

This method returns an object of type DataRecordFile.  It behaves like a Python
dictionary, with the variable names as keys.  The values in the dictionary are
instances of DataRecordVariable, which behaves like a Numpy ndarray.

To load all of the data record files in a RUN directory, use trickpy.load_run:

  >>> data = trickpy.load_run('/path/to/RUN_directory')

This method returns an object of type DataRecordRun.  It behaves like a Python
dictionary, with the log group names as keys.

To load all of the data record files in all of the runs of a MONTE directory,
use trickpy.load_monte:

  >>> data = trickpy.load_monte('/path/to/MONTE_directory')

This method returns an object of type DataRecordMonte.  It behaves like a
Python dictionary, with the run numbers as keys.


Data Collection Files
---------------------

To load a single data collection file, use trickpy.load_data_collection:

  >>> data = trickpy.load_data_collection('/path/to/data_collection_file.h5')

Ths method returns the data collection data in an object of type
DataCollectionFile.  The variables, conditions, values, times, and units
members can be accessed directly, and the class also provides a number of
convenience functions.

To return the values for an inputted condition, variable, or condition and
variable pair, use the get_times method:

  >>> values = data.get_values(condition='condition_name',
          variable='variable_name')

To return the times for an inputted condition, variable, or condition and
variable pair, use the get_times method:

  >>> times = data.get_times(condition='condition_name',
          variable='variable_name')


Monte Carlo Dispersion Files
----------------------------

The dispersions from a Monte Carlo are stored in a file in the MONTE directory
called monte_runs.  You can load the dispersions from a MONTE directory using
trickpy.load_dispersions:

  >>> data = trickpy.load_dispersions('/path/to/MONTE_directory')

This function returns a Python dictionary, where the keys are the names of the
input deck variables, and the values are an array of the dispersed values,
indexed by the run number.

"""

# Load the required modules
import sys
import os
import numpy as np
import struct
import time
import subprocess

# Attempt to load the h5py module
_has_h5py = False
try:
    import h5py
    _has_h5py = True
except ImportError:
    pass

# Create a list of possible file extensions for time history log files
_extensions = ['.csv', '.trk']
if _has_h5py:
    _extensions.append('.h5')

class DataRecordVariable(np.ndarray):
    """
    Store and interact with a variable from a Trick data record file.

    Objects of this type behave like Numpy ndarrays.
    """

    def __new__(cls, input_array, units):
        obj = np.asarray(input_array).view(cls)
        obj.units = units
        return obj

    def __array_finalize__(self, obj):
        if obj is None: return
        self.units = getattr(obj, 'units', None)

class DataRecordFile(dict):
    """
    Store and interact with the variables from a Trick data record file.

    Objects of this type behave like Python dictionaries, with variable names
    as keys.
    """

    def __init__(self, path):
        dict.__init__(self)
        self.path = path

class DataRecordRun(dict):
    """
    Store and interact with the files from a Trick RUN directory.

    Objects of this type behave like Python dictionaries, with log groups as
    keys.
    """

    def __init__(self, path):
        dict.__init__(self)
        self.path = path

class DataRecordMonte(dict):
    """
    Store and interact with the runs from a Trick MONTE directory.

    Objects of this type behave like Python dictionaries, with run numbers as
    keys.
    """

    def __init__(self, path):
        dict.__init__(self)
        self.path = path

class DataCollectionFile:
    """
    Store and interact with FAST data collection data.
    """

    def __init__(self, path):
        self.path = path
        self.directory = ''
        self.runs = []
        self.conditions = []
        self.variables = []
        self.units = []
        self.times = []
        self.values = []

    def _get_data(self, data, condition, variable):

        # If neither a condition or a variable were given, raise an exception
        if condition == None and variable == None:
            raise KeyError('Neither a condition name nor a variable name were provided.')

        # Check that the condition and/or variable are in the lists
        if condition != None and condition not in self.conditions:
            raise KeyError('Condition \'%s\' is not in the list of condition names.' % condition)
        if variable != None and variable not in self.variables:
            raise KeyError('Variable \'%s\' is not in the list of variable names.' % variable)

        # If only a condition is given, return a slice of all variables and runs
        if variable == None:
            icon = np.where(self.conditions == condition)[0][0]
            return data[icon, ...]

        # If only a variable is given, return a slice of all conditions and runs
        elif condition == None:
            ivar = np.where(self.variables == variable)[0][0]
            return data[:, ivar, ...]

        # Otherwise, if both a condition and a variable are given, return a
        # slice of all runs
        else:
            icon = np.where(self.conditions == condition)[0][0]
            ivar = np.where(self.variables == variable)[0][0]
            return data[icon, ivar, ...]

    def get_values(self, condition=None, variable=None):
        """
        Return the values corresponding to the *condition*, *variable*, or both.

        If both *condition* and *variable* are supplied and the data is from a
        Monte Carlo, then the output is an array of the values for each run.
        If the data is from a single run, then the output is the scalar value
        for that run.

        If only *condition* is supplied and the data is from a Monte Carlo,
        then the output is a 2D array of the values, with the first axis
        corresponding to the variables and the second to the runs.  If the data
        is from a single run, then the output is a 1D array of the values for
        each variable.

        If only *variable* is supplied and the data is from a Monte Carlo, then
        the output is a 2D array of the values, with the first axis
        corresponding to the conditions and the second to the runs.  If the
        data is from a single run, then the output is a 1D array of the values
        for each condition.
        """
        return self._get_data(self.values, condition, variable)

    def get_times(self, condition=None, variable=None):
        """
        Return the times corresponding to the *condition*, *variable*, or both.

        The output rules are the same as those for :method:`get_times`.
        """
        return self._get_data(self.times, condition, variable)

    def get_units(self, variable):
        """
        Return the units of *variable*.
        """
        if variable not in self.variables:
            raise KeyError("Variable '%s' is not in the list of variable names." % (variable))
        ivar = np.where(self.variables == variable)[0][0]
        return self.units[ivar]

class ProgressBar:

    def __init__(self, name, size):
        self.name = name
        self.size = size
        self.size_digits = len(str(size))
        self.refresh = 1.0
        self.last_time = time.time()

        p = subprocess.Popen(['stty', 'size'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = p.communicate()
        if p.returncode == 0: self.tty_width = int(stdout.split()[1])
        else: self.tty_width = 50

    def show(self, current):
        now = time.time()
        if now > self.last_time + self.refresh or current == 0:
            self.last_time = now
            head = self.name + ' ['
            tail = '] ' + ('%%%ii / %%%ii' % (self.size_digits, self.size_digits)) % (current, self.size)
            bar_size = self.tty_width - len(head) - len(tail) - 1
            bar_percent = float(current)/self.size
            bar_done_size = max(1, int(bar_size*bar_percent))
            bar = '-'*(bar_done_size-1) + '>' + ' '*(bar_size-bar_done_size)
            sys.stderr.write('\r%s%s%s' % (head, bar, tail))
            sys.stderr.flush()

    def clear(self):
        sys.stderr.write('\r%s\r' % (' '*self.tty_width))

def _load_hdf5(path, variables):
    """
    Load a data record file in the HDF5 format.
    """

    with h5py.File(path,'r') as file:

        # Parse the header information
        header = file['header']
        param_units = header['param_units']
        param_names = header['param_names']

        # Initialize the output
        out = DataRecordFile(os.path.abspath(path))

        # Add each parameter to the output dictionary
        for name, units in zip(param_names, param_units):
            if variables == None or name in variables:
                out[name] = DataRecordVariable(np.array(file[name]), units)

    return out

def _trick10_format(t, s):
    """
    Convert a Trick 10 type enumerated value to a format string.
    """

    if t == 1:
        return 'c'
    elif t == 2:
        return 'B'
    elif t == 3:
        return 'S' + str(s)
    elif t == 4:
        return 'h'
    elif t == 5:
        return 'H'
    elif t == 6:
        return 'i'
    elif t == 7:
        return 'I'
    elif t == 8:
        return 'l'
    elif t == 9:
        return 'L'
    elif t == 10:
        return 'f'
    elif t == 11:
        return 'd'
    elif t == 14:
        return 'q'
    elif t == 15:
        return 'Q'
    elif t == 17:
        return '?'
    elif t == 21:
        return 'i'
    else:
        raise NotImplementedError

def _trick7_format(t, s):
    """
    Convert a Trick 7 type enumerated value to a format string.
    """

    if t == 0:
        return 'c'
    elif t == 1:
        return 'B'
    elif t == 2:
        return 'S' + str(s)
    elif t == 3:
        return 'h'
    elif t == 4:
        return 'H'
    elif t == 5:
        return 'i'
    elif t == 6:
        return 'I'
    elif t == 7:
        return 'l'
    elif t == 8:
        return 'L'
    elif t == 9:
        return 'f'
    elif t == 10:
        return 'd'
    elif t == 13:
        return 'q'
    elif t == 14:
        return 'Q'
    elif t == 16:
        return '?'
    elif t == 17:
        return '?'
    elif t == 102:
        return 'i'
    else:
        raise NotImplementedError

def _load_trk(path, variables):
    """
    Load a data record file in the Trick binary format.
    """

    with open(path,'rb') as file:

        # Read the file header information
        trickVer = file.read(10).decode('ascii')
        title, version, endianness = trickVer.split('-')
        if endianness == 'L':
            endianness = '<'
        else:
            endianness = '>'
        version = int(version)
        # Get the number of parameters recorded
        number_parameters = struct.unpack(endianness + 'i', file.read(4))[0]

        # For each parameter, read the header information
        names = []
        units = []
        codes = []
        sizes = []
        for index in range(number_parameters):
            name_length = struct.unpack(endianness + 'i', file.read(4))[0]
            names.append(file.read(name_length).decode('ascii'))
            unit_length = struct.unpack(endianness + 'i', file.read(4))[0]
            units.append(file.read(unit_length).decode('ascii'))
            codes.append(struct.unpack(endianness + 'i', file.read(4))[0])
            sizes.append(struct.unpack(endianness + 'i', file.read(4))[0])

        # Select the code mapping
        if version == 10:
            trick_format = _trick10_format
        elif version == 7:
            trick_format = _trick7_format
        else:
            raise IOError('Unsupported Trick version')

        # Build the numpy dtype list
        dtype = []
        fields = []
        try:
            for index, code, size in zip(list(range(len(codes))), codes, sizes):
                field = 'f%d' % (index)
                dtype.append((field, endianness + trick_format(code, size)))
                fields.append(field)
        except NotImplementedError:
            raise IOError('Unsupported type')

        # Load the data in one shot
        # Major kudos to James Dearman for figuring this one out!
        data = np.fromfile(file, dtype=dtype)

        # Initialize the output
        out = DataRecordFile(os.path.abspath(path))

        # Store the data for each variable
        for field, name, unit in zip(fields, names, units):
            if variables == None or name in variables:
                out[name] = DataRecordVariable(np.array(data[field]), unit)

    return out

def _load_csv(path, variables):
    """
    Load a data record file in the Trick ASCII (CSV) format.
    """

    with open(path) as file:

        # Read the header
        header = file.readline()

        # Parse the header
        entries = header.split(',')

        # Read the data
        data = []
        for line in file:
            line = line.strip()
            if len(line) > 0:
                data.append(line.split(','))
        data = list(zip(*data))

        # Convert the data to floats
        for i in range(len(data)):
            try:
                data[i] = [float(x) for x in data[i]]
            except:
                pass

        # Initialize the output
        out = DataRecordFile(os.path.abspath(path))

        # Save the data
        for index, entry in enumerate(entries):
            variable, units = entry.split()
            if variables == None or variable in variables:
                units = units.strip('{}')
                out[variable] = DataRecordVariable(np.array(data[index]), units)

    return out

def _load_file(path, variables):
    """
    Determine the file type and load the file.
    """

    root, ext = os.path.splitext(os.path.basename(path))

    # Make sure we are loading a data record file
    if not root.startswith('log'):
        raise IOError("Inputted file does not appear to be a data record file.")

    if ext == '.h5' and _has_h5py:
        return _load_hdf5(path, variables)
    elif ext == '.trk':
        return _load_trk(path, variables)
    elif ext == '.csv':
        return _load_csv(path, variables)
    else:
        raise IOError("Inputted file has unsuppoted extension.")

def _load_run(path, groups, variables, show_progress):
    """
    Load all Trick data record files in a RUN directory.
    """

    # Initialize the output
    out = DataRecordRun(os.path.abspath(path))

    # Get a list of all the data record files in the directory
    filelist = []
    grouplist = []
    for f in os.listdir(path):
        root, ext = os.path.splitext(f)
        if root.startswith('log_') and ext in _extensions:
            group = root[4:]
            if groups == None or group in groups:
                filelist.append(f)
                grouplist.append(group)

    if show_progress:
        prog = ProgressBar(name="Data Record Files", size=len(filelist))

    # Load all of the files with recognized extensions
    skipped_files = []
    for i, f, g in zip(list(range(len(filelist))), filelist, grouplist):

        if show_progress:
            prog.show(i)

        try:
            out[g] = _load_file(os.path.join(path, f), variables)
        except:
            skipped_files.append(f)

    if show_progress:
        prog.clear()

    return out, skipped_files

def _load_monte(path, runs, groups, variables, show_progress):
    """
    Load all Trick data record files in a MONTE directory.
    """

    # Initialize the output
    out = DataRecordMonte(os.path.abspath(path))

    # Get a list of all of the RUN directories
    dirs = [dir for dir in os.listdir(path) if 'RUN' in dir]

    if show_progress:
        prog = ProgressBar(name="RUN Directories", size=len(dirs))

    # Load each RUN directory
    runs_with_skipped_files = []
    for i, dir in enumerate(dirs):

        if show_progress:
            prog.show(i)

        # Get the run number
        run = int(dir.split('_')[1])

        # Load the RUN
        if runs == None or run in runs:
            out[run], skipped_files = _load_run(os.path.join(path,dir), groups, variables, False)
            if skipped_files:
                runs_with_skipped_files.append(run)

    if show_progress:
        prog.clear()

    return out, runs_with_skipped_files

def get_data_and_vars(path):
    """
    Get variable names and units from a Trick data record file.
    """

    varnames = []
    units = []
    file = load_file(path)
    for var in list(file.keys()):
        varnames.append(var)
        units.append(file[var].units)

    return file, varnames, units

def load_file(path, variables=None):
    """
    Load data from a Trick data record file.
    """

    # Check that the path exists
    if not os.path.exists(path):
        raise IOError("Path does not exist.")

    # Make sure the path is to a file
    if not os.path.isfile(path):
        raise IOError("Path is not a file.")

    # Load the file
    out = _load_file(path, variables)

    return out

def load_run(path, groups=None, variables=None, show_progress=True, show_warnings=True):
    """
    Load data from a Trick RUN directory.
    """

    # Check that the path exists
    if not os.path.exists(path):
        raise IOError("Path does not exist.")

    # Make sure the path is to a directory
    if not os.path.isdir(path):
        raise IOError("Path is not a directory.")

    # Load the files in the directory
    out, skipped_files = _load_run(path, groups, variables, show_progress)

    if skipped_files and show_warnings:
        if len(skipped_files) > 1:
            print("The following {} files were skipped:".format(len(skipped_files)))
        else:
            print("The following file was skipped:")
        for f in skipped_files:
            print("  {}".format(f))

    return out

def load_monte(path, runs=None, groups=None, variables=None, show_progress=True, show_warnings=True):
    """
    Load data from a Trick MONTE directory.
    """

    # Check that the path exists
    if not os.path.exists(path):
        raise IOError("Path does not exist.")

    # Make sure the path is to a directory
    if not os.path.isdir(path):
        raise IOError("Path is not a directory.")

    # Load the data from the MONTE directory
    out, runs_with_skipped_files = _load_monte(path, runs, groups, variables, show_progress)

    if runs_with_skipped_files and show_warnings:
        if len(runs_with_skipped_files) > 1:
            print("There were {} runs with skipped files".format(len(runs_with_skipped_files)))
        else:
            print("There was 1 run with skipped files")

    return out

def load_data_collection(path):
    """
    Load data from a FAST data collection file.
    """

    # Check that h5py module was loaded
    if not _has_h5py:
        raise IOError("Loading .h5 files requires the h5py module.")

    # Check that the path exists
    if not os.path.exists(path):
        raise IOError("Inputted path is not valid.")

    # Make sure the path is to a file
    if not os.path.isfile(path):
        raise IOError("Inputted path is not to a data collection file.")

    # Make sure the extension is recognized
    root, ext = os.path.splitext(path)
    if ext != '.h5':
        raise IOError("Inputted file is not a recognized data collection file.")

    # Initialize the output data
    out = DataCollectionFile(os.path.abspath(path))

    # Attempt to open the file
    try:
        with h5py.File(path,'r') as file:

            # Save the data collection data
            out.directory = file['directory'].value
            if 'runs' in file:
                out.runs = np.array(file['runs'].value)
            else:
                out.runs = np.array([0])
            out.conditions = np.array(file['conditions'].value)
            out.variables = np.array(file['variables'].value)
            out.units = np.array(file['units'].value)
            out.times = np.array(file['times'].value)
            out.values = np.array(file['values'].value)

    # If the file can't be opened, raise an exception
    except IOError:
            raise IOError('Failed to load file at ' + path)

    return out

def load_dispersions(path):
    """
    Load dispersions from a MONTE directory.
    """

    # Check that the path exists
    if not os.path.exists(path):
        raise IOError("Inputted path is not valid.")

    # Make sure the path is to a directory
    if not os.path.isdir(path):
        raise IOError("Inputted path is not to a directory.")

    # We want to load the monte_runs file
    filepath = os.path.join(path, 'monte_runs')

    # Check for the monte_runs file
    if not (os.path.exists(filepath) and os.path.isfile(filepath)):
        raise IOError("Inputted directory does not contain a monte_runs file.")

    # Initialize the output data
    out = dict()

    # Attempt to open the file
    try:
        with open(filepath) as file:

            # Read the header
            header = file.readline()

            # Parse the header
            header = header.lstrip('#').strip()
            variables = header.split()

            # Read the data
            data = []
            for line in file:
                line = line.strip()
                data.append([])
                data[-1] = [float(x) for x in line.split()]
            data = list(zip(*data))

            # Save the data
            for index, variable in enumerate(variables):
                out[variable] = np.array(data[index])

    # If the file can't be opened, raise an exception
    except IOError:
        raise IOError('Failed to load file at ' + filepath)

    return out
