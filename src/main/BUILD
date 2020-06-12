cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = ["//src/lib:SolutionLib"],
)

cc_binary(
    name = "main_logger",
    srcs = ["main_logger.cc"],
    deps = [
        "//src/lib:SolutionLib",
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
