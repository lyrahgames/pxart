# Setup with build2
!!!note
    You only have to choose one of the following variants.
    We suggest to take the first alternative.

## Alternative 1: build2 Package Dependency for build2 Projects

Add the following entry to the `repositories.manifest` file of your build2 package.

    :
    role: prerequisite
    location: https://github.com/lyrahgames/pxart.git

Instead of the GitHub repository you can also use the official `pkg.cppget.org` package repositories.
There you could also add a trusted key if needed.

    :
    role: prerequisite
    location: https://pkg.cppget.org/1/alpha

Furthermore, add the following dependency entry to the `manifest` file.
Here, you are allowed to specify the version range.

    depends: pxart ^0.1.0

Now, import the library in the according `buildfile` and link it to your target by putting it in the prerequisites.

    import pxart_lib = pxart%lib{pxart}
    # ...
    exe{your_exe}: {hxx cxx}{**} $pxart_lib

## Alternative 2: build2 Package Installation for build2 Projects and Projects without Build System

Create a build2 configuration for packages if it does not exist already.
Define a valid installation path which can be found by the compiler.
Use specific options, such as to state the compiler with its flags, if necessary.

    bpkg -d build2-packages cc \
      config.install.root=/usr/local \
      config.install.sudo=sudo

Get the latest package release and build it.

    bpkg build https://github.com/lyrahgames/pxart.git

Install the built package.

    bpkg install pxart

For uninstalling, do the following.

    bpkg uninstall pxart

Because the library consists only of header files, the following can be omitted.
But it is recommended to do it otherwise, such that all dependencies are stated explicitly.
In the appropriate `buildfile`, import the library by the following code and put the variable into the prerequisites of your target.

    import pxart_lib = pxart%lib{pxart}

If you are using a `manifest` file, you can state `pxart` as a requirement.
So build2 will try to find the appropriate `pkg-config` file in the standard paths when importing pxart in a buildfile.

    requires: pxart

Alternatively, if your package uses an explicit `depends: pxart` make sure to initialize this dependency as a system dependency when creating a new configuration.

    bdep init -C @build cc config.cxx=g++ "config.cxx.coptions=-O3" -- "?sys:pxart/*"
