CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

INCLUDES =	process.h

OBJS =		main.o

LIBS =

TARGET =	processtree

$(TARGET):	$(OBJS) $(INCLUDES)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
