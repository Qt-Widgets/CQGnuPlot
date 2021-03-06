set title "Heat Map generated from a file containing Z values only"
unset key
set tic scale 0

# Color runs from white to green
set palette rgbformula -7,2,-7
set cbrange [0:5]
set cblabel "Score"
unset cbtics

set xrange [-0.5:4.5]
set yrange [-0.5:4.5]

$map1 << EOD
5 4 3 1 0
2 2 0 0 1
0 0 0 1 0
0 0 0 2 3
0 1 2 4 3
EOD

set view map
splot '$map1' matrix with image
