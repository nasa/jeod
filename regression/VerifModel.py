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

from enum import Enum
from VerifSim import VerifSim
from verif_utilities import tprint
#*****************************************************************************
# VerifModel
# Provides all the verification information about any given model
#*****************************************************************************
class VerifModel:
    Status = Enum( 'Status', 'NOT_STARTED SIM_FAIL SUCCESS')

    def __init__(self):
        self.model_dir = ""        # model directory wrt to top level of project
        self.sims = []             # VerifSim objects
        self.status = self.Status.NOT_STARTED

    #*****************************************************************************
    # parse_model_info
    # Parses the model information passed up from VerifPackage.
    #*****************************************************************************
    def parse_model_info (self, model_info, myArgs, make_file_comps):
        if not model_info:
            tprint( "ERROR: model information not provided to "+
                    "parse_model_info method", 'DARK_RED' )
            return False
        if len(model_info) != 2:
            tprint("ERROR: malformed model configuration: " + model_info,
                   'DARK_RED')
            return False

        self.model_dir = model_info[0]
        for sim_info in model_info[1]:
            sim = VerifSim( self.model_dir )
            # If data can be parsed, add it to the list of sims:
            if sim.parse_sim_info( sim_info, myArgs, make_file_comps):
                self.sims.append(sim)

    #*************************************************************************
    # report
    # Produce the model-specific report data.
    #*************************************************************************
    def report( self):
        indent = "    "
        color = "DARK_CYAN"
        if self.status is self.Status.SUCCESS:
            color = "GREEN"
        elif self.status is self.Status.NOT_STARTED:
            color = "DARK_RED"
        tprint("Model status: "+self.status.name+"  "+self.model_dir, color)

        for sim in self.sims:
            sim.report()
