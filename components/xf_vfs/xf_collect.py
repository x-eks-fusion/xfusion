import xf_build

module_enabled = xf_build.get_define("XF_VFS_ENABLE")
if module_enabled == "y":
    srcs = [
        "xf_vfs/src/*.c",
        "*.c",
    ]
    incs = [
        "xf_vfs/src",
        ".",
    ]
    reqs = [
        "xf_utils",
    ]
    select_enabled = xf_build.get_define("XF_VFS_SUPPORT_SELECT_ENABLE")
    if select_enabled == "y":
        reqs.append("xf_osal")
    xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
