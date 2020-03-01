# pXart: Packed Extensions for Advanced Random Techniques

Header-only library for fast random number generators

![](https://img.shields.io/github/languages/top/lyrahgames/pxart.svg?style=for-the-badge)
![](https://img.shields.io/github/languages/code-size/lyrahgames/pxart.svg?style=for-the-badge)
![](https://img.shields.io/github/repo-size/lyrahgames/pxart.svg?style=for-the-badge)
![](https://img.shields.io/github/license/lyrahgames/pxart.svg?style=for-the-badge&color=blue)
[![Website lyrahgames.github.io/pxart](https://img.shields.io/website/https/lyrahgames.github.io/pxart.svg?down_message=offline&label=Documentation&style=for-the-badge&up_color=blue&up_message=online)](https://lyrahgames.github.io/pxart)

<b>
<table>
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/lyrahgames/pxart">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/pxart/master.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/pxart/tree/master"><img src="https://circleci.com/gh/lyrahgames/pxart/tree/master.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/pxart">
              <img src="https://codecov.io/gh/lyrahgames/pxart/branch/master/graph/badge.svg" />
            </a>
        </td>
    </tr>
    <!-- <tr>
        <td>
            develop
        </td>
        <td>
            <a href="https://github.com/lyrahgames/pxart/tree/develop">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/pxart/develop.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/pxart/tree/develop"><img src="https://circleci.com/gh/lyrahgames/pxart/tree/develop.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/pxart">
              <img src="https://codecov.io/gh/lyrahgames/pxart/branch/develop/graph/badge.svg" />
            </a>
        </td>
    </tr> -->
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/lyrahgames/pxart">
                <img src="https://img.shields.io/github/commit-activity/y/lyrahgames/pxart.svg?logo=github&logoColor=white">
            </a>
        </td>
        <!-- <td>
            <img src="https://img.shields.io/github/release/lyrahgames/pxart.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/lyrahgames/pxart.svg?label=pre-release&logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag/lyrahgames/pxart.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/lyrahgames/pxart.svg?label=latest%20tag&logo=github&logoColor=white">
        </td> -->
    </tr>
</table>
</b>

## Building
For building the library use the following commands inside the project's root directory.

    mkdir build
    cd build
    cmake ..
    cmake --build .

Through the standard CMake package export you can externally use the library from the build tree by using the command `find_package(pxart)` and the imported target `pxart::pxart` inside your `CMakeLists.txt` file.

## Installing
If you have already built the library use the following command in the build dirctory.

    sudo cmake --build . --target install

If you have not built the library yet, use the following commands inside the project's root directory.

    mkdir build
    cd build
    cmake ..
    sudo cmake --build . --target install

After the installation the `pXart` can be used by external projects through CMake by using the command `find_package(pxart)`.
In this case the imported target `pxart::pxart` will be generated.

## Uninstalling
If you want to uninstall the library and you have also access to the original build directory then you can use the following commands.

    sudo cmake --build . --target uninstall

## Requirements
