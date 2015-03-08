#
# Example of using histogram modes
#
reset
set title "US immigration from Europe by decade"
set datafile missing "-"
set xtics nomirror rotate by -45
set key noenhanced
#
# 'newhistogram' keyword to plot
#
set title "Default Histogram Colouring"
set key under
set key invert autotitle columnhead
set style data histogram
set style histogram clustered gap 1 title offset 2,-2 font ",11" boxed
set style fill solid noborder
set boxwidth 0.95
unset xtics
set xtics rotate by -45
set xlabel "Immigration from different regions" offset 0, 0
set ytics
set grid y
set auto y
set bmargin 12
#
plot newhistogram "Northern Europe", 'data/immigration.dat' using 6:xtic(1), '' u 13, '' u 14, newhistogram "Southern Europe", '' u 9:xtic(1), '' u 17 , '' u 22 , newhistogram "British Isles", '' u 10:xtic(1) , '' u 21
