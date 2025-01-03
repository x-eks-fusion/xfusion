import xf_build

srcs = [
    "*.c",
]

incs = [
    ".",
]

reqs = [
    "xf_utils",
    "xf_sys",
]

cflags = [
    "-Wno-error",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs, cflags=cflags)
