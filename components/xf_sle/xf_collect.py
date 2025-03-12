import xf_build

srcs = [
    "./xf_sle/src/utils/*.c",
]

incs = [
    ".",
    "./xf_sle/src",
    "./xf_sle/src/utils",
]

reqs = [
    "xf_utils",
    "xf_log"
]

module_enabled = xf_build.get_define("XF_SLE_ENABLE")
if module_enabled == "n":
    srcs.clear()
    incs.clear()
    reqs.clear()

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)