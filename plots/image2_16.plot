print ""
print "Gnuplot understands a few common binary formats.  Internally"
print "a function is linked with various extensions.  When the"
print "extension is specified at the command line or recognized via"
print "a special file type called \'auto\', Gnuplot will call the"
print "function that sets up the necessary binary information.  The"
print "known extensions are displayed using the \'show filetype\'"
print "command.  E.g.,"
print ""
show datafile binary filetypes
print "Here's an example where an EDF file is recognized when Gnuplot"
print "is in \'auto\' mode.  Details are pulled from the header of"
print "file itself and not specified at the command line.  The command"
print "line can still be used to over-ride in-file attributes."
print ""
set title "Automatically recognizing file type and extracting file information"
plot 'data/demo.edf' binary filetype=auto with image
pause -1 "Hit return to continue"
