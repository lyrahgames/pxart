---
title: Requirements and Support
---

<!-- <p class="justify"> -->
pXart requires no external dependencies but uses some features of the C++17 standard.
It is possible to use pXart without any support for the Intel SSE or AVX vector intrinsics but we recommend to at least use a processor featuring the SSE4.1 instruction set.
For the best results, the AVX2 instruction set should be enabled.
The following table shows supported and tested configurations.
<!-- </p> -->

<table class="table table-striped table-hover ">
    <thead>
        <tr>
            <th>Operating System</th>
            <th>Compilers</th>
            <th>Instruction Set Features</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Linux</td>
            <td>
                <span class="label label-primary">GCC 9</span>
                <span class="label label-primary">Clang 9</span>
                <span class="label label-primary">ICC 19</span>
            </td>
            <td>
                <span class="label label-default">SSE4.1</span>
                <span class="label label-default">AVX2</span>
            </td>
        </tr>
        <tr>
            <td>Windows</td>
            <td>
                <span class="label label-primary">MSVC 17</span>
                <span class="label label-primary">MSVC 19</span>
                <span class="label label-primary">MinGW GCC</span>
                <span class="label label-primary">Clang</span>
                <span class="label label-primary">ICC 19</span>
            </td>
            <td>
                <span class="label label-default">AVX2</span>
            </td>
        </tr>
        <tr>
            <td>MacOS</td>
            <td>
                <span class="label label-primary">Clang</span>
            </td>
            <td></td>
        </tr>
    </tbody>
</table>