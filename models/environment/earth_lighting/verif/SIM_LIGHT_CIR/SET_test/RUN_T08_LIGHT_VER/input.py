# This is an input file for the lighting verification
#
# Scott Piggott
# July 2006
#

exec(compile(open("Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))

trick.add_read(1.0, """
light.r_bottom  = 3.0
light.r_top     = 1.5
light.d_centers = 4.25
""")
