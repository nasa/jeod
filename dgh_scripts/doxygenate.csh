#!/bin/tcsh -f

chdir $JEOD_HOME

set scripts = $JEOD_HOME/dgh_scripts

printf "Searching for files to doxygenate... "
set mainline_files = `find models -name '*.[ch][ch]' | \
                      grep -v -E '/(verif|\.svn|unit_test|data)/'`
set test_harness_files = `find tools/test_harness -name '*.[ch][ch]' | \
                          grep -v '/unit_tests/'`
set files = ($mainline_files $test_harness_files)
printf "Done.\n"

set doxygen_config = $scripts/html_config.dox

if (-d doxygen) then
   printf "Removing previously generated jeod/doxygen... "
   rm -rf doxygen
   printf "Done.\n"
endif

mkdir -p doxygen/doxygen
foreach file ($files)
   mkdir -p doxygen/$file:h
   cp -p $file doxygen/$file
end

printf "End of jeod_pp processing\n\n" >> doxygen/doxygen.err

cp docs/templates/jeod/mainpage.txt doxygen/mainpage.txt

cp README_jeod doxygen/README
cp verif/SIM_dyncomp/S_define doxygen/S_define
foreach jobclass (initialization scheduled derivative)
   cp docs/tex/figs/phasing/$jobclass.dot doxygen/$jobclass.dot
end

printf "Generating doxygen HTML. See jeod/doxygen/doxygen.out and doxygen.err for detailed report... "
chdir doxygen
(doxygen $doxygen_config >> doxygen.out) >>& doxygen.err
printf "Done.\n"

chdir $JEOD_HOME
if (-d html) then
   printf "Replacing previously generated doxygen HTML... "
   rm -rf html
   mkdir html
   mv doxygen/doxygen/html html/jeod
   printf "Done.\n"
endif
