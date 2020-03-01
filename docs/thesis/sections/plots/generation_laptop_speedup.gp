set datafile separator ','
set size 2,1
# unset key
set key right
set key spacing 1 box opaque
# set yr [0:0.5]
set yrange [0:0.001 < * < 1000]
# set ytics 0.25
set xr [-0.75:19]
set xtics rotate by 40 right
set yl "Speed-Up"
set format y "%.1f"
# set logscale y
bs = 0.75
scale_mt = 13.64
scale_xrsr = 3.07
scale_msws = 8.96
plot \
  "generation_laptop.dat" every :::0::0 u 0:(scale_mt/$3):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#990000" title "\\footnotesize STL MT19937",\
  "" every :::1::1 u ($0+1):(scale_mt/$3):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#0000bb" title "\\footnotesize Boost MT19937",\
  "" every :::2::2 u ($0+2):(scale_mt/$3):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#339933" title "\\footnotesize MT19937",\
  "" every 2:::3::3 u (2*$0+5+1):(scale_xrsr/$3):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#000000" title "\\footnotesize Xoroshiro128+",\
  "" every 2::1:3::3 u (2*$0+5+1+1):(scale_xrsr/$3):(bs):xtic(1) with boxes fs solid 0.25 lc rgb "#000000" notitle,\
  "" every 2:::4::4 u (2*$0+11+2):(scale_msws/$3):(bs):xtic(1) with boxes fs solid 0.5 lc rgb "#0055ff" title "\\footnotesize MSWS",\
  "" every 2::1:4::4 u (2*$0+11+2+1):(scale_msws/$3):(bs):xtic(1) with boxes fs solid 0.25 lc rgb "#0055ff" notitle,\
  [-0.375:4.375] 1 with l dt 2 lc black notitle,\
  [5.875:11.375] 1 with l dt 2 lc black notitle,\
  [12.875:18.375] 1 with l dt 2 lc black notitle,\
