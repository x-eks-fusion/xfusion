import xf_build

srcs = [
    "*.c",
    "xf_sle/*.c",
    "xf_sle/porting/*.c",
    "xf_wifi/*.c",
]

incs = [
    ".",
    "xf_sle",
    "xf_sle/porting",
    "xf_wifi",
]

reqs = [
    "xf_utils",
    "xf_nal",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
