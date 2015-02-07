# OpenGL_Framework

## First Steps
This repository is versioned using `git`. If you do not know git, you should first consider learning the basics. A good starting point could be, for instance [Pro git](http://git-scm.com/book/en/v1)

**TL;DR:**
* Know the basics of git :)


---

## Installation
**OpenGL_Framework** is build with the CMake build system. CMake works cross platform, meaning it can build the code on many possible systems and the developer can choose how to write and change source code (e.g. your favorite IDE, where to put 3rd party libraries, ...). First of all you have to get CMake for your system [www.cmake.org/](http://www.cmake.org/).

1. **Cloning the repository to your local machine**

    `git clone https://github.com/treevg/OpenGL_Framework.git`
    Or using a [UI client](http://git-scm.com/download/gui/linux)

2. **Create a build directory**

    Create a directory on your system (e.g. `build-vs2012` for Visual Studio 2012 or just simply `build`). In this directory, CMake will create project files, you can open with an IDE (e.g. Visual Studio). The root path of this folder is generally unrelevant, but commonlly, it is created inside the local git repository.**But remember to not push the build folder to the remote repository.**

3. **Run CMake**

    From the build directory, run Cmake: `cmake ../src` or using the [CMake GUI](http://www.cmake.org/runningcmake/)

4. **Open the generated project files**

    CMake will create project files inside your build folder (e.g. for Visual Studio). You can open them and compile them from within your ID.

**TL;DR:**
```
1. git clone https://github.com/treevg/OpenGL_Framework.git
2. mkdir build
3. cd build && cmake ../src
4. make
```

### Generate a documentation
If you would like to have a documentation of all classes and functions of the libraries, you can do this by passing an argument to CMake. Option A is by running `cmake ../src -DBUILD_DOCUMENTATION=true` and option B is to change the value of **BUILD_DOCUMENTATION** under *Ungrouped Entries* using the CMake GUI. Subsequently you can use `make Docs` to generate the documentation. This can then be found under `Documentation/html/` and can be opened using your favourite browser (simply open `index.html`)

**TL;DR:**
```
1. cmake ../src -DBUILD_DOCUMENTATION=true
2. make Docs
3. $BROWSER Documentation/html/index.html
```

---

## Structure

- `resources` Contains all files, beeing no source code (like textures, models)
    - `└` For each file extension, a subfolder exists (e.g. `jpg` for jpeg-image or `obj` for object models)
- `src` Contains all source code files
    - `└ cmake` Contains CMake modules, e.g. to find 3rd party libraries
    - `└ executable` Contains source code of all executables
    - `└ libraries` Contains source code of all libraries
    - `└ shaders` Contains shader source code
---

## OpenGL Debugging Tools

Programming OpenGL can be frustrating and debugging can be a hard task. Indeed there are some tools to explorefor instance function calls, frames, textures, buffers, uniforms, etc. Here are most commonly known:

* [apitrace](https://github.com/apitrace/apitrace)
* [vogl](https://github.com/ValveSoftware/vogl)
* [gDebugger](http://www.gremedy.com/)
* [AMD CodeXL](http://developer.amd.com/tools-and-sdks/opencl-zone/codexl/)
* [Nvidia Nsight](http://www.nvidia.com/object/nsight.html)