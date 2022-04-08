add_rules("mode.debug", "mode.release")

set_languages("c99", "c++17")

-- package("raylib")
--     add_deps("cmake")
--     set_sourcedir(path.join(os.scriptdir(), "raylib"))
-- 	add_syslinks("gdi32", "user32", "winmm", "shell32")

--     on_install(function (package)
--         local configs = {}
--         table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
--         table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
--         import("package.tools.cmake").install(package, configs)
--     end)
--     on_test(function (package)
--         -- assert(package:has_cfuncs("add", {includes = "foo.h"}))
--     end)
-- package_end()

package("raylib")
	add_deps("cmake")
	set_sourcedir(path.join(os.scriptdir(), "raylib"))

    if is_plat("macosx") then
        add_frameworks("CoreVideo", "CoreGraphics", "AppKit", "IOKit", "CoreFoundation", "Foundation")
    elseif is_plat("windows") then
        add_syslinks("gdi32", "user32", "winmm", "shell32")
    elseif is_plat("linux") then
        add_syslinks("pthread", "dl", "m")
        add_deps("libx11", "libxrandr", "libxrender", "libxinerama", "libxcursor", "libxi", "libxfixes", "libxext")
    end

    add_deps("opengl", {optional = true})

    on_install("windows", "linux", "macosx|arm64", function (package)
        local configs = {"-DBUILD_EXAMPLES=OFF"}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs, {packagedeps = {"libx11", "libxrender", "libxrandr", "libxinerama", "libxcursor", "libxi", "libxfixes", "libxext"}})
    end)
package_end()

add_includedirs(".")
add_requires("raylib", { debug = true })

target("raylib_hello")
	set_kind("binary")
	add_files("raylib_hello/*.cpp")
	add_packages("raylib")
	set_rundir("$(projectdir)/raylib_hello")

target("grid_algos")
	set_kind("binary")
	add_files("grid_algos/*.cpp")
	add_packages("raylib")
	set_rundir("$(projectdir)/grid_algos")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

