atm: atm.o
	g++ -o atm atm.o -pthread 

atm.o:  atm.cpp User.h
	g++ -c atm.cpp

clean:
	rm *.o atm
