includes(path.join(os.getenv("XF_TARGET_PATH"),
    ".xfusion/xmake.lua"))

target("port_xf")
    set_kind("static")
    add_deps("xf_build_desc")
    add_includedirs(".")
    add_includedirs("json")
    add_includedirs("tcp", {public = true})
    add_files("*.c")
    add_files("json/*.c")
    add_files("tcp/*.c")
    add_ldflags("-luv")
