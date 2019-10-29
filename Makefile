objects=Main.o SocketApp.o
Main:$(objects)
	g++ -o Main $(objects)

Main.o:Main.cpp
	g++ -c Main.cpp

SocketApp.o:SocketApp.cpp
	g++ -c SocketApp.cpp

clean:
	rm *.o
