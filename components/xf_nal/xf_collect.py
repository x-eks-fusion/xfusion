import xf_build

srcs = [
    "*.c",
    "xf_netif/*.c",
]

incs = [
    ".",
    "xf_netif",
]

reqs = [
    "xf_utils"
]

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
