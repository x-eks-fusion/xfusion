import xf_build

srcs = [
    "xf_init/src/*.c",
    "xf_init/src/section/*.c",
    "xf_init/src/registry/*.c",
    "*.c",
]

incs = [
    ".",
    "xf_init/src"
]

reqs = [
    "xf_utils",
    "xf_osal",
    "xf_sys",
    "xf_log"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
