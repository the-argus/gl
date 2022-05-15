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
import json
import subprocess

CACHEDIR = ".cache"
CACHEFILE = os.path.join(CACHEDIR, "pybuild.json")


class Configuration():
    def __init__(self):
        """linux build configuration"""
        self.secondary_targets = os.path.join("deps", "glad.c")
        self.source_dir = "src"
        self.object_dir = "obj"
        self.cc = "g++"
        self.target = "gl"
        self.ccflags = ["-g", "-Wall", "-lglfw3", "-lGL", "-lX11",
                        "-lpthread", "-lXrandr", "-lXi", "-ldl"]
        self.include = ["-I", "include/"]
        self.link = []

        if os.name == "nt":
            self.windows_overrides()

    def windows_overrides(self):
        """windows build configuration"""
        self.target = "gl.exe"
        self.ccflags = ["-g", "-Wall", "-lglfw3", "-lGdi32"]
        self.link = ["-L\"C:/Program Files (x86)/GLFW/lib\""]
        self.include = ["-I", "include/",
                        "-I", "\"C:/Program Files (x86)/GLFW/include\""]


conf = Configuration()
if not os.path.exists(CACHEDIR):
    os.makedirs(CACHEDIR)

modified_cache = {}
try:
    with open(CACHEFILE) as c:
        # dictionary of filenames to their last modified dates
        modified_cache = json.load(c)
except FileNotFoundError:
    pass


def modified(filepath, filename=None):
    if filename is None:
        filename = filepath
    last_modified = os.stat(filepath).st_mtime

    if filename not in modified_cache:
        # unknown files are considered modified
        modified_cache[filename] = last_modified
        return True

    if modified_cache[filename] != last_modified:
        # update with new modified date and rebuild this file
        modified_cache[filename] = last_modified
        return True
    return False


def execute(commands):
    """
    Accepts a list of shell commands, and then executes them
    in subproccesses.
    Returns True if all commands exited with code 0 (success).
    """
    success = True
    for command in commands:
        print(" ".join(command))
        result = subprocess.run(command)
        if result.returncode != 0:
            # failure, but allow all the other processes to spawn
            success = False

    return success


def build():
    objects = []  # list of object files that get compiled
    compilations = [["mkdir", conf.object_dir]]  # commands to run

    # explicitly compile main.cpp (the only src file in the root dir)
    main = os.path.join(conf.object_dir, "main.o")
    objects.append(main)

    if modified("main.cpp"):
        compilations.append([
                conf.cc, "-c", ] + conf.ccflags + [
                "-o", main,  # target
                "main.cpp",  # source
                ] + conf.link + conf.include
                )

    # scan through source dir and sub directories, build all object files
    for root, dir, files in os.walk(conf.source_dir):
        for file in files:
            if file[-4:] != ".cpp":
                continue

            fullname = os.path.join(root, file)

            prefix = os.path.basename(file)  # just file
            prefix = os.path.splitext(prefix)[0]  # name without extension

            object = f"{os.path.join(conf.object_dir, prefix)}.o"
            if modified(fullname, file):
                compilations.append([
                        conf.cc, "-c"] + conf.ccflags +
                        ["-o", object,  # target
                         fullname,  # source
                         ] + conf.link + conf.include
                        )
            objects.append(object)

    # link the final executable
    compilations.append([
            conf.cc, conf.secondary_targets, "-o", conf.target,
            ] + objects + conf.include + conf.link + conf.ccflags
            )

    # compile everything
    success = execute(compilations)

    # update cache if compilation was successful
    if success:
        with open(CACHEFILE, "w") as c:
            json.dump(modified_cache, c)


def clean():
    commands = [
            ["rm", "-rf", conf.object_dir],
            ["rm", conf.target]
            ]

    execute(commands)


def run():
    commands = [
            [f"./{conf.target}"]
            ]

    execute(commands)


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
