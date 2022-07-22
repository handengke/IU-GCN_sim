main: main.o Locator.o
	g++ -g -o ./build/main ./build/main.o ./build/Locator.o
main.o: ./src/main.cpp ./src/Locator.h
	g++ -c ./src/main.cpp -o ./build/main.o
Locator.o: ./src/Locator.cpp ./src/Locator.h
	g++ -c ./src/Locator.cpp -o ./build/Locator.o
clean:
	rm ./build/*.o ./build/main ./build/run_log.txt ./build/Lislands.txt
run:
	./build/main>./build/run_log.txt
debug:
	gdb ./build/main
