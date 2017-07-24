CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include -L/usr/lib
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm
CMPL = $(CXX) $(CXXFLAGS)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)
#LIBS = $(INCLUDES) -lstdc++ $(GL) $(ADDITIONALS) $(MATH)


all : TerrainMap

clean :
	rm -f *.o

refresh : clean all


TerrainMap : GLSLloader.o Area.o Camera.o Detailing.o Earth.o TerrainMap.o
	$(CMPL) GLSLloader.o Area.o Camera.o Detailing.o Earth.o TerrainMap.o -o TerrainMap $(LIBS)

TerrainMap.o : TerrainMap.cpp
	$(CMPL) -c TerrainMap.cpp -o TerrainMap.o $(LIBS)

Camera.o : Camera.cpp
	$(CMPL) -c Camera.cpp -o Camera.o $(LIBS)

Area.o : Area.cpp
	$(CMPL) -c Area.cpp -o Area.o $(LIBS)

Detailing.o : Detailing.cpp
	$(CMPL) -c Detailing.cpp -o Detailing.o $(LIBS)

Earth.o : Earth.cpp
	$(CMPL) -c Earth.cpp -o Earth.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(CMPL) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)
