#!/bin/tcsh -f

set target = $1

set copy_these = ( \
   docs/JEOD.pdf \
   docs/coordinates/COORDFRAME.pdf \
   docs/coding_standards/coding_standards.pdf \
   docs/templates/jeod/dynmath.pdf \
   docs/tex/refs/GGM.pdf \
   docs/tex/refs/NavigationStandards.pdf \
   docs/tex/refs/constellation_coord_sys.pdf \
   docs/tex/refs/de.pdf \
   docs/tex/refs/gottlieb_NASA_CR188243.pdf \
   docs/tex/refs/gram99.pdf \
   docs/tex/refs/gravityval.pdf \
   docs/tex/refs/lunar_cmd_2005_jpl_d32296.pdf \
   docs/tex/refs/met2.pdf \
   docs/tex/refs/met98.pdf \
   models/environment/atmosphere/docs/refs/gram99.pdf \
   models/environment/atmosphere/docs/refs/marshallMETTM.pdf \
   models/environment/atmosphere/docs/refs/met98.pdf \
   models/environment/gravity/docs/gottlieb_1993.pdf \
   models/environment/gravity/docs/thompson_AAS_08-205.pdf \
   models/utils/integration/docs/reference_materials/dlsode.pdf \
   models/utils/integration/docs/reference_materials/LSODE_description_u113855.pdf \
   models/utils/memory/docs/tex/atomic_methods.pdf \
   models/utils/orientation/docs/tex/representations.pdf \
   verif/SIM_dyncomp/docs/DYNCOMP.pdf \
   verif/SIM_dyncomp/docs/tex/figs/derivative.png \
   verif/SIM_dyncomp/docs/tex/figs/initialization.png \
   verif/SIM_dyncomp/docs/tex/figs/scheduled.png \
   verif/SIM_dyncomp/docs/tex/paper/siw.pdf)

foreach f ($copy_these)
   if (! -d $target/$f:h) mkdir -p $target/$f:h
   cp -p $f $target/$f:h
end

foreach d (models/*/*/docs)
   mkdir -p $target/$d
   cp -p $d/$d:h:t.pdf $target/$d
   cp -p $d/refman.pdf $target/$d
end
