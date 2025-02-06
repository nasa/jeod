################TRICK HEADER#######################################
#PURPOSE:
#  (To define the shadowing effects)
####################################################################################

third_body = trick.RadiationThirdBody()
third_body.thisown = 0
third_body.set_name("Earth")
third_body.active = True
third_body.shadow_geometry = trick.RadiationThirdBody.Conical

radiation.rad_pressure.add_third_body(third_body)


third_body = trick.RadiationThirdBody()
third_body.thisown = 0
third_body.set_name("Moon")
third_body.active = True
third_body.shadow_geometry = trick.RadiationThirdBody.Conical

radiation.rad_pressure.add_third_body(third_body)
