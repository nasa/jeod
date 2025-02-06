# add more kernels
env.spice.metakernel_filename = "data/more_kernels.tm"

# Load Itokawa and Phobos from SPICE
env.spice.add_planet_name("Itokawa")
env.spice.add_planet_name("Phobos")

env.spice.add_orientation("Itokawa")
env.spice.add_orientation("Phobos")

# Log the results
exec(compile(open("../Log_data/phobos.dr", "rb").read(), "../Log_data/phobos.dr", 'exec'))
exec(compile(open("../Log_data/itokawa.dr", "rb").read(), "../Log_data/itokawa.dr", 'exec'))
