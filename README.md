# C++ Template for Bazel and Google Test

> Can be used in Visual Studio Code

Features:

- [x] Building C++ files using Bazel in Visual Studio Code
- [x] Testing your code using [Google Test](https://github.com/google/googletest)
- [x] Debugging with [CodeLLDB Extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) to provide pretty printing for STL containers such as `std::map` and `std::vector`.

You can use this template for most of your C++ projects without the need for changing the BUILD files.

## Installation

```bash
git clone https://github.com/ourarash/cpp-template.git
```

### Run main:

You can run this using `blaze`:

```bash
bazel run src/main:main
```

### Run Tests:

You can run unit tests using [`blaze`](installing-bazel):

```bash
bazel test tests:tests
```

## Directory Structure

<img alt="Directory Structure" src="https://github.com/ourarash/cpp-template/blob/master/tree.png?raw=true" width="400">

## Installing Bazel

This repo uses `Bazel` for building C++ files.
You can install Bazel using this [link](https://docs.bazel.build/versions/master/install.html).

## Debugging with Bazel

There are two configurations available: `(lldb) launch` and `CodeLLDB`. You can use `(lldb) launch` without any modifications, but Currently only `CodeLLDB` provides correct pretty printing for STL containers such as map and vector.

### Using CodeLLDB

<img alt="Directory Structure" src="https://github.com/ourarash/cpp-template/blob/master/codelldb1.png?raw=true" width="400">

In order for CodeLLDB to work with Bazel on Visual studio code and provide pretty printing, you need the following:

- Install [CodeLLDB Extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb)

<img alt="Directory Structure" src="https://github.com/ourarash/cpp-template/blob/master/codelldb2.png?raw=true" width="400">

- Run this command to create Bazel symlinks:
```bash
bazel build src/main:main
```

- Run one of the following commands depending on your system (copied from [launch.json](.vscode/launch.json)) to build with bazel for debug.

```bash
"Linux": "bazel  build --cxxopt='-std=c++11' src/main:main -c dbg",
"windows": "bazel build --cxxopt='-std=c++11' src/main:main --experimental_enable_runfiles -c dbg"
"mac":"command": "bazel build --cxxopt='-std=c++11' src/main:main -c dbg --spawn_strategy=standalone"
```

- Run this in the root of your workspace to find the target of `bazel-cpp-template` symlink that Bazel creates based. These symlinks are documented [here](https://docs.bazel.build/versions/master/output_directories.html):

```bash
readlink -n bazel-cpp-template
```

- Put the output of that command in [launch.json](.vscode/launch.json)'s sourcemap section:

```json
"sourceMap": {
        "[output of readlink -n bazel-cpp-template]": "${workspaceFolder}/"
 }
```

Example:

```json
"sourceMap": {
        "/private/var/tmp/_bazel_ari/asdfasdfasdfasdfasdfgadfgasdg/execroot/__main__": "${workspaceFolder}/"
 }
```

- Start debugging!

Here is a video that explains more about how to use Visual Studio Code for debugging C++ programs:

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/https://-TUogVOs1Qg/0.jpg)](https://www.youtube.com/watch?v=-TUogVOs1Qg)

<iframe width="560" height="315"
src="https://www.youtube.com/embed/MUQfKFzIOeU" 
frameborder="0" 
allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" 
allowfullscreen></iframe>



# More Info On Debugging in VCS:

Check this [page](https://code.visualstudio.com/docs/cpp/cpp-debug).

### Credit

The initial version of this repo was inspired by [this post](https://www.ratanparai.com/c++/writing-unit-tests-with-bazel/).
