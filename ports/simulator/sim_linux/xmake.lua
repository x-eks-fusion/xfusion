includes(path.join(os.getenv("XF_TARGET_PATH"),
    ".xfusion/xmake.lua"))

target("port_xf")
    set_kind("static")
    add_deps("xf_build_desc")
    add_includedirs(".")
    add_files("*.c")