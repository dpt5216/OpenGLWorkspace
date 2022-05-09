#g++ -pthread -o test test.c -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

CC=g++
CFLAGS=-I. -Wall -c -pthread
LINKARGS=-g
LIBS=-L. -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lpthread -lGLEW -lGLU -lGL


.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $<


OBJECT_FILES=	test.o\
				renderer.o\
				vertexBuffer.o\
				indexBuffer.o

all : test

test : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean : 
	rm -f test $(OBJECT_FILES)

debug: CFLAGS+=-g
debug: all

run : test
	./test