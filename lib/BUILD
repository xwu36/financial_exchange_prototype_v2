load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "price4",
    srcs = ["price4.cc"],
    hdrs = ["price4.h"],
)

cc_library(
    name = "timestamp",
    hdrs = ["timestamp.h"],
)

cc_test(
    name = "price4_test",
    srcs = ["price4_test.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "price4",
    ],
)

cc_test(
    name = "timestamp_test",
    srcs = ["timestamp_test.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "timestamp",
    ],
)
