# This is an input file for the lighting verification
#
# Scott Piggott
# July 2006
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

trick.add_read(1.0, """
light.r_bottom  = 4.0
light.r_top     = 4.0
light.d_centers = 0.0
""")
