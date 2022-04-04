add_rules("mode.debug", "mode.release")

package("sfml")
	add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "sfml"))

    add_configs("graphics",   {description = "Use the graphics module", default = true, type = "boolean"})
    add_configs("window",     {description = "Use the window module", default = true, type = "boolean"})
    add_configs("audio",      {description = "Use the audio module", default = true, type = "boolean"})
    add_configs("network",    {description = "Use the network module", default = true, type = "boolean"})
    add_configs("main",       {description = "Link to the sfml-main library", default = true, type = "boolean"})

    on_load("windows", "linux", "macosx", "mingw", function (package)
        if package:is_plat("windows") then
            package:add("deps", "cmake")
        end

        if not package:config("shared") then
            package:add("defines", "SFML_STATIC")
        end

        local e = ""
        local a = "sfml-"
        if package:is_plat("windows", "mingw") then
            if not package:config("shared") then
                e = "-s"
            end
            if package:debug() then
                e = e .. "-d"
            end
        end
        local main_module = a .. "main"
        if package:debug() then
            main_module = main_module .. "-d"
        end

        if package:config("graphics") then
            if package:is_plat("mingw") then
                package:add("links", a .. "graphics" .. e)
                package:add("links", "freetype")
            end
        end
        if package:config("window") or package:config("graphics") then
            if package:is_plat("mingw") then
                package:add("links", a .. "window" .. e)
            end
            if package:is_plat("windows", "mingw") then
                package:add("syslinks", "opengl32", "gdi32", "user32", "advapi32")
            end
            if package:is_plat("linux") then
                package:add("deps", "libx11", "libxrandr", "freetype", "eudev")
                package:add("deps", "opengl", "glx", {optional = true})
            end
        end
        if package:config("audio") then
            if package:is_plat("mingw") then
                package:add("links", a .. "audio" .. e)
                package:add("links", "openal32", "flac", "vorbisenc", "vorbisfile", "vorbis", "ogg")
            elseif package:is_plat("linux") then
                package:add("deps", "libogg", "libflac", "libvorbis", "openal-soft")
            end
        end
        if package:config("network") then
            if package:is_plat("mingw") then
                package:add("links", a .. "network" .. e)
            end
            if package:is_plat("windows", "mingw") then
                package:add("syslinks", "ws2_32")
            end
        end
        if package:is_plat("mingw") then
            package:add("links", a .. "system" .. e)
            package:add("links", main_module)
        end
        if package:is_plat("windows", "mingw") then
            package:add("syslinks", "winmm")
        end
    end)

    on_install("windows", "linux", function (package)
        local configs = {"-DSFML_BUILD_DOC=OFF", "-DSFML_BUILD_EXAMPLES=OFF"}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        if package:config("shared") then
            table.insert(configs, "-DBUILD_SHARED_LIBS=ON")
        else
            table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
            if package:is_plat("windows") and package:config("vs_runtime"):startswith("MT") then
                table.insert(configs, "-DSFML_USE_STATIC_STD_LIBS=ON")
            end
        end
        table.insert(configs, "-DSFML_BUILD_AUDIO=" .. (package:config("audio") and "ON" or "OFF"))
        table.insert(configs, "-DSFML_BUILD_GRAPHICS=" .. (package:config("graphics") and "ON" or "OFF"))
        table.insert(configs, "-DSFML_BUILD_WINDOW=" .. (package:config("window") and "ON" or "OFF"))
        table.insert(configs, "-DSFML_BUILD_NETWORK=" .. (package:config("network") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)

    on_install("macosx", "mingw", function (package)
        os.cp("lib", package:installdir())
        os.cp("include", package:installdir())
        if package:is_plat("mingw") then
            os.cp("bin/*", package:installdir("lib"), {rootdir = "bin"})
        end
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            void test(int args, char** argv) {
                sf::Clock c;
                c.restart();
            }
        ]]}, {includes = "SFML/System.hpp"}))
    end)
package_end()

add_includedirs(".")
add_requires("sfml", { debug = true })

target("sfml_hello")
	set_languages("c99", "c++17")
    set_kind("binary")
    add_files("sfml_hello/*.cpp")
	add_packages("sfml")
	set_rundir("$(projectdir)/sfml_hello")

target("grid_algos")
	set_languages("c99", "c++17")
    set_kind("binary")
    add_files("grid_algos/*.cpp")
	add_packages("sfml")
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

