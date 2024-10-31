import xf_build

srcs = [
    "xf_log/src/*.c"
]

incs = [
    ".",
]

xf_build.collect(srcs=srcs, inc_dirs=incs)
