all: clean main.c run
main.c:
	gcc src/main.c -o bin/pongremastered.exe -O1 -Wall -std=c99 -Wno-missing-braces -I C:\raylib\raylib\src -L C:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
clean:
	rm -rf bin/*
run:
	bin/pongremastered