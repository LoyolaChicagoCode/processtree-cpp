CXXFLAGS =	-O3 -Wall -fmessage-length=0

INCLUDES =	process.h parsing.h io.h

OBJS_STDIO =		main.o parsing.o io_stdio.o
OBJS_IOSTREAM =		main.o parsing.o io_iostream.o

LIBS =

TARGET =	processtree

all:	$(TARGET)-iostream $(TARGET)-stdio

$(TARGET)-iostream:	$(OBJS_IOSTREAM) $(INCLUDES)
	$(CXX) -o $(TARGET)-iostream $(OBJS_IOSTREAM) $(LIBS)

$(TARGET)-stdio:	$(OBJS_STDIO) $(INCLUDES)
	$(CXX) -o $(TARGET)-stdio $(OBJS_STDIO) $(LIBS)

clean:
	rm -f *.o $(TARGET)*

io_iostream.o:	io_iostream.cpp io.h
io_stdio.o:	io_stdio.cpp io.h
