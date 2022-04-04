load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//src:__subpackages__"])

cc_test(
    name = "orders_reader_test",
    srcs = ["orders_reader_test.cc"],
    deps = [
        "//src/order:order",
        ":orders_reader",
        "//lib:price4",
        "@com_google_googletest//:gtest_main",
    ],
    data = ["test_data/orders.jsonl"]
)

cc_library(
    name = "orders_reader",
    hdrs = ["orders_reader.h"],
    srcs = ["orders_reader.cc"],
    deps = [
        "//src/order:order",
        "//src/order:order_parser",
        "@com_github_nlohmann_json//:json",
    ],
)