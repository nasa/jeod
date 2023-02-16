#// Initialize from UTC calendar date.
jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.time_tai.initialize_from_name = "UTC"
jeod_time.time_ut1.initialize_from_name = "TAI"
jeod_time.time_tt.initialize_from_name  = "TAI"
jeod_time.time_gmst.initialize_from_name  = "UT1"

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_ut1.update_from_name = "TAI"
jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_tt.update_from_name  = "TAI"
jeod_time.time_gmst.update_from_name  = "UT1"

jeod_time.time_utc.true_utc = False
jeod_time.time_ut1.true_ut1 = False

def set_utc( yr, mn, dy, hr, mt, sec) :
  jeod_time.time_utc.calendar_year   = yr
  jeod_time.time_utc.calendar_month  = mn
  jeod_time.time_utc.calendar_day    = dy
  jeod_time.time_utc.calendar_hour   = hr
  jeod_time.time_utc.calendar_minute = mt
  jeod_time.time_utc.calendar_second = sec

def set_state(name) :
  #defaults
  vehicle.trans_init.set_subject_body( vehicle.dyn_body )
  vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
  vehicle.trans_init.body_frame_id     = "composite_body"

  vehicle.lvlh_init.set_subject_body( vehicle.dyn_body )
  vehicle.lvlh_init.planet_name                = "Earth"
  vehicle.lvlh_init.body_frame_id              = "composite_body"
  vehicle.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  vehicle.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  vehicle.lvlh_init.orientation.euler_angles   = [ 0.0, 0.0, 0.0]
  vehicle.lvlh_init.ang_velocity               = [ 0.0, 0.0, 0.0]

  if name == "clementine" :
    set_utc( 1994, 3, 1, 0, 0, 0.0)
    vehicle.trans_init.reference_ref_frame_name = "Moon.inertial"
    vehicle.trans_init.position  = trick.attach_units( "km",[  1296.944012, -1060.824450, 2522.289146])
    vehicle.trans_init.velocity  = trick.attach_units( "km/s",[ -.930578, -.439312, .862075])

  elif name == "rosetta" :
    set_utc( 2009, 11, 13, 5, 0, 0.0)
    vehicle.trans_init.position  = trick.attach_units( "km",[   87396.6219145 ,  23042.6606938  ,  -48761.8708343  ])
    vehicle.trans_init.velocity  = trick.attach_units( "km/s",[   -7.8839651,      -3.2492092,       4.7952127 ])

  return
