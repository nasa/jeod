exec(compile(open("SET_test/RUN_test0/input.py", "rb").read(), "SET_test/RUN_test0/input.py", 'exec'))
# make adjustments to vehB state:
vehB.trans_init.velocity  = [ 0.0, 0.0, 1.0 ]

# Adjust the theta-dot
rel_state.vehA_in_vehB_curvilvlh.use_theta_dot_correction = False

# Set simulation stop time.
trick.stop(0)
