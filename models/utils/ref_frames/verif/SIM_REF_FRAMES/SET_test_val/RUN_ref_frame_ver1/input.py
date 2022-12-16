# This is an input file for the ref frames verification
#
# Andrew Spencer
# October, 2009
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

# structure 1 setup


ref_frames.frame1B.state.trans.position = [1848.16, 9048.81, 9797.48]
ref_frames.frame1B.state.trans.velocity = [-408.72, -594.896, 262.212]

ref_frames.frame1B.state.rot.T_parent_this = [
[0.02165177281818631, -0.9708670732541415, 0.2386384855901647  ],
[-0.1303430129048092, 0.2339159463811323, 0.9634801653461872   ],
[-0.9912325154678525, -0.05196591286176722, -0.1214810445284236]]

ref_frames.frame1B.state.rot.ang_vel_this = [11.7418, -29.6676, 31.8778]


  
ref_frames.frame1C.state.trans.position = [-2372.84,   -4588.49,    9630.89]
ref_frames.frame1C.state.trans.velocity = [488.898,     624.06,   -679.136]
                 



ref_frames.frame1C.state.rot.T_parent_this = [
[0.7059970877834382, -0.6729909535450022, -0.2205703708295675],
[0.3550094808358443, 0.06680598428211491, 0.9324726424838222 ],
[-0.6128102320930751, -0.73662754286728, 0.2860829993749103  ]]
                 
ref_frames.frame1C.state.rot.ang_vel_this = [  -3.77389,    88.5168,    91.3287]

# structure 2 setup

 
ref_frames.frame2B.state.trans.position = [-7790.52,    7150.37,   -9037.21]
ref_frames.frame2B.state.trans.velocity = [-197.81,    30.5409,   -744.074]
                

ref_frames.frame2B.state.rot.T_parent_this = [
[0.9055697770991294, 0.4006122484928437, 0.1394747473994526   ],
[-0.4100036383807383, 0.9109467042835068, 0.04553151073239373 ],
[-0.1088135805825256, -0.09841711392093039, 0.9891782834091561]]
                
ref_frames.frame2B.state.rot.ang_vel_this = [-60.9867,   -61.7666,    85.9442]
                
                
ref_frames.frame2C.state.trans.position = [-5004.72,    4710.88,    596.189]
ref_frames.frame2C.state.trans.velocity = [-521.65,     -96.73,    818.149]
                

ref_frames.frame2C.state.rot.T_parent_this = [
[0.09641301427589605, -0.8301616645135473, 0.5491230658516651 ],
[-0.6488031425776438, 0.3659638227289848, 0.6671768600865603  ],
[-0.7548238290221164, -0.4205973029225039, -0.5033278215186462]]
                
ref_frames.frame2C.state.rot.ang_vel_this = [-65.9605,   -51.8595,    97.2975]


trick.exec_set_terminate_time(1.0)
