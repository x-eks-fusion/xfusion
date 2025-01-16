import xf_build

module_enabled = xf_build.get_define("PLATFORM_SIM_LINUX")
if module_enabled == "y":
    srcs = [
        "*.c",
    ]
    incs = [
        ".",
    ]
    reqs = [
        "xf_utils",
    ]
    xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
