# This is an input file for the lighting verification
#
# Scott Piggott
# July 2006
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

trick.add_read(1.0, """
light.r_bottom  = 0.25
light.r_top     = 0.25
light.d_centers = 1.0
""")
