import xf_build

srcs = [
    "*.c",
]

incs = [
    ".",
]

osal = xf_build.get_define("XF_OSAL_ENABLE")
if osal == "y":
    freertos = xf_build.get_define("XF_OSAL_FREERTOS")
    cmsis = xf_build.get_define("XF_OSAL_CMSIS")
    if freertos == "y":
        srcs.append("xf_osal/port/freeRTOS/*.c")
    elif cmsis == "y":
        srcs.append("xf_osal/port/cmsis-os2/*.c")

    xf_build.collect(srcs=srcs, inc_dirs=incs, requires=["xf_utils"])
else:
    xf_build.collect()
