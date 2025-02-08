files := gavcc.h gavcc.cpp scanner.cpp parser.cpp semanal.cpp eval.cpp
exec := gavcc.o

$(exec): $(files)
	g++ gavcc.cpp -std=c++20 -g -o $(exec)

run: $(exec)
	./$(exec)
