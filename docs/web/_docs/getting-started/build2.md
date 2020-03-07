---
title: build2
---

**1. build2 Package Dependency for build2 Projects**

Add the following entry to the file `repositories.manifest`.
```
:
role: prerequisite
location: https://github.com/lyrahgames/pxart.git
```
Add the following dependency entry to the file `manifest`.
Optionally, you can specify the version range.
```
depends: pxart
```
Import the library in the according `buildfile` and link it to your target by putting it in the prerequisites.
```
import perfevent_lib = pxart%lib{pxart}
```

---

**2. build2 Package Installation for build2 Projects and Projects without Build System**

Create a build2 configuration for packages if it does not exist already.
Define a valid installation path which can be found by the compiler.
Use specific options, such as to state the compiler with its flags, if necessary.
```
bpkg -d build2-packages cc \
  config.install.root=/usr/local \
  config.install.sudo=sudo
```
Get the latest package release and build it.
```
bpkg build https://github.com/lyrahgames/pxart.git
```
Install the built package.
```
bpkg install pxart
```
For uninstalling, do the following.
```
bpkg uninstall pxart
```
Because the library consists only of header files, the following can be omitted.
But it is recommended to do it otherwise, such that all dependencies are stated explicitly.
In the appropriate `buildfile`, import the library by the following code and put the variable into the prerequisites of your target.
```
import pxart_lib = pxart%lib{pxart}
```
If you are using a `manifest` file, state `pxart` as a requirement.
```
requires: pxart
```
