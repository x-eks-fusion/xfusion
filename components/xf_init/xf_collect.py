import xf_build

srcs = [
    "xf_init/src/*.c",
    "*.c",
]

incs = [
    ".",
    "xf_init/src"
]

reqs = [
    "xf_utils"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
