CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDE = -I/usr/local/include -L/usr/local/lib
GLBASE = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
GLADD = -ldl -lXinerama -lXcursor
CMPL = $(CXX) $(CXXFLAGS)
LIBS = $(INCLUDE) -lstdc++ $(GLBASE) $(GLADD) -lm


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

