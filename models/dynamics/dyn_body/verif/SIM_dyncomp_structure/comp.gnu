set term pdf
set output 'SIM_dyncomp_compare.pdf'
set ylabel 'Position Difference (micrometers)'
set xlabel 'Sim Time (seconds)'
set key top left
plot 'RUN_10A.dat' using 1:8 w lines t '  RUN\_10A', 'RUN_10B.dat' using 1:8 w lines t '  RUN\_10B', 'RUN_10C.dat' using 1:8 w lines t '  RUN\_10C', 'RUN_10D.dat' using 1:8 w lines t '  RUN\_10D'
