# This is an input file for the ref frames verification
#
# Andrew Spencer
# October, 2009
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

# structure 1 setup



ref_frames.frame1B.state.trans.position = [-8033.64,    604.712,   -3992.58]
ref_frames.frame1B.state.trans.velocity = [627.973,   -291.984,    431.651]

ref_frames.frame1B.state.rot.T_parent_this = [
[0.4272687159464157, -0.3061054736119365, -0.8507296182679391],
[0.1149180951186981, 0.9516978685416249, -0.2847191571138446 ],
[0.8967916568455525, 0.02388736147296944, 0.4418077841936113 ]]

ref_frames.frame1B.state.rot.ang_vel_this = [10.6216,     37.241,   -19.8118]

  

ref_frames.frame1C.state.trans.position = [-4177.44,   -9830.52,   -3014.55]
ref_frames.frame1C.state.trans.velocity = [698.106,   -666.528,    178.132]
                 

ref_frames.frame1C.state.rot.T_parent_this = [
[0.6361951982813826, 0.3331380394803984, 0.695898495712463   ],
[0.564199301993485, 0.414337780792221, -0.7141451890464865   ],
[-0.5262459665534518, 0.8469611856864128, 0.07564345726425621]]
                 
ref_frames.frame1C.state.rot.ang_vel_this = [-3.26008,      56.12,    88.1867]

# structure 2 setup

 
 
ref_frames.frame2B.state.trans.position = [-4282.53,    4820.22,    1206.12]
ref_frames.frame2B.state.trans.velocity = [-582.986,    251.806,   -290.441]
                
ref_frames.frame2B.state.rot.T_parent_this = [
[0.9945282144718409, -0.1036578778650371, -0.01298749305944376],
[0.09724675025241425, 0.9640168959942612, -0.2474156296658203 ],
[0.03816674186771286, 0.2447988329100178, 0.9688223940542955  ]]
                
ref_frames.frame2B.state.rot.ang_vel_this = [-98.2663,    73.0249,   -34.3877]
                
  
ref_frames.frame2C.state.trans.position = [1614.85,   -1787.66,    4228.86]
ref_frames.frame2C.state.trans.velocity = [695.949,    699.888,    638.531]
                
ref_frames.frame2C.state.rot.T_parent_this = [
[0.5617023094980858, -0.1323969387778994, 0.8166771492497862],
[0.7140778066635471, 0.5761043624264366, -0.3977394242765062],
[-0.4178317861712943, 0.8065821806830784, 0.4181408665383421]]
                
ref_frames.frame2C.state.rot.ang_vel_this = [-53.0864,   -65.4446,   -40.7619]


trick.exec_set_terminate_time(1.0)
