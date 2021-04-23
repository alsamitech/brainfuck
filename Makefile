
all: clean build

clean:
	rm -f brainfuck

build:
	gcc brainfuck.c -o brainfuck

install: ; mv brainfuck /usr/bin
