#!/bin/python3
# Simple build script for this project.
# Run "python build.py" to build the project,
# and "python build.py commands" to also update
# the compile_commands.json with bear.
import sys
import os


class Configuration():
    def __init__(self):
        """linux build configuration"""
        self.secondary_targets = [
                "deps/glad.c"
            ]
        self.source_dir = "src"
        self.object_dir = "obj"
        self.cc = "g++"
        self.target = "triangle"
        self.ccflags = "-g -Wall -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl"
        self.include = "-I include/"
        self.link = ""

        if os.name == "nt":
            self.windows_overrides()

    def windows_overrides(self):
        """windows build configuration"""
        self.target = "triangle.exe"
        self.ccflags = "-g -Wall -lglfw3 -lGdi32"
        self.link = "-L\"C:/Program Files (x86)/GLFW/lib\""
        self.include = "-I include/ -I \"C:/Program Files (x86)/GLFW/include\""


conf = Configuration()


def build(build_compile_commands=None):
    if build_compile_commands:
        # have bear track the building process
        os.system('bear --output compile_commands.json -- "python build.py"')
        return

    # scan through source dir and sub directories, build all object files
    for root, dir, files in os.walk(conf.source_dir):
        for file in files:
            fullname = os.path.join(root, file)
            prefix = os.path.basename(file) # just file
            prefix = os.path.splitext(prefix)[0]# name without extension

            os.system(
                    f"{conf.cc} -c {conf.ccflags} -o"
                    )


if __name__ == "__main__":
    try:
        build_compile_commands = sys.argv[2]
        if build_compile_commands != "commands":
            raise AttributeError(f"unknown argument {build_compile_commands}")
        print("Building compile_commands.json...")
        build(build_compile_commands)
    except IndexError:
        print("Building project...")
        build()
