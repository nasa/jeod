# This is an input file for the lighting verification
#
# Scott Piggott
# July 2006
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

trick.add_read(1.0, """
LIGHT.r_bottom  = 3.0
LIGHT.r_top     = 2.0
LIGHT.d_centers = 0.5
""")
