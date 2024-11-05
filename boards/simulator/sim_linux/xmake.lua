includes(path.join(os.getenv("XF_ROOT"),
    "/ports/simulator/sim_linux/xmake.lua"))

target(xf_project_name)
    set_kind("binary")
    add_includedirs(".")
    add_files("./main/*.c")
    add_deps("port_xf")

    -- 对应生成 -Wl,--whole-archive -la -lb -Wl,--no-whole-archive 链接选项
    -- 对于不支持的平台和编译，会退化成 -la -lb
    add_linkgroups("port_xf", "xf_build_desc", {whole = true})
    

