# Parallel-implementation-of-Submatrix-Search-Final-project
This project is aimed at developing a simplified recognition algorithm for identifying objects in each given picture with an appropriate matching value. The program takes a set of pictures and a set of objects, and searches for three different objects in each picture. The output includes the list of objects found in each picture.

The program runs on the Linux operating system.

To compile the code write in the terminal: make all

To run the program write in the terminal: make run

To clear all write in the terminal: make clean

Works for both the sequential solution and the parallel solution.

The parallelization uses MPI and OpenMP, with MPI dividing the work on the pictures between two processes
and OpenMP was used to assign each thread to process a subset of pictures and to execute multiple iterations of a loop in parallel.
Each process recived two threads.

Overall with the sequential solution under Linux operating system with 4 cores runs on avg about 32.5 seconds and the parallel solution for about 17.6 seconds. 
