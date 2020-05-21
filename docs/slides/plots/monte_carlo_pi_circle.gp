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
set style fill transparent solid 0.35

set label 1 at 0.5, 0.5 "$A$" center front

plot \
  sqrt(1-x**2) with filledcurves y1=0 fc rgb "#3333ff" lt -1 lw 4 lc black,\
  sqrt(1-x**2) with l lw 4 lc black