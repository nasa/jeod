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

# PURPOSE:
#     (Plate model aero initialization.)
#
# REFERENCE:
#     ((None))
#
# ASSUMPTIONS AND LIMITATIONS:
#   (One-plate shape model for aerodynamic forces and torques.
#    Objects are numbered in the original input file as follows:
#      1 = core body
#
# 

# cache the method for attaching units
set_units = trick.sim_services.attach_units

NUM_PLATE = 2

SPEC_DRAG_COEF = 5.0
DIFF_DRAG_COEF = 5.0

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0,  0.0,   0.0]
flat_plate.area  =  1.0
flat_plate.normal = [1.0 , 0.0 , 0.0 ]
flat_plate.param_name = "flat_plate_material"
flat_plate.temperature = 70

aero_test.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0,  0.0,   0.0]
flat_plate.area  =  1.0
flat_plate.normal = [-1.0 , 0.0 , 0.0 ]
flat_plate.param_name = "flat_plate_material"
flat_plate.temperature = 70

aero_test.surface.add_facet(flat_plate)

params = trick.FlatPlateAeroParams()
params.thisown = 0

params.drag_coef_norm              = SPEC_DRAG_COEF
params.drag_coef_spec              = SPEC_DRAG_COEF
params.drag_coef_tang          = DIFF_DRAG_COEF
params.drag_coef_diff          = DIFF_DRAG_COEF
params.epsilon = 0.0
params.coef_method = trick.AeroDragEnum.Calc_coef
params.calculate_drag_coef = False
params.set_name("flat_plate_material")

aero_test.surface_factory.add_facet_params(params)
