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

import sys
import os
import filecmp
import shutil
import signal
import operator
import re
import numpy as np
import matplotlib
import logging
import stat
import pdb
import argparse
from operator import itemgetter
from textwrap import wrap
from ColorStr import ColorStr
from verif_utilities import tprint, validateOutputPath

import trickpy

# Get python version
pyversion = sys.version[:3]

def signal_handler(signal, frame):
    """
    Prints out warning if a SIGINT is caught

    Parameters
    ----------
    signal
    Imported package

    frame
    ???
    """
    tprint('\n===============================================================',
           'DARK_YELLOW')
    tprint('Caught SIGINT, shutting down... Your results might be corrupted!',
           'DARK_YELLOW')
    tprint('=================================================================',
           'DARK_YELLOW')
    logging.shutdown()
    sys.exit(1)

signal.signal(signal.SIGINT, signal_handler)

def in1d_with_tol(list1, list2, tol = 1e-9, assume_unique = False):
    """
    Mod of np.intersect1d() to include tolerance when comparing list elements

    Parameters
    ----------
    list1, list2
    Lists of numbers to be compared/intersected

    tol
    Tolerance value considered when intersecting input lists

    assume_unique
    Bool to decide if list1 is to be made unique

    Source: Numpy github issue 7784, user jaimefrio
    """
    if assume_unique:
        list1 = np.unique(list1)
    intervals = np.empty(2*list1.size, float)
    intervals[::2] = list1 - tol
    intervals[1::2] = list1 + tol
    overlaps = intervals[:-1] >= intervals[1:]
    overlaps[1:] = overlaps[1:] | overlaps[:-1]
    keep = np.concatenate((~overlaps, [True]))
    intervals = intervals[keep]
    return np.searchsorted(intervals, list2, side='right') & 1 == 1

class CompareData:

    @staticmethod
    def files_matching_pattern(dirpath, pattern='.*', inverse=False):
        """
        Return a list of files in dirpath matching/not-matching regex pattern

        Parameters
        ----------
        dirpath
        Path to directory containing files to compare against pattern

        pattern
        Regex pattern to compare file names against

        inverse
        Bool to flip comparison to output files that DON'T match pattern
        """
        files = []
        for f in os.listdir(dirpath):
            if inverse:
                if not re.match(pattern, f):
                    files.append(f)
            else:
                if re.match(pattern, f):
                    files.append(f)

        return files

    @staticmethod
    def create_common_time(data1,data2,tol = 1e-9):
        """
        Create common time vector and common indices for coplotting data

        Parameters
        ----------
        data1,data2
        Dictionaries of variable names (keys) and data (values)

        tol
        Tolerance value considered when intersecting input lists

        t
        A dictionary containing comparison information between data1, data2
        with the following keys/values:
        'time': an array of time-values that are common
        'data1_bool': an array equal in size to the data1 pass-in, but with
                      each element either yes/no - is common with data2 or not
        'data2_bool': complement of t['data1_bool']
        'data1_index': array equal in size to t['time'] with each entry being
                       the index of the corresponding common time in the data1
                       array.
        'data2_index': Complement of t['data1_index']
        """
        data_varname = 'sys.exec.out.time'
        data1_time = data1[data_varname]
        data2_time = data2[data_varname]
        comm_time = data1_time[(np.abs(data2_time[:,None] - data1_time) < tol).any(0)]
        data1_b = in1d_with_tol(comm_time, data1_time, tol=tol)
        data2_b = in1d_with_tol(comm_time, data2_time, tol=tol)
        data1_ix = np.nonzero(data1_b)
        data2_ix = np.nonzero(data2_b)
        t = {'time':comm_time,
             'data1_bool':data1_b,
             'data2_bool':data2_b,
             'data1_index':data1_ix,
             'data2_index':data2_ix}
        return t

    @staticmethod
    def coplots_adjust(ax, trickvar, trickunit, extr0, extr1):
        """
        Adjust the plot specs of the tri-plot made by coplots()

        Parameters
        ----------
        ax
        subplots() instance of axes object

        trickvar
        Variable name used in plot

        trickunit
        Unit of trickvar

        extr0,extr1
        Upper and lower bounds of values before output gets less readable
        """
        # Set y-axis label based on axes scales of ax[0] and ax[1]
        if (extr0 > 9999 or extr0 < 0.001) \
          and (extr1 <= 9999 and extr1 >= 0.001):
            ylabel_pad = 60
            fig_xoffset = 0.05
        elif (extr1 > 9999 or extr1 < 0.001):
            ylabel_pad = 15
            fig_xoffset = 0.05
        else:
            ylabel_pad = 25
            fig_xoffset = 0.03

        # Move figures to the right to account for extra space padding between
        # figures and the y-axis label
        ax[1].set_ylabel('\n'.join(wrap(trickvar + '  ' + trickunit,65)),
                         labelpad=ylabel_pad)
        pos0 = ax[0].get_position()
        pos1 = ax[1].get_position()
        pos2 = ax[2].get_position()
        pos0new = [pos0.x0 + fig_xoffset, pos0.y0, pos0.width, pos0.height]
        pos1new = [pos1.x0 + fig_xoffset, pos1.y0, pos1.width, pos1.height]
        pos2new = [pos2.x0 + fig_xoffset, pos2.y0, pos2.width, pos2.height]
        ax[0].set_position(pos0new)
        ax[1].set_position(pos1new)
        ax[2].set_position(pos2new)

        return

    def __init__(self):
        """Set up overall status"""
        # Initialize overall status list
        num_fail_cond = 5
        self.overall_status = [False]*num_fail_cond

        self.current_dir = os.getcwd()
        return

    def main(self, test_path, reg_path, datafile_pattern,
             screenplot, logfile, html_publish):
        """
        Run the comparison on each applicable data set in the given paths

        test_path <Test data directory>
        Path to directory containing test data to compare against reg_path

        reg_path <Regression data directory>
        Path to directory containing baseline regression data

        datafile_pattern
        String pattern to match log data files to be considered for comparison
        Must be compatible with regex

        screenplot
        Flag to make plots of differing variables appear on screen

        logfile
        Path to file location for CompareData log output

        html_publish
        Flag to publish plots of differences to html

        main() includes one failure condition of its own:
        5. A RUN_* directory doesn't exit in the test directory when it does
           exist in the regression directory.

        This failure condition, if met, is set to True in handle_given_paths()

        All the returned failure values are summed up and returned at the
        end of the script's execution.
        """

        self.screenplot = screenplot
        self.html_publish = html_publish

        # This is a hack that defers the imports of these modules until we know
        # whether or not the user want's to use the --plot function which
        # requires a display.
        if not self.screenplot:
            # If we want plots, enable backend with display
            matplotlib.use('pdf')
        try:
            # This line fails if no display given and backend needs display
            import matplotlib.pyplot as plt

            import matplotlib.ticker as mtick
            from matplotlib.backends.backend_pdf import PdfPages
            self.plt = plt
            self.mtick = mtick
            self.PdfPages = PdfPages
        except:
            tprint ('The user asked for --plot but no display could be found.'
                    ' Exiting.', 'DARK_RED')
            sys.exit(1)

        # If html_publish flag true and python version is 2.7, import plotly
        if self.html_publish:
            if pyversion == '2.6':
                tprint('Publishing is not available for your version of '+
                       'python. Ignoring "--html" request.',
                       'DARK_RED','ERROR')
                self.html_publish = False
            else:
                import plotly.offline as py
                self.py = py

        # Set up logging
        logging.basicConfig(filename=logfile, filemode='w',
                            level=logging.DEBUG)

        # Make full paths out of input paths, set up other parameters
        self.test_path = validateOutputPath(test_path, 'isDir')
        self.reg_path = validateOutputPath(reg_path, 'isDir')
        self.datafile_pattern = datafile_pattern

        # Find comparison targets
        compare_these = self.handle_given_paths()

        # Create Docs/ directory in test_path, delete one if it already exists
        self.docs_dir = os.path.join(self.test_path,'verif_docs')
        if os.path.isdir(self.docs_dir):
            shutil.rmtree(self.docs_dir)
        os.mkdir(self.docs_dir)

        tprint("Comparison results will be written to \n  " + self.docs_dir)

        # Run comparison if there is at least one common .trk or .csv file
        # If not, those directories are skipped
        for reg_run_path in compare_these:
            # Create paths for each run
            test_run_path = compare_these[reg_run_path]

            test_data_files = [f for f in os.listdir(test_run_path) if re.match(self.datafile_pattern, f)]
            reg_data_files = [f for f in os.listdir(reg_run_path) if re.match(self.datafile_pattern, f)]

            if not any(np.intersect1d(test_data_files,reg_data_files)):
                tprint('No common log files were found in either '+
                       'directories. Ignoring.',
                       'DARK_YELLOW','WARNING')
                continue

            per_run_status = self.run_comparison(test_run_path,
                                                 reg_run_path)

            # Update overall status list
            self.overall_status[:-1] = \
              np.logical_or(self.overall_status[:-1],per_run_status)

        return self.get_overall_status()

    def run_comparison(self,
                       testdata_path,
                       regdata_path):
        """
        Quickly compare data files between two RUN directories.

        Parameters
        ----------
        testdata_path
        path to a RUN_* directory containing test log data

        regdata_path
        path to a RUN_* directory containing baseline regression log data

        per_run_status
        Output list containing status results

        run_comparison() has 4 failure conditions, all return a value of 1:
        1. A data file in regdata_path doesn't exist in testdata_path
        2. A variable in a common data file exists in the regression data
           but not in the test data
        3. The contrary of 2. A variable in a common data file exists in the
           test data but not in the regression data
        4. Data in common variables in common log files between regdata_path
           and testdata_path do not match. Most common failure condition.


        Repeat failures of the same condition do NOT add to the returned
        failure value
        """

        # Initialize the per-run status
        per_run_status = [False]*4

        # Get just the RUN* name if the test path given was more than just RUN*
        self.test_run = os.path.basename(os.path.normpath(testdata_path))

        tprint('Evaluating ' + self.test_run + ' ...')

        # Create html directory in Docs/ directory if it doesn't already exist
        self.html_dir = os.path.join(self.docs_dir,self.test_run + '_html_files')
        if os.path.isdir(self.html_dir) and self.html_publish:
            shutil.rmtree(self.html_dir)
        if self.html_publish:
            os.mkdir(self.html_dir)

        # Define output paths
        self.pdf_filename = os.path.join(
            self.test_path,"verif_docs",self.test_run + "_plots.pdf")
        self.text_filename = os.path.join(
            self.test_path,"verif_docs",self.test_run + "_summary.txt")

        # This is to build the 'ignore' list so we can compare only the data
        # files that match pattern
        ignore_list_reg = \
          self.files_matching_pattern(dirpath=regdata_path,
                                      pattern=self.datafile_pattern,
                                      inverse=True)
        ignore_list_test = \
          self.files_matching_pattern(dirpath=testdata_path,
                                      pattern=self.datafile_pattern,
                                      inverse=True)
        ignore_list_all = set(ignore_list_reg) | set(ignore_list_test)

        # Create dircmp object and run the report.  Easy peasy lemon squeezy.
        dcmp = filecmp.dircmp(testdata_path, regdata_path,
                              ignore=list(ignore_list_all))

        if dcmp.right_only:
            tprint("  Files missing from test directory:".format(dcmp.right))
            tprint(dcmp.right_only, 'DARK_RED', indent=2)
            per_run_status[0] = True
        if dcmp.diff_files:
            tprint("  Files with differences:")
            tprint(dcmp.diff_files, 'DARK_RED', indent=2)
        if dcmp.same_files:
            tprint("  Files that match:")
            tprint(dcmp.same_files, 'DARK_GREEN', indent=2)

        # Number of found differences to trigger a warning of slowness
        warn_length = 10
        if len(dcmp.right_only) > warn_length \
          or len(dcmp.diff_files) > warn_length/2:
            wrn_txt = " WARNING: Many large data files within one directory\n"
            wrn_txt += "          will produce large PDFS and matplotlib \n"
            wrn_txt += "          v0.99 struggles to render plots \n"
            wrn_txt += "          efficiently when PDFS gro to 5+ MB. This \n"
            wrn_txt += "          could take a while. Consider using the \n"
            wrn_txt += "          --include switch to narrow what data gets\n"
            wrn_txt += "          processed.\n"
            tprint(wrn_txt,'DARK_YELLOW','WARNING')
        elif not dcmp.right_only and not dcmp.diff_files \
          and not dcmp.same_files:
            tprint("ERROR: No data files found matching pattern " \
                   + self.datafile_pattern + ". Nothing to process.",'DARK_RED')
            per_run_status[0] = True

        if dcmp.diff_files:
            # Create instantiation of pdf object, get empty file size
            self.pdf = self.PdfPages(self.pdf_filename)
            pdf_size_init = os.stat(self.pdf_filename).st_size

            # Initialize summary text file
            self.textfile = open(self.text_filename,'w')
            self.textfile.write('==========================================\n')
            self.textfile.write('     ' + self.test_run + '\n')
            self.textfile.write('==========================================\n')

            self.text_output = '\n====================\n'
            self.text_output += 'DIFFERENCES EXIST!!!\n'
            self.text_output += '====================\n'
            self.text_output_list = []
            self.mmu_text_output = ''

            # Initialize html index file for plotly stuff
            if self.html_publish:
                html_index_file = os.path.join(self.html_dir,'index.html')
                html_index_text = "<!DOCTYPE html>\n"
                html_index_text += "  <html>\n"
                html_index_text += "    <head>\n"
                html_index_text += "      <title>DATA</title>\n"
                html_index_text += "    </head>\n"
                html_index_text += "    <body style='background-color:WhiteSmoke;'>\n"
                with open(html_index_file,'w') as hif:
                    hif.write(html_index_text)

            # Run through files with differences, plot and write to files
            for datafile in dcmp.diff_files:
                tprint('  Running comparison on %s' % (datafile))
                self.text_output += datafile + ":\n"

                # Load the files:
                self.test_data, self.test_vars, self.test_units = \
                  trickpy.get_data_and_vars(os.path.join(dcmp.left,datafile))
                self.regr_data, self.regr_vars, self.regr_units = \
                  trickpy.get_data_and_vars(os.path.join(dcmp.right,datafile))

                # Wrap units with () for plotting purposes
                self.test_units = ['('+unit+')' for unit in self.test_units]
                self.regr_units = ['('+unit+')' for unit in self.regr_units]

                common_vars = np.intersect1d(self.test_vars,self.regr_vars)
                test_only_vars = np.setdiff1d(self.test_vars,self.regr_vars)
                regr_only_vars = np.setdiff1d(self.regr_vars,self.test_vars)

                # Plot variables that are ONLY in self.regr_data into
                # MISSING_vars.pdf
                if any(regr_only_vars):
                    per_run_status[1] = True
                    self.plot_regr_only_vars(datafile,regr_only_vars)

                # Plot variables that are ONLY in self.test_data into
                # NEW_vars.pdf
                if any(test_only_vars):
                    per_run_status[2] = True
                    self.plot_test_only_vars(datafile,test_only_vars)

                # Verify whether the data are the same length.  If they do,
                # can move straight to comparing the two sets.
                # If not:
                #   a: compare the sim-stop times
                #   b: compare the time-step between data points
                #   c: create a list of time references that are common
                #      between the two sets
                if (len(self.test_data['sys.exec.out.time']) \
                  != len(self.regr_data['sys.exec.out.time'])):
                    # a) Verify whether they end at the same time.
                    self.plot_mismatched_stop_time()

                    # b) Verify whether they have the same time step.
                    self.plot_mismatched_time_step()

                    # c) Create a collection of data from each run that were
                    #    made at matching times.
                    self.t = self.create_common_time(self.test_data,
                                                     self.regr_data)
                    self.abbrv_file = 1
                else: # data sets are equally sized.
                    self.t = 0
                    self.abbrv_file = 0

                # Compare and contrast each common element:
                self.text_output += "Testing the following variables:\n"
                self.text_output += "Those prefixed with ----<time>-------"
                self.text_output += " show diffs and will be plotted.\n"
                self.text_output += "The value <time> is the time at which the"
                self.text_output += " first divergence was noticed for the"
                self.text_output += " respective variable.\n"
                self.textfile.write(self.text_output)

                self.plot_common_vars(common_vars)

                # Remove pdf if nothing was plotted
                # If something was plotted, return corresponding failure value
                if not os.path.isfile(self.pdf_filename):
                    tprint( "No diff plots generated; possibly limited "+
                            "to units mismatch",
                            'DARK_YELLOW','WARNING')
                else:
                    per_run_status[3] = True

                self.text_output = ""

            # Print time-ordered list to summary text file
            self.textfile.write("\n\n################################\n" +
                               "# CHRONOLOGICALLY ORDERED LIST #\n" +
                               "################################\n")
            for element in sorted(self.text_output_list,
                                  key = operator.itemgetter(0)):
                self.textfile.write( \
                  str(element[0]) + " ----- " + element[1] + "\n")
            del self.text_output_list[:]

            # If there were any mismatched units, print to textfile
            if self.mmu_text_output:
                per_run_status[3] = True
                tprint("    %s log data has variables that have mismatched"
                       " units between the regression data and test data\n"
                       % (self.test_run), color='DARK_RED')
                sys.stderr.write("  Writing mismatched units details to: \n"
                                 "    %s ... \n" % self.text_filename )

                mmu_text_output_heading = '\n###############################\n'
                mmu_text_output_heading += '  MISMATCHED UNITS\n'
                mmu_text_output_heading += '###############################\n'
                mmu_text_output_heading += 'regr_unit | test_unit | variable\n'
                mmu_text_output_heading += '================================\n'
                self.mmu_text_output = \
                  mmu_text_output_heading + self.mmu_text_output
                self.textfile.write(self.mmu_text_output)


            self.pdf.close()
            if os.stat(self.pdf_filename).st_size == pdf_size_init:
                tprint( "Removing empty file"+ self.pdf_filename,
                        'DARK_RED', 'WARNING')
                os.remove(self.pdf_filename)
            self.textfile.close()

        if sum(per_run_status):
            tprint('  %s comparison FAILED with value: %d\n'
                    % (self.test_run, sum(per_run_status)), 'DARK_RED')
        else:
            tprint('  %s comparison PASSED with value: %d\n'
                   % (self.test_run, sum(per_run_status)), 'DARK_GREEN')

        return per_run_status

    def handle_given_paths(self):
        """
        Evaluate the command line arguments and infer whether we are to compare
        two run directories or two sets of run directories given the content in
        those directories.

        Parameters:
        -----------
        compare_these
        Dictionary returned with directories to compare against.
        Regression directories are the keys, test directories are the values
        """

        # Check to see if given TestData and RegData directories exist
        if not os.path.isdir(self.test_path):
            tprint('Test data directory %s does not exist' % (self.test_path),
                   color = 'DARK_RED')
            sys.exit(1)

        if not os.path.isdir(self.reg_path):
            tprint('Regression data directory % does not exist'
                   % (self.reg_path), 'DARK_RED')
            sys.exit(1)

        compare_these = {}

        # Determine whether we were given a single run dir or set of run dirs,
        # using the reg area as the basis for the determination
        num_reg_log_files = 0
        for obj in os.listdir(self.reg_path):
            if re.match(self.datafile_pattern, obj):
                num_reg_log_files += 1

        # If greater than zero we were given one directory
        if num_reg_log_files > 0:
            tprint("Detected a single regression/test run path. Proceeding"
                   " with 1 vs 1 comparison, using")
            compare_these[self.reg_path] = self.test_path
        # If zero, see if we were given a set of directories
        else:
            reg_dirs = self.get_subdirs_with_logged_data(self.reg_path)
            test_dirs = self.get_subdirs_with_logged_data(self.test_path)

            if reg_dirs and test_dirs:
                tprint("Detected a set of regression/test run paths."
                       " Proceeding with set vs set comparison, using")

            # Check fail condition #5: Directories in regression set that
            # don't appear in test set
            reg_dirs_no_path = \
              [os.path.basename(reg_dir) for reg_dir in reg_dirs]
            test_dirs_no_path = \
              [os.path.basename(test_dir) for test_dir in test_dirs]

            if set(reg_dirs_no_path).difference(set(test_dirs_no_path)):
                tprint("There are directories with log data in the regression"
                       " path that do not appear in the test path", 'DARK_RED')
                tprint("Regression directories:", 'DARK_YELLOW','DEBUG')
                tprint(reg_dirs_no_path, 'DARK_YELLOW','DEBUG')
                tprint("Test directories:", 'DARK_YELLOW','DEBUG')
                tprint(test_dirs_no_path, 'DARK_YELLOW','DEBUG')
                tprint (
                    set(reg_dirs_no_path).difference(set(test_dirs_no_path)),
                    'DARK_YELLOW','DEBUG')

                self.overall_status[4] = True

            # Build dictionary of regression and test directories with same
            # named data directories
            for reg_dir in reg_dirs:
                for test_dir in test_dirs:
                    if os.path.basename(reg_dir) == os.path.basename(test_dir):
                        compare_these[reg_dir] = test_dir

        if not compare_these:
            tprint("Couldn't associate any comparable runs given\n"+
                   "  regression data path: " + self.reg_path +"\n"+
                   "  test data path:       " + self.test_path,
                   'DARK_RED' )
        else:
            tprint("logged data from " + self.reg_path +
                   "\nas the basis of expected logged data files with\n"
                   "--include pattern = " + self.datafile_pattern + " ..." )
            for reg_path in compare_these:
                tprint("\nData directory comparisons to be performed:\n"+
                       "  regression: " +reg_path +"\n"+
                       "    vs.\ntest: " +compare_these[reg_path])

        return compare_these

    def get_subdirs_with_logged_data(self, path):
        """
        Get set of all dirs one level below path with data that matches pattern

        Parameters
        ----------
        path
        Path to find directories with matching data files
        """

        os.chdir(path)
        all_dirs = []
        for run_dir in os.listdir(path):
            if os.path.isdir(run_dir):
                for f in os.listdir(run_dir):
                    if re.match(self.datafile_pattern, f):
                        all_dirs.append(os.path.join(path,run_dir))
            else:
                # We've hit on a non-directory, like a README.txt
                continue
        os.chdir(self.current_dir)

        return(set(all_dirs))

    def screenplot_show(self):
        """If screenplot is turned on, show plots"""
        if self.screenplot:
            if pyversion == '2.6':
                self.plt.show(mainloop=False)
            else:
                self.plt.show(block=False)

        return

    def coplot(self,
               trickvar,
               trickunit,
               del_data,
               mismatch_t):
        """
        Produces tri-plot for one variable comparing regression to test data

        Parameters
        ----------
        trickvar
        name of the variable

        trickunit
        unit of the variable

        del_data
        (self.test_data - self.regr_data) at common times

        mismatch_t
        time of first mismatch
        """
        # Coplot of both data sets onto one plot
        f,ax = self.subplots(3,sharex=True)
        f.set_size_inches((11.0,8.5),forward=True)
        f.subplots_adjust(hspace=0.25)
        f.canvas.set_window_title(trickvar)

        # First plot: Test data and regression data on same figure
        ax[0].plot(self.regr_data['sys.exec.out.time'],
                   self.regr_data[trickvar],
                   'k',linewidth=2)
        ax[0].plot(self.test_data['sys.exec.out.time'],
                   self.test_data[trickvar],
                   'r',linewidth=1)
        ax[0].axvline(mismatch_t, color = 'b', linestyle = '--')
        ax[0].legend(("regression", "new-data"), loc='best', fancybox=True)
        ax[0].set_title('\n'.join(wrap(trickvar + " vs. Time",70)))
        ax[0].grid('on')
        # Find the largest magnitude data point to use the right axes scale
        # string format
        min0 = min(min(self.regr_data[trickvar]),min(self.test_data[trickvar]))
        max0 = max(max(self.regr_data[trickvar]),max(self.test_data[trickvar]))
        extr0 = max(abs(min0),abs(max0))
        if extr0 > 9999 or extr0 < 0.001:
            ax[0].yaxis.set_major_formatter(
                self.mtick.FormatStrFormatter('%.2e'))
        else:
            ax[0].yaxis.set_major_formatter(
                self.mtick.FormatStrFormatter('%g'))

        # Second plot: Difference between the two data sets
        ax[1].set_title("Delta [new-data - regression]  " + trickunit)
        ax[1].grid('on')
        if (self.abbrv_file):
            ax[1].plot(self.t['time'],del_data,'k',linewidth=1)
            ax[1].plot(self.t['time'],del_data,'g.', mew = 2)
        else:
            ax[1].plot(self.regr_data['sys.exec.out.time'],
                       del_data,'k',linewidth=1)
            ax[1].plot(self.regr_data['sys.exec.out.time'],
                       del_data,'g.', mew = 1)
        ax[1].axvline(mismatch_t, color = 'b', linestyle = '--')
        min1 = min(del_data)
        max1 = max(del_data)
        extr1 = max(abs(min1),abs(max1))
        if extr1 > 9999 or extr1 < 0.001:
            ax[1].yaxis.set_major_formatter(
                self.mtick.FormatStrFormatter('%.2e'))
        else:
            ax[1].yaxis.set_major_formatter(
                self.mtick.FormatStrFormatter('%g'))

        # Third plot: Relative difference between the two data sets on log axes
        scldata, out_of_range, minlogaxis, maxlogaxis = self.get_rel_diff_data(
            trickvar, del_data)
        ax[2].set_title("Delta [new-data - regression] / regression")
        ax[2].set_xlabel("Time(s)")
        ax[2].grid('on')
        if (self.abbrv_file):
            ax[2].semilogy(self.t['time'],scldata,'k',linewidth=1)
            ax[2].semilogy(self.t['time'],out_of_range,'r.', mew = 1)
            ax[2].semilogy(self.t['time'],scldata,'b.', mew = 1)
        else:
            ax[2].semilogy(self.regr_data['sys.exec.out.time'],
                           scldata,
                           'k',linewidth=1)
            ax[2].semilogy(self.regr_data['sys.exec.out.time'],
                           out_of_range,
                           'r.',mew = 1)
            ax[2].semilogy(self.regr_data['sys.exec.out.time'],
                           scldata,
                           'b.',mew = 1)
        ax[2].set_ylim([minlogaxis,maxlogaxis])
        ax[2].axvline(mismatch_t, color = 'g', linestyle = '--')

        # Adjust the plot specs for readability
        self.coplots_adjust(ax, trickvar, trickunit, extr0, extr1)

        self.screenplot_show()
        #pdb.set_trace()
        self.pdf.savefig(f)

        # Create html plots
        if self.html_publish:
            self.plot_html(f, trickvar)

    # This is the subplots function as copied from matplotlib v1.0.0, the
    # inclusion of this function is essentially a hack to make our use of
    # matplotlib compatible with the CentOS 6.5 stock package of matplotlib,
    # which is v0.99.  The subplots function was not introduced until the first
    # major release version of v1.0.0 and its usage here appears fully
    # compatible with the rest of the v0.99 release. The motivation for this
    # is to not require python2.7, which may not be available to all users.
    # We do not want to put the burden of installing alternate python versions
    # on users, so this approach of shaping our use of matplotlib to be
    # compatible with v0.99 was chosen.
    def subplots(self, nrows=1, ncols=1, sharex=False, sharey=False,
                 squeeze=True, subplot_kw=None, **fig_kw):
        """
        Kludgey matplotlib subplots copy compatible with older python versions
        """
        if subplot_kw is None:
            subplot_kw = {}

        fig = self.plt.figure(**fig_kw)

        # Create empty object array to hold all axes. It's easiest to make it
        # 1-d so we can just append subplots upon creation, and then
        nplots = nrows*ncols
        axarr = np.empty(nplots, dtype=object)

        # Create first subplot separately, so we can share it if requested
        ax0 = fig.add_subplot(nrows, ncols, 1, **subplot_kw)
        if sharex:
            subplot_kw['sharex'] = ax0
        if sharey:
            subplot_kw['sharey'] = ax0
        axarr[0] = ax0

        # Note off-by-one counting because add_subplot uses the MATLAB 1-based
        # convention.
        for i in range(1, nplots):
            axarr[i] = fig.add_subplot(nrows, ncols, i+1, **subplot_kw)

        if squeeze:
            # Reshape array to have the final desired dimension (nrow,ncol),
            # though discarding unneeded dimensions that equal 1. If we only
            # have one subplot, just return it instead of a 1-element array.
            if nplots==1:
                return fig, axarr[0]
            else:
                return fig, axarr.reshape(nrows, ncols).squeeze()
        else:
            # returned axis array will be always 2-d, even if nrows=ncols=1
            return fig, axarr.reshape(nrows, ncols)

    def get_rel_diff_data(self, trickvar, del_data):
        """
        Prep relative deltas for scaled plotting and out-of-range indicators

        Parameters
        ----------
        trickvar
        Name of the variable

        del_data
        Differenced data (test data - reg data)

        scldata
        Scaled difference data

        out_of_range
        Out-of-range indicators

        minlogaxis, maxlogaxis
        Logarithmic axes boundaries
        """
        # Set the nominal min and max log axes
        minlogaxis = 1E-10
        maxlogaxis = 10

        # make a new array to hold the scaled/relative difference
        scldata = [0.0] * len(del_data)

        for ii in range(len(del_data)):
            # Extract the regression data value for this data element
            if self.abbrv_file:
                regr_data_pt = \
                  self.regr_data[trickvar][self.t['data2_index'][0][ii]]
            else:
                regr_data_pt = self.regr_data[trickvar][ii]

            # Compute the relative delta:
            # if delta = 0.0, save the computation:
            if del_data[ii] == 0.0:
                scldata[ii] = 0.0
            # if reference value = 0, protect against divide by 0.0
            elif regr_data_pt == 0.0:
                scldata[ii] = maxlogaxis*10
            # and most of the time, compute the relative delta:
            else:
                scldata[ii] = np.abs(del_data[ii] / float(regr_data_pt))

            if del_data[ii] != 0.0:
                if scldata[ii] == 0.0:
                    tprint ('ERROR:' + str(del_data[ii])+' / '+
                            str(regr_data_pt)+' = '+
                            str( del_data[ii] / regr_data_pt)+':'+
                            str(scldata[ii]),
                            'DARK_RED','ERROR')

        # Scale the plot appropriately. Do not exceed the specified min and max
        minfrac = min(scldata)
        maxfrac = max(scldata)
        # Quick test, make sure we have something to plot.
        # This should never get hit.
        if maxfrac == 0.0:
            scldata[0] = 1E-60

        # Raise the lower threshold if all of the data has large deltas:
        if minfrac > minlogaxis:
            minlogaxis = np.power(10,(np.floor(np.log10(minfrac))))
        # If all data is below minimum threshold, reset max threshold to give
        # some minimal plot size
        if maxfrac <= minlogaxis:
            maxlogaxis = minlogaxis * 10.0
        # Otherwise, lower the upper threshold if
        #  1.  all data has small deltas and
        #  2.  some or all of it lies above the minimum threshold:
        elif 2*maxfrac < maxlogaxis:
            maxlogaxis = np.power(10,(np.ceil(np.log10(2*maxfrac))))

        # Prepare out-of-range indicators for data that lie beyond the manually
        # specified range.
        out_of_range = [0.0] * len(del_data)
        for ii in range(len(del_data)):
            if scldata[ii] < minlogaxis:
                out_of_range[ii] = minlogaxis
            elif scldata[ii] > maxlogaxis:
                out_of_range[ii] = maxlogaxis

        return scldata, out_of_range, minlogaxis, maxlogaxis

    def plot_html(self, f, var):
        """
        Plot figures as html files

        Parameters
        ----------
        f
        Figure handle created in coplots()

        var
        Name of the variable
        """
        clean_var = var.replace('.','').replace('[','').replace(']','')
        with open('{}/{}.html'.format(self.html_dir,clean_var),'w') as fhtml:
            pFig = self.py.offline.tools.mpl_to_plotly(f,resize=True)
            pFig['layout'].update({'autosize':True})
            fhtml.write('<script src="https://cdn.plot.ly/plotly-latest.min.js"></script>')
            fhtml.write(self.py.plot(pFig, include_plotlyjs=False, output_type='div',show_link=False))
        with open(html_index_file,'a') as hif:
            relative_path_html_file = '{}.html'.format(clean_varname)
            relative_path_png_file = '{}.png'.format(clean_varname)
            index_fig = plt.figure(figsize=(11,3))
            self.plt.plot(self.regr_data['sys.exec.out.time'],self.regr_data[var],'k',lw=2,label='REG')
            self.plt.plot(self.test_data['sys.exec.out.time'],self.test_data[var],'r',lw=1,label='TEST')
            self.plt.legend(loc='best')
            self.plt.savefig(os.path.join(self.html_dir,relative_path_png_file))
            self.plt.close(index_fig)
            hif.write('<div id="div_{0}" style="text-align:center;width:864px;">\n'.format(var))
            hif.write('<a id="{0}" title="{0}" href="{1}" target="_blank">\n'.format(var,relative_path_html_file))
            hif.write('<font face="Courier"><span>{0}</span></font>\n'.format(var))
            hif.write('<img id="{0}" src="{1}" style="border: 0; float: left; margin-right: 15px" />\n'.format(var,relative_path_png_file))
            hif.write('</a>\n')
            hif.write('</div>\n')

    def plot_var_subset(self, data, var, unit, pdf_subset):
        """
        Plot a subset of data

        Parameters
        ----------

        data, var, unit
        Should be self-explanatory

        pdf_subset
        PdfPages instantiation for the MISSING_var or NEW_vars pdf
        """
        self.plt.figure(figsize=(11.0,8.5))
        self.plt.plot(data['sys.exec.out.time'], data[var],'r',
                      linewidth=1)
        self.plt.title('\n'.join(wrap(var + " vs. Time",70)))
        self.plt.ylabel('\n'.join(wrap(var + '  ' + unit,65)))
        self.plt.grid('on')
        self.screenplot_show()

        pdf_subset.savefig()

    def plot_test_only_vars(self,datafile,test_only_vars):
        """
        Plot new set of variables to pdf

        Parameters
        ----------
        datafile
        Name of data file with differences

        test_only_vars
        Set of variables present in only the test data
        """
        tprint("     %s/%s has variables that only show up in the"
                           " test data directory\n" % (self.test_run, datafile),
                           color='DARK_RED')
        self.text_output += "  New variables in test data (omitted"
        self.text_output += " from comparison):\n"

        pdf1_filename = os.path.join(
            self.test_path,"verif_docs",self.test_run + "_NEW_vars.pdf")
        pdf1 = self.PdfPages(pdf1_filename)
        sys.stderr.write(
            '  Generating plots for new variables and writing to: '
            '\n    %s ... \n' % pdf1_filename )

        counter = 0
        for var in test_only_vars:
            sys.stderr.write(
                '    ( %d / %d ) %-5.2f %% Complete\r' % \
                (counter, len(test_only_vars),
                float(counter/float(len(test_only_vars)) * 100.0)))
            counter += 1
            unit = self.test_units[self.test_vars.index(var)]

            self.text_output += "    " + var + "\n"
            self.plot_var_subset(self.test_data, var, unit, pdf1)

        pdf1.close()

    def plot_regr_only_vars(self,datafile,regr_only_vars):
        """
        Plot missing set of variables to pdf

        Parameters
        ----------
        datafile
        Name of data file with differences

        regr_only_vars
        Set of variables present in only regression data
        """
        tprint("     %s/%s has variables that only show up in the"
               " regression data directory\n" %
               (self.test_run, datafile), color='DARK_RED')
        self.text_output += "  Variables not found in Test data"
        self.text_output += " (omitted from comparison):\n"
        pdf1_filename = os.path.join(
            self.test_path,"verif_docs",
            self.test_run + "_MISSING_vars.pdf")
        pdf1 = self.PdfPages(pdf1_filename)
        sys.stderr.write(
            '  Generating plots for missing variables and writing'
            ' to: \n    %s ... \n' % pdf1_filename )
        counter = 0
        for var in regr_only_vars:
            sys.stderr.write(
                '    ( %d / %d ) %-5.2f %% Complete\r' % (counter,
                len(regr_only_vars),
                float(counter/float(len(regr_only_vars)) * 100.0)))
            counter += 1
            unit = self.regr_units[self.regr_vars.index(var)]

            self.text_output += "    " + var + "\n"
            self.plot_var_subset(self.regr_data, var, unit, pdf1)

        pdf1.close()

    def plot_mismatched_stop_time(self):
        """Plot mismatched stop times"""
        if (len(self.test_data['sys.exec.out.time']) == 0):
            stop_time_test = 0
        else:
            stop_time_test = self.test_data['sys.exec.out.time'][-1]

        if (len(self.regr_data['sys.exec.out.time']) == 0):
            stop_time_regr = 0
        else:
            stop_time_regr = self.regr_data['sys.exec.out.time'][-1]

        if (stop_time_test != stop_time_regr):
            # Drop a cheesy pdf of t-vs-t into the main document
            self.text_output += "  Stop times do not match: Regression"
            self.text_output += " data stops at t = "
            self.text_output += str(stop_time_regr) + "\n"

            self.text_output += " Test data stops at t = "
            self.text_output += str(stop_time_test) + "\n"

            self.text_output += "  Comparison made of common simulation"
            self.text_output += " period only.\n"

            self.plt.figure()
            self.plt.plot(self.regr_data['sys.exec.out.time'],
                          self.regr_data['sys.exec.out.time'],
                          'k',linewidth=2)
            self.plt.plot(self.test_data['sys.exec.out.time'],
                          self.test_data['sys.exec.out.time'],
                          'r',linewidth=1)
            self.plt.title("Stop Times differ// Regr:"
                           + str(stop_time_regr) + "  //  Test:"
                           + str(stop_time_test))
            self.pdf.savefig()
            self.screenplot_show()

    def plot_mismatched_time_step(self):
        """Plot mismatched time steps"""
        if (len(self.test_data['sys.exec.out.time']) >= 2):
            time_step_test = self.test_data['sys.exec.out.time'][1] \
                             - self.test_data['sys.exec.out.time'][0]
        else:
            time_step_test = 0

        if (len(self.regr_data['sys.exec.out.time']) >= 2):
            time_step_regr = self.regr_data['sys.exec.out.time'][1] \
                             - self.regr_data['sys.exec.out.time'][0]
        else:
            time_step_regr = 0

        if (time_step_test != time_step_regr):
            # Drop a cheesy pdf of the 1st 6 timestamps (t-vs-t)
            # into the document
            self.text_output += "  Logging frequencies do not match:"
            self.text_output += " Regression time interval = "
            self.text_output += str(time_step_regr)+"\n"

            self.text_output += " Test time interval       = "
            self.text_output += str(time_step_test)+"\n"

            self.text_output += "  Comparison points limited to where"
            self.text_output += " times match.\n"

            self.plt.figure()
            self.plt.plot(self.regr_data['sys.exec.out.time'][0:6],
                          self.regr_data['sys.exec.out.time'][0:6],
                          'k.-',linewidth=2)
            self.plt.plot(self.test_data['sys.exec.out.time'][0:6],
                          self.test_data['sys.exec.out.time'][0:6],'r.')
            self.plt.title("Logging Frequencies differ// Regr:"
                           + str(time_step_regr) + "  //  Test:"
                           + str(time_step_test))
            self.pdf.savefig()
            self.screenplot_show()

    def plot_common_vars(self, common_vars):
        """Plot differences between common vars in the data sets"""
        counter = 0
        sys.stderr.write('  Generating co-plots and writing to: \n'
                         '    %s ... \n' % self.pdf_filename )
        for var in common_vars:
            #print('DEBUG: ' + str(var))
            sys.stderr.write(
                '    ( %d / %d ) %-5.2f %% Complete\r'
                % (counter, len(common_vars),
                float(counter/float(len(common_vars)) * 100.0)))
            counter += 1

            # Check to see if the units have changed
            t_unit = self.test_units[self.test_vars.index(var)]
            r_unit = self.regr_units[self.regr_vars.index(var)]

            if (t_unit != r_unit):
                self.mmu_text_output += '-- ' + r_unit + ' ----- '
                self.mmu_text_output += t_unit + ' ----- ' + var + '\n'

            # In case there are deltas, use the test unit for plotting.
            unit = t_unit

            # Generate the delta between the two runs.
            # Two methods, depending on whether the full data set is
            # being used or just those data points at matching times.
            if self.abbrv_file:
                # Treat all data as float type when differencing to get
                # around weird data types like 'S1', as well as account
                # for unsigned int maximum wrap around issues
                try:
                    del_data = \
                      self.test_data[var][self.t['data1_bool']].astype(float) \
                      - self.regr_data[var][self.t['data2_bool']].astype(float)
                except:
                    # TypeError: unsupported operand type(s)
                    # 'DataRecordVariable' and 'DataRecordVariable
                    tprint("Variable %s has an unsupported data type"
                           " %s\n" % (var, self.test_data[var].dtype),
                           color='DARK_MAGENTA')
                    del_data = []
            else:
                try:
                    del_data = (self.test_data[var].astype(float) - \
                           self.regr_data[var].astype(float))
                except:
                    tprint("Variable %s has an unsupported data type"
                           " %s\n" % (var, self.test_data[var].dtype),
                           color='DARK_MAGENTA')
                    del_data = []
            # Assume all data points match, then look for deltas that
            # disprove this assumption. mismatch_t is the time at which
            # the first delta occurs.
            match = 1
            mismatch_t = -1.0
            for ii in range(len(del_data)):
                if (del_data[ii] != 0.0):
                    # If any delta is found make note of the time of
                    # the divergence. Any delta requires the plotting
                    # of the entire history for this variable, so once
                    # one is found we can stop looking.
                    match = 0
                    if self.abbrv_file:
                        t_div_index = self.t['data1_index'][0][ii]
                        mismatch_t = \
                          self.test_data['sys.exec.out.time'][t_div_index]
                    else:
                        mismatch_t = self.test_data['sys.exec.out.time'][ii]
                    break

            # Output a message to indicate whether the variable matches
            # If "match" is still != 0, all compared instances matched
            # If not, flag the variable and generate a plot window to
            # show the differences.
            if match:
                self.text_output = var + '\n'
            else:
                self.text_output = "---- " + str(mismatch_t) + " ---- "
                self.text_output += var + '\n'

                self.text_output_list.append([mismatch_t, var])
                self.coplot(var,
                            unit,
                            del_data,
                            mismatch_t)
            # Write self.text_output to summary text file
            self.text_output += '\n'
            self.textfile.write(self.text_output)

    def get_overall_status(self):
        """Sum up all of overall status list"""
        return sum([int(x) for x in self.overall_status])
