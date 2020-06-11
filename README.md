# C++ Template for Bazel, Google Test, and Google Log

> Can be used in Visual Studio Code

Features:

- [x] Building C++ files using Bazel in Visual Studio Code
- [x] Testing your code using [Google Test](https://github.com/google/googletest)
- [x] Using Google's [glog](https://github.com/google/glog) logger for logging
- [x] Debugging with [CodeLLDB Extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) to provide pretty printing for STL containers such as `std::map` and `std::vector`.

You can use this template for most of your C++ projects without the need for changing the BUILD files.

See [this link](https://docs.bazel.build/versions/master/install.html) to install Bazel.
 
## Installation

```bash
git clone https://github.com/ourarash/cpp-template.git
```

### Run main:

You can run this using `bazel`:

```bash
bazel run src/main:main
```

### Google's glog demo:

You can run this using `bazel`:

```bash
bazel run src/main:main_logger
```

### Run Tests:

You can run unit tests using [`bazel`](installing-bazel):

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

<table><tr><td>

<a href="https://www.youtube.com/watch?v=-TUogVOs1Qg/">
<img alt="Debugging C++ in Visual Studio Code using gcc/gdb and Bazel" src="https://raw.githubusercontent.com/ourarash/cpp-template/master/bazel_yt.png" width="400">
</a>
</td></tr></table>

# More on using Google Test with Bazel in Visual Studio Code:

Here is a video that explains more about how to use Google Test with Bazel in Visual Studio Code:

<table><tr><td>

<a href="https://www.youtube.com/watch?v=0wMNtl2xDT0/">
<img border="5" alt="Debugging C++ in Visual Studio Code using gcc/gdb and Bazel" src="https://raw.githubusercontent.com/ourarash/cpp-template/master/VSCDebug_yt.png" width="400">
</a>
</td></tr></table>

# More info on GLOG
GLOG is the C++ implementation of the Google logging module. 
You can see complete usage instructions [here](https://github.com/google/glog/blob/master/doc/glog.html)

A sample usage is included in this repo [here](src/main/main_logger.cc):

```cpp
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;

  std::vector<int> x = {1, 2, 3, 4};
  std::map<int, int> y = {{1, 2}, {2, 3}};

  LOG(INFO) << "ABC, it's easy as "
            << "{" << x << "}";
  LOG(INFO) << "ABC, it's easy as " << y;

  LOG(INFO) << "This is an info  message";
  LOG(WARNING) << "This is a warning message";
  LOG(INFO) << "Hello, world again!";
  LOG(ERROR) << "This is an error message";
  LOG(FATAL) << "This is a fatal message";
  CHECK(5 == 4) << "Check failed!";

  return 0;
}
```

# More Info On Debugging in VCS:

Check this [page](https://code.visualstudio.com/docs/cpp/cpp-debug).

### Credit

The initial version of this repo was inspired by [this post](https://www.ratanparai.com/c++/writing-unit-tests-with-bazel/).
