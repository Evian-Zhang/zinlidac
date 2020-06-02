cc_library(
    name = "system",
    srcs = [
        "os.cpp",
        "filesystem.cpp",
        "time.cpp",
        "locale.cpp",
        "user.cpp",
        "process.cpp"
    ],
    hdrs = [
        "system.h", 
        "util.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/error:error",
    ],
)