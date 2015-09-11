all:  lib/vec.o lib/c_num.o lib/timer.o lib/fft.o lib/ocean.o lib/cube.o lib/sdl_manager.o
	g++ main.cpp lib/vec.o lib/c_num.o lib/timer.o lib/fft.o lib/ocean.o lib/cube.o lib/sdl_manager.o -o bin/main -framework OpenGL -framework SDL2 -framework SDL2_image

lib/ocean.o: src/ocean.h src/ocean.cpp
	g++ src/ocean.cpp -c -o lib/ocean.o

lib/vec.o: src/vec.h src/vec.cpp
	g++ src/vec.cpp -c -o lib/vec.o

lib/c_num.o: src/c_num.h src/c_num.cpp
	g++ src/c_num.cpp -c -o lib/c_num.o

lib/timer.o: src/timer.h src/timer.cpp
	g++ src/timer.cpp -c -o lib/timer.o

lib/fft.o: src/fft.h src/fft.cpp
	g++ src/fft.cpp -c -o lib/fft.o

lib/cube.o: src/cube.h src/cube.cpp
	g++ src/cube.cpp -c -o lib/cube.o

lib/sdl_manager.o: src/sdl_manager.h src/sdl_manager.cpp
	g++ src/sdl_manager.cpp -c -o lib/sdl_manager.o
clean:
	@rm -f *~ src/*~ lib/* bin/*
