# pXart: Packed Extensions for Advanced Random Techniques

Header-only library for fast random number generators

## Building
For building the library use the following commands inside the project's root directory.

    mkdir build
    cd build
    cmake ..
    cmake --build .

Through the standard CMake package export you can externally use the library from the build tree by using the command `find_package(pxart)` and the imported target `pxart::pxart` inside your `CMakeLists.txt` file.

## Installation
If you have already built the library use the following command in the build dirctory.

    sudo cmake --build . --target install

If you have not built the library yet, use the following commands inside the project's root directory.

    mkdir build
    cd build
    cmake ..
    sudo cmake --build . --target install

After the installation the `pXart` can be used by external projects through CMake by using the command `find_package(pxart)`.
In this case the imported target `pxart::pxart` will be generated.

## Uninstallation
If you want to uninstall the library and you have also access to the original build directory then you can use the following commands.

    sudo cmake --build . --target uninstall

## Requirements
