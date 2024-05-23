# Readme for x86 Assembly Injection Functions  
Created by Sean P  
  
# Description:  
This project is a compilation of a few useful and portable (Linux or Windows) functions/utilities that can be used by a program to inject x86 Assembly code into its own address space. For more information about specific functions, see the function descriptions in the code comments. NOTE: This code is specifically for use in 32-bit programs, and is not compatible with 64-bit projects.  
  
  
# Object file compilation on Linux:  
To compile object files (.o) of this code for use with other projects, use the Makefile command 'all' (or 'all64' if compiling on a 64-bit machine). NOTE: Compilation on a 64-bit machine requires the the packages gcc-multilib and g++-multilib.  
