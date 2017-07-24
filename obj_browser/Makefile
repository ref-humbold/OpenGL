CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include -L/usr/lib
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
ADDITIONALS = -ldl -lXinerama -lXcursor
MATH = -lm
CMPL = $(CXX) $(CXXFLAGS)
LIBS = -lstdc++ $(GL) $(ADDITIONALS) $(MATH)
#LIBS = $(INCLUDES) -lstdc++ $(GL) $(ADDITIONALS) $(MATH)


all : ObjBrowser

clean :
	rm -f *.o

refresh : clean all


ObjBrowser : GLSLloader.o GraphicObject.o Camera.o FileReader.o ObjBrowser.o
	$(CMPL) GLSLloader.o GraphicObject.o Camera.o FileReader.o ObjBrowser.o -o ObjBrowser $(LIBS)

ObjBrowser.o : ObjBrowser.cpp
	$(CMPL) -c ObjBrowser.cpp -o ObjBrowser.o $(LIBS)

GraphicObject.o : GraphicObject.cpp
	$(CMPL) -c GraphicObject.cpp -o GraphicObject.o $(LIBS)

Camera.o : Camera.cpp
	$(CMPL) -c Camera.cpp -o Camera.o $(LIBS)

FileReader.o : FileReader.cpp
	$(CMPL) -c FileReader.cpp -o FileReader.o $(LIBS)

GLSLloader.o : GLSLloader.cpp
	$(CMPL) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)
