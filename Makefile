all: clean main.c run

main.c:
	gcc src/main.c -o bin/pongremastered.exe -O3 -mwindows -DNDEBUG -s -Wall -std=c99 -Wno-missing-braces -I C:\raylib\raylib\src -L C:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -Wl,--gc-sections

clean:
	rm -rf bin/*

run:
	bin/pongremastered.exe