Verification for the Thermal-Rider Model:

The Thermal-Rider model is intended to provide generic thermal modeling
capabilities to surface interaction models, particularly radiation pressure
and, as needed, aerodynamics.  It is not a stand-alone model, indeed it would
be a very unusual situation in which the temperature profile of a vehicle
perfectly insulated from the surrounding environment would provide significant
data.  At the very least, it would usually make sense to consider the effect of
radiative emissions (for example, simply modeling the temperature of the 
radiators of a spacecraft is meaningless unless they can radiate).

Therefore, the verification of the thermal model must be performed in the
context in which it is being applied.  For that reason, the verification of 
the thermal model has been handled by verification tests included in the 
radiation pressure model.  These test both the temperature and physical response 
of a vehicle within various radiation environments.

The Radiation Pressure model is located within the same interaction model
collection as this model, at models/interactions/radiation_pressure.

In particular, the tests located at 
radiation_pressure/verif/SIM_2_SHADOW_CALC/RUN_shadow_cooling
radiation_pressure/verif/SIM_2_SHADOW_CALC/RUN_ten_plates
radiation_pressure/verif/SIM_1_BASIC/RUN_thermal_dump
specifically test the thermal model, and are documented in the thermal model
documentation.
