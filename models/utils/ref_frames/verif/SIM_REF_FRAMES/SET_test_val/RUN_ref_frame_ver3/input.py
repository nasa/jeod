# This is an input file for the ref frames verification
#
# Andrew Spencer
# October, 2009
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

# structure 1 setup

ref_frames.frame1B.state.trans.position = [2664.71,   -1536.57,    2810.05]
ref_frames.frame1B.state.trans.velocity = [-875.372,   -518.052,    943.623]



ref_frames.frame1B.state.rot.T_parent_this = [
[0.8532057436490301, 0.2501060894816011, -0.4576973923986534 ],
[-0.3517114947196397, 0.9238850041384419, -0.1507830348885044],
[0.3851480020542523, 0.2896263854215159, 0.8762291785721845  ]]

ref_frames.frame1B.state.rot.ang_vel_this = [-67.6122,    28.9065,    67.1808]

 

ref_frames.frame1C.state.trans.position = [6021.7,    3867.71,    9159.91]
ref_frames.frame1C.state.trans.velocity = [-460.916,     770.16,    322.472]
                 

ref_frames.frame1C.state.rot.T_parent_this = [
[-0.9655570135702486, -0.1962017385355807, -0.1708933332254907],
[0.1197804372000868, 0.2478810463059914, -0.9613571832292113  ],
[0.2309811689498931, -0.9487148489814329, -0.2158421527706723 ]]
                 
ref_frames.frame1C.state.rot.ang_vel_this = [17.5874,    72.1758,   -47.3486]

# structure 2 setup

 
 
ref_frames.frame2B.state.trans.position = [-2874.98,   -911.135,    5762.09]
ref_frames.frame2B.state.trans.velocity = [-644.443,   -647.618,    679.017]
                

ref_frames.frame2B.state.rot.T_parent_this = [
[-0.1766406884173243, -0.3584722730823265, -0.9166764405321235],
[-0.07753089247582201, 0.9334947713181074, -0.3501092295750815],
[0.9812171155801128, 0.009227207212613942, -0.1926858342995125]]
                
ref_frames.frame2B.state.rot.ang_vel_this = [-70.9282,    23.6231,    11.9396]
                
  
                
ref_frames.frame2C.state.trans.position = [-2564.41,   -6134.61,    5822.49]
ref_frames.frame2C.state.trans.velocity = [-318.074,    119.215,   -939.829]
                



ref_frames.frame2C.state.rot.T_parent_this = [
[-0.9774834677306422, 0.1620640475981063, -0.1351344322865095],
[0.04508295495462142, 0.786020216558487, 0.6165547391220908  ],
[0.2061397523182708, 0.5965798049192318, -0.7756280931456088 ]]
                
ref_frames.frame2C.state.rot.ang_vel_this = [-54.4716,   -64.7311,   -54.3886]


trick.exec_set_terminate_time(1.0)
