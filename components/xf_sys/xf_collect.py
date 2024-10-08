import xf_build

srcs = [
    "xf_sys/src/*.c",
    "*.c",
]

incs = [
    ".",
    "xf_sys/src"
]

reqs = [
    "xf_utils"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
