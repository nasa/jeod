# This is an input file for the surface model verification
#
# Andrew Spencer
# August, 2010
#
#
# NOTE: THE DOCUMENTATION REFERS TO MASS BODY C AS A DIRECT
# CHILD OF MASS BODY A. IN THE CODE, THIS IS ACTUALLY MASS BODY
# D. MASS BODY C ITSELF IS NEVER USED IN THIS PARTICULAR
# DEMONSTRATION. MASS BODY D IN CODE = MASS BODY C IN DOCUMENTATION
exec(compile(open("SET_test/RUN_articulation_1/input.py", "rb").read(), "SET_test/RUN_articulation_1/input.py", 'exec'))
articulation.surface.struct_body_name = "mass_bodyc"
