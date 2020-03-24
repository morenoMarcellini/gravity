#DEBUG=-g -ggdb3 -pipe
DEBUG=-pipe -Wfatal-errors -std=c++11
#DEBUG=-O2 -pipe -march=native -Wfatal-errors
LIBS= -lGL -lGLU -lglut

integrator.hpp.gch : integrator.hpp
	g++ -c -Wall $(DEBUG) integrator.hpp

utilities.hpp.gch : utilities.hpp
	g++ -c -Wall $(DEBUG) utilities.hpp

colormap.hpp.gch : colormap.hpp
	g++ -c -Wall $(DEBUG) colormap.hpp
	
open3D.hpp.gch : open3D.hpp
	g++ -c -Wall $(DEBUG) open3D.hpp
	
engine.hpp.gch : engine.hpp
	g++ -c -Wall $(DEBUG) engine.hpp

planet.hpp.gch : planet.hpp
	g++ -c -Wall $(DEBUG) planet.hpp
	
main.hpp.gch : main.hpp
	g++ -c -Wall $(DEBUG) main.hpp

integrator.o : integrator.cpp integrator.hpp.gch
	g++ -c -Wall $(DEBUG) integrator.cpp

utilities.o : utilities.cpp utilities.hpp.gch
	g++ -c -Wall $(DEBUG) utilities.cpp
	
open3D.o : open3D.cpp open3D.hpp.gch 
	g++ -c -Wall $(DEBUG) open3D.cpp
	
engine.o : engine.cpp engine.hpp.gch main.hpp.gch 
	g++ -c -Wall $(DEBUG) engine.cpp
	
planet.o : planet.cpp planet.hpp.gch main.hpp.gch 
	g++ -c -Wall $(DEBUG) planet.cpp
	
main.o : main.cpp main.hpp.gch open3D.hpp.gch planet.hpp.gch engine.hpp.gch \
		utilities.hpp.gch integrator.hpp.gch
	g++ -c -Wall $(DEBUG) main.cpp
	
gravity : main.o planet.o engine.o open3D.o utilities.o
	g++ -o gravity main.o planet.o engine.o open3D.o utilities.o $(LIBS)

clean : *.gch *.o
	rm *.gch *.o
