# PURPOSE:
#     (Log data for earth lightin verification
#
# REFERENCES:
#     (None)
#
# ASSUMPTIONS AND LIMITATIONS:
#     (None)
#
#  

try:
   DR_GROUP_ID
except NameError:
   def initialize_globals():
      global DR_GROUP_ID
      global drg
      DR_GROUP_ID = 0
      drg = []
   initialize_globals()

# def create_dr_group() :
#    global DR_GROUP_ID  # Create global variable when not defined
#    global drg          # Create a data recording group variable
#    DR_GROUP_ID = 0
#    drg = []            # Initialize variable as an empty array

def log_lighting_verif(log_cycle) :

#   try:
#      DR_GROUP_ID
#   except NameError:
#      create_dr_group()
#
   # Build a data recording group name.
   group_name = "REF_FRAME_VER"
   global DR_GROUP_ID
   global drg
   drg.append(trick.DRBinary(group_name))
   drg[DR_GROUP_ID].set_freq(trick.DR_Always)
   drg[DR_GROUP_ID].enable()
   drg[DR_GROUP_ID].set_cycle(log_cycle)

   drg[DR_GROUP_ID].add_variable("ref_frames.state1AwrtC.rot.Q_parent_this.scalar")
   drg[DR_GROUP_ID].add_variable("ref_frames.state1AwrtC.rot.ang_vel_mag")
   drg[DR_GROUP_ID].add_variable("ref_frames.state1CwrtA.rot.Q_parent_this.scalar")
   drg[DR_GROUP_ID].add_variable("ref_frames.state1CwrtA.rot.ang_vel_mag")
   drg[DR_GROUP_ID].add_variable("ref_frames.state2BwrtC.rot.Q_parent_this.scalar")
   drg[DR_GROUP_ID].add_variable("ref_frames.state2BwrtC.rot.ang_vel_mag")
   drg[DR_GROUP_ID].add_variable("ref_frames.state2CwrtB.rot.Q_parent_this.scalar")
   drg[DR_GROUP_ID].add_variable("ref_frames.state2CwrtB.rot.ang_vel_mag")

   for ii in range(0,3) :
      var = "ref_frames.pos1AfromC[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.pos1CfromA[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.pos2BfromC[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.pos2CfromB[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1AwrtC.trans.position[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1AwrtC.trans.velocity[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1AwrtC.rot.Q_parent_this.vector[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1AwrtC.rot.ang_vel_unit[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1AwrtC.rot.ang_vel_this[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1CwrtA.trans.position[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1CwrtA.trans.velocity[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1CwrtA.rot.Q_parent_this.vector[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1CwrtA.rot.ang_vel_unit[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state1CwrtA.rot.ang_vel_this[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2BwrtC.trans.position[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2BwrtC.trans.velocity[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2BwrtC.rot.Q_parent_this.vector[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2BwrtC.rot.ang_vel_unit[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2BwrtC.rot.ang_vel_this[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2CwrtB.trans.position[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2CwrtB.trans.velocity[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2CwrtB.rot.Q_parent_this.vector[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2CwrtB.rot.ang_vel_unit[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)
      var = "ref_frames.state2CwrtB.rot.ang_vel_this[" + str(ii) + "]"
      drg[DR_GROUP_ID].add_variable(var)

      for jj in range(0,3) :
         var = "ref_frames.state1AwrtC.rot.T_parent_this[" + str(ii) + "][" + str(jj) + "]"
         drg[DR_GROUP_ID].add_variable(var)
         var = "ref_frames.state1CwrtA.rot.T_parent_this[" + str(ii) + "][" + str(jj) + "]"
         drg[DR_GROUP_ID].add_variable(var)
         var = "ref_frames.state2BwrtC.rot.T_parent_this[" + str(ii) + "][" + str(jj) + "]"
         drg[DR_GROUP_ID].add_variable(var)
         var = "ref_frames.state2CwrtB.rot.T_parent_this[" + str(ii) + "][" + str(jj) + "]"
         drg[DR_GROUP_ID].add_variable(var)

   # Add the data recording group to Trick's data recording.
   trick.add_data_record_group(drg[DR_GROUP_ID], trick.DR_Buffer)
   DR_GROUP_ID += 1

   return
