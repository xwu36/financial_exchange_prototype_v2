load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//lib:price4",
        "//src/order:order",
        "//src/util:orders_reader",
        "//src/feed_event:feed_event",
        "//src/matching_engine:matching_engine",
        "//src/publisher:events_publisher",
        "@com_google_absl//absl/status:statusor",
        "@com_github_nlohmann_json//:json",
    ],
    data = ["data/orders.jsonl"]
)
