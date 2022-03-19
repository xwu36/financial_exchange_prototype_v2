load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "order",
    hdrs = ["order.h"],
    deps = [
        "//lib:price4",
        "//src/stock:symbol",
        "@com_google_absl//absl/strings",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_test(
    name = "order_test",
    srcs = ["order_test.cc"],
    deps = [
        ":order",
        ":order_parser",
        "@com_github_nlohmann_json//:json",
        "@com_google_googletest//:gtest_main",
    ],
    data = ["test_data/orders.jsonl"]

)

cc_library(
    name = "order_parser",
    hdrs = ["order_parser.h"],
    deps = [
        "//lib:price4",
        "//src/stock:symbol",
        "//src/order:order",
        "@com_github_nlohmann_json//:json",
    ],
)