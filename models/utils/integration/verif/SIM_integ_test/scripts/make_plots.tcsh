#!/bin/tcsh -f

if ($#argv == 0) then
   set nrevlist = ( 1 3 10 30)
else
   set nrevlist = $argv
endif

foreach nrev ( $nrevlist )
   echo $nrev
   scripts/scan_runs.pl -gnu -plots 4 -xrange 3.16e-05:31.6 -yrange 1e-14:10 \
      -prefix monte_revs_${nrev}_plots/plot monte_revs_${nrev}.dat
   rm ../../docs/tex/figures/plot_*revs_${nrev}_monte_err.pdf
   cp -p monte_revs_${nrev}_plots/plot_*.pdf ../../docs/tex/figures/
end
