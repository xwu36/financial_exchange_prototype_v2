# C++ Template for Bazel and Google Test

> Can be used in Visual Studio Code

You can use this template for most of your C++ projects without the need for changing the BUILD files.

## Installation

You can copy this using this command:

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
There are two configurations available: `(lldb) launch` or `CodeLLDB`. You can use `(lldb) launch` without any modifications, but Currently only `CodeLLDB` provides correct pretty printing for STL containers such as map and vector.

In order for CodeLLDB to work with Bazel on Visual studio code and provide pretty printing, you need the following:

- Install [CodeLLDB Extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb)
- Run one of these commands depending on your system (copied from [launch.json](launch.json))

```
"Linux": "bazel  build --cxxopt='-std=c++11' src/main:main -c dbg",
"windows": "bazel build --cxxopt='-std=c++11' src/main:main --experimental_enable_runfiles -c dbg"
"mac":"command": "bazel build --cxxopt='-std=c++11' src/main:main -c dbg --spawn_strategy=standalone"
```

- Run this in the root of your workspace:
```
readlink -n bazel-cpp-template
```
- Put the output of that command in [launch.json](launch.json)'s sourcemap section:
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


### Credit

This repo was inspired by [this post](https://www.ratanparai.com/c++/writing-unit-tests-with-bazel/).
