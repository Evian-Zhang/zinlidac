load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

new_local_repository(
    name = "python27_include",
    build_file_content = """
package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "include",
    srcs = glob(["**/*.h"]),
    includes = ["."]
)
""",
    path = "/usr/include/python3.6",
)

# need pip install numpy, and CHANGE THE USER NAME!
new_local_repository(
    name = "python27_numpy_include",
    build_file_content = """
package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "include",
    srcs = glob(["**/*.h"]),
    includes = ["."]
)
""",
    path = "/home/evian/.local/lib/python3.6/site-packages/numpy/core/include",
)


git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "0f64534c46155ac3760882727bed5be39c39a309",
    remote = "https://github.com/t4n0/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()