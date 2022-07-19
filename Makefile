main: main.o Locator.o
	g++ -g -o main main.o Locator.o
main.o: ./src/main.cpp ./src/Locator.h
	g++ -c ./src/main.cpp
Locator.o: ./src/Locator.cpp ./src/Locator.h
	g++ -c ./src/Locator.cpp
clean:
	rm *.o main run_log.txt
run:
	./main>run_log.txt
debug:
	gdb ./main
