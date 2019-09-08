all:
	mkdir -p build ; cd build ; cmake .. ; make
	build/chess-ai "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -" 1.0
clean:
	git clean -dfqX -- .