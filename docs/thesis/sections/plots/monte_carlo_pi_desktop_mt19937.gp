set datafile separator ','
set size 2,1
# unset key
set key left
# set yr [0:0.5]
set yrange [0:0.001 < * < 1000]
# set ytics 0.25
set xr [-0.75:12]
set xtics rotate by 35 right
set yl "speed-up"
set format y "%.1f"
bs = 0.75
scale = 0.37
plot \
  "monte_carlo_pi_desktop.dat" every :::0::0 u 0:(scale/$2):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#ff5500" title "STL",\
  "" every :::1::1 u ($0+3):(scale/$2):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#335577" title "Boost",\
  "" every :::2::2 u ($0+6):(scale/$2):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#0055ff" title "pxart",\
  1 with l dt 2 lc black notitle,\