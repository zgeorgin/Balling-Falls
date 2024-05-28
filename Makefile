main: main.cpp primitives.o physics.o
	g++ -I src/include -L src/lib -o main main.cpp primitives.o physics.o -lmingw32 -lSDL2main -lSDL2 -fopenmp
physics.o: physics.cpp
	g++ -c -o physics.o physics.cpp -fopenmp
primitives.o: primitives.cpp
	g++ -I src/include -L src/lib -c -o primitives.o primitives.cpp -lmingw32 -lSDL2main -lSDL2