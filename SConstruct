#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "TrackIR-plugin"
projectdir = "example-project"

localEnv = Environment(tools=["default"], PLATFORM="")


customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=["src/", "include/"])
sources = Glob("src/*.cpp")

env.Append(LIBS=["advapi32", "user32", "kernel32"])
env.Append(CXXFLAGS=["/DUNICODE"])


if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

# put compiled binaries in trackir_plugin/bin/
library = env.SharedLibrary(
    "trackir_plugin/bin/{}".format(lib_filename),
    source=sources,
)


# copy .dll files etc. into addons/ folder
copy_dll = env.Install("{}/addons/trackir_plugin/bin/".format(projectdir), library)

# get list of all files (except for bin/ folder) from example-project/addons/trackir_plugin/ 
filenames = []
for file in os.listdir("{}/addons/trackir_plugin".format(projectdir)):
    if file != "bin":
        filenames.append("{}/addons/trackir_plugin/".format(projectdir) + file)

# copy files to trackir_plugin/
copy_plugin = env.Install("trackir_plugin", filenames)

default_args = [library, copy_dll, copy_plugin]
Default(*default_args)
