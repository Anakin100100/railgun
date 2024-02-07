compile:
	g++ -std=c++23 -o railgun.out main.cpp
run:
	rm railgun.pgn
	cutechess-cli -engine cmd=./railgun.out name=Railgun -debug -engine cmd=stockfish name=Stockfish tc=inf -each tc=10/2 proto=uci -pgnout railgun.pgn
benchmark:
	rm railgun.pgn
	cutechess-cli -engine cmd=./railgun.out name=Railgun -debug -engine cmd=stockfish name=Stockfish tc=inf -each tc=10/2 proto=uci -pgnout railgun.pgn -rounds 100 -games 2