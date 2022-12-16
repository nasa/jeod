def two_sided_plate( epsilon = 0.0,
                     coeff_method = trick.AeroDragEnum.Calc_coef,
                     calc_drag_coeff = True,
                     z_pos = 0.0):
  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = [0.0,  0.0,   z_pos]
  flat_plate.area  =  1.0
  flat_plate.normal = [1.0 , 0.0 , 0.0 ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  flat_plate = trick.FlatPlate()
  flat_plate.thisown = 0
  flat_plate.position = [0.0,  0.0,   z_pos]
  flat_plate.area  =  1.0
  flat_plate.normal = [-1.0 , 0.0 , 0.0 ]
  flat_plate.param_name = "flat_plate_material"
  flat_plate.temperature = 70

  aero_test.surface.add_facet(flat_plate)

  params = trick.FlatPlateAeroParams()
  params.thisown = 0
  params.drag_coef_norm  = 5.0
  params.drag_coef_spec  = 5.0
  params.drag_coef_tang  = 5.0
  params.drag_coef_diff  = 5.0
  params.epsilon = epsilon #input argument
  params.coef_method = coeff_method #input argument
  params.calculate_drag_coef = calc_drag_coeff #input argument
  params.set_name("flat_plate_material")

  aero_test.surface_factory.add_facet_params(params)
