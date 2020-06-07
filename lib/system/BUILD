cc_library(
    name = "system",
    srcs = [
        "os.cpp",
        "filesystem.cpp",
        "time.cpp",
        "locale.cpp",
        "user.cpp",
        "process.cpp",
        "network.cpp",
        "util.cpp"
    ],
    hdrs = [
        "system.h", 
        "util.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/error:error",
    ],
    linkopts = [
        # see https://stackoverflow.com/a/58846856/10005095
        "-lprocps",
    ],
)
exports_files(["system.h"])