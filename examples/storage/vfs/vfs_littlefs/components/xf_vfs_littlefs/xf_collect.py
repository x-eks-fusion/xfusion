import xf_build

srcs = [
    "littlefs/*.c",
    "*.c",
]
incs = [
    ".",
]
reqs = [
    "xf_utils", 
    "xf_sys", # 获取时间戳
    "xf_vfs",
    "xf_fal",
]
xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)