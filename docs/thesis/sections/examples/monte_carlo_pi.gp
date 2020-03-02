set xr [0:1]
set yr [0:1]

set xtics 0.5
set ytics 0.5

set size square
unset key
set rmargin 0.5
set tmargin 0.5
set lmargin 3
set bmargin 1.5

set samples 1000

plot sqrt(1-x**2) with l lw 4 lc black,\
  'monte_carlo_pi_data_in.txt' u 1:2 with p pt 13 ps 0.5 lt rgb '#0077ff', \
  'monte_carlo_pi_data_out.txt' u 1:2 with p pt 13 ps 0.5 lt rgb '#ff7700'