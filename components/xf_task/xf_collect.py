import xf_build

srcs = [
    "xf_task/src/*/*.c",
    "*.c"
]

incs = [
    ".",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=["xf_utils"])
