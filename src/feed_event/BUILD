
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "feed_event",
    hdrs = ["feed_event.h"],
    deps = [
        "//lib:price4",
        "@com_github_nlohmann_json//:json",
    ],
)