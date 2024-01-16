set_languages("c99", "c++17")
set_project("json_parser")
set_version("0.0.1")
set_xmakever("2.6.9")

add_rules("mode.debug", "mode.release")
add_requires("gtest", {system = true, optional = true})

target("lib")
    set_kind("shared")
    add_files("$(scriptdir)/src/*.cpp")
    add_includedirs("$(scriptdir)/include", {public = true})
    

target("test_main")
    set_kind("binary")
    set_group("tests")
    add_files("test/main.cpp")
    add_deps("lib")
    add_packages("gtest")