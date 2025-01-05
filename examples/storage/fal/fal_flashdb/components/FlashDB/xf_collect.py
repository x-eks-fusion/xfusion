import xf_build

srcs = [
    "FlashDB/src/fdb_file.c",
    "FlashDB/src/fdb_kvdb.c",
    "FlashDB/src/fdb_tsdb.c",
    "samples/*.c",
    "cover/fdb.c",
    "cover/fdb_utils.c",
    "*.c",
]

incs = [
    ".",
    "cover",
]

reqs = [
    "xf_utils", 
    "xf_fal"
]
xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)