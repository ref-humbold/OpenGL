CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include -L/usr/lib
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm
CMPL = $(CXX) $(CXXFLAGS)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)
#LIBS = $(INCLUDES) -lstdc++ $(GL) $(ADDITIONALS) $(MATH)


all : Arkanoid

clean :
	rm -f *.o

refresh : clean all


Arkanoid : GLSLloader.o GameElement.o GameController.o Arkanoid.o
	$(CMPL) GLSLloader.o GameElement.o GameController.o Arkanoid.o -o Arkanoid $(LIBS)

Arkanoid.o : Arkanoid.cpp
	$(CMPL) -c Arkanoid.cpp -o Arkanoid.o $(LIBS)

GameElement.o : GameElement.cpp
	$(CMPL) -c GameElement.cpp -o GameElement.o $(LIBS)

GameController.o : GameController.cpp
	$(CMPL) -c GameController.cpp -o GameController.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(CMPL) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)

