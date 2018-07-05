atm: atm.o
	g++ -o atm atm.o -lcurses 

atm.o:  atm.cpp User.h
	g++ -c atm.cpp

clean:
	rm *.o atm
