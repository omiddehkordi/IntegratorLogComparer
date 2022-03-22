Description:

The program takes in multiple log files with data given for each component on the same circuit board in multiple test runs. The program then compares the multiple values for the same component over multiple tests to display the differences in outputs given by the test machine. This can be used to see whether the test machine is working correctly.





Once you compile the code


1) Drag the file onto your Desktop


2) Open your Terminal and change the directory to the Folder Name (use the command cd PATHNAME; e.g. cd /Users/omid/Desktop/logcompare)


3) use the command ./compare PATH1 PATH2 (e.g. ./compare /Users/omid/Desktop/log1.log  /Users/omid/Desktop/log2.log)


4) The program will then produce an output file in the same folder as the program that has the difference between measurements and tolerances printed with the corresponding component name



Let me know if there are any issues.
