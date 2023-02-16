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

