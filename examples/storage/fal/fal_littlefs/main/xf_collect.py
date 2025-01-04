import xf_build

srcs = [
    "*.c",
]
incs = [
    ".",
]

xf_build.collect(srcs=srcs, inc_dirs=incs)