import os, sys
from verif_utilities import sys_cmd, tprint
from enum import Enum
#*****************************************************************************
# VerifFile
# Provides the verification information about specific files.  Exists as an
# element inside a VerifRun
#*****************************************************************************
class VerifFile:

    Status = Enum('Status', 'NOT_STARTED FAIL SUCCESS')

    def __init__(self, test_file, baseline_file):

        self.test_file     = test_file # Test file with respect
                                       #   to sim directory
        self.baseline_file = baseline_file  # Baseline file wrt sim directory
        self.status        = self.Status.NOT_STARTED   # Status of comparison

    #*************************************************************************
    # report
    # Produce the file-specific report
    #*************************************************************************
    def report( self):
        indent = "            "
        color = "DARK_RED"
        if self.status is self.Status.SUCCESS:
            color = "GREEN"
        elif self.status is self.Status.NOT_STARTED:
            color = "DARK_YELLOW"
        tprint(indent + "File: "+self.status.name+"  "+self.baseline_file, color)


    #*************************************************************************
    # compare_date
    # executes the comparison between the two files.
    #*************************************************************************
    def compare_data(self):
        # baseline_file should exist, it was the specified file.  Need to check
        # on the presence of test_file.  For safety, also check on the presence of
        # baseline_file
        self.status = self.Status.FAIL
        for file_ in [self.test_file, self.baseline_file]:
            if not os.path.isfile(file_):
                tprint("  ERROR: file "+file_+" is missing", 'DARK_RED')
                return

        try:
            # execute a md5sum on each file, read the stdout, split into words,
            # take the 1st word and remove extraneous whitespace.
            md5_cmd = "md5sum "
            if sys.platform == 'darwin':
                md5_cmd = "md5 "

            md5_test = sys_cmd(md5_cmd+self.test_file).split(' ')[0].strip()
            md5_base = sys_cmd(md5_cmd+self.baseline_file).split(' ')[0].strip()

        except Exception as e:
            tprint("  ERROR: Unable to perform md5sum, exception is: ", 'DARK_RED')
            tprint("         " +str(e) , 'DARK_RED')
            return

        if md5_test == md5_base:
            self.status = self.Status.SUCCESS
