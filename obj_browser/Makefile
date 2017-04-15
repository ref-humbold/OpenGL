CXX = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
ANL = -I/home/anl/include/ -L/home/anl/libs/
USR = -I/usr/include/ -L/usr/local/libs/
GL = -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
KALETA = -ldl -lXinerama -lXcursor
MATH = -lm

COMMAND = $(CXX) $(CXXFLAGS) $(USR) $(ANL)
LIBS = -lstdc++ $(GL) $(KALETA) $(MATH)

all : ObjBrowser

clean :
	rm -f *.o

refresh : clean all

ObjBrowser : ObjBrowser.o GraphicObject.o Camera.o FileReader.o GLSLloader.o
	$(COMMAND) ObjBrowser.o GraphicObject.o Camera.o FileReader.o GLSLloader.o -o ObjBrowser $(LIBS)

ObjBrowser.o : ObjBrowser.cpp GraphicObject.hpp Camera.hpp FileReader.hpp GLSLloader.hpp
	$(COMMAND) -c ObjBrowser.cpp -o ObjBrowser.o $(LIBS)

GraphicObject.o : GraphicObject.cpp GraphicObject.hpp
	$(COMMAND) -c GraphicObject.cpp -o GraphicObject.o $(LIBS)

Camera.o : Camera.cpp Camera.hpp GraphicObject.hpp
	$(COMMAND) -c Camera.cpp -o Camera.o $(LIBS)

FileReader.o : FileReader.cpp FileReader.hpp GraphicObject.hpp
	$(COMMAND) -c FileReader.cpp -o FileReader.o $(LIBS)

GLSLloader.o : GLSLloader.cpp GLSLloader.hpp
	$(COMMAND) -c GLSLloader.cpp -o GLSLloader.o $(LIBS)
