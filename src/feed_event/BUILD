
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "feed_event",
    hdrs = ["feed_event.h"],
    srcs = ["feed_event.cc"],
    deps = [
        "//lib:price4",
        "//src/order:order",
        "@com_google_absl//absl/strings",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_test(
    name = "feed_event_test",
    srcs = ["feed_event_test.cc"],
    deps = [
        ":feed_event",
        "@com_google_googletest//:gtest_main",
    ],
)