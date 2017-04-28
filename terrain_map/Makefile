CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
ANL = -I/home/anl/include/ -L/home/anl/libs/
USR = -I/usr/include/ -L/usr/local/libs/
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
KALETA = -ldl -lXinerama -lXcursor
MATH = -lm

COMMAND = $(CXX) $(CXXFLAGS) $(USR) $(ANL)
LIBS = -lstdc++ $(GL) $(KALETA) $(MATH)

all : TerrainMap

clean :
	rm -f *.o

refresh : clean all

TerrainMap : GLSLloader.o Area.o Camera.o Detailing.o Earth.o TerrainMap.o
	$(COMMAND) GLSLloader.o Area.o Camera.o Detailing.o Earth.o TerrainMap.o -o TerrainMap $(LIBS)

TerrainMap.o : TerrainMap.cpp
	$(COMMAND) -c TerrainMap.cpp -o TerrainMap.o $(LIBS)

Camera.o : Camera.cpp
	$(COMMAND) -c Camera.cpp -o Camera.o $(LIBS)

Area.o : Area.cpp
	$(COMMAND) -c Area.cpp -o Area.o $(LIBS)

Detailing.o : Detailing.cpp
	$(COMMAND) -c Detailing.cpp -o Detailing.o $(LIBS)

Earth.o : Earth.cpp
	$(COMMAND) -c Earth.cpp -o Earth.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(COMMAND) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)
