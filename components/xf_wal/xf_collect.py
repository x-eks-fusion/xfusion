import xf_build

srcs = [
    "*.c",
    "xf_ble/*.c",
    "xf_sle/*.c",
]

incs = [
    ".",
    "xf_ble",
    "xf_sle",
]

reqs = [
    "xf_utils"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
