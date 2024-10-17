import xf_build

srcs = [
    "*.c",
]

incs = [
    ".",
]

reqs = [
    "xf_utils",
    "xf_osal",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
