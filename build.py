#!/bin/python3

# Simple build script for this project.
# Builds main.cpp, and then recursively builds
# all cpp files inside of the src/ directory.
#
# Run "python build.py" to build the project,
# and "python build.py commands" to also update
# the compile_commands.json with bear.
# Run "python build.py clean" to remove built
# objects and executables.

import sys
import os


class Configuration():
    def __init__(self):
        """linux build configuration"""
        self.secondary_targets = "deps/glad.c"
        self.source_dir = "src"
        self.object_dir = "obj"
        self.cc = "g++"
        self.target = "gl"
        self.ccflags = "-g -Wall -lglfw3 -lGL -lX11 "\
            "-lpthread -lXrandr -lXi -ldl"
        self.include = "-I include/"
        self.link = ""

        if os.name == "nt":
            self.windows_overrides()

    def windows_overrides(self):
        """windows build configuration"""
        self.target = "gl.exe"
        self.ccflags = "-g -Wall -lglfw3 -lGdi32"
        self.link = "-L\"C:/Program Files (x86)/GLFW/lib\""
        self.include = "-I include/ -I \"C:/Program Files (x86)/GLFW/include\""


conf = Configuration()


def build():
    objects = []  # list of object files that get compiled
    compilations = [f"mkdir {conf.object_dir}"]  # commands to run

    # explicitly compile main.cpp (the only src file in the root dir)
    main = os.path.join(conf.object_dir, "main.o")
    compilations.append(
            f"{conf.cc} -c {conf.ccflags} "
            f"-o {main} "  # target
            f"main.cpp "  # source
            f"{conf.link} {conf.include} "
            )
    objects.append(main)

    # scan through source dir and sub directories, build all object files
    for root, dir, files in os.walk(conf.source_dir):
        for file in files:
            if file[-4:] != ".cpp":
                continue
            fullname = os.path.join(root, file)
            prefix = os.path.basename(file)  # just file
            prefix = os.path.splitext(prefix)[0]  # name without extension

            object = f"{os.path.join(conf.object_dir, prefix)}.o"
            compilations.append(
                    f"{conf.cc} -c {conf.ccflags} "
                    f"-o {object} "  # target
                    f"{fullname} "  # source
                    f"{conf.link} {conf.include} "
                    )
            objects.append(object)

    # convert objects to string
    objects = " ".join(objects)
    # link the final executable
    compilations.append(
            f"{conf.cc} {conf.secondary_targets} -o {conf.target} "
            f"{objects} {conf.include} {conf.link} {conf.ccflags}"
            )

    # compile everything
    for command in compilations:
        print(command)
        os.system(command)


def clean():
    commands = [
            f"rm -rf {conf.object_dir}",
            f"rm {conf.target}"
            ]
    for command in commands:
        print(command)
        os.system(command)


def run():
    commands = [
            f"./{conf.target}",
            ]
    for command in commands:
        print(command)
        os.system(command)


if __name__ == "__main__":
    try:
        build_compile_commands = sys.argv[1]

        if build_compile_commands == "commands":
            print("Building compile_commands.json...")
            os.system('bear --output compile_commands.json'
                      ' -- python build.py')

        elif build_compile_commands == "clean":
            print("Cleaning built object files and executables...")
            clean()

        elif build_compile_commands == "run":
            print("Running executable...")
            run()

        else:
            raise AttributeError(f"unknown argument {build_compile_commands}")
    except IndexError:
        print("Building project...")
        build()
