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
    "xf_sys",
]

apps_enabled = xf_build.get_define("XF_NET_APPS_ENABLE")
if apps_enabled == "y":
    if xf_build.get_define("XF_NET_APPS_IPERF_ENABLE") == "y":
        srcs.append("xf_iperf/*.c")
        incs.append("xf_iperf")
    if xf_build.get_define("XF_NET_APPS_PING_ENABLE") == "y":
        srcs.append("xf_ping/*.c")
        incs.append("xf_ping")
    xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
else:
    xf_build.collect()