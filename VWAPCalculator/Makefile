VWAPCalculator: VWAPCalculator.o Parser.o
	g++ -std=c++11 VWAPCalculator.o Parser.o -o VWAPCalculator -lfl

VWAPCalculator.o: VWAPCalculator.cpp
	g++ -std=c++11 -c VWAPCalculator.cpp

Parser.o: Parser.cpp
	g++ -std=c++11 -c Parser.cpp

clean:
	rm -f *.o VWAPCalculator