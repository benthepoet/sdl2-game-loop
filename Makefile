main: main.c
	gcc -g -o bin/main main.c -lSDL2

xml: xml.c
	gcc -g -o bin/xml xml.c -lSDL2 -I/usr/include/libxml2 -lxml2
