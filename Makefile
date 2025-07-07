CC=g++
CCWIN=i586-mingw32msvc-g++
CFLAGS=-c -Wall

all: subnets
	wc -l *.cpp *.hpp

subnets: main.o network.o functions.o struct.o
	$(CC) main.o network.o functions.o struct.o -o subnets

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

network.o: network.cpp
	$(CC) $(CFLAGS) network.cpp

struct.o: struct.cpp
	$(CC) $(CFLAGS) struct.cpp

functions.o: functions.cpp
	$(CC) $(CFLAGS) functions.cpp

win:
	$(CCWIN) $(CFLAGS) main.cpp
	$(CCWIN) $(CFLAGS) network.cpp
	$(CCWIN) $(CFLAGS) struct.cpp
	$(CCWIN) $(CFLAGS) functions.cpp
	$(CCWIN) main.o network.o functions.o struct.o -o Subnets.exe
	wc -l *.cpp *.hpp

install:
	cp subnets /usr/bin

uninstall:
	rm /usr/bin/subnets

clean:
	rm *o subnets *.exe 2>/dev/null
