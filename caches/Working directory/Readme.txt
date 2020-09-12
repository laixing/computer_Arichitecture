this file will incllude all the instructions to run the program and simulations.

in my experiment, i created a folder ~/project all the source files were in project folder, and location for Gem5 is ~/gem5

1)in task1, for the linear-chase, the command should be 
make -f assignment.mak linear-chase

then program will have output: stride from 8-1200 and related average access time
after run this program, you can clean the .o file 
make -f assignment.mak clean 

2)in task1, for the random-chase, you can change the MIN_SIZE and MAX_SIZE in random-chase.cpp, 1024 means 1kB, 1024*1024 mean 1MB.
in task1, set MIN_SIZE 1024, set MAX_SIZE 1024*1024*128, then 
make -f assignment.mak random-chase 

then program will have output: memsize from 1kB to 128MB and related average access time 
after run this program, you can clean the .o file 
make -f assignment.mak clean 

3)in task2, for Gem5 simulations, i used random-chase to do this simulation, each time set the same MAX_SIZE and MIN_SIZE in random-chase.cpp
for example, set MAX_SIZE and MIN_SIZE to 1024, so it will simulate for the execution time when the buffer is 1kB
generate the execution file random-chase
make -f assignment.mak random-chase
then the file random-chase will in folder ~/project 
then the instruction should be( cd ~/gem5) 
  ./build/X86/gem5.opt --outdir=../project configs/example/se.py --cpu-type=DerivO3CPU --caches --l1i_size=32kB --l1d_size=64kB --l2cache --l2_size=256kB --l2_rpp="LRURP()" -c ../project/random-chase

then the stats.txt will be in ~/project folder, then you can find some parameters and values about execution time and miss rate in stats.txt

set MAX_SIZE and MIN_SIZE to 1024*4(4kB), do the simulation again

then set MAX_SIZE and MIN_SIZE to 1024*8(8kB), do the simulation again

then set MAX_SIZE and MIN_SIZE to 1024*8(16kB), do the simulation again

...

so you can have the stats.txt with the buffer size you want.
