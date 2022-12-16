# Kernel files to use
env.spice.metakernel_filename = "data/kernels.tm"

# Objects to load from SPICE
env.spice.add_planet_name("Sun")
env.spice.add_planet_name("Earth")
env.spice.add_planet_name("Moon")
env.spice.add_planet_name("Mars")

env.spice.add_orientation("Earth")
env.spice.add_orientation("Moon")
env.spice.add_orientation("Mars")
