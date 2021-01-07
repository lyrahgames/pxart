# Setup
## Clone, Initialize, and Build
By using build2, pxart can be easily tested with multiple configurations.
For this, create a new folder `pxart` in your favorite project folder, like `~/projects`.

    mkdir pxart

We call this one the developer folder, because it will contain the actual Git repository of the code as well as all the configurations the tests will be built for.
Go into this folder and clone this repository.

    cd pxart
    git clone https://github.com/lyrahgames/pxart

We call this one the repository folder, the source folder, or the project folder.
Change the directory into the source folder and create all the configurations that you would like to test for.
In this example, we will create two configurations for the default GCC and Clang compiler with maximal optimization enabled.

    cd pxart
    bdep init -C @gcc cc config.cxx=g++ "config.cxx.coptions=-03 -march=native" config.install.root=../install
    bdep init -C @clang cc config.cxx=clang++ "config.cxx.coptions=-03 -march=native"

These commands have create two configurations, namely `@gcc` and `@clang`, with their respective folders `pxart-gcc` and `pxart-clang` in the developer folder.
The configuration `@gcc` is the default configuration and we have enabled local installation support for the folder named `install` inside the developer folder to be able to test the installation process.
The folder structure should now look like the following.

    pxart/                        # Developer Folder
    │
    ├── pxart/...                 # Project Folder
    │
    ├── pxart-gcc/...             # Configuration Folder 'gcc'
    │
    └── pxart-clang/...           # Configuration Folder 'clang'

To build all the packages of the project for all initialized configurations, run the following command.

    bdep update -a

Furthermore, we would like to create symbolic links to the executable for the default configuration.
For this, we have to run the build system of build2 directly.

    b

To test the installation run the build system with the appropriate target.

    b install

Your developer folder should now contain a folder, called `install`, with all installed packages.
To only have a look at the library installation itself, uninstall all files and go into the library package inside the project folder and run the installation command again.

    b uninstall
    cd pxart
    b install

The folder structure of `install` should look similar to the following.

```
install/
├── include
│   └── pxart/...              # pXart Header Files
├── lib
│   └── pkgconfig
│       ├── libpxart.pc
│       ├── libpxart.shared.pc
│       └── libpxart.static.pc
└── share
    └── doc
        └── pxart
            ├── LICENSE
            ├── manifest
            └── README.md

```

## Tests
To run all the unit tests for all configurations, go into the `tests` package of the project folder and run the tests.

    cd tests
    bdep test -a

## Benchmarks
Running the benchmarks for all configurations at the same time seems to be inconvenient.
Therefore use the name to specify the configuration.
To run the benchmarks, we use `perfevent` which calls `perf` in C++.
We first have to allow the paranoid mode.

    sudo sysctl -w kernel.perf_event_paranoid=-1

Then go into the `benchmarks` package of the project folder and execute the benchmarks by using the build2.

    bdep test @gcc

## Examples
### Basics
There is more than one example package.
Typically, the executables of the examples have to be called manually.
For the `basics` example, the `test` command can still be used.

    cd examples/basics
    bdep test @gcc

### Photon Propagation
To run the photon propagation example in default configuration, do the following.

    cd examples/photon_propagation
    photons/photons
