import xf_build

srcs = ["xf_lp/src/*.c"]
incs = ["."]
reqs = [
    "xf_utils",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
