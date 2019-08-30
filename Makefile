all:
	mkdir -p build ; cd build ; cmake ../src-cpp ; make
	build/chess.exe "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -" 1.0
clean:
	git clean -dfqX -- .