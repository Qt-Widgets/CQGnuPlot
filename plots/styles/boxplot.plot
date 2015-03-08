set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

unset key
set border 2
set xtics ("A" 1, "B" 2) scale 0.0
set xtics nomirror
set ytics nomirror
#set yrange [0:100]

plot 'data/silver.dat' using (1):2, '' using (2):(5*$3)
