import xf_build

srcs = [
    "*.c",
]

incs = [
    ".",
]

define = xf_build.get_define("XF_OSAL_FREERTOS")
if define == "y":
    srcs.append("xf_osal/port/freeRTOS/*.c")

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=["xf_utils"])
