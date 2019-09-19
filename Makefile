all:
	mkdir -p build ; cd build ; cmake .. ; make
	cp C:/cygwin64/bin/cygstdc++-6.dll    build || true
	cp C:/cygwin64/bin/cyggcc_s-seh-1.dll build || true
	cp C:/cygwin64/bin/cygwin1.dll        build || true
	build/chess-ai "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -" 1.0
clean:
	git clean -dfqX -- .