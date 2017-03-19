all: clean build

LDFLAGS = -lglut -lGL -lGLU
FLAGS = -v -g -Wall -c -traditional-cpp

build: 
	g++ -o t5p1.bin t5p1.cpp $(LDFLAGS)
	g++ -o t5p3.bin t5p3.cpp $(LDFLAGS)

1:
	g++ -o t5p1.bin t5p1.cpp $(LDFLAGS)

3:
	g++ -o t5p3.bin t5p3.cpp $(LDFLAGS)

clean:
	rm -rf t5p1.bin t5p3.bin
