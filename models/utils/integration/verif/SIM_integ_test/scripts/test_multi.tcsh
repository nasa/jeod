#!/bin/tcsh -f
if (-d test_multi) then
   rm -rf test_multi
endif
mkdir test_multi

set integs = (0 1 3 5 9 10 11)
set classes = ( \
   'RotationTestTorqueFreeSymTop' \
   'RotationTestSHOSphere' \
   'TranslationTestOrbit' \
   'TranslationTestSpringDamper' )

set class_args = '';
foreach class ($classes)
   set class_args = "$class_args -class $class";
end
  

set nruns = 10
set failed = 0
foreach integ_opt ($integs)
   set prefix = test_multi/run_${integ_opt}
   foreach class ($classes)
      scripts/run_cases.pl -prefix ${prefix}_series \
         -end $nruns -run 1 -min -1 -max -1 -ncpus 1 \
         -class $class -integ $integ_opt > /dev/null
   end
   scripts/run_cases.pl -prefix ${prefix}_parallel \
      -end $nruns -run $nruns -min -1 -max -1 -ncpus 1 \
      $class_args -integ $integ_opt > /dev/null
   set diff_count = `diff ${prefix}_series.dat ${prefix}_parallel.dat | wc -l`
   if ($diff_count == 0) then
      set interp = 'passed'
   else
      set interp = "failed $diff_count"
      set failed = 1
   endif
   echo integ_opt=$integ_opt $interp
end
if ($failed == 0) then
   echo "All tests pass"
endif
