import xf_build

srcs = [
    "*.c",
]

incs = [
    ".",
]

reqs = [
    "ex_easy_wifi",
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
