import xf_build

srcs = [
    "xf_utils/src/*/*.c",
    "*.c"
]

incs = [
    ".",
    "xf_utils/src/"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=["xf_log", "xf_heap"])
