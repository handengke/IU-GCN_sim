all:main

algo_src_path:=./src/algo_src
sim_src_path:=./src/sim_src
target_path:=./build

main: main.o Locator.o
	g++ -g -o $(target_path)/main $(target_path)/main.o $(target_path)/Locator.o
main.o: ./src/main.cpp $(algo_src_path)/Locator.h
	g++ -c ./src/main.cpp -o $(target_path)/main.o
Locator.o: $(algo_src_path)/Locator.cpp $(algo_src_path)/Locator.h
	g++ -c $(algo_src_path)/Locator.cpp -o $(target_path)/Locator.o


clean:
	rm $(target_path)/*.o $(target_path)/main $(target_path)/run_log.txt $(target_path)/Lislands.txt
.PHONY: clean

run:
	$(target_path)/main>$(target_path)/run_log.txt
.PHONY: run

debug:
	gdb $(target_path)/main
.PHONY: debug
