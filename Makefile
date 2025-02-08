files := gavcc.h gavcc.cpp scanner.cpp parser.cpp semanal.cpp eval.cpp
exec := gavcc.o

$(exec): $(files)
	g++ gavcc.cpp -Wall -Wextra -Wno-switch -Wno-missing-field-initializers -std=c++20 -g -O0 -o $(exec)

run: $(exec)
	./$(exec)
