CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include -L/usr/lib
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm
CMPL = $(CXX) $(CXXFLAGS)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)
#LIBS = $(INCLUDES) -lstdc++ $(GL) $(ADDITIONALS) $(MATH)


all : MortalBubbles

clean :
	rm -f *.o

refresh : clean all


MortalBubbles : GLSLloader.o GameAquarium.o GameBubble.o GamePlayer.o GameControler.o MortalBubbles.o
	$(CMPL) GLSLloader.o GameAquarium.o GameBubble.o GamePlayer.o GameControler.o MortalBubbles.o -o MortalBubbles $(LIBS)

MortalBubbles.o : MortalBubbles.cpp
	$(CMPL) -c MortalBubbles.cpp -o MortalBubbles.o $(LIBS)

GameControler.o : GameControler.cpp
	$(CMPL) -c GameControler.cpp -o GameControler.o $(LIBS)

GamePlayer.o : GamePlayer.cpp
	$(CMPL) -c GamePlayer.cpp -o GamePlayer.o $(LIBS)

GameBubble.o : GameBubble.cpp
	$(CMPL) -c GameBubble.cpp -o GameBubble.o $(LIBS)

GameAquarium.o : GameAquarium.cpp
	$(CMPL) -c GameAquarium.cpp -o GameAquarium.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(CMPL) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)

