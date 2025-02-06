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
