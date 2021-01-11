all:
	g++ -Wall -std=c++11 -c monitor.cpp
	g++ -Wall -std=c++11 -c personagem.cpp
	g++ -Wall -std=c++11 main.cpp personagem.o monitor.o -lpthread -o tp1

clean:
	rm monitor.o personagem.o tp1