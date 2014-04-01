CC = g++
CFLAGS = -Wall -std=c++11
GLFLAGS = -lGL -lGLU -lglut

HEADERS = scene.h scene.cpp view.h view.cpp light.h light.cpp mesh.h mesh.cpp camera.h camera.cpp

test1:
	$(CC) $(CFLAGS) $(GLFLAGS) $(HEADERS) -DTEST1 main.cpp -o test1.out

test2:
	$(CC) $(CFLAGS) $(GLFLAGS) $(HEADERS) -DTEST2 main.cpp -o test2.out

clean:
	rm -f *.o *.out *.gch
