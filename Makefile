# the compiler: gcc for C program, define as g++ for C++
CC = g++

# cross platform vars ---------------------
WIN_TARGET = triangle.exe
UNIX_TARGET = triangle.out

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
UNIX_CCFLAGS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
WIN_CCFLAGS = -lglfw3 -lGdi32

UNIX_INCLUDE = -I ./include/
WIN_INCLUDE = -I ./include/ -I "C:/Program Files (x86)/GLFW/include"

UNIX_LINKFLAGS = 
WIN_LINKFLAGS = -L"C:/Program Files (x86)/GLFW/lib"

# platform detection ----------------------

# copied from https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
# set flags
	CCFLAGS  = $(WIN_CCFLAGS)
	TARGET = $(WIN_TARGET)
	INCLUDE = $(WIN_INCLUDE)
	LINKFLAGS = $(WIN_LINKFLAGS)
# if using windows, do processor architecture detection
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
else
CCFLAGS  = $(UNIX_CCFLAGS)
TARGET = $(UNIX_TARGET)
INCLUDE = $(UNIX_INCLUDE)
LINKFLAGS = $(UNIX_LINKFLAGS)

    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif

# untested platforms.
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

# building --------------------------------

all: $(TARGET)

$(TARGET): triangle.cpp glad.c
	$(CC) triangle.cpp glad.c -o $(TARGET) $(INCLUDE) $(LINKFLAGS) $(CCFLAGS)

clean:
	$(RM) $(TARGET)
