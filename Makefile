all:
	g++ -Wall -c monitor.cpp
	g++ -Wall -c personagem.cpp
	g++ -Wall main.cpp personagem.o monitor.o -lpthread -o tp1

clean:
	rm monitor.o personagem.o tp1