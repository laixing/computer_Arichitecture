# run main.cpp with the command line below, it will generate a.out file, then run a.out file.
g++ -std=c++11 main.cpp -lOpenCL
./a.out

# we can use CapsBasic tools to list platforms and devices
./capsbasic
./capsbasic CPU   #list CPU information
./capsbasic Graphics  #list GPU information