
# Rendezvous, Proximity Operations and Capture (RPOC) Model Suite
#
# Copyright (C) 2001 Titan Systems Corporation - Astronautics Engineering
# Operations, Houston, TX. All rights reserved.
#
# Copyrighted by Titan Systems Corporation (TSC) and proprietary to it. Any
# unauthorized use of the RPOC models including source code, object code
# or executables is strictly prohibited and TSC assumes no liability for
# such actions or results thereof.
#
# The RPOC Model Suite has been developed under NASA Government Contracts and
# access to it may be granted for Government work by the following contact:
#
# Contact: Allan L. DuPont, EG4
#          Aeroscience and Flight Mechanics Division
#          NASA - Johnson Space Center, Houston, TX

# cache the method for attaching units
set_units = trick.sim_services.attach_units

def six_plate_orbiter( calc_coeff):

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1255.0 , 0.0, 383.4 ])
  flat_plate.area  =  119.4454385
  flat_plate.normal = [1.0 , 0.0 , 0.0  ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1069.0 , 0.0 , 396.5 ])
  flat_plate.area  =  229.91644
  flat_plate.normal = [0.0, 1.0, 0.0]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1059.6 , 0.0 , 332.0 ])
  flat_plate.area  =  454.4538
  flat_plate.normal = [0.0 , 0.0 , 1.0]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1255.0 , 0.0 , 383.4 ])
  flat_plate.area  =  119.4454385
  flat_plate.normal = [-1.0 , 0.0 , 0.0 ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1069.0 , 0.0 , 396.5 ])
  flat_plate.area  =  229.91644
  flat_plate.normal = [0.0 , -1.0 , 0.0 ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = set_units("in", [1059.6 , 0.0 , 332.0 ])
  flat_plate.area  =  454.4538
  flat_plate.normal = [0.0 , 0.0 , -1.0 ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  params = trick.FlatPlateAeroParams()
  params.thisown = 0
  params.drag_coef_norm = 2
  params.drag_coef_tang = 2
  params.drag_coef_spec = 2
  params.drag_coef_diff = 2
  params.epsilon = 0.0
  params.coef_method = calc_coeff # input argument
  params.calculate_drag_coef = calc_drag_coeff  = True
  params.set_name("flat_plate_material")

  aero_test.surface_factory.add_facet_params(params)
