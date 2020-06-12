# C++ Template for Bazel, Google Test, Google Log, and Abseil (ABSL)

> Can be used in Visual Studio Code

Features:

- [x] Building C++ files using Bazel in Visual Studio Code
- [x] [Google Test](https://github.com/google/googletest) for unit tests
- [x] Google's [glog](https://github.com/google/glog) logger for logging
- [x] Google's [Abseil library](https://github.com/abseil/abseil-cpp)
- [x] Debugging with [CodeLLDB Extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) to provide pretty printing for STL containers such as `std::map` and `std::vector`.

You can use this template for most of your C++ projects without the need for changing the BUILD files.

## Prerequisite: Installing Bazel

This repo uses `Bazel` for building C++ files.
You can install Bazel using this [link](https://docs.bazel.build/versions/master/install.html).

## Installation

```bash
git clone https://github.com/ourarash/cpp-template.git
```
## Examples:

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

### Google's Abseil's flags demo:

You can run this using `bazel`:

```bash
bazel run src/main:main_flags_absl
```

# Using Google Test with Bazel in Visual Studio Code:

Here is a video that explains more about how to use Google Test with Bazel in Visual Studio Code:

<table><tr><td>

<a href="https://www.youtube.com/watch?v=0wMNtl2xDT0/">
<img border="5" alt="Debugging C++ in Visual Studio Code using gcc/gdb and Bazel" src="https://raw.githubusercontent.com/ourarash/cpp-template/master/VSCDebug_yt.png" width="400">
</a>
</td></tr></table>

## Example of running a test:

You can run unit tests using [`bazel`](installing-bazel):

```bash
bazel test tests:tests
```

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

# More Info On Abseil Library:

[Abseil library](https://github.com/abseil/abseil-cpp) is an open-source collection of C++ code (compliant to C++11) designed to augment the C++ standard library.

A sample usage is included in this repo [here](src/main/main_flags_absl.cc):

Abseil contains the following C++ library components:

- [`base`](https://github.com/abseil/abseil-cpp/tree/master/absl/base/) Abseil Fundamentals
  <br /> The `base` library contains initialization code and other code which
  all other Abseil code depends on. Code within `base` may not depend on any
  other code (other than the C++ standard library).
- [`algorithm`](https://github.com/abseil/abseil-cpp/tree/master/absl/algorithm/)
  <br /> The `algorithm` library contains additions to the C++ `<algorithm>`
  library and container-based versions of such algorithms.
- [`container`](https://github.com/abseil/abseil-cpp/tree/master/absl/container/)
  <br /> The `container` library contains additional STL-style containers,
  including Abseil's unordered "Swiss table" containers.
- [`debugging`](https://github.com/abseil/abseil-cpp/tree/master/absl/debugging/)
  <br /> The `debugging` library contains code useful for enabling leak
  checks, and stacktrace and symbolization utilities.
- [`hash`](https://github.com/abseil/abseil-cpp/tree/master/absl/hash/)
  <br /> The `hash` library contains the hashing framework and default hash
  functor implementations for hashable types in Abseil.
- [`memory`](https://github.com/abseil/abseil-cpp/tree/master/absl/memory/)
  <br /> The `memory` library contains C++11-compatible versions of
  `std::make_unique()` and related memory management facilities.
- [`meta`](https://github.com/abseil/abseil-cpp/tree/master/absl/meta/)
  <br /> The `meta` library contains C++11-compatible versions of type checks
  available within C++14 and C++17 versions of the C++ `<type_traits>` library.
- [`numeric`](https://github.com/abseil/abseil-cpp/tree/master/absl/numeric/)
  <br /> The `numeric` library contains C++11-compatible 128-bit integers.
- [`strings`](https://github.com/abseil/abseil-cpp/tree/master/absl/strings/)
  <br /> The `strings` library contains a variety of strings routines and
  utilities, including a C++11-compatible version of the C++17
  `std::string_view` type.
- [`synchronization`](https://github.com/abseil/abseil-cpp/tree/master/absl/synchronization/)
  <br /> The `synchronization` library contains concurrency primitives (Abseil's
  `absl::Mutex` class, an alternative to `std::mutex`) and a variety of
  synchronization abstractions.
- [`time`](https://github.com/abseil/abseil-cpp/tree/master/absl/time/)
  <br /> The `time` library contains abstractions for computing with absolute
  points in time, durations of time, and formatting and parsing time within
  time zones.
- [`types`](https://github.com/abseil/abseil-cpp/tree/master/absl/types/)
  <br /> The `types` library contains non-container utility types, like a
  C++11-compatible version of the C++17 `std::optional` type.
- [`utility`](https://github.com/abseil/abseil-cpp/tree/master/absl/utility/)
  <br /> The `utility` library contains utility and helper code.

# Debugging with Bazel

There are two configurations available: `(lldb) launch` and `CodeLLDB`. You can use `(lldb) launch` without any modifications, but Currently only `CodeLLDB` provides correct pretty printing for STL containers such as map and vector.

## Using CodeLLDB

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

More Info On Debugging in VSC is [here](https://code.visualstudio.com/docs/cpp/cpp-debug).

### Credit

The initial version of this repo was inspired by [this post](https://www.ratanparai.com/c++/writing-unit-tests-with-bazel/).
