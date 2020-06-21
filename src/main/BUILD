load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = ["//src/lib:CPPLib"],
)

cc_binary(
    name = "main_fib",
    srcs = ["main_fib.cc"],
    deps = ["//src/lib:CPPLib"],
)

cc_binary(
    name = "main_logger",
    srcs = ["main_logger.cc"],
    deps = [
        "//src/lib:CPPLib",
        "@com_github_gflags_gflags//:gflags",
        "@glog",
    ],
)

cc_binary(
    name = "main_flags_absl",
    srcs = ["main_flags_absl.cc"],
    deps = [
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/flags:usage",
        "@glog",
    ],
)

cc_binary(
    name = "main_benchmark",
    srcs = ["main_benchmark.cc"],
    deps = [
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "main_benchmark_sort",
    srcs = ["main_benchmark_sort.cc"],
    deps = [
        "//src/lib/sort:SortLib",
        "@com_google_benchmark//:benchmark",
    ],
)

cc_binary(
    name = "main_benchmark_search",
    srcs = ["main_benchmark_search.cc"],
    deps = [
        "//src/lib/search:SearchLib",
        "@com_google_benchmark//:benchmark",
    ],
)
