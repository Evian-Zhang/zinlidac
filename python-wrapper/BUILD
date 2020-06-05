cc_binary(
    name = "libzinlidacpy.so",
    srcs = ["python-interface.cpp"],
    deps = [
        "//lib/error:error",
        "//lib/hardware:hardware",
        "//lib/system:system",
        "@boost//:python"
    ],
    linkshared = True,
)