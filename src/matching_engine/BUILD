
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "matching_engine",
    hdrs = ["matching_engine.h"],
    srcs = ["matching_engine.cc"],
    deps = [
        ":tick_size_rule",
        "//lib:timestamp",
        "//src/stock:symbol",
        "//src/feed_event:feed_event",
        "//src/order:order",
        "//src/order:order_book",
        "//src/order:order_parser",
        "//src/util:orders_reader",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/status",
    ],
    data = ["data/tick_size_rule.json"]
)

cc_test(
    name = "matching_engine_test",
    srcs = ["matching_engine_test.cc"],
    deps = [
        ":matching_engine",
        "//src/order:order",
        "//src/order:order_parser",
        "@com_google_googletest//:gtest_main",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_library(
    name = "tick_size_rule",
    hdrs = ["tick_size_rule.h"],
    srcs = ["tick_size_rule.cc"],
    deps = [
        "//lib:price4",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_test(
    name = "tick_size_rule_test",
    srcs = ["tick_size_rule_test.cc"],
    deps = [
        ":tick_size_rule",
        "@com_google_googletest//:gtest_main",
    ],
)