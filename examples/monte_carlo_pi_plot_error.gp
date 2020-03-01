set size 1.5,1
unset key

set logscale x
set format x "$10^{%L}$"
set format y "$%.2f$"

# set rmargin 1
# set tmargin 0.5
# set lmargin 4
# set bmargin 1.5

set xl "Samples $n$"
set yl "Relative Error"

set xr [10:10**7]
set yr [-0.01:0.12]

set xtics 10, 100
set ytics 0.0, 0.05

pi = 3.141592
f(x) = sqrt((4 - pi) / (pi * x))

plot 0 with lines lc black dt 2 lw 3, \
  f(x) with lines lc black lw 3, \
  'monte_carlo_pi_plot_1.txt' u 1:4 with linespoints pt 13 ps 0.8 lc rgb '#ff5500' lw 3, \
  'monte_carlo_pi_plot_2.txt' u 1:4 with linespoints pt 13 ps 0.8 lc rgb '#0055ff' lw 3