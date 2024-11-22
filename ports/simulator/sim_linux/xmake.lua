add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

includes(path.join("../../../boards/simulator/sim_linux/.xfusion/xmake.lua"))

target("port_xf")
    set_kind("static")
    set_optimize("none")
    add_deps("xf_build_desc")
    add_includedirs(".")
    add_includedirs("json")
    add_includedirs("tcp", {public = true})
    add_files("*.c")
    add_files("json/*.c")
    add_files("osal/*.c")
    add_ldflags("-lpthread")
    add_files("tcp/*.c")
    add_ldflags("-luv")