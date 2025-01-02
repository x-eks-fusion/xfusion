import xf_build

module_enabled = xf_build.get_define("XF_FAL_ENABLE")
if module_enabled == "y":
    srcs = [
        "xf_fal/src/*.c",
        "*.c",
    ]
    incs = [
        "xf_fal/src",
        ".",
    ]
    reqs = [
        "xf_utils",
    ]
    xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
else:
    xf_build.collect(srcs=[], inc_dirs=[], requires=[])
