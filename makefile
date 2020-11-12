all:
	g++ -O3 -std=c++14 -o solution main.cpp TVector.cpp TVector.h
clean:
	rm -rf solution
