# Readme for x86 Assembly Injection Example/Demo Program  
Created by Sean P  
  
# Description:  
This is an example program to demonstrate the use of my (32-bit) x86 Assembly injection functions. The program iterates through a loop 10 times, printing a number each time. In the loop, there are statements that add to and subtract from the number, but the net sum of these changes is zero, so the number remains unchanged at the end of each iteration.  
  
Once the loop has executed 4 times, the program injects an empty assembly function into itself, overwriting one of the arithmetic statements that changes the number. With this statement removed, the sum of the arithmatic changes to the number is no longer zero (it becomes -1), and the number changes during each iteration for the remainder of execution.  
  
If the program runs correctly, the output should be as follows (though the address values will be different):  
  
  * Press enter to begin.  
  * Value = 10  
  * Value = 10  
  * Value = 10  
  * Value = 10  
  * Value = 10  
  * Injecting assembly at 0x8048be1 with return address 0x8048be6...  
  * Value = 10  
  * Value = 11  
  * Value = 12  
  * Value = 13  
  * Value = 14  
  

This program was tested on gcc version 5.4.0 (Ubuntu 16.04) and Visual Studio 2015 (Windows 10). If your output is different, you may be using a different compiler (or settings) from the ones this program was tested with.  
  


# Compiling on Linux:  
To compile this program on Linux, navigate to this directory and use the Makefile commands. On 32-bit systems, use the command 'all' (or 'run' to automatically launch the program after compilation). On 64-bit systems, use the commands 'all64' or 'run64' (NOTE: compilation from a 64-bit system will require the packages gcc-multilib and g++-multilib). The compiled executable will be in the /test/bin/ folder.  
  

# Compiling on Windows:  
To compile this program on Windows:  
  * Open Visual Studio and choose File->New->Project from existing code...  
  * Use the /test/ directory as your project path.  
  * Make sure "Add files to the project from these folders" is checked.  
  * Add the /test/src/ and /test/include/ folders to the project, as well as the /src/ and /include/ folders from the parent directory.  
  * On the "Specify Project Settings" screen, choose "Use Visual Studio" and "Console application project" (do not check any of the boxes).  
  * On the "Specify Debug Configuration Settings" screen, add the previously-mentioned directories under "Include search paths".  
  * On the "Specify Release Configuration Settings" screen, check off "Same as Debug Configuration" and then "Finish".  
  * At the top of the screen, make sure the build configuration is set to "Release" (not "Debug") and "x86".  
  * Double-check that the necessary folders (same as before) are included under Project->Project Properties->C/C++->General->Additional Include Directories  
  * Compile the program by going to Build->Build Solution  
  * The compiled program can be found in the /Release/ folder in the project directory.  

