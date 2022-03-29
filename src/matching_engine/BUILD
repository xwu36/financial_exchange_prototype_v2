
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "matching_engine",
    hdrs = ["matching_engine.h"],
    srcs = ["matching_engine.cc"],
    deps = [
        "//src/stock:symbol",
        "//src/feed_event:feed_event",
        "//src/order:order",
        "//src/order:order_book",
    ],
)