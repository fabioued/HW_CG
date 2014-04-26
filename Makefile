CC = g++
CFLAGS = -Wall -std=c++11
GLFLAGS = -lGL -lGLU -lglut -lfreeimage

HEADERS := $(wildcard src/include/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(HEADERS:.cpp=.o)))
GL_FILES := $(wildcard GL/*.h)
MAIN_FILE = src/main.cpp

INC = -I src/include
LIB = -L lib

run: main
	@echo -e "ChessScene\nChess" | ./main.out

checkdirs:
	@mkdir -p obj

obj/%.o: src/include/%.cpp
	g++ $(CFLAGS) -c -o $@ $<

main: checkdirs $(OBJ_FILES)
	$(CC) $(CFLAGS) $(GLFLAGS) $(INC) $(LIB) $(OBJ_FILES) $(MAIN_FILE) -o main.out

clean:
	rm -rf *.o *.out *.gch obj/
