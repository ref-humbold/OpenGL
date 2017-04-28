CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
USR = -I/usr/include/ -L/usr/local/libs/
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm

COMMAND = $(CXX) $(CXXFLAGS) $(USR)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)

all : MemoryGame

clean :
	rm -f *.o

refresh : clean all

MemoryGame : GLSLloader.o GameController.o MemoryGame.o
	$(COMMAND) GLSLloader.o GameController.o MemoryGame.o -o MemoryGame $(LIBS)

MemoryGame.o : MemoryGame.cpp
	$(COMMAND) -c MemoryGame.cpp -o MemoryGame.o $(LIBS)

GameController.o : GameController.cpp
	$(COMMAND) -c GameController.cpp -o GameController.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(COMMAND) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)

