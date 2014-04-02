CC = g++
CFLAGS = -Wall -std=c++11
GLFLAGS = -lGL -lGLU -lglut

HEADERS := $(wildcard src/include/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(HEADERS:.cpp=.o)))
GL_FILES := $(wildcard GL/*.h)
MAIN_FILE = src/main.cpp

INC = -I src/include

checkdirs:
	@mkdir -p obj

obj/%.o: src/include/%.cpp
	g++ $(CFLAGS) -c -o $@ $<

test1: checkdirs $(OBJ_FILES)
	$(CC) $(CFLAGS) $(GLFLAGS) $(INC) $(OBJ_FILES) -DTEST1 $(MAIN_FILE) -o test1.out

test2: checkdirs $(OBJ_FILES)
	$(CC) $(CFLAGS) $(GLFLAGS) $(INC) $(OBJ_FILES) -DTEST2 $(MAIN_FILE) -o test2.out

clean:
	rm -rf *.o *.out *.gch obj/
