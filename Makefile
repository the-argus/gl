# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I ./include/

# The build target 
TARGET = triangle

LINKED = glad.c

all: $(TARGET).o

$(TARGET).o: $(TARGET).cpp
	$(CC) -o $(TARGET).o $(TARGET).cpp $(LINKED) $(CFLAGS)

clean:
	$(RM) $(TARGET).o
