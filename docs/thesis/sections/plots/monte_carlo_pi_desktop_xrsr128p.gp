set datafile separator ','
set size 2,1
unset key
# set key left
# set yr [0:0.5]
set yrange [0:0.001 < * < 1000]
# set ytics 0.25
set xr [-0.5:6]
set xtics rotate by 35 right
set yl "speed-up"
set format y "%.1f"
bs = 0.35
scale = 0.25
plot \
  "monte_carlo_pi_desktop.dat" every 2:::3::3 u 0:(scale/$2):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#0055ff" title "single instance",\
  "" every 2::1:3::3 u ($0+1.15*bs):(scale/$2):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#ff5500" title "double instance",\
  1 with l dt 2 lc black notitle,\
