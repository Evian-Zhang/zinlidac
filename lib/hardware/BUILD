cc_library(
    name = "hardware",
    srcs = [
        "cpu.cpp",
        "cache.cpp",
        "memory.cpp",
        "util.cpp"
    ],
    hdrs = [
        "hardware.h", 
        "util.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/error:error",
    ],
)
exports_files(["hardware.h"])