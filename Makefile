CC=g++
SOURCES=main.cpp udp.cpp packet.cpp crc.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXE=server
LDFLAGS= -lboost_program_options -lboost_system
LIBS=

all : $(EXE)

$(EXE) : $(OBJECTS)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

clean :
	$(RM) $(OBJECTS) $(EXE)
