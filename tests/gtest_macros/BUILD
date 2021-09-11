load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "expect_pred",
    srcs = ["expect_pred.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "simple_assertions",
    srcs = ["simple_assertions.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "exceptions",
    srcs = ["exceptions.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "floating_points",
    srcs = ["floating_points.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "disabled_tests",
    srcs = ["disabled_tests.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "c_string_assertions",
    srcs = ["c_string_assertions.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "prime_table",
    srcs = [
        "prime_table.cc",
    ],
    hdrs = ["prime_table.h"],
)

cc_test(
    name = "typed_tests",
    srcs = ["typed_tests.cc"],
    deps = [
        ":prime_table",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "private_members",
    srcs = ["private_members.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)


cc_test(
name = "filtered_tests",
srcs = ["filtered_tests.cc"],
deps = [
"@com_google_googletest//:gtest_main",
],
)

cc_test(
name = "typed_tests_findmax",
srcs = ["typed_tests_findmax.cc"],
deps = [
"@com_google_googletest//:gtest_main",
],
)