set multiplot layout 2, 2 title "Autoscaling with constraints (y-axis always unaffected)"

unset key
# set title offset 0,-0.4
# set xlabel offset 0,0.4

set ytics 5

set title "unconstrained"
set ylabel "[*:*]"
set yrange [*:*]
set xlabel "[*:*]"
set xrange [*:*]
plot "data/3.dat"

set title "minimum range guaranteed"
set ylabel "[*<-5:5<*]"
set yrange [*<-5:5<*]
set xlabel "[*<-40:30<*]"
set xrange [*<-40:30<*]
plot "data/3.dat"

set title "clip to maximum range"
set ylabel "[-20<*:*<20]"
set yrange [-20<*:*<20]
set xlabel "[-30<*:*<10]"
set xrange [-30<*:*<10]
plot "data/3.dat"

set title "mixed"
set xlabel "[-30<*<10:10<*<15]"
set xrange [-30<*<10:10<*<15]
set ylabel "[*:0<*<10]"
set yrange [*:0<*<10]
plot "data/3.dat"

unset multiplot

pause -1 "Hit return to continue"