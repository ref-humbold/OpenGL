CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
USR = -I/usr/include/ -L/usr/local/libs/
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm

COMMAND = $(CXX) $(CXXFLAGS) $(USR)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)

all : MortalBubbles

clean :
	rm -f *.o

refresh : clean all

MortalBubbles : GLSLloader.o GameAquarium.o GameBubble.o GamePlayer.o GameControler.o MortalBubbles.o
	$(COMMAND) GLSLloader.o GameAquarium.o GameBubble.o GamePlayer.o GameControler.o MortalBubbles.o -o MortalBubbles $(LIBS)

MortalBubbles.o : MortalBubbles.cpp
	$(COMMAND) -c MortalBubbles.cpp -o MortalBubbles.o $(LIBS)

GameControler.o : GameControler.cpp
	$(COMMAND) -c GameControler.cpp -o GameControler.o $(LIBS)

GamePlayer.o : GamePlayer.cpp
	$(COMMAND) -c GamePlayer.cpp -o GamePlayer.o $(LIBS)

GameBubble.o : GameBubble.cpp
	$(COMMAND) -c GameBubble.cpp -o GameBubble.o $(LIBS)

GameAquarium.o : GameAquarium.cpp
	$(COMMAND) -c GameAquarium.cpp -o GameAquarium.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(COMMAND) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)

