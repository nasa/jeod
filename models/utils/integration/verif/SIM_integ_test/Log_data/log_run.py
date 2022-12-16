def log_data (logger, log_cycle, run_tests) :

   # There are two kinds of tests, rotation and translation, each with
   # their own set of data to be logged.
   rot_tests = ('torque_free_sphere',
                'torque_free_symtop',
                'torque_free_symtop_b',
                'sho_sphere')
   rot_state_items = (
      ("Q_inertial_body", 'quaternion'),
      ("body_rate",       'vector'))
   rot_items = (
      ("orient_error.theta_",  'scalar'),
      ("rate_error_mag",       'scalar'),
      ("rel_energy_error",     'scalar'),
      ("true_canon_state",     'set',     rot_state_items),
      ("prop_integ_state",     'set',     rot_state_items))

   trans_tests = ('orbit', 'spring_damper')
   trans_items = (
      ("rel_position_err_mag", 'scalar'),
      ("rel_velocity_err_mag", 'scalar'),
      ("rel_energy_error",     'scalar'),
      ("true_canon_state",     'trans_state'),
      ("prop_integ_state",     'trans_state'))


   # Create the "integ" data recording group.
   logger.open_group (log_cycle, "integ")

   # Add items to be logged for each active test to the recording group.
   for test_name in run_tests :
      if test_name in rot_tests :
         logger.log_set ('test.' + test_name, rot_items)
      elif test_name in trans_tests :
         logger.log_set ('test.' + test_name, trans_items)
      else :
         raise RuntimeError ('Test "%s" is of an unknown type' % (test_name))

   # Close the recording group.
   logger.close_group ()
