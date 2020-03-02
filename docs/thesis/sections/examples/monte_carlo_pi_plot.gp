set size 1.5,1
unset key

set logscale x
set format x "$10^{%L}$"
set format y "$%.1f$"

# set rmargin 1
# set tmargin 0.5
# set lmargin 4
# set bmargin 1.5

set xl "Samples $n$"
set yl "Sampled $\\mathrm{MCI}_n(f)$"

set xr [10:10**7]
set yr [2.8:3.5]

set xtics 10, 100
set ytics 2.8, 0.2

pi = 3.14159
set ytics add ("$\\pi$" pi)

plot pi with lines lc black dt 2 lw 3, \
  'monte_carlo_pi_plot_1.txt' u 1:2 with linespoints pt 13 ps 0.8 lc rgb '#ff5500' lw 3, \
  'monte_carlo_pi_plot_2.txt' u 1:2 with linespoints pt 13 ps 0.8 lc rgb '#0055ff' lw 3