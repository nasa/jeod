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
