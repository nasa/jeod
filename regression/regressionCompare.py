#=============================================================================
# Notices:
#
# Copyright © 2023 United States Government as represented by the Administrator
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

#!/usr/bin/env python2

import argparse, sys
from CompareData import CompareData
class regressionCompare(object):
    def __init__(self):

        # Create top-level parser
        self.parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)

        # Create parser for inheriting
        self._add_arguments()
        self.parser.parse_args(namespace=self)

    def _add_arguments(self):
        """Adds arguments to CompareData. Defaults added in child classes."""
        self.parser.add_argument(
          '-t','--test_dir',
          type=str,
          nargs='?',
          help='Test data directory path. This must be a run directory '
               'containing logged data or a directory containing many run '
               'directories with logged data.')

        self.parser.add_argument(
          '-r','--reg_dir',
          type=str,
          nargs='?',
          help='Regression data RUN directory path. This must be a run '
               'directory containing logged data or a directory containing '
               'many run directories with logged data.')

        self.parser.add_argument(
          '-i','--only-include',
          dest='include_pattern',
          help='Specify regular expression pattern to restrict which data '
               'files are processed. Must be regex compatible.')

        self.parser.add_argument(
          '-p','--plot',
          action='store_true',
          help='Instruction to show plots on the screen')

        self.parser.add_argument(
          '-l', '--log-file',
          dest='logfile',
          help='Path to file where logging output for this script should be '
               'written')

        self.parser.add_argument(
          '--html',
          dest='html_publish',
          action='store_true',
          help='Publish HTML files using Plotly. Requires Plotly package')

    def regressionCompare_run(self):
        """Create instance of CompareData() and run the comparison"""
        compare_data_instance = CompareData()
        return compare_data_instance.main(self.test_dir,
                                          self.reg_dir,
                                          self.include_pattern,
                                          self.plot,
                                          self.logfile,
                                          self.html_publish)

if __name__ == "__main__":
    sys.exit(regressionCompare().regressionCompare_run())
