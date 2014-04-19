all:
	g++ -std=c++11 -c Lab4.cpp GLSL_helper.cpp MStackHelp.cpp Methods.cpp -lglu -lGLEW -lGLU -lGL
	g++ Lab4.o -o Lab4.exec GLSL_helper.cpp MStackHelp.cpp Methods.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL
