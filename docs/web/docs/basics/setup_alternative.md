# Alternative Setup by Inlining

pXart explicitly supports build2 and CMake.
If you would like to use another build system or no build system at all, you will need to directly set a standard include path for the pXart library by using the specific compiler flags.

**Project Inlining for build2/CMake Projects and Projects without Build Systems**

Go into your project folder.
Optionally, go into the specific folder of your project for external source code.
Copy the folder `pxart/pxart` of the git repository into the current directory.
If you are using no build system, make sure to add the current directory to the standard include paths of your compiler by using a flag.
In CMake, you can do this by using `include_directories` or `target_include_directories`.
In build2, use the configuration variable `cxx.poptions` in your `buildfile` and add the specific include flag.