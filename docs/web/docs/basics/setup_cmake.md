# Setup with CMake
!!!note
    You only have to choose one of the following variants.
    We suggest to take the first alternative.
    
## Alternative 1: CMake Package Installation for CMake Projects and Projects without Build System

Download the repository and create a configuration.
```
git clone https://github.com/lyrahgames/pxart.git
mkdir pxart-cmake-build
cd pxart-cmake-build
cmake ../pxart
```
Optionally, build and run the tests.
```
cmake --build .
ctest --verbose
```
Install the library and the CMake package.
```
sudo cmake --build . --target install
```
To uninstall the library do the following.
```
sudo cmake --build . --target uninstall
```
Because the library consists only of header files, the following can be omitted.
But it is recommended to do it otherwise, such that all dependencies are stated explicitly.
In the appropriate `CMakeLists.txt` file, use `find_package(pxart)` to find the library and link with the imported target `pxart::pxart` by using `target_link_libraries`.
The following code shows an example.
```cmake
find_package(pxart REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE pxart::pxart)
```

## Alternative 2: CMake Package Export from Build Configuration for CMake Projects

Download the repository and create a configuration.
```
git clone https://github.com/lyrahgames/pxart.git
mkdir pxart-cmake-build
cd pxart-cmake-build
cmake ../pxart
```
Through the standard CMake package export you can externally use the library from the build tree.
In the appropriate `CMakeLists.txt` file, use `find_package(pxart)` to find the library and link with the imported target `pxart::pxart` by using `target_link_libraries`.
The following code shows an example.
```cmake
find_package(pxart REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE pxart::pxart)
```
