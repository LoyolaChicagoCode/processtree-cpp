CXXFLAGS =	-O3 -Wall -fmessage-length=0

INCLUDES =	process.h parsing.h io.h

OBJS =		main.o parsing.o io_stdio.o

LIBS =

TARGET =	processtree

$(TARGET):	$(OBJS) $(INCLUDES)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

io_iostream.o:	io_iostream.cpp io.h
io_stdio.o:	io_stdio.cpp io.h
