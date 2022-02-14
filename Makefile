# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I include/

# The build target 
TARGET = triangle

all: $(TARGET).o

$(TARGET).o: $(TARGET).cpp
	# CFLAGS MUST COME AFTER TARGET FOR CORRECT LINKING
	#  :(
	$(CC) -o $(TARGET).o $(TARGET).cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
