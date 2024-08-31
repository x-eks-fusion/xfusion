import xf_build

srcs = [
    "xf_hal/src/*/*.c",
    "*.c"
]

incs = [
    "."
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=["xf_utils"])
