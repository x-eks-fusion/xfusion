import xf_build

srcs = [
    "*.c",
    "xf_ble/*.c",
    "xf_ble/porting/*.c",
    "xf_sle/*.c",
    "xf_wifi/*.c",
]

incs = [
    ".",
    "xf_ble",
    "xf_ble/porting",
    "xf_sle",
    "xf_wifi",
]

reqs = [
    "xf_utils",
    "xf_nal",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
