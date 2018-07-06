atm: atm.o
	g++ -o atm atm.o -std=c++11 -lcurses 

atm.o:  atm.cpp User.h
	g++ -std=c++11 -c atm.cpp

clean:
	rm *.o atm
