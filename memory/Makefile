CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include -L/usr/lib
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm
CMPL = $(CXX) $(CXXFLAGS)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)
#LIBS = $(INCLUDES) -lstdc++ $(GL) $(ADDITIONALS) $(MATH)


all : MemoryGame

clean :
	rm -f *.o

refresh : clean all


MemoryGame : GLSLloader.o GameController.o MemoryGame.o
	$(CMPL) GLSLloader.o GameController.o MemoryGame.o -o MemoryGame $(LIBS)

MemoryGame.o : MemoryGame.cpp
	$(CMPL) -c MemoryGame.cpp -o MemoryGame.o $(LIBS)

GameController.o : GameController.cpp
	$(CMPL) -c GameController.cpp -o GameController.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(CMPL) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)

