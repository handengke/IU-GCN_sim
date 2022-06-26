main: main.o Locator.o
	g++ -g -o main main.o Locator.o
main.o: main.cpp Locator.h
	g++ -c main.cpp
Locator.o: Locator.cpp Locator.h
	g++ -c Locator.cpp
clean:
	rm *.o main run.txt
run:
	./main>run.txt
debug:
	gdb ./main
