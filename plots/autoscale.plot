#
# $Id: autoscale.dem,v 1.2 2014/01/17 06:02:21 sfeam Exp $
# Demo of constraint autoscaling: Autoscaling with limits.
# 

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


set multiplot layout 2, 2 title "Autoscaling with constraints (x-axis always unaffected)"

unset key
set ytics 5

set title "unconstrained"
set ylabel "[*:*]"
set yrange [*:*]
set xlabel "[*:*]" offset 0, 0.4
set xrange [*:*]
plot "data/3.dat"

set title "minimum range guaranteed"
set ylabel "[*<-15:20<*]"
set yrange [*<-15:20<*]
set xlabel "[*<-10:10<*]"
set xrange [*<-10:10<*]
plot "data/3.dat"

set ytics 2

set title "clip to maximum range"
set ylabel "[-8<*:*<5]"
set yrange [-8<*:*<5]
set xlabel "[-40<*:*<30]"
set xrange [-40<*:*<30]
plot "data/3.dat"

set title "mixed"
set xlabel "[-30<*<10:-10<*<45]"
set xrange [-30<*<10:-10<*<45]
set ylabel "[-8<*<-6:7<*<9]"
set yrange [-8<*<-6:7<*<9]
plot "data/3.dat"

unset multiplot

pause -1 "Hit return to continue"
unset xlabel
unset ylabel

set multiplot layout 2, 2 title "Autoscaling with constraints"

set ytics 50

set autoscale xy
set title "autoscale xy"
plot "data/silver.dat"

set yrange [15<*<25:*]
set title "set yrange [15<*<25:*]"
plot "data/silver.dat"

set autoscale ymin
set title "set autoscale ymin"
plot "data/silver.dat"

set ytics 20
set yrange [15<*<25:135<*<225]
set title "set yrange [15<*<25:135<*<225]"
plot "data/silver.dat"

unset multiplot

pause -1 "Hit return to continue"
reset
