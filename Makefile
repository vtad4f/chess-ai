all:
	mkdir -p build ; cd build ; cmake ../src-cpp ; make
clean:
	git clean -dfqX -- .